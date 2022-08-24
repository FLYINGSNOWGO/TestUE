#pragma once
#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "TaskInfo.h"
#include "FileDownType.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGameHotUpdate, Log, All);

enum class RESOURCESDOWNLOADER_API EDownloadTaskState : uint8
{
    Pending,
    DownLoading,
    Finished,
    DownFailed,
};


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTaskEvent,  EDownloadTaskEvent,  FTaskInfo&);

class RESOURCESDOWNLOADER_API FDownloadTask final
{
public:
    FDownloadTask(const FString& URL, const FString& SaveRoot, const FString& FileName,
                  const int64 FileSize);
    virtual ~FDownloadTask();
    static const TCHAR* DownloadTaskEventToString(const EDownloadTaskEvent TaskEvent);
    void Start();

    void Stop();

    bool IsPending() const
    {
        return State == EDownloadTaskState::Pending;
    }

    bool IsFinished() const
    {
        return State == EDownloadTaskState::Finished;
    }

    bool IsDowning() const
    {
        return State == EDownloadTaskState::DownLoading;
    }

    FString GetFilePath() const;

    FGuid GetGuid() const;

    FTaskInfo GetTaskInfo() const;

    FOnTaskEvent OnTaskEvent;

    static FString TempFileExtension;

    int ReDownloadCount = 0;
    void ReDownload();



    int BreakPointReDownCount = 5;
    bool BreakPointReDownWait();
    bool Tick(float f);
    int BreakPointReDownCD = 0;
    FTimerDelegate TimerDelegate;

    FTickerDelegate TickerDelegate;
    FTSTicker::FDelegateHandle TickDelegateHandle;
    void BreakPointReDown();

	

	static TArray< TSharedPtr<class IHttpRequest, ESPMode::ThreadSafe> > HttpList1;
	static TArray< TSharedPtr<class IHttpRequest, ESPMode::ThreadSafe> > HttpList2;

    void ReqGetChunk();
protected:
    void ReqGetHead();

    void RetGetHead(FHttpRequestPtr, const FHttpResponsePtr Response, const bool bConnectedSuccessfully);
    void OnRequestHeadHeaderReceived(FHttpRequestPtr RequestPtr, const FString& InHeaderName, const FString& InNewHeaderValue);



    void GetChunkProgress(FHttpRequestPtr, int32 BytesSent, int32 BytesReceived);

    void RetGetChunk(FHttpRequestPtr, const FHttpResponsePtr Response, const bool bConnectedSuccessfully);

    void OnWriteChunkEnd(int64 BufferSize);

    void OnTaskCompleted();

    FString GetEncodedURL() const;

    bool IsFileExist() const;



private:
    FTaskInfo TaskInfo;

    FString Root;

    EDownloadTaskState State;

    const int64 ChunkSize = 4 * 1024 * 1024;

    FString TempFileName;

    IFileHandle* TempFileHandle;

    TSharedPtr<class IHttpRequest, ESPMode::ThreadSafe> Request;

    FString RedirectUrl = "";

    FTimerHandle BreakpointReDownTimer;

    static TSharedPtr<class IHttpRequest, ESPMode::ThreadSafe> GetHttp(int chn);
    static void  CacheHttp(TSharedPtr<class IHttpRequest, ESPMode::ThreadSafe> http, int chn);
};
