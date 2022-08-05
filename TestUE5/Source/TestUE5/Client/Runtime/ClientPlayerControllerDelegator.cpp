// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientPlayerControllerDelegator.h"
#include "Common/NCLogChannels.h"
#include "Kismet/GameplayStatics.h"
#include "Common/Runtime/NCPlayerController.h"

#include "Blueprint/UserWidget.h"


UClientPlayerControllerDelegator::UClientPlayerControllerDelegator(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/) : Super(ObjectInitializer)
{
	UE_LOG(LogClientNC, Log, TEXT("Enter UClientPlayerControllerDelegate::UClientPlayerControllerDelegator[%s], this:%p"), *GetClientServerContextString(this),this);

	UE_LOG(LogClientNC, Log, TEXT("Exit UClientPlayerControllerDelegate::UClientPlayerControllerDelegator[%s], this:%p"), *GetClientServerContextString(this),this);
}

void UClientPlayerControllerDelegator::TestPlayerController()
{
	UE_LOG(LogClientNC,Log,TEXT("Enter UClientPlayerControllerDelegate::TestPlayerController[%s], this:%p"),*GetClientServerContextString(this),this);

	UE_LOG(LogClientNC,Log,TEXT("Exit UClientPlayerControllerDelegate::TestPlayerController[%s], this:%p"),*GetClientServerContextString(this),this);
}

void UClientPlayerControllerDelegator::BeginPlay()
{
	UE_LOG(LogClientNC, Log, TEXT("Enter UClientPlayerControllerDelegator::BeginPlay[%s], this:%p"), *GetClientServerContextString(this),this);
	FString Name = TEXT("WidgetBlueprint'/Game/UI/Test.Test_C'");
	UObject* Obj = LoadObject<UObject>(NULL, *Name);
	UClass* ObjClass = (UClass*)(Obj);
	if (ObjClass)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(PlayerController->GetWorld(),ObjClass);
		Widget->AddToViewport();
	}
	UE_LOG(LogClientNC, Log, TEXT("Exit UClientPlayerControllerDelegator::BeginPlay[%s], this:%p"), *GetClientServerContextString(this),this);
}

void UClientPlayerControllerDelegator::ConnectDS()
{
	UE_LOG(LogClientNC, Log, TEXT("Enter UClientPlayerControllerDelegator::ConnectDS[%s], this:%p"), *GetClientServerContextString(this),this);
	if (PlayerController.IsValid())
	{
		PlayerController->ClientTravel(TEXT("127.0.0.1:17777"), ETravelType::TRAVEL_Absolute);
	}
	UE_LOG(LogClientNC, Log, TEXT("Exit UClientPlayerControllerDelegator::ConnectDS[%s], this:%p"), *GetClientServerContextString(this),this);
}