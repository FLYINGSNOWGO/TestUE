#include "DownTask/DownLoadTask.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/KismetStringLibrary.h"

DEFINE_LOG_CATEGORY(LogGameHotUpdate);

FString FDownloadTask::TempFileExtension = TEXT(".tmp");
TArray< TSharedPtr<class IHttpRequest, ESPMode::ThreadSafe> >FDownloadTask::HttpList1;
TArray< TSharedPtr<class IHttpRequest, ESPMode::ThreadSafe> >FDownloadTask::HttpList2;

#define BreakPointReDownCDTime 10

FDownloadTask::FDownloadTask(const FString& URL, const FString& SaveRoot, const FString& FileName,
                             const int64 FileSize) : State(EDownloadTaskState::Pending), TempFileHandle(nullptr),
                                                     Request(nullptr)
{
#if PLATFORM_ANDROID
	if (!FPlatformFileManager::Get().GetPlatformFile().GetLowerLevel()->DirectoryExists(*SaveRoot))
#elif PLATFORM_WINDOWS
    if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*SaveRoot))
#endif
    {
        if (!FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*SaveRoot))
        {
            UE_LOG(LogGameHotUpdate, Warning, TEXT("Cannot create directory : %s"), *SaveRoot);
        }
    }

    Root = SaveRoot;

    TaskInfo.URL = URL;

    TaskInfo.FileName = FileName;

    TaskInfo.FileSize = FileSize;
    TaskInfo.TotalSize = FileSize;

	TickerDelegate.BindRaw(this, &FDownloadTask::Tick);
	TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(TickerDelegate, 1.0f);
}

FDownloadTask::~FDownloadTask()
{
    UE_LOG(LogGameHotUpdate, Warning, TEXT("~FDownloadTask"));
    FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
}


const TCHAR* FDownloadTask::DownloadTaskEventToString(const EDownloadTaskEvent TaskEvent)
{
    switch (TaskEvent)
    {
    case EDownloadTaskEvent::REQ_HEAD:
        return TEXT("REQ_HEAD");
    case EDownloadTaskEvent::RET_HEAD:
        return TEXT("RET_HEAD");
    case EDownloadTaskEvent::BEGIN_DOWNLOAD:
        return TEXT("BEGIN_DOWNLOAD");
    case EDownloadTaskEvent::UPDATE_DOWNLOAD:
        return TEXT("UPDATE_DOWNLOAD");
    case EDownloadTaskEvent::END_DOWNLOAD:
        return TEXT("END_DOWNLOAD");
    case EDownloadTaskEvent::TE_ERROR:
        return TEXT("ERROR");
    default:
        return TEXT("UNKNOWN");
    }
}

bool FDownloadTask::IsFileExist() const
{
    return IFileManager::Get().FileExists(*FPaths::Combine(Root, TaskInfo.FileName));
}

void FDownloadTask::Start()
{
    if (TaskInfo.FileName.IsEmpty())
    {
        TaskInfo.FileName = FPaths::GetCleanFilename(TaskInfo.URL);
    }

    if (TaskInfo.URL.IsEmpty() || TaskInfo.FileName.IsEmpty())
    {
        OnTaskEvent.Broadcast(EDownloadTaskEvent::TE_ERROR, TaskInfo);

        return;
    }

    if (!IsPending() || IsFinished())
    {
        return;
    }

    State = EDownloadTaskState::DownLoading;

    ReqGetHead();

    //TimerDelegate.BindRaw(this, &FDownloadTask::BreakPointReDown);
    
 

}

void FDownloadTask::Stop()
{
    if (TempFileHandle != nullptr)
    {
        delete TempFileHandle;

        TempFileHandle = nullptr;
    }

    if (Request.IsValid())
    {
        Request->OnProcessRequestComplete().Unbind();

        Request->OnRequestProgress().Unbind();

        FDownloadTask::CacheHttp(Request,2);
        
        Request = nullptr;
    }

    State = EDownloadTaskState::Finished;

    OnTaskEvent.Clear();

    
}

FTaskInfo FDownloadTask::GetTaskInfo() const
{
    return TaskInfo;
}

void FDownloadTask::ReqGetHead()
{
	
    const auto& EncodedURL = GetEncodedURL();

    if (EncodedURL.IsEmpty())
    {
        UE_LOG(LogGameHotUpdate, Warning, TEXT("Error URL"))

        return;
    }
  

	Request = FDownloadTask::GetHttp(1);

	//RedirectUrl = Request->GetRedirectUrl(TaskInfo.URL);
	if (RedirectUrl.IsEmpty())
	{
		RedirectUrl = TaskInfo.URL;
	}
    Request->SetVerb("HEAD");

    Request->SetURL(RedirectUrl);

    Request->OnProcessRequestComplete().BindRaw(this, &FDownloadTask::RetGetHead);
    Request->OnHeaderReceived().BindRaw(this, &FDownloadTask::OnRequestHeadHeaderReceived);

    UE_LOG(LogGameHotUpdate, Log, TEXT("TaskInfo.URL:%s; RedirectUrl:%s"), *TaskInfo.URL, *RedirectUrl);
    Request->ProcessRequest();

    OnTaskEvent.Broadcast(EDownloadTaskEvent::REQ_HEAD, TaskInfo);

    
}

void FDownloadTask::ReDownload()
{
	if (Request.IsValid())
	{
		Request->OnProcessRequestComplete().Unbind();

		Request->OnRequestProgress().Unbind();

		Request.Reset();
        
		Request = nullptr;
	}

    this->ReDownloadCount +=1;
    this->State = EDownloadTaskState::Pending;
    auto str = *this->TaskInfo.URL;
    UE_LOG(LogGameHotUpdate, Warning, TEXT("DownloadTask ReDownload :%s,times=%d"),str,ReDownloadCount);

    this->Start();

}

bool FDownloadTask::Tick(float f)
{
    if (BreakPointReDownCD > 0 && this->BreakPointReDownCount > 0)
    {
        
        BreakPointReDownCD -= 1;

        UE_LOG(LogGameHotUpdate,Warning,TEXT("Waiting BreakPointReDown %ds,f:%f"), BreakPointReDownCD,f);

        if (BreakPointReDownCD <= 0.0f)
        {
			this->BreakPointReDownCount -= 1;
			BreakPointReDownCD -= 1;

            this->BreakPointReDown();
        }

        
       
        
    }
    return true;
}

bool FDownloadTask::BreakPointReDownWait()
{
    //if (BreakPointReDownCount > 0)
    {
       /* const TIndirectArray<FWorldContext>& cons = GEngine->GetWorldContexts();
        UWorld* World = cons[0].World();
        GWorld->GetTimerManager().SetTimer(BreakpointReDownTimer, TimerDelegate,1.0f, true);
        BreakPointReDownCount -=1;*/

        this->BreakPointReDownCD = BreakPointReDownCDTime;
        UE_LOG(LogGameHotUpdate, Warning, TEXT("DownloadTask BreakPointReDownWait CD=%d"), this->BreakPointReDownCD);
       
        return true;
    }

    return false;

}

void FDownloadTask::BreakPointReDown()
{
    UE_LOG(LogGameHotUpdate, Warning, TEXT("DownloadTask BreakPointReDown "));
    if (Request.IsValid())
    {
        Request->CancelRequest();
    }
    else
    {
        OnTaskEvent.Broadcast(EDownloadTaskEvent::TE_ERROR,TaskInfo);
        return;
    }

	const auto& EncodedURL = GetEncodedURL();
	if (EncodedURL.IsEmpty())
	{
		UE_LOG(LogGameHotUpdate, Warning, TEXT("Error URL"))

			return;
	}

	const auto BeginPosition = TaskInfo.CurrentSize;
	auto EndPosition = BeginPosition + ChunkSize - 1;
	if (EndPosition >= TaskInfo.TotalSize)
	{
		EndPosition = TaskInfo.TotalSize - 1;
	}

	const auto& RangeStr = FString("bytes=") + FString::FromInt(BeginPosition) + FString(TEXT("-")) + FString::FromInt(EndPosition);
    UE_LOG(LogGameHotUpdate, Log, TEXT("Start Down BeginPosition=%d,EndPosition=%d, TotalSize=%d, %s "),BeginPosition,EndPosition, TaskInfo.TotalSize, *GetEncodedURL());
	if (!Request.IsValid())
	{
		return ;
	}

	if (false)
	{
		Request = FDownloadTask::GetHttp(2); // Request =  FHttpModule::Get().CreateRequest(); //
	}
	else
	{
		Request->OnProcessRequestComplete().Unbind();
		Request->OnHeaderReceived().Unbind();
		Request->OnRequestProgress().Unbind();

	}
	Request->SetVerb("GET");
	Request->SetURL(EncodedURL);
	Request->AppendToHeader(FString("Range"), RangeStr);
	Request->OnProcessRequestComplete().BindRaw(this, &FDownloadTask::RetGetChunk);
	Request->OnRequestProgress().BindRaw(this, &FDownloadTask::GetChunkProgress);
	Request->ProcessRequest();
	OnTaskEvent.Broadcast(EDownloadTaskEvent::BEGIN_DOWNLOAD, TaskInfo);
}


