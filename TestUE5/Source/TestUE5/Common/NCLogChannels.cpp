// Fill out your copyright notice in the Description page of Project Settings.


#include "NCLogChannels.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY(LogNC);
DEFINE_LOG_CATEGORY(LogClientNC);
DEFINE_LOG_CATEGORY(LogServerNC);
DEFINE_LOG_CATEGORY(LogClientTestLogin);
DEFINE_LOG_CATEGORY(LogServerTestLogin);

FString GetClientServerContextString(UObject* ContextObject)
{
	ENetRole Role = ROLE_None;

	if (AActor* Actor = Cast<AActor>(ContextObject))
	{
		Role = Actor->GetLocalRole();
	}
	else if (UActorComponent* Component = Cast<UActorComponent>(ContextObject))
	{
		Role = Component->GetOwnerRole();
	}

	if (Role != ROLE_None)
	{
		return (Role == ROLE_Authority) ? TEXT("Server") : TEXT("Client");
	}
	else
	{
#if WITH_EDITOR
		if (GIsEditor)
		{
			extern ENGINE_API FString GPlayInEditorContextString;
			return GPlayInEditorContextString;
		}
#endif
	}

	return TEXT("[]");
}
