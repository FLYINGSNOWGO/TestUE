
/*
*游戏tick接口，用于TS中tick接入
*/


#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JsEnv.h"
#include "ArrayBuffer.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Common/Runtime/Script/InputBind.h"
//#include "Common/Runtime/Script/TSDelegateDeclare.h"
#include "Common/Runtime/Script/TSEngine.h"
#include "TSServerEngine.generated.h"


//零时定义一些Flag 
enum EWriteType
{
	CLEAR,
	WRITE,
};
template<typename Type>
class FlagOp
{
public:
	FORCEINLINE FlagOp<Type>() { FMemory::Memset(this, 0, sizeof(FlagOp<Type>)); }
	FORCEINLINE FlagOp<Type>(const Type& aFlags) { Flags = aFlags; }
	FORCEINLINE void Reset() { FMemory::Memset(this, 0, sizeof(FlagOp<Type>)); }
	template<EWriteType WriteType = EWriteType::WRITE> FORCEINLINE bool WriteFlag(Type inType) { if (WriteType == EWriteType::WRITE) { Flags = Flags | inType; }if (WriteType == EWriteType::CLEAR) { Flags = Flags & (~inType); }return true; }
	FORCEINLINE bool IsType(Type inType)const { return  Flags & inType ? true : false;}
	FORCEINLINE bool IsTypeOnly(Type inType)const { return  Flags & ~inType ? false : true;}
	FORCEINLINE Type Get()const { return Flags; }
	FORCEINLINE bool HasAnyFlag() { return  0 != Flags; }
private:
	Type Flags;
};

//Some flag define begin
typedef FlagOp<uint8> FlagOp_uint8;
typedef FlagOp<uint16> FlagOp_uint16;
typedef FlagOp<uint32> FlagOp_uint32;
typedef FlagOp<uint64> FlagOp_uint64;
//Some flag define end




UENUM(BlueprintType)
enum class SERVER_EVENT2TS : uint8
{
	//客户端连接DS成功初始化角色事件
	//EVENT_InitLocalCharacter,
	//UMediaPlayerBPLibrary start
	EVENT_MediaPlayerStart,
	//UMediaPlayerBPLibrary 
	EVENT_MediaPlayerStop,
	//EVENT_ClientGameAINotify,

	//Start Server GameAI [6/14/2022 admin]
	EVENT_InitServerCharacter,  //for Server Character
	EVENT_ServerGameAINotify,

	//End Server GameAI [6/14/2022 admin]
	//端资源更新完成
	EVENT_UPDATE_COMPLETE,
};

class UBTTaskNode;

//行为树传递到TS的通知 --end
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FBTTaskNodeDelegate, const FString&, RobotParam , UBTTaskNode*, Node, UBTTaskNode*, InstNode);
DECLARE_DYNAMIC_DELEGATE_FourParams(FBTTaskFinishDelegate, const FString&, RobotParam, UBTTaskNode*, Node, UBTTaskNode*, InstNode, EBTNodeResult::Type,Ret);
//行为树传递到TS的通知 --end

//角色身上
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnSmartObjInteractive,ACharacter*, Node, AActor*, SmartObj, TArray<FString>, ProposeCmd);
//脚本挂在物体资源上面


UCLASS()
class TESTUE5_API UTSServerEngine : public UTSEngine
{
	GENERATED_BODY()
public:
	UTSServerEngine();
		
	UFUNCTION(BlueprintCallable, Category = "SmartObj", meta = (BlueprintInternalUseOnly = "true"))
	static void SmartObjInteractive(ACharacter* Node, AActor*  SmartObj, TArray<FString> ProposeCmd);

public:
	UFUNCTION()
	static UTSServerEngine* GetTSInstance()
	{
		if (!Instance)
		{
			Instance = NewObject<UTSServerEngine>();
			Instance->AddToRoot();
		}
		return Instance;
	};
private:
	static UTSServerEngine* Instance ;

public:
	
	virtual ~UTSServerEngine();
	virtual void Init(TWeakObjectPtr<UGameInstance> InInstance);
	virtual void OnStart();
	virtual FString GetTSDirectory();
	/**
	 * 获取debug调试端口
	 */
	virtual int GetDebugPort();
	/**
	 * 获取入口脚本
	 */
	virtual FString GetTSEntrance();

	//事件通知
	UFUNCTION()
	virtual void NotifyToTSEvent(SERVER_EVENT2TS eventCmd, const FString& B);
	
	// Delegate to Ts ....
	UPROPERTY()
	FBTTaskNodeDelegate WrappedExecuteTaskDelegate;
	
	UPROPERTY()
	FBTTaskNodeDelegate WrappedAbortTaskDelegate;
    
	UPROPERTY()
	FBTTaskFinishDelegate WrappedTaskFinishDelegate;
};