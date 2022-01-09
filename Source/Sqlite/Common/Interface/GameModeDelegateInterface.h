#pragma once

class ASqliteGameModeBase;

class FGameModeDelegateInterface
{
protected:
	ASqliteGameModeBase* SqliteGameMode;

public:
	void SetGameMode(ASqliteGameModeBase* GameMode){ SqliteGameMode = GameMode; }
	ASqliteGameModeBase* GetGameMode(){ return SqliteGameMode;}

	virtual void BeginPlay() {}
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage){}

};