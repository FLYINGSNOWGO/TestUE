#pragma once

#include "CoreMinimal.h"

class ASqliteCharacter;

class FCharacterDelegateInterface
{
public:
	// Common
	void SetCharacter(ASqliteCharacter * InPlayerController) { SqliteCharacter = InPlayerController; }
	ASqliteCharacter* GetCharacter() { return SqliteCharacter; }

	// Server Function
	UClass* GetDefaultPawn_Implementation() { return nullptr; }

	// Client Function
	virtual void tick(float DeltaTime) {};
	virtual void CheckClientNetwork(float DeltaTime) {}
protected:
	ASqliteCharacter* SqliteCharacter;
};