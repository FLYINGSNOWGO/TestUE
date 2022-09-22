
/*
*游戏tick接口，用于TS中tick接入
*/


#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JsEnv.h"
#include "ArrayBuffer.h"
#include "Common/Runtime/Script/InputBind.h"
#include "Common/Runtime/Script/TSDelegateDeclare.h"
#include "Common/Runtime/Script/TSEngine.h"
#include "TSClientEngine.generated.h"




UENUM(BlueprintType)
enum class CLIENT_EVENT2TS : uint8
{
	//客户端连接DS成功初始化角色事件
	EVENT_InitLocalCharacter,
	//UMediaPlayerBPLibrary start
	EVENT_MediaPlayerStart,
	//UMediaPlayerBPLibrary 
	EVENT_MediaPlayerStop,

	EVENT_ClientGameAINotify,
	//BaseGameCharacter BeginPlay
	EVENT_BaseGameCharacterBeginPlay,
	// BaseGameCharacter EndPlay
	EVENT_BaseGameCharacterEndPlay,
	//端资源更新完成
	EVENT_UPDATE_COMPLETE,
};



class ARobotEngineCharacter;

UCLASS()
class TESTUE5_API UTSClientEngine : public UTSEngine
{
	GENERATED_BODY()

	UTSClientEngine();

public:
	UFUNCTION()
	static UTSClientEngine* GetTSInstance()
	{
		if (!Instance)
		{
			Instance = NewObject<UTSClientEngine>();
			Instance->AddToRoot();
		}
		return Instance;
	};
private:
	static UTSClientEngine* Instance ;
private:

public:
	
	virtual ~UTSClientEngine();

	virtual void Init( TWeakObjectPtr<UGameInstance> InInstance);


	virtual FString GetTSDirectory();

	/**
	 * 获取debug调试端口
	 */
	virtual int GetDebugPort();

	/**
	 * 获取入口脚本
	 */
	virtual FString GetTSEntrance() ;

	virtual void OnStart();

	/**
	 * 事件通知
	 */
	UFUNCTION()
	void NotifyToTSEvent(CLIENT_EVENT2TS CMD, const FString& param);

	/**
	 * 收到网路断开
	 */
	virtual void HandleDisconnect(UWorld* InWorld, UNetDriver* NetDriver);
	/**
	 * 收到网络错误
	 */
	virtual void OnNetworkError(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	/*用于在TS中测试发送数据转换成流数据*/
	UFUNCTION()
	void OnRobotMessage_test(const FString& msgId, const FString& data, int dataLength);


	UFUNCTION()
	void OnProcessDSRobotOperateMsg(const FString& cmdName, const FString& cmdJson);


	UFUNCTION()
	void OnProcessDSRobotConfigMsg(const FString& cmdName, const FString& cmdJson);


	UFUNCTION()
	void OnProcessDSRobotQueryMsg(const FString& cmdName, const FString& cmdJson);

	UFUNCTION()
	void SendMsg2DS(const FString& type, const FString& data);



	//网络错误
	/*UPROPERTY()
		FNotifyWithNetErr NetErrorDelegate;*/

	////网络断开委托
	UPROPERTY()
		FNotifyWithNothing NetDisConnectDelegate;

	//UPROPERTY()
	//FOnRobotMessageDeleagte OnRobotMessageDeleagte;
	//UPROPERTY()
	//FNotifyWithNothing OnRobotConnectedDelegate;
	//UPROPERTY()
	//FNotifyWithNothing OnRobotDisConnectedDeletage;
	//UPROPERTY()
	//FOnRobotInfoDeleagte OnRobotInfoDeleagte;


	//UPROPERTY()
	//FNotifyToTSDeleagte OnProcessDSRobotOperateMsgDelegate;

	//UPROPERTY()
	//FNotifyToTSDeleagte OnProcessDSRobotConfigMsgDelegate;

	//UPROPERTY()
	//FNotifyToTSDeleagte OnProcessDSRobotQueryMsgDelegate;


	//UPROPERTY()
	//FNotifyToTSDeleagte OnSendRocCmd2RobotDelegate;

	//
	//UPROPERTY()
	//FNotifyToTSDeleagte OnSendSwitchCmd2RobotDelegate;
};