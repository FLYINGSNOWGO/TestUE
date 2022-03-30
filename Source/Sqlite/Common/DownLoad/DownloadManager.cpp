// ClouddMinds RobotEngine  Powered by EPIC game


#include "DownloadManager.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IHttpRequest.h"
#include "HttpModule.h"
#include "PlatformHttp.h"
#include "Containers/Ticker.h"

DEFINE_LOG_CATEGORY(LogDownloadManager)

TSharedPtr<FDownloadManager> FDownloadManager::PakDownloader;

FDownloadManager::FDownloadManager(const uint32 RetryLimitCount,const double RetryTimeout)
{
	if (RetryTimeout == 0.f)
	{
		HttpRetryManager = MakeShared<FHttpRetrySystem::FManager>(
			FHttpRetrySystem::FRetryLimitCountSetting(RetryLimitCount),
			FHttpRetrySystem::FRetryTimeoutRelativeSecondsSetting()
			);
	}
	else
	{
		HttpRetryManager = MakeShared<FHttpRetrySystem::FManager>(
			FHttpRetrySystem::FRetryLimitCountSetting(RetryLimitCount),
			FHttpRetrySystem::FRetryTimeoutRelativeSecondsSetting(RetryTimeout)
			);
	}
	TickDelegate = FTickerDelegate::CreateRaw(this, &FDownloadManager::Tick);
	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(TickDelegate);
}

FDownloadManager::~FDownloadManager()
{
	// Unregister ticker delegate
	FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
	CancelAllRemainRequest();
}

bool FDownloadManager::Tick(float DeltaSeconds)
{
	if (HttpRetryManager.IsValid())
	{
		HttpRetryManager->Update();
	}
	return true;
}

void FDownloadManager::CancelAllRemainRequest()
{
	for (auto & Request : RequestList)
	{
		Request->CancelRequest();
	}
}

void FDownloadManager::DownLoad(const FDownloadFileEntry& DownloadFileEntry)
{
	UE_LOG(LogDownloadManager, Log, TEXT("FDownloadManager::DownLoad->start download:%s,save Path:%s"), *DownloadFileEntry.RelativeUrl, *DownloadFileEntry.FileFullPath);
	// TODO add config values for retries, for now, using default
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = HttpRetryManager->CreateRequest();
	HttpRequest->SetURL(DownloadFileEntry.RelativeUrl);
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->SetHeader("FileFullPath", DownloadFileEntry.FileFullPath);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/octet-stream"));
	//DownloadFileEntry.DownloadFileWillRetryDelegate.ExecuteIfBound(DownloadFileEntry.FileName);
	TWeakPtr<FDownloadManager> WeakThisPtr = AsShared();
	HttpRequest->OnRequestWillRetry().BindLambda([WeakThisPtr, DownloadFileEntry](FHttpRequestPtr Request, FHttpResponsePtr Response, float SecondsToRetry){
		TSharedPtr<FDownloadManager> SharedThis = WeakThisPtr.Pin();
		if (SharedThis.IsValid())
		{
			UE_LOG(LogDownloadManager,Log,TEXT("FDownloadManager::OnRequestWillRetry->Request Will Retry:FileName:%s,url:%s"), *DownloadFileEntry.FileName, Request.IsValid() ? *Request->GetURL() : TEXT("Request Invalid"));
			FString FileName = DownloadFileEntry.FileName;
			UE_LOG(LogDownloadManager, Log, TEXT("FDownloadManager::OnRequestWillRetry->FileName:%s"), *FileName);
			DownloadFileEntry.DownloadFileWillRetryDelegate.ExecuteIfBound(DownloadFileEntry.FileName);
		}
		else
		{
			UE_LOG(LogDownloadManager,Error,TEXT("FDownloadManager::OnRequestWillRetry->SharedThis Invalid"));
		}
	});

	HttpRequest->OnRequestProgress().BindLambda([WeakThisPtr, DownloadFileEntry](FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived){
		TSharedPtr<FDownloadManager> SharedThis = WeakThisPtr.Pin();
		if (SharedThis.IsValid())
		{
			if (!Request.IsValid())
			{
				UE_LOG(LogDownloadManager, Error, TEXT("FDownloadManager::OnRequestProgress->Request Invalid"));
				return;
			}
			UE_LOG(LogDownloadManager,Log,TEXT("FDownloadManager::OnRequestProgress->%s Progress:BytesReceived %d,BytesSent %d"),*Request->GetURL(),BytesReceived, BytesSent);
			DownloadFileEntry.DownloadProgressDelegate.ExecuteIfBound(DownloadFileEntry.FileName, BytesReceived,DownloadFileEntry.FileSize);
		}
		else
		{
			UE_LOG(LogDownloadManager, Error, TEXT("FDownloadManager::OnRequestProgress->SharedThis Invalid"));
		}
	});

	HttpRequest->OnProcessRequestComplete().BindLambda([WeakThisPtr, DownloadFileEntry](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess){
		TSharedPtr<FDownloadManager> SharedThis = WeakThisPtr.Pin();
		if (SharedThis.IsValid())
		{
			if (bSuccess && Request.IsValid() && Response.IsValid())
			{
				SharedThis->RequestList.Remove((TSharedRef<IHttpRequest, ESPMode::ThreadSafe>)Request.ToSharedRef());
				FString FileFullPath = Request->GetHeader("FileFullPath");
				if (FileFullPath.IsEmpty())
				{
					UE_LOG(LogDownloadManager, Error, TEXT("FDownloadManager::OnProcessRequestComplete->FileFullPath Empty!"));
					DownloadFileEntry.DownloadFileErrorDelegate.ExecuteIfBound(DownloadFileEntry.FileName, EDownloadErrorType::SaveFilePathInvalid);
					return;
				}
				bool bDownloadSuccess = FFileHelper::SaveArrayToFile(Response->GetContent(), *FileFullPath);
				if (bDownloadSuccess)
				{
					UE_LOG(LogDownloadManager, Log, TEXT("FDownloadManager::OnProcessRequestComplete->%s save success"), *FileFullPath);
					if (DownloadFileEntry.RemoteMapMd5.IsEmpty())
					{
						UE_LOG(LogDownloadManager, Warning, TEXT("FDownloadManager::OnProcessRequestComplete->RemoteMapMd5 Empty"));
						AsyncTask(ENamedThreads::GameThread, [=]() {
							DownloadFileEntry.DownloadFileCompleteDelegate.ExecuteIfBound(FileFullPath);
							});
					}
					else
					{

						FMD5Hash Hash = FMD5Hash::HashFile(*FileFullPath);
						FString LocalMapMd5 = LexToString(Hash);
						UE_LOG(LogDownloadManager, Log, TEXT("RemoteMapMd5:%s;LocalMapMd5:%s"), *DownloadFileEntry.RemoteMapMd5, *LocalMapMd5);
						if (LocalMapMd5.Equals(DownloadFileEntry.RemoteMapMd5))
						{
							AsyncTask(ENamedThreads::GameThread, [=]() {
								DownloadFileEntry.DownloadFileCompleteDelegate.ExecuteIfBound(FileFullPath);
							});
						}
						else
						{
							UE_LOG(LogDownloadManager, Error, TEXT("RemoteMapMd5 is not the same as LocalMapMd5 "));
							DownloadFileEntry.DownloadFileErrorDelegate.ExecuteIfBound(DownloadFileEntry.FileName, EDownloadErrorType::VerifyMD5Fail);
						}
					}
				}
				else
				{
					UE_LOG(LogDownloadManager, Error, TEXT("FDownloadManager::OnProcessRequestComplete->%s save fail"), *FileFullPath);
					DownloadFileEntry.DownloadFileErrorDelegate.ExecuteIfBound(DownloadFileEntry.FileName, EDownloadErrorType::SaveFileFail);
				}
			}
			else
			{
				FString DownloadFaildInfo = TEXT("FDownloadManager::OnProcessRequestComplete->Faild,Info:");

				DownloadFaildInfo = FString::Printf(TEXT("%s Url:%s; "), 
					*DownloadFaildInfo, 
					Request.IsValid() ? *Request->GetURL() : TEXT("Request Invalid"));

				DownloadFaildInfo = FString::Printf(TEXT("%s Code:%s"),
					*DownloadFaildInfo, 
					Response.IsValid() ? *FString::FromInt(Response->GetResponseCode()) : TEXT("Response Invalid"));

				UE_LOG(LogDownloadManager, Error, TEXT("%s"), *DownloadFaildInfo);

				EDownloadErrorType ErrorType = EDownloadErrorType::None;
				if (!bSuccess)
				{
					if (!Request.IsValid())
					{
						ErrorType = EDownloadErrorType::RequestError;
					}
					else
					{
						switch (Request->GetStatus())
						{
						case EHttpRequestStatus::Type::Failed_ConnectionError:
							ErrorType = EDownloadErrorType::ConnectionError;
						break;
						case EHttpRequestStatus::Type::Failed:
							ErrorType = EDownloadErrorType::ServiceError;
						break;
						default:
							break;
						}
					}
					DownloadFileEntry.DownloadFileErrorDelegate.ExecuteIfBound(DownloadFileEntry.FileName, ErrorType);
				}
			}
		}
		else
		{
			UE_LOG(LogDownloadManager, Error, TEXT("FDownloadManager::OnProcessRequestComplete->SharedThis Invalid"));
			DownloadFileEntry.DownloadFileErrorDelegate.ExecuteIfBound(DownloadFileEntry.FileName, EDownloadErrorType::DownLoadManagerInvalid);
		}
	});

	HttpRequest->ProcessRequest();
	RequestList.Add(HttpRequest);
}

TSharedRef<FDownloadManager> FDownloadManager::Get()
{
	if (!PakDownloader.IsValid())
	{
		//TODO:You can modify the values of RetryLimitCount and RetryTimeout through configuration
		uint32 RetryLimitCount = 3;
		// Unlimited timeout  [3/3/2022 CarlZhou]
		double RetryTimeout = 0.f;
		PakDownloader = MakeShareable(new FDownloadManager(RetryLimitCount, RetryTimeout));
		PakDownloader->HttpRetryManager->SetRandomFailureRate(0.6);
	}
	return PakDownloader->AsShared();
}

const TCHAR* FDownloadManager::DownloadErrorTypeToString(const EDownloadErrorType ErrorType)
{
	switch (ErrorType)
	{
	case FDownloadManager::EDownloadErrorType::None:
		return TEXT("None");
	case FDownloadManager::EDownloadErrorType::DownLoadManagerInvalid:
		return TEXT("DownLoadManagerInvalid");
	case FDownloadManager::EDownloadErrorType::ConnectionError:
		return TEXT("ConnectionError");
	case FDownloadManager::EDownloadErrorType::RequestError:
		return TEXT("RequestError");
	case FDownloadManager::EDownloadErrorType::ServiceError:
		return TEXT("ServiceError");
	case FDownloadManager::EDownloadErrorType::SaveFileFail:
		return TEXT("SaveFileFail");
	case FDownloadManager::EDownloadErrorType::VerifyMD5Fail:
		return TEXT("VerifyMD5Fail");
	case FDownloadManager::EDownloadErrorType::SaveFilePathInvalid:
		return TEXT("SaveFilePathInvalid");
	default:
		return TEXT("Unknown");
	}
}

FDownloadManager::FDownloadFileEntry::FDownloadFileEntry()
{
	UE_LOG(LogDownloadManager, Log, TEXT("FDownloadFileEntry::FDownloadFileEntry"));
}

FDownloadManager::FDownloadFileEntry::FDownloadFileEntry(const FDownloadFileEntry& Entry)
{
	UE_LOG(LogDownloadManager, Log, TEXT("FDownloadFileEntry::Copy construction"));
	this->DownloadFileCompleteDelegate = Entry.DownloadFileCompleteDelegate;
	this->DownloadFileErrorDelegate = Entry.DownloadFileErrorDelegate;
	this->DownloadFileWillRetryDelegate = Entry.DownloadFileWillRetryDelegate;
	this->DownloadProgressDelegate = Entry.DownloadProgressDelegate;
	this->FileFullPath = Entry.FileFullPath;
	this->FileName = Entry.FileName;
	this->FileSize = Entry.FileSize;
	this->RelativeUrl = Entry.RelativeUrl;
	this->RemoteMapMd5 = Entry.RemoteMapMd5;
}

FDownloadManager::FDownloadFileEntry::~FDownloadFileEntry()
{
	UE_LOG(LogDownloadManager, Log, TEXT("FDownloadFileEntry::~FDownloadFileEntry"));
}

FDownloadManager::FDownloadFileEntry& FDownloadManager::FDownloadFileEntry::operator=(const FDownloadFileEntry& Entry)
{
	UE_LOG(LogDownloadManager, Log, TEXT("FDownloadFileEntry::operator="));
	this->DownloadFileCompleteDelegate = Entry.DownloadFileCompleteDelegate;
	this->DownloadFileErrorDelegate = Entry.DownloadFileErrorDelegate;
	this->DownloadFileWillRetryDelegate = Entry.DownloadFileWillRetryDelegate;
	this->DownloadProgressDelegate = Entry.DownloadProgressDelegate;
	this->FileFullPath = Entry.FileFullPath;
	this->FileName = Entry.FileName;
	this->FileSize = Entry.FileSize;
	this->RelativeUrl = Entry.RelativeUrl;
	this->RemoteMapMd5 = Entry.RemoteMapMd5;
	return *this;
}