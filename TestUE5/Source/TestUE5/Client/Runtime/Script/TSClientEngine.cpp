#include "TSClientEngine.h"
#include "Kismet/KismetSystemLibrary.h"


UTSClientEngine* UTSClientEngine::Instance = nullptr;


UTSClientEngine::UTSClientEngine()
{

}



UTSClientEngine::~UTSClientEngine()
{
	UE_LOG(LogTemp, Log, TEXT("UTSClientEngine::~UTSClientEngine"));
}



void UTSClientEngine::Init(TWeakObjectPtr<UGameInstance> InInstance)
{
	UTSEngine::Init(InInstance);
	
	/*UMediaPlayerBPLibrary::OnMediaPlayerStartMulDelegate.AddLambda([this]
	{
		this->NotifyToTSEvent(CLIENT_EVENT2TS::EVENT_MediaPlayerStart, "");
	});*/

}

void UTSClientEngine::OnStart()
{

	UE_LOG(LogTemp, Warning, TEXT("UTSClientEngine::OnStart"));

	UTSEngine::OnStart();

}


/**
 * ��ȡdebug���Զ˿�
 */
 int UTSClientEngine::GetDebugPort()
{
	return 8080;
}

 FString UTSClientEngine::GetTSDirectory()
 {
	return TEXT("JavaScript/TSClient");
 }

/**
 * ��ȡ��ڽű�
 */
 FString UTSClientEngine::GetTSEntrance()
{
	return  TEXT("Main");
}





void UTSClientEngine::NotifyToTSEvent(CLIENT_EVENT2TS CMD, const FString& param)
{
	/*if (this->EventToTSDelegate.IsBound())
	{
		int cmd = int(CMD);
		this->EventToTSDelegate.Broadcast(cmd, param);
	}*/
}

void UTSClientEngine::HandleDisconnect(UWorld* InWorld, UNetDriver* NetDriver)
{
	this->NetDisConnectDelegate.Broadcast();
}


void UTSClientEngine::OnNetworkError(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	/*if (NetErrorDelegate.IsBound())
	{
		NetErrorDelegate.Broadcast(FailureType, ErrorString);
	}*/

	UE_LOG(LogTemp, Log, TEXT("UTSClientEngine::OnNetworkError FailureType:%d,ErrorString:%s"), FailureType, *ErrorString);
}

void UTSClientEngine::OnProcessDSRobotOperateMsg(const FString& cmdName, const FString& cmdJson)
{
	/*if (this->OnProcessDSRobotOperateMsgDelegate.IsBound())
	{
		this->OnProcessDSRobotOperateMsgDelegate.Broadcast(cmdName,cmdJson);
	}*/
}



void UTSClientEngine::OnProcessDSRobotConfigMsg(const FString& cmdName, const FString& cmdJson)
{
	/*if (this->OnProcessDSRobotConfigMsgDelegate.IsBound())
	{
		this->OnProcessDSRobotConfigMsgDelegate.Broadcast(cmdName,cmdJson);
	}*/
}



void UTSClientEngine::OnProcessDSRobotQueryMsg(const FString& cmdName, const FString& cmdJson)
{
	/*if (this->OnProcessDSRobotQueryMsgDelegate.IsBound())
	{
		this->OnProcessDSRobotQueryMsgDelegate.Broadcast(cmdName,cmdJson);
	}*/
}


void UTSClientEngine::OnRobotMessage_test(const FString& msgId, const FString& data, int dataLength)
{
	std::string cstr(TCHAR_TO_UTF8(*data));
	int lent = cstr.length();
	//this->OnRobotMessage(msgId,cstr, lent);
}

void UTSClientEngine::SendMsg2DS(const FString& type, const FString& data) 
{
	//HarixRobotMsgDispatch::GetInstance().SendMsg2DS(type,data);
}

