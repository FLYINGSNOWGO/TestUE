
/*
*游戏tick接口，用于TS中tick接入
*/


#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InputBind.h"
#include "JsEnv.h"
#include "ArrayBuffer.h"
//#include "TSDelegateDeclare.h"
#include "TSEngine.generated.h"

class UGameInstance;

UCLASS()
class TESTUE5_API UTSEngine : public UObject
{
	GENERATED_BODY()

	
public:
	UTSEngine();

	TSharedPtr<puerts::FJsEnv> GameScript;
	TWeakObjectPtr<UGameInstance> GameInstance;
public:
	
	virtual ~UTSEngine();

	virtual void Init(TWeakObjectPtr<UGameInstance> InInstance);
	

	virtual void OnStart();

	/**
	 * 获取debug调试端口
	 */
	virtual int GetDebugPort();
	
	/**
	 * 获取TS脚本所在目录 Game/下面的
	 */
	virtual FString GetTSDirectory();

	/**
	 * 获取入口脚本
	 */
	virtual FString GetTSEntrance();


	//virtual void GameTick(float DeltaSeconds);
	//virtual void EngineTick(float DeltaSeconds, bool bIdleMode);
	virtual bool Tick(float DeltaTime);



	//GameInstance Shutdown调用来的，比GameExit调用在前,如果有JS相关的delegate调用，应放这里面处理，因为shutdown之后会清理js环境
	virtual void Shutdown();

	//FCoreUObjectDelegates::PostLoadMapWithWorld 调用
	virtual void LoadMapComplete(UWorld* LoadedWorld);

	//FCoreUObjectDelegates::PreLoadMap 调用
	virtual void LoadMapBegin(const FString& MapName);


	//由GameInstance调用的
	virtual void GameInstanceLoadComplete(const float LoadTime, const FString& MapName);


	//消息通知
	UFUNCTION()
	virtual void MessageNotify(const FString&A,const FString& B);

	//设置TS脚本Debug等待，VScode中按F5开启断点调式之后才继续运行
	UFUNCTION()
	virtual void SetDebugWaiting();

	/**
	 * 目前再BaseGameCharacter中会调用，后面需要BaseActor类可能也需要，方便通知TS Actor的生成
	 */
	UFUNCTION()
	void OnActorBeginPlay(AActor* actor);

	/**
	* 目前再BaseGameCharacter中会调用，后面需要BaseActor类可能也需要，方便通知TS Actor的销毁
	*/
	UFUNCTION()
	void OnActorEndPlay(AActor* actor);

		
	UFUNCTION() 
	virtual  TWeakObjectPtr<UGameInstance> GetGameInstance();

	//一启动游戏，调用Start()之后就会一直tick 
	//UPROPERTY()
 //   FNotifyWithFloat EngineTickDelegate; 
	////跟关卡level tick同步，加载完成地图才会tick
	//UPROPERTY()
	//FNotifyWithFloat GameTickDelegate;
	////退出游戏委托
	//UPROPERTY()
	//FNotifyWithNothing GameShutDownDelegate;
	
	//输入时间数组 测试
	UPROPERTY()
	TArray <UInputBind*> InputBindArr;


	//UPROPERTY();
	//FLevelLoadCompletedDelgate LevelStreamLoadCompleteDelegate;
	//UPROPERTY();
	//FLevelLoadCompletedDelgate LevelLoadCompleteDelegate;

	/*UPROPERTY();
	FLevelLoadBeginDelgate MapLoadBeginDelegate;
	UPROPERTY();
	FLevelLoadEndDelgate MapLoadCompleteDelegate;*/

	/*UPROPERTY();
	FGameInstanceLoadCompleteDelegate GameInstanceLoadCompleteDelegate;*/

	//UPROPERTY();
	//FNotifyToTSDeleagte EventNotifyDelegate;
	
	/*UPROPERTY();
	FNotifyToTSDeleagte MessageNotifyDelegate;

	UPROPERTY();
	FEventToTSDeleagte EventToTSDelegate;*/

	//临时使用，使用后会清空
	UPROPERTY();
	TArray<AActor*> TempArr;

	//临时
	UPROPERTY();
	TArray<FVector> TempVectorArray;

	/**测试*/
	UFUNCTION()
	UInputBind* CreateInputBindObject(bool isRetain);


	/**
	 * 目前再BaseGameCharacter中会调用，后面需要BaseActor类可能也需要，方便通知TS Actor的生成
	 */
	//UPROPERTY();
	//FActorBeginEndPlayToTSDeleagte OnActorBeginPlayToTSDeleagte;

	///**
	//* 目前再BaseGameCharacter中会调用，后面需要BaseActor类可能也需要，方便通知TS Actor的生成
	//*/
	//UPROPERTY();
	//FActorBeginEndPlayToTSDeleagte OnActorEndPlayToTSDeleagte;
	//

	//UPROPERTY();
	//FOnSmartObjNewDelegate OnSmartObjNewDelegate;

	//UPROPERTY();
	//FOnSmartObjectStateDelegate OnSmartObjStateDelegate;
};