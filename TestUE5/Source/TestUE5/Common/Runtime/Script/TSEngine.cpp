#include "TSEngine.h"
#include "Kismet/KismetSystemLibrary.h"


UTSEngine::~UTSEngine()
{
	UE_LOG(LogTemp, Log, TEXT("UTSServerEngine::~UTSServerEngine"));
}

UTSEngine::UTSEngine()
{
	GameInstance = nullptr;
	this->GameScript = nullptr;
}

void UTSEngine::Init(TWeakObjectPtr<UGameInstance> InInstance)
{
	UE_LOG(LogTemp, Log, TEXT("UTSEngine::Init"));
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UTSEngine::LoadMapBegin);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UTSEngine::LoadMapComplete);
	FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UTSEngine::Tick));

	this->GameInstance = InInstance;
	
	//Init Engine
}

void UTSEngine::OnStart()
{

	if (GameScript == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("UTSEngine::Start"));
	
		// Purts vm
		GameScript = MakeShared<puerts::FJsEnv>(std::make_unique<puerts::DefaultJSModuleLoader>(this->GetTSDirectory()), std::make_shared<puerts::FDefaultLogger>(), this->GetDebugPort());
		// GameScript->WaitDebugger();

		TArray<TPair<FString, UObject*>> arguments;
		arguments.Add(TPair<FString, UObject*>(TEXT("TSEngine"), this));
		GameScript->Start(this->GetTSEntrance(), arguments);
	
	
	}
	
}

/**
 * 获取debug调试端口
 */
int UTSEngine::GetDebugPort()
{
	return 8080;
}

/**
 * 获取TS脚本所在目录 Game/下面的
 */
FString UTSEngine::GetTSDirectory()
{
	return TEXT("JavaScript");
}

/**
 * 获取入口脚本
 */
FString UTSEngine::GetTSEntrance()
{
	return TEXT("Main.ts");
}


bool UTSEngine::Tick(float DeltaTime)
{
	// UE_LOG(LogTemp, Log, TEXT("UTSServerEngine::Tick DeltaTime:%f"),DeltaTime);
	/*if (GameTickDelegate.IsBound())
	{
		GameTickDelegate.Broadcast(DeltaTime);
	}*/
	return true;
}

void UTSEngine::Shutdown()
{
	/*if (GameShutDownDelegate.IsBound())
	{
		GameShutDownDelegate.Broadcast();
	}*/

	////Purts vm
	GameScript.Reset();

	UE_LOG(LogTemp, Log, TEXT("UTSEngine::Shutdown"))

}

void UTSEngine::LoadMapBegin(const FString& MapName)
{
	UE_LOG(LogTemp, Log, TEXT("UTSEngine::LoadMapBegin MapName:%s"), *MapName);
	//MapLoadBeginDelegate.ExecuteIfBound(MapName);
}

void UTSEngine::LoadMapComplete(UWorld* LoadedWorld)
{
	// this cause crash already. so ..
	if (LoadedWorld)
	{
		UE_LOG(LogTemp, Log, TEXT("UTSEngine::LoadMapComplete WorldName:%s"), *LoadedWorld->GetMapName());
	}
	//MapLoadCompleteDelegate.ExecuteIfBound(LoadedWorld);
}



void UTSEngine::GameInstanceLoadComplete(const float LoadTime, const FString& MapName)
{
	UE_LOG(LogTemp, Log, TEXT("UTSEngine::GameInstanceLoadComplete loadtime:%f,mapName:%s"), LoadTime, *MapName);

	//GameInstanceLoadCompleteDelegate.ExecuteIfBound(LoadTime, MapName);
}



void UTSEngine::SetDebugWaiting()
{
	GameScript->WaitDebugger();
}

TWeakObjectPtr<UGameInstance> UTSEngine::GetGameInstance()
{
	return this->GameInstance;
}

UInputBind* UTSEngine::CreateInputBindObject(bool isRetain)
{
	UInputBind* ib = NewObject<UInputBind>(GetTransientPackage());
	if (isRetain)
	{
		this->InputBindArr.Add(ib);
	}
	return ib;
}



void UTSEngine::MessageNotify(const FString& A, const FString& B)
{
	UE_LOG(LogTemp, Log, TEXT("UTSEngine:MessageNotify"));
	/*if (this->MessageNotifyDelegate.IsBound())
	{
		this->MessageNotifyDelegate.Broadcast(A, B);
	}*/
}

void UTSEngine::OnActorBeginPlay(AActor* actor)
{
	//this->OnActorBeginPlayToTSDeleagte.Broadcast(actor);
}


void UTSEngine::OnActorEndPlay(AActor* actor)
{
	//this->OnActorEndPlayToTSDeleagte.Broadcast(actor);
}
