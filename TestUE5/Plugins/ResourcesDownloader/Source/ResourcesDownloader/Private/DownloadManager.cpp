// ClouddMinds RobotEngine  Powered by EPIC game


#include "DownloadManager.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IHttpRequest.h"
#include "HttpModule.h"
#include "PlatformHttp.h"
#include "Containers/Ticker.h"
#include "DownTask/TaskInfo.h"



DEFINE_LOG_CATEGORY(LogDownloadManager);


TSharedPtr<FDownloadManager> FDownloadManager::DownloadManager;

FDownloadManager::FDownloadManager(const uint32 RetryLimitCount, const double RetryTimeout)
{
	/*if (RetryTimeout == 0.f)
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
	}*/
	/*TickDelegate = FTickerDelegate::CreateRaw(this, &FDownloadManager::Tick);
	TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(TickDelegate);*/
}

FDownloadManager::~FDownloadManager()
{
	// Unregister ticker delegate
	//FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
	CancelAllRemainRequest();
}

//bool FDownloadManager::Tick(float DeltaSeconds)
//{
//	if (HttpRetryManager.IsValid())
//	{
//		HttpRetryManager->Update();
//	}
//	return true;
//}

void FDownloadManager::CancelAllRemainRequest()
{
	for (TPair<FGuid, TSharedPtr<FDownloadTask>>& element : TaskArray) 
	{
		element.Value->Stop();
	}
	TaskArray.Empty();
}


void FDownloadManager::DownLoad(const FDownloadFileEntry& DownloadFileEntry)
{
	if (DownloadFileEntry.RelativeUrl.IsEmpty())
	{
		UE_LOG(LogDownloadManager, Error, TEXT("FDownloadManager::DownLoad:RelativeUrl is Empty!%s "), *DownloadFileEntry.FileName);
		return;
	}
	FString savePath,RS;
	DownloadFileEntry.FileFullPath.Split(DownloadFileEntry.FileName,&savePath,&RS);
	UE_LOG(LogDownloadManager, Log, TEXT("savePath:%s"), *savePath);
	
	TSharedRef < FDownloadTask> downProxy  = MakeShared<FDownloadTask>(DownloadFileEntry.RelativeUrl, savePath, DownloadFileEntry.FileName, DownloadFileEntry.FileSize); //NewObject<FDownloadTask>(GetTransientPackage(), FDownloadTask::StaticClass());

	TaskArray.Add(downProxy->GetGuid(),downProxy);

	

	downProxy->OnTaskEvent.AddLambda([this,DownloadFileEntry](EDownloadTaskEvent DownEvent, FTaskInfo TaskInfo)
	{
		const FString& FileFullPath = DownloadFileEntry.FileFullPath;
		if (DownEvent == EDownloadTaskEvent::END_DOWNLOAD)
		{

			UE_LOG(LogDownloadManager, Log, TEXT("FDownloadManager::OnProcessRequestComplete->%s save success"), *FileFullPath);
			if (DownloadFileEntry.RemoteMd5.IsEmpty())
			{
				UE_LOG(LogDownloadManager, Warning, TEXT("FDownloadManager::OnProcessRequestComplete->RemoteMapMd5 Empty"));
				DownloadFileEntry.DownloadFileCompleteDelegate.ExecuteIfBound(FileFullPath);
			}
			else
			{
				FMD5Hash Hash = FMD5Hash::HashFile(*FileFullPath);
				FString LocalMapMd5 = LexToString(Hash);
				UE_LOG(LogDownloadManager, Log, TEXT("RemoteMapMd5:%s;LocalMapMd5:%s"), *DownloadFileEntry.RemoteMd5, *LocalMapMd5);
				if (LocalMapMd5.Equals(DownloadFileEntry.RemoteMd5))
				{

					DownloadFileEntry.DownloadFileCompleteDelegate.ExecuteIfBound(FileFullPath);
				}
				else
				{
					UE_LOG(LogDownloadManager, Error, TEXT("RemoteMapMd5 is not the same as LocalMapMd5 "));
					DownloadFileEntry.DownloadFileErrorDelegate.ExecuteIfBound(DownloadFileEntry.FileName, EDownloadErrorType::VerifyMD5Fail);
				}
			}

			this->TaskArray.Remove(TaskInfo.GUID);
		}
		else if(DownEvent == EDownloadTaskEvent::TE_ERROR)
		{
			UE_LOG(LogDownloadManager, Error, TEXT("FDownloadManager::OnProcessRequestComplete->%s save fail"), *FileFullPath);
			DownloadFileEntry.DownloadFileErrorDelegate.ExecuteIfBound(DownloadFileEntry.FileName, EDownloadErrorType::SaveFileFail);
			TSharedPtr<FDownloadTask> task = this->TaskArray.FindRef(TaskInfo.GUID);
			if (task.Get())
			{
				task->Stop();
				this->TaskArray.Remove(TaskInfo.GUID);
			}
		}
		else if (DownEvent == EDownloadTaskEvent::UPDATE_DOWNLOAD)
		{
			DownloadFileEntry.DownloadProgressDelegate.ExecuteIfBound(DownloadFileEntry.FileName, TaskInfo.CurrentSize, TaskInfo.TotalSize);
		}
		else
		{
			UE_LOG(LogDownloadManager, Log, TEXT("FDownloadManager::OnProcessRequestComplete-> Event:%s"), FDownloadTask::DownloadTaskEventToString(DownEvent));
		}


	

	});

	downProxy->Start();




}


