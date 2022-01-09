// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#if UE_SERVER
#include "Server/Runtime/ServerPlayerControllerDelegate.h"
#define PLAYERCONTROLLER_DELEGATOR FServerPlayerControllerDelegator
#else
#include "Client/Runtime/ClientPlayerControllerDelegate.h"
#define PLAYERCONTROLLER_DELEGATOR FClientPlayerControllerDelegator
#endif
#include "SqlitPlayerController.generated.h"
DECLARE_STATS_GROUP(TEXT("SqlitPlayerController"), STATGROUP_SqlitPlayerController, STATCAT_Advanced);
/**
 * 
 */
UCLASS()
class SQLITE_API ASqlitPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	PLAYERCONTROLLER_DELEGATOR PlayerControllerDelegator;
public:
	ASqlitPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	~ASqlitPlayerController();

public:
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, Category = "BaseInfo")
		FString UserId;
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, Category = "BaseInfo")
		FString Platform;

protected:
	// Begin Actor Interface [11/8/2021 CarlZhou]
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// End Actor Interface [11/8/2021 CarlZhou]
public:
	// APlayerController start Interface [10/20/2021 admin]
	virtual void CleanupPlayerState() override;
	virtual void Destroyed() override;
	virtual void PlayerTick(float DeltaTime) override;
	// APlayerController end Interface [10/20/2021 admin]

	//~ Begin UObject Interface
	virtual void BeginDestroy() override;
	/** Returns properties that are replicated for the lifetime of the actor channel */
	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const override;
	//~ End UObject Interface

public:
	UFUNCTION(BlueprintCallable,Category="Login")
		void ConnectDS();
};
