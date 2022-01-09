// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SQLiteBlueprintNodes.h"
#include "SQLiteDatabaseStructs.h"
#include "SQLiteManagerBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class SQLITEMANAGER_API USQLiteManagerBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Sample function", Keywords = "SQLiteManager sample test testing"), Category = "SQLiteManagerTesting")
	static float SQLiteManagerSampleFunction(float Param);
	
public:

	UFUNCTION(BlueprintCallable, Category = "SQLite|Value Conversion")
		static int32 CastToInt(FString SQLiteResultValue);

	UFUNCTION(BlueprintCallable, Category = "SQLite|Value Conversion")
		static bool CastToBoolean(FString SQLiteResultValue);

	UFUNCTION(BlueprintCallable, Category = "SQLite|Value Conversion")
		static float CastToFloat(FString SQLiteResultValue);
	/**
	* Blueprint nodes for building queries.
	*/

	/** Start a new query block. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Query Start", CompactNodeTitle = "("), Category = "SQLite|Query")
		static FSQLiteQueryTermExpectedNode QueryStart(FSQLiteQueryTermExpectedNode LogicOperationOrNone);

	/** End a query block. NOTE: To link this query to a Get Data node's query pin, link a Finalize Query node to the end of the query chain and link that to the pin. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Query End", CompactNodeTitle = ")"), Category = "SQLite|Query")
		static FSQLiteQueryLogicExpectedNode QueryEnd(const FSQLiteQueryLogicExpectedNode& LastQueryTerm);

	/** A query term. Basically creating a "Key <operator> Value" comparison. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Query Term"), Category = "SQLite|Query")
		static FSQLiteQueryLogicExpectedNode QueryTerm(const FSQLiteQueryTermExpectedNode& LogicOperation, FString Field, FString Operator, FString Value);

	/** AND logic node, combining two terms. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "AND", CompactNodeTitle = "AND"), Category = "SQLite|Query|Logic")
		static FSQLiteQueryTermExpectedNode QueryLogicAnd(const FSQLiteQueryLogicExpectedNode& LHS);

	/** OR logic node, combining two terms. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "OR", CompactNodeTitle = "OR"), Category = "SQLite|Query|Logic")
		static FSQLiteQueryTermExpectedNode QueryLogicOr(const FSQLiteQueryLogicExpectedNode& LHS);

	/** Finalizes the query. This node's output can be linked to a Get Data node's query source pin. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Finalize Query"), Category = "SQLite|Query")
		static FSQLiteQueryFinalizedQuery QueryFinal(const FSQLiteQueryLogicExpectedNode& QueryEndNode);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "INTEGER (SQLite)"), Category = "SQLite|Query|DataTypes")
		static FSQLiteTableField SQLiteINTEGER(FString FieldName, const bool PK, const bool AI, const bool Unique);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "TEXT (SQLite)"), Category = "SQLite|Query|DataTypes")
		static FSQLiteTableField SQLiteTEXT(const FString FieldName, const bool PK, const bool Unique);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "REAL (SQLite)"), Category = "SQLite|Query|DataTypes")
		static FSQLiteTableField SQLiteREAL(const FString FieldName, const bool PK, const bool Unique);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "NUMERIC (SQLite)"), Category = "SQLite|Query|DataTypes")
		static FSQLiteTableField SQLiteNUMERIC(const FString FieldName, const bool PK, const bool Unique);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Primary Key (SQLite)"), Category = "SQLite|Query|DataTypes")
		static FSQLitePrimaryKey SQLitePrimaryKey(const TArray<FSQLiteTableField> Fields);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Index (SQLite)"), Category = "SQLite|Query|DataTypes")
		static FSQLiteIndex SQLiteIndexFunction(const TArray<FSQLiteTableField> Fields, FString idxName, bool Unique);
	
};
