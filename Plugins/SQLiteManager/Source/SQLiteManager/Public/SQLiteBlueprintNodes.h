
#pragma once
#include "SQLiteBlueprintNodes.generated.h"


USTRUCT(BlueprintType)
struct SQLITEMANAGER_API FSQLiteQueryLogicExpectedNode
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SQLite Database Query")
	FString Query;

	FSQLiteQueryLogicExpectedNode(){}
	FSQLiteQueryLogicExpectedNode(FString LHSQuery, FString Append) : Query(LHSQuery) 
	{
		Query += Append;
	}
};

USTRUCT(BlueprintType)
struct SQLITEMANAGER_API FSQLiteQueryTermExpectedNode
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SQLite Database Query")
	FString Query;

	FSQLiteQueryTermExpectedNode(){}
	FSQLiteQueryTermExpectedNode(FString LHSQuery, FString Append) : Query(LHSQuery) 
	{
		Query += Append;
	}
};

USTRUCT(BlueprintType)
struct SQLITEMANAGER_API FSQLiteQueryFinalizedQuery
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SQLite Database Query")
	FString Query;

	FSQLiteQueryFinalizedQuery(){}
	FSQLiteQueryFinalizedQuery(FString Query) : Query(Query){}
};