TSharedPtr<class IHttpRequest, ESPMode::ThreadSafe> FDownloadTask::GetHttp(int chn)
{

    TArray<TSharedPtr<class IHttpRequest, ESPMode::ThreadSafe>>* list =nullptr;
    if (chn == 1)
    {
        list = &FDownloadTask::HttpList1;
    }
    else
    {

        list = &FDownloadTask::HttpList2;
    }
    

    if (list->Num() > 0 )
    {
        TSharedPtr<class IHttpRequest, ESPMode::ThreadSafe> http = (*list)[0];
        list->RemoveAt(0);
        UE_LOG(LogGameHotUpdate, Log, TEXT("Get http in cache list %d"), chn);
        return http;
    }
    else
    {
        auto http = FHttpModule::Get().CreateRequest();
        UE_LOG(LogGameHotUpdate, Log, TEXT("Create a new http %d"),  chn);
        return http;
    }
}

void FDownloadTask::CacheHttp(TSharedPtr<class IHttpRequest, ESPMode::ThreadSafe> http,int chn)
{
    if (chn == 1)
    {
        
        FDownloadTask::HttpList1.Add(http);
    }
    else
    {
        FDownloadTask::HttpList2.Add(http);
    }
}


void FDownloadTask::OnRequestHeadHeaderReceived(FHttpRequestPtr RequestPtr, const FString& InHeaderName, const FString& InNewHeaderValue)
{

	UE_LOG(LogGameHotUpdate, Log, TEXT("OnRequestHeadHeaderReceived::Header Name:%s\tHeaderValue:%s"), *InHeaderName, *InNewHeaderValue);

	if (InHeaderName.Equals(TEXT("Content-Length")))
	{
        TaskInfo.TotalSize =  FCString::Atoi64(*InNewHeaderValue);//UKismetStringLibrary::Conv_StringToInt(InNewHeaderValue);
	}
}

void FDownloadTask::RetGetHead(FHttpRequestPtr, const FHttpResponsePtr Response, const bool bConnectedSuccessfully)
{
    if (!Response.IsValid() || !bConnectedSuccessfully)
    {
        UE_LOG(LogGameHotUpdate, Warning, TEXT("RetGetHead Response error"));
        this->State = EDownloadTaskState::DownFailed;
        OnTaskEvent.Broadcast(EDownloadTaskEvent::TE_ERROR, TaskInfo);

        return;
    }

    const auto ResponseCode = Response->GetResponseCode();
    
    if (ResponseCode >= 400 || ResponseCode < 200)
    {
        UE_LOG(LogGameHotUpdate, Warning, TEXT("Http Response code error : %d"), ResponseCode);

        if (TempFileHandle != nullptr)
        {
            delete TempFileHandle;

            TempFileHandle = nullptr;
        }

        this->State = EDownloadTaskState::DownFailed;
        OnTaskEvent.Broadcast(EDownloadTaskEvent::TE_ERROR, TaskInfo);

        return;
    }

    OnTaskEvent.Broadcast(EDownloadTaskEvent::RET_HEAD, TaskInfo);

    TaskInfo.CurrentSize = 0;
    
    TempFileName = GetFilePath() + TempFileExtension;

    const auto& SavePath = FPaths::GetPath(TempFileName);

   

    FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*SavePath);

    if (!IFileManager::Get().DirectoryExists(*SavePath))
    {
        UE_LOG(LogGameHotUpdate, Error, TEXT("%s, create temp directory error"), *SavePath);
        OnTaskEvent.Broadcast(EDownloadTaskEvent::TE_ERROR, TaskInfo);
        return;
    }

    auto& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

    /*delete Exist File*/
    if (PlatformFile.FileExists(*TempFileName))
    {
        PlatformFile.DeleteFile(*TempFileName);
    }

    TempFileHandle = PlatformFile.OpenWrite(*TempFileName, true);

    if (TempFileHandle == nullptr)
    {
        UE_LOG(LogGameHotUpdate, Warning, TEXT("%s, create temp file error"), *TempFileName);

        OnTaskEvent.Broadcast(EDownloadTaskEvent::TE_ERROR, TaskInfo);

        return;
    }

    UE_LOG(LogGameHotUpdate, Log, TEXT("Create temp file success! Start downloading: %s"), *TempFileName);

    this->CacheHttp(Request,1);
    Request = nullptr;

    ReqGetChunk();
}

void FDownloadTask::ReqGetChunk()
{
    const auto& EncodedURL = GetEncodedURL();

    if (EncodedURL.IsEmpty())
    {
        UE_LOG(LogGameHotUpdate, Warning, TEXT("Error URL"))

        return;
    }

    const auto BeginPosition = TaskInfo.CurrentSize;

    auto EndPosition = BeginPosition + ChunkSize - 1;

    if (EndPosition >= TaskInfo.TotalSize)
    {
        EndPosition = TaskInfo.TotalSize - 1;
    }

    const auto& RangeStr = FString("bytes=") + FString::FromInt(BeginPosition) +
        FString(TEXT("-")) + FString::FromInt(EndPosition);


    if(true)
    {
        Request = FDownloadTask::GetHttp(2); // Request =  FHttpModule::Get().CreateRequest(); //
    }
    else
    {
        Request->OnProcessRequestComplete().Unbind();
        Request->OnHeaderReceived().Unbind();
        Request->OnRequestProgress().Unbind();
    }
    

    Request->SetVerb("GET");

    Request->SetURL(EncodedURL);

    Request->AppendToHeader(FString("Range"), RangeStr);

    Request->OnProcessRequestComplete().BindRaw(this, &FDownloadTask::RetGetChunk);

    Request->OnRequestProgress().BindRaw(this, &FDownloadTask::GetChunkProgress);

    Request->ProcessRequest();
    Request->SetTimeout(3);

    OnTaskEvent.Broadcast(EDownloadTaskEvent::BEGIN_DOWNLOAD, TaskInfo);
}

FString FDownloadTask::GetFilePath() const
{
    return FPaths::Combine(Root, TaskInfo.FileName);
}



void FDownloadTask::RetGetChunk(FHttpRequestPtr, const FHttpResponsePtr Response, const bool bConnectedSuccessfully)
{
    if (!Response.IsValid() || !bConnectedSuccessfully)
    {
        UE_LOG(LogGameHotUpdate, Warning, TEXT("RetGetChunk Response error"));

        if (!BreakPointReDownWait())
        {
            OnTaskEvent.Broadcast(EDownloadTaskEvent::TE_ERROR, TaskInfo);
        }
        return;
    }

    const auto ResponseCode = Response->GetResponseCode();

    if (ResponseCode >= 400 || ResponseCode < 200)
    {
        UE_LOG(LogGameHotUpdate, Warning, TEXT("%d, ResponseCode code error"), ResponseCode);

        if (TempFileHandle != nullptr)
        {
            delete TempFileHandle;
            TempFileHandle = nullptr;
        }

        OnTaskEvent.Broadcast(EDownloadTaskEvent::TE_ERROR, TaskInfo);

        return;
    }

    const auto& Buffer = Response->GetContent();

    if (TempFileHandle != nullptr)
    {
        TempFileHandle->Seek(TaskInfo.CurrentSize);
        auto number = Buffer.Num();

        if (TempFileHandle->Write(Buffer.GetData(), number))
        {
            this->BreakPointReDownCount = 5;
            
			UE_LOG(LogGameHotUpdate, Log, TEXT("Write data Len %d:%lld,file:%s"), number,TaskInfo.FileSize,*GetEncodedURL());


            TempFileHandle->Flush();
            OnWriteChunkEnd(Buffer.Num());
        }
        else
        {
            UE_LOG(LogGameHotUpdate, Warning, TEXT("write file error"));

            OnTaskEvent.Broadcast(EDownloadTaskEvent::TE_ERROR, TaskInfo);
        }
    }
}

void FDownloadTask::GetChunkProgress(FHttpRequestPtr, int32, const int32 BytesReceived)
{
    int64 DownloadSize = TaskInfo.CurrentSize + BytesReceived;

    
    if (DownloadSize > TaskInfo.TotalSize)
    {
        //  �����Ϊ http head�����󷵻ص� content - length �ֶβ����ڵ��£�ԭ��http������������Gzipѹ��
        //UE_LOG(LogGameHotUpdate, Warning, TEXT("%s, DownloadSize is more than total size"), *GetFilePath());
        //return;
        
    }
    TaskInfo.DownloadSize = DownloadSize;

    OnTaskEvent.Broadcast(EDownloadTaskEvent::UPDATE_DOWNLOAD, TaskInfo);
}

void FDownloadTask::OnTaskCompleted()
{
    if (TempFileHandle != nullptr)
    {
        delete TempFileHandle;

        TempFileHandle = nullptr;
    }

    State = EDownloadTaskState::Finished;
    UE_LOG(LogGameHotUpdate, Log, TEXT("GetFilePath:%s;TempFileName:%s"), *GetFilePath(),*TempFileName);
    if (!IFileManager::Get().Move(*GetFilePath(), *TempFileName))
    {
        UE_LOG(LogGameHotUpdate, Error, TEXT("Move error:%s"), *GetFilePath());
        OnTaskEvent.Broadcast(EDownloadTaskEvent::TE_ERROR, TaskInfo);
    }

    if (IsFileExist())
    {
        TaskInfo.TotalSize = IFileManager::Get().FileSize(*GetFilePath());

        if (TaskInfo.CurrentSize != TaskInfo.TotalSize)
        {
            TaskInfo.CurrentSize = TaskInfo.TotalSize;
        }
        OnTaskEvent.Broadcast(EDownloadTaskEvent::END_DOWNLOAD, TaskInfo);
    }
}

void FDownloadTask::OnWriteChunkEnd(const int64 BufferSize)
{
    TaskInfo.CurrentSize = TaskInfo.CurrentSize + BufferSize;

    OnTaskEvent.Broadcast(EDownloadTaskEvent::UPDATE_DOWNLOAD, TaskInfo);

    if (TaskInfo.CurrentSize < TaskInfo.TotalSize)
    {
       ReqGetChunk();
    }
    else
    {
        OnTaskCompleted();
    }

	
}

FString FDownloadTask::GetEncodedURL() const
{
    
    if (!RedirectUrl.IsEmpty())
    {
        return RedirectUrl;
    }

    const auto URLSplit = TaskInfo.URL.Find(FString("/"),
                                            ESearchCase::IgnoreCase, ESearchDir::FromStart, 14) + 1;

    if (URLSplit > 0)
    {
        auto SubURL = TaskInfo.URL.Mid(URLSplit);

        TArray<FString> URLUnit;

        SubURL.ParseIntoArray(URLUnit, TEXT("/"));

        SubURL.Empty();

        for (auto i = 0; i < URLUnit.Num(); ++i)
        {
            SubURL += FGenericPlatformHttp::UrlEncode(URLUnit[i]);

            if (i < URLUnit.Num() - 1)
            {
                SubURL += "/";
            }
        }

        return TaskInfo.URL.Left(URLSplit) + SubURL;
    }

    return FString();
}

FGuid FDownloadTask::GetGuid() const
{
    return TaskInfo.GUID;
}
