#include "DelegatorFactory.h"

#include "Common/Delegate/GameInstanceDelegator.h"
#include "Server/Runtime/ServerGameInstanceDelegator.h"
#include "Client/Runtime/ClientGameInstanceDelegator.h"

#include "Common/Delegate/PlayerControllerDelegator.h"
#include "Server/Runtime/ServerPlayerControllerDelegator.h"
#include "Client/Runtime/ClientPlayerControllerDelegator.h"

#include "Common/Delegate/CharacterDelegator.h"
#include "Server/Runtime/ServerCharacterDelegator.h"
#include "Client/Runtime/ClientCharacterDelegator.h"

FDelegatorFactory::Target FDelegatorFactory::GetType(bool IsServer)
{
	if(IsServer)
		return Target::TARGET_SERVER;
	else
	{
		return Target::TARGET_CLIENT;
	}
}


TObjectPtr<UCharacterDelegator> FDelegatorFactory::CreateCharacterDelegator(Target InTarget)
{
	switch (InTarget)
	{
		case Target::TARGET_SERVER: return NewObject<UServerCharacterDelegator>();
		case Target::TARGET_CLIENT: return NewObject<UClientCharacterDelegator>();
		default: return nullptr;
	}
}

TObjectPtr<UGameInstanceDelegator> FDelegatorFactory::CreateGameInstanceDelegator(Target InTarget)
{
	switch (InTarget)
	{
	case Target::TARGET_SERVER: return NewObject<UServerGameInstanceDelegator>();
	case Target::TARGET_CLIENT: return NewObject<UClientGameInstanceDelegator>();
	default: return nullptr;
	}
}

TObjectPtr<UPlayerControllerDelegator> FDelegatorFactory::CreatePlayerControllerDelegator(Target InTarget)
{
	switch (InTarget)
	{
		case Target::TARGET_SERVER: return NewObject<UServerPlayerControllerDelegator>();
		case Target::TARGET_CLIENT: return NewObject<UClientPlayerControllerDelegator>();
		default: return nullptr;
	}
}

