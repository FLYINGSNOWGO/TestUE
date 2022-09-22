#include "TSServerEngine.h"
// #include "MediaPlayerBPLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#define DebugEngineTS  0


UTSServerEngine* UTSServerEngine::Instance = nullptr;

UTSServerEngine::UTSServerEngine()
{

}


UTSServerEngine::~UTSServerEngine()
{
#if DebugEngineTS
	UE_LOG(LogTemp, Log, TEXT("UTSServerEngine::~UTSServerEngine"));
#endif
}



void UTSServerEngine::SmartObjInteractive(ACharacter* Node, AActor* SmartObj, TArray<FString> ProposeCmd)
{

}


void UTSServerEngine::Init(TWeakObjectPtr<UGameInstance> InInstance)
{
	UTSEngine::Init(InInstance);

	// UMediaPlayerBPLibrary::OnMediaPlayerStartMulDelegate.AddLambda([this]
	// {
	// 	this->NotifyToTSEvent(SERVER_EVENT2TS::EVENT_MediaPlayerStart, "");
	// });
}


void UTSServerEngine::OnStart()
{
	UE_LOG(LogTemp,Warning,TEXT("UTSServerEngine::OnStart"));
	UTSEngine::OnStart();
}



/**
 * 获取debug调试端口
 */
int UTSServerEngine::GetDebugPort()
{
	return 8081;
}


FString UTSServerEngine::GetTSDirectory()
{
	return TEXT("JavaScript/TSServer");
}


/**
 * 获取入口脚本
 */
FString UTSServerEngine::GetTSEntrance()
{
	return  TEXT("Main");
}


void UTSServerEngine::NotifyToTSEvent(SERVER_EVENT2TS CMD, const FString& param)
{
	/*if (this->EventToTSDelegate.IsBound())
	{
		int cmd = int(CMD);
		this->EventToTSDelegate.Broadcast(cmd, param);
	}*/
}

