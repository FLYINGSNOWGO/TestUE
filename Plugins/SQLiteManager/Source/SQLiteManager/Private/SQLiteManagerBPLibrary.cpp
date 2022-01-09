// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SQLiteManagerBPLibrary.h"
#include "SQLiteManager.h"

USQLiteManagerBPLibrary::USQLiteManagerBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float USQLiteManagerBPLibrary::SQLiteManagerSampleFunction(float Param)
{
	return -1;
}

int32 USQLiteManagerBPLibrary::CastToInt(FString SQLiteResultValue)
{
	return FCString::Atoi(*SQLiteResultValue);
}


bool USQLiteManagerBPLibrary::CastToBoolean(FString SQLiteResultValue)
{
	return FCString::Atoi(*SQLiteResultValue) > 0;
}

//--------------------------------------------------------------------------------------------------------------

float USQLiteManagerBPLibrary::CastToFloat(FString SQLiteResultValue)
{
	return FCString::Atof(*SQLiteResultValue);
}

//--------------------------------------------------------------------------------------------------------------

FSQLiteQueryTermExpectedNode USQLiteManagerBPLibrary::QueryStart(FSQLiteQueryTermExpectedNode LogicOperationOrNone)
{
	return FSQLiteQueryTermExpectedNode(LogicOperationOrNone.Query, TEXT("("));
}

//--------------------------------------------------------------------------------------------------------------

FSQLiteQueryLogicExpectedNode USQLiteManagerBPLibrary::QueryEnd(const FSQLiteQueryLogicExpectedNode& LastQueryTerm)
{
	return FSQLiteQueryLogicExpectedNode(LastQueryTerm.Query, TEXT(")"));
}

//--------------------------------------------------------------------------------------------------------------

FSQLiteQueryLogicExpectedNode USQLiteManagerBPLibrary::QueryTerm(const FSQLiteQueryTermExpectedNode& LogicOperation, FString Field, FString Operator, FString Value)
{
	return FSQLiteQueryLogicExpectedNode(LogicOperation.Query, FString::Printf(TEXT("%s %s \"%s\""), *Field, *Operator, *Value));
}

//--------------------------------------------------------------------------------------------------------------

FSQLiteQueryTermExpectedNode USQLiteManagerBPLibrary::QueryLogicAnd(const FSQLiteQueryLogicExpectedNode& LHS)
{
	return FSQLiteQueryTermExpectedNode(LHS.Query, TEXT(" AND "));
}

//--------------------------------------------------------------------------------------------------------------

FSQLiteQueryTermExpectedNode USQLiteManagerBPLibrary::QueryLogicOr(const FSQLiteQueryLogicExpectedNode& LHS)
{
	return FSQLiteQueryTermExpectedNode(LHS.Query, TEXT(" OR "));
}
//--------------------------------------------------------------------------------------------------------------

FSQLiteQueryFinalizedQuery USQLiteManagerBPLibrary::QueryFinal(const FSQLiteQueryLogicExpectedNode& QueryEndNode)
{
	return FSQLiteQueryFinalizedQuery(QueryEndNode.Query);
}

FSQLiteTableField USQLiteManagerBPLibrary::SQLiteINTEGER(FString FieldName, const bool PK, const bool AI, const bool Unique)
{

	FSQLiteTableField f;
	f.FieldType = "INTEGER";

	f.FieldName = FieldName;

	FString outStr = FieldName + " INTEGER ";
	if (PK && AI) {
		outStr += " PRIMARY KEY AUTOINCREMENT ";
	}
	else if (AI) {
		outStr += " PRIMARY KEY AUTOINCREMENT ";
	}

	if (Unique) {
		outStr += " UNIQUE ";
	}

	f.ResultStr = outStr;

	return f;
}

FSQLiteTableField USQLiteManagerBPLibrary::SQLiteTEXT(const FString FieldName, const bool PK,
	const bool Unique)
{
	FSQLiteTableField f;
	f.FieldType = "TEXT";

	f.FieldName = FieldName;

	FString outStr = FieldName + " TEXT ";
	if (Unique) {
		outStr += " UNIQUE ";
	}

	f.ResultStr = outStr;

	return f;
}

FSQLiteTableField USQLiteManagerBPLibrary::SQLiteREAL(const FString FieldName, const bool PK,
	const bool Unique)
{
	FSQLiteTableField f;
	f.FieldType = "REAL";

	f.FieldName = FieldName;

	FString outStr = FieldName + " REAL ";
	if (Unique) {
		outStr += " UNIQUE ";
	}
	f.ResultStr = outStr;

	return f;

}

FSQLiteTableField USQLiteManagerBPLibrary::SQLiteNUMERIC(const FString FieldName, const bool PK,
	const bool Unique)
{
	FSQLiteTableField f;
	f.FieldType = "NUMERIC";

	f.FieldName = FieldName;

	FString outStr = FieldName + " NUMERIC ";
	if (Unique) {
		outStr += " UNIQUE ";
	}

	f.ResultStr = outStr;

	return f;

}

FSQLitePrimaryKey USQLiteManagerBPLibrary::SQLitePrimaryKey(const TArray<FSQLiteTableField> Fields)
{
	FSQLitePrimaryKey result;
	result.ResultStr = "";

	FString i = "";
	for (const FSQLiteTableField& field : Fields) {
		if (field.FieldName.Len() > 1) {
			i += field.FieldName + ", ";
		}
	}

	FString o = "";

	if (i.Len() > 1) {
		o = " PRIMARY KEY( ";
		o += i;
		o = o.Left(o.Len() - 2);
		o += ")";
	}

	result.ResultStr = o;
	return result;
}

FSQLiteIndex USQLiteManagerBPLibrary::SQLiteIndexFunction(const TArray<FSQLiteTableField> Fields, FString idxName, bool Unique) {

	FSQLiteIndex i;
	i.IndexName = idxName;

	FString o = "CREATE ";

	if (Unique) {
		o += "UNIQUE ";
	}

	o += "INDEX " + idxName + " ON $$$TABLE_NAME$$$ (";

	for (const FSQLiteTableField& fld : Fields) {
		o += fld.FieldName + ", ";
	}

	o = o.Left(o.Len() - 2);
	o += ");";

	i.ResultStr = o;

	return i;

}

