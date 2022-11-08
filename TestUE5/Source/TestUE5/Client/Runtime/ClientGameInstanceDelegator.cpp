// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientGameInstanceDelegator.h"
#include "Common/NCLogChannels.h"
#include "Kismet/GameplayStatics.h"
#include "Client/Runtime/Script/TSClientEngine.h"
#include "GameDelegates.h"

#if PLATFORM_ANDROID
#include "AndroidPermissionFunctionLibrary.h"
#include "AndroidPermissionCallbackProxy.h"
#endif

void UClientGameInstanceDelegator::OnNetworkError(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	UE_LOG(LogClientNC, Log, TEXT("Enter UClientGameInstanceDelegator::OnNetworkError[%s], this:%p"), *GetClientServerContextString(this), this);
	UE_LOG(LogClientNC, Log, TEXT("UREGameInstance::OnNetworkError switch case to 'ENetworkFailure::%s'. ErrorString is %s"), ENetworkFailure::ToString(FailureType), *ErrorString);
	UTSClientEngine::GetTSInstance()->OnNetworkError(World, NetDriver, FailureType, ErrorString);
	UE_LOG(LogClientNC, Log, TEXT("Exit UClientGameInstanceDelegator::OnNetworkError[%s], this:%p"), *GetClientServerContextString(this), this);
}

void UClientGameInstanceDelegator::HandleDisconnect(UWorld* InWorld, UNetDriver* NetDriver)
{
	UE_LOG(LogClientNC, Log, TEXT("Enter UClientGameInstanceDelegator::HandleDisconnect[%s], this:%p"), *GetClientServerContextString(this), this);
	UTSClientEngine::GetTSInstance()->HandleDisconnect(InWorld, NetDriver);
	UE_LOG(LogClientNC, Log, TEXT("Exit UClientGameInstanceDelegator::HandleDisconnect[%s], this:%p"), *GetClientServerContextString(this), this);
}

UClientGameInstanceDelegator::UClientGameInstanceDelegator(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/) : Super(ObjectInitializer)
{
	UE_LOG(LogClientNC, Log, TEXT("Enter UClientGameInstanceDelegator::UClientGameInstanceDelegator[%s], this:%p"), *GetClientServerContextString(this),this);
	UE_LOG(LogClientNC, Log, TEXT("Exit UClientGameInstanceDelegator::UClientGameInstanceDelegator[%s], this:%p"), *GetClientServerContextString(this),this);
}

void UClientGameInstanceDelegator::TestGameInstanceDelegator()
{
	UE_LOG(LogClientNC,Log,TEXT("Enter UClientGameInstanceDelegator::TestGameInstanceDelegator[%s], this:%p"),*GetClientServerContextString(this),this);
	UE_LOG(LogClientNC,Log,TEXT("Exit UClientGameInstanceDelegator::TestGameInstanceDelegator[%s], this:%p"),*GetClientServerContextString(this),this);
}

void UClientGameInstanceDelegator::Initialize(UGameInstance* InGameInstance)
{
	UE_LOG(LogClientNC, Log, TEXT("Enter UClientGameInstanceDelegator::Initialize[%s], this:%p"), *GetClientServerContextString(this), this);
	Super::Initialize(InGameInstance);
	GameInstance->GetEngine()->OnNetworkFailure().AddUObject(this, &UClientGameInstanceDelegator::OnNetworkError);
	FGameDelegates::Get().GetHandleDisconnectDelegate().AddUObject(this, &UClientGameInstanceDelegator::HandleDisconnect);
	UTSClientEngine::GetTSInstance()->Init(GameInstance);
	UE_LOG(LogClientNC, Log, TEXT("Exit UClientGameInstanceDelegator::Initialize[%s], this:%p"), *GetClientServerContextString(this), this);
}

void UClientGameInstanceDelegator::OnStart()
{
	UE_LOG(LogClientNC, Log, TEXT("Enter UClientGameInstanceDelegator::OnStart[%s], this:%p"), *GetClientServerContextString(this), this);
	// 开始权限动态申请 [10/20/2022 CarlZhou]
#if PLATFORM_ANDROID
	TArray<FString> AndroidPermissions{
		"android.permission.REQUEST_IGNORE_BATTERY_OPTIMIZATIONS",
		//"android.permission.START_FOREGROUND_SERVICES_FROM_BACKGROUND"
	};
	UAndroidPermissionCallbackProxy* AndroidPermissionCallbackProxy = UAndroidPermissionFunctionLibrary::AcquirePermissions(AndroidPermissions);
	AndroidPermissionCallbackProxy->OnPermissionsGrantedDelegate.AddUObject(this, &UClientGameInstanceDelegator::DoPermissionsGranted);
#endif
	//////////////////////////////////////////////////////////////////////////

	UTSClientEngine::GetTSInstance()->OnStart();
	UE_LOG(LogClientNC, Log, TEXT("Exit UClientGameInstanceDelegator::OnStart[%s], this:%p"), *GetClientServerContextString(this), this);
}

#if PLATFORM_ANDROID
void UClientGameInstanceDelegator::DoPermissionsGranted(const TArray<FString>& Permissions, const TArray<bool>& GrantResults)
{
	auto BoolToString = [](bool Value)
	{
		return Value ? TEXT("True") : TEXT("False");
	};
	bool bPermissionsSuccess = true;
	for(int Index = 0;Index < Permissions.Num();Index++)
	{
		UE_LOG(LogClientNC,Log,TEXT("%s Grant Result: %s"),*Permissions[Index],BoolToString(GrantResults[Index]));
		if(!bPermissionsSuccess)
			continue;
		if(!GrantResults[Index])
		{
			bPermissionsSuccess = false;
		}
	}
	UE_LOG(LogClientNC,Log,TEXT("Permissions Success: %s"),BoolToString(bPermissionsSuccess));
	if(!bPermissionsSuccess)
	{
		AsyncTask(ENamedThreads::GameThread, [=]() {
			FPlatformMisc::RequestExit(true);
		});
	}
}
#endif

void UClientGameInstanceDelegator::Shutdown()
{
	UE_LOG(LogClientNC, Log, TEXT("Enter UClientGameInstanceDelegator::Shutdown[%s], this:%p"), *GetClientServerContextString(this), this);
	UTSClientEngine::GetTSInstance()->Shutdown();
	UE_LOG(LogClientNC, Log, TEXT("Exit UClientGameInstanceDelegator::Shutdown[%s], this:%p"), *GetClientServerContextString(this), this);
}

void UClientGameInstanceDelegator::LoadComplete(const float LoadTime, const FString& MapName)
{
	UE_LOG(LogClientNC, Log, TEXT("Enter UClientGameInstanceDelegator::LoadComplete[%s], this:%p"), *GetClientServerContextString(this), this);
	UTSClientEngine::GetTSInstance()->GameInstanceLoadComplete(LoadTime, MapName);;
	UE_LOG(LogClientNC, Log, TEXT("Exit UClientGameInstanceDelegator::LoadComplete[%s], this:%p"), *GetClientServerContextString(this), this);
}
