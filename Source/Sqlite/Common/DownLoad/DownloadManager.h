// ClouddMinds RobotEngine  Powered by EPIC game

#pragma once

#include "CoreMinimal.h"
#include "HttpRetrySystem.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDownloadManager, Log, All)

/**
 * 
 */
class SQLITE_API FDownloadManager : public TSharedFromThis<FDownloadManager>
{
private:
	TSharedPtr<class FHttpRetrySystem::FManager> HttpRetryManager;
	/** Delegate for callbacks to Tick */
	FTickerDelegate TickDelegate;
	/** Handle to various registered delegates */
	FDelegateHandle TickDelegateHandle;
	static TSharedPtr<FDownloadManager> PakDownloader;
	TArray<TSharedRef<IHttpRequest, ESPMode::ThreadSafe>> RequestList;
private:
	/**
	 * RetryLimitCount:Number of retries
	 * RetryTimeout = 0.f,Unlimited timeout
	 */
	FDownloadManager(const uint32 RetryLimitCount, const double RetryTimeout);
	bool Tick(float DeltaSeconds);
	void CancelAllRemainRequest();
public:
	~FDownloadManager();
	static TSharedRef<FDownloadManager> Get();
	enum class EDownloadErrorType : uint8
	{
		None,
		DownLoadManagerInvalid,
		ConnectionError,
		RequestError,
		ServiceError,
		SaveFileFail,
		VerifyMD5Fail,
		SaveFilePathInvalid
	};
	static const TCHAR* DownloadErrorTypeToString(const EDownloadErrorType ErrorType);

	DECLARE_DELEGATE_OneParam(FDownloadFileCompleteDelegate, const FString& /*FileFullPath*/);
	DECLARE_DELEGATE_OneParam(FDownloadFileWillRetryDelegate, const FString& /*RetryFileName*/);
	DECLARE_DELEGATE_ThreeParams(FDownloadProgressDelegate, const FString& /*FileName*/, const int32 /*BytesReceived*/, const int32 /*FileSize*/);
	DECLARE_DELEGATE_TwoParams(FDownloadFileErrorDelegate, const FString& /*FileName*/, const EDownloadErrorType /*ErrorType*/);
	struct FDownloadFileEntry
	{
		FString FileName;
		// final size of the file in bytes
		uint64 FileSize = 0;
		FString RelativeUrl;
		FString FileFullPath;
		FString RemoteMapMd5;
		FDownloadFileCompleteDelegate DownloadFileCompleteDelegate;
		FDownloadFileWillRetryDelegate DownloadFileWillRetryDelegate;
		FDownloadProgressDelegate DownloadProgressDelegate;
		FDownloadFileErrorDelegate DownloadFileErrorDelegate;
		FDownloadFileEntry();
		FDownloadFileEntry(const FDownloadFileEntry& Entry);
		FDownloadFileEntry& operator=(const FDownloadFileEntry& Entry);
		~FDownloadFileEntry();
	};

	/*
	 * Download Pak file from FileServer
	 */
	void DownLoad(const FDownloadFileEntry& DownloadFileEntry);
};