TSharedRef<FDownloadManager> FDownloadManager::Get()
{
	if (!DownloadManager.IsValid())
	{
		//TODO:You can modify the values of RetryLimitCount and RetryTimeout through configuration
		uint32 RetryLimitCount = 3;
		// Unlimited timeout  [3/3/2022 CarlZhou]
		double RetryTimeout = 0.f;
		DownloadManager = MakeShareable(new FDownloadManager(RetryLimitCount, RetryTimeout));
	}
	return DownloadManager->AsShared();
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


FString FDownloadManager::GetPakSaveRoot()
{
	FString path;
#if PLATFORM_DESKTOP &&  WITH_EDITOR
		path = FPaths::Combine(FPaths::ProjectSavedDir(),TEXT("Paks"));
#else
		path = FPaths::Combine(FPaths::RootDir(), TEXT("Paks"));
#endif
	UE_LOG(LogDownloadManager, Warning, TEXT("[FDownloadManager] SetPakSaveRoot :%s"), *path);
	return path;
}








//test DownTask
void FDownloadManager::AddTask(const FString& URL, const FString& Name, const int64 Size)
{
/*

	auto Task = MakeShared<FDownloadTask>(URL, GetPakSaveRoot(), Name, Size);

	if (TaskArray.Contains(Task->GetGuid()))
	{
		return;
	}

	Task->OnTaskEvent.BindRaw(this, &FDownloadManager::OnTaskEvent);

	TaskArray.Add(Task->GetGuid(), Task);*/
}

void FDownloadManager::OnTaskEvent(const EDownloadTaskEvent InEvent, const FTaskInfo& InInfo)
{
	/*switch (InEvent)
	{
	case EDownloadTaskEvent::BEGIN_DOWNLOAD:
	{
		OnDownloadEvent.Execute(EDownloadState::BEGIN_FILE_DOWNLOAD, InInfo);
	}
	break;
	case EDownloadTaskEvent::UPDATE_DOWNLOAD:
	{
		const auto CurrentTime = GWorld->GetTimeSeconds();

		if (CurrentTime - LastUpdateTime < 0.1)
		{
			return;
		}

		OnDownloadEvent.Execute(EDownloadState::UPDATE_DOWNLOAD, InInfo);

	}
	break;
	case EDownloadTaskEvent::END_DOWNLOAD:
	{
		OnDownloadEvent.Execute(EDownloadState::END_FILE_DOWNLOAD, InInfo);

		UE_LOG(LogGameHotUpdate, Log, TEXT("%s download finish"), *InInfo.FileName);

		OnTaskFinish(InInfo, true);

		if (IsSuccessful())
		{
			UE_LOG(LogGameHotUpdate, Log, TEXT("All tasks download finish"));

			OnAllTaskFinish();
		}
	}
	break;
	case EDownloadTaskEvent::ERROR:
	{
		UE_LOG(LogGameHotUpdate, Log, TEXT("%s download failed"), *InInfo.URL);

		OnTaskFinish(InInfo, false);
	}
	break;
	default:
		break;
	}*/
}