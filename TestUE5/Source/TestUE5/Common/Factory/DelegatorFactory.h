#pragma once

class UGameInstanceDelegator;
class  UPlayerControllerDelegator;
class UCharacterDelegator;

class FDelegatorFactory
{
public:
	enum class Target : uint8
	{
		TARGET_SERVER,
		TARGET_CLIENT,
		TARGET_OPERATOR
	};

	static Target GetType(bool IsServer);
	
	static TObjectPtr<UGameInstanceDelegator> CreateGameInstanceDelegator(Target InTarget);
	static TObjectPtr<UPlayerControllerDelegator> CreatePlayerControllerDelegator(Target InTarget);
	static TObjectPtr<UCharacterDelegator> CreateCharacterDelegator(Target InTarget);

};