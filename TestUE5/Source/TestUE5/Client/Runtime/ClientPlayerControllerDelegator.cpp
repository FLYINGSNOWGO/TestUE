// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientPlayerControllerDelegator.h"
#include "Common/NCLogChannels.h"
#include "Kismet/GameplayStatics.h"
#include "Common/Runtime/NCPlayerController.h"

#include "Blueprint/UserWidget.h"


UClientPlayerControllerDelegator::UClientPlayerControllerDelegator(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/) : Super(ObjectInitializer)
{
	UE_LOG(LogClientNC, Log, TEXT("Enter UClientPlayerControllerDelegate::UClientPlayerControllerDelegator[%s]"), *GetClientServerContextString(this));

	UE_LOG(LogClientNC, Log, TEXT("Exit UClientPlayerControllerDelegate::UClientPlayerControllerDelegator[%s]"), *GetClientServerContextString(this));
}

void UClientPlayerControllerDelegator::TestPlayerController()
{
	UE_LOG(LogClientNC,Log,TEXT("Enter UClientPlayerControllerDelegate::TestPlayerController[%s]"),*GetClientServerContextString(this));

	UE_LOG(LogClientNC,Log,TEXT("Exit UClientPlayerControllerDelegate::TestPlayerController[%s]"),*GetClientServerContextString(this));
}

void UClientPlayerControllerDelegator::BeginPlay()
{
	FString Name = TEXT("WidgetBlueprint'/Game/UI/Test.Test_C'");
	UObject* Obj = LoadObject<UObject>(NULL, *Name);
	UClass* ObjClass = (UClass*)(Obj);
	if (ObjClass)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(PlayerController->GetWorld(),ObjClass);
		Widget->AddToViewport();
	}
}

void UClientPlayerControllerDelegator::ConnectDS()
{
	UE_LOG(LogClientNC, Log, TEXT("Enter UClientPlayerControllerDelegator::ConnectDS[%s]"), *GetClientServerContextString(this));
	if (PlayerController.IsValid())
	{
		PlayerController->ClientTravel(TEXT("127.0.0.1:17777"), ETravelType::TRAVEL_Absolute);
	}
	UE_LOG(LogClientNC, Log, TEXT("Exit UClientPlayerControllerDelegator::ConnectDS[%s]"), *GetClientServerContextString(this));
}