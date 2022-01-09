// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#if UE_SERVER
#include "Server/Runtime/ServerCharacterDelegate.h"
#define CHARACTER_DELEGATOR FServerCharacterDelegator
#else
#include "Client/Runtime/ClientCharacterDelegate.h"
#define CHARACTER_DELEGATOR FClientCharacterDelegator
#endif
#include "SqliteCharacter.generated.h"

UCLASS(BlueprintType)
class SQLITE_API ASqliteCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	CHARACTER_DELEGATOR  CharacterDelegator;

public:
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, Category = "BaseInfo")
		FString UserId;
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, Category = "BaseInfo")
		FString Platform;

public:
	// Sets default values for this character's properties
	ASqliteCharacter();

protected:
	// Begin Actor Interface [11/8/2021 CarlZhou]
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// End Actor Interface [11/8/2021 CarlZhou]

public:
	//~ Begin UObject Interface
	virtual void BeginDestroy() override;
	/** Returns properties that are replicated for the lifetime of the actor channel */
	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const override;
	//~ End UObject Interface

	//~ Begin Actor Interface
	virtual void Tick(float DeltaTime) override;
	//~ End Actor Interface

	//~ Begin ACharacter Interface
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//~ End ACharacter Interface


};
