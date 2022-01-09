// ClouddMinds RobotEngine  Powered by EPIC game


#include "SQLiteManagerObject.h"
#include "Runtime/Core/Public/HAL/PlatformFilemanager.h"
#include "SQLiteResultSet.h"
#include "Database.h"

DEFINE_LOG_CATEGORY_STATIC(LogSQLiteManager, Log, All);

#define LOGSQLITE(verbosity, text) UE_LOG(LogSQLiteManager, verbosity, TEXT("SQLite: %s"), text)

USQLiteManagerObject* USQLiteManagerObject::Singleton = nullptr;

TMap<FString, TSharedRef<FSQLiteDatabase>> USQLiteManagerObject::Databases;

USQLiteManagerObject::USQLiteManagerObject(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

USQLiteManagerObject::~USQLiteManagerObject()
{
	if (Singleton)
	{
		Singleton->RemoveFromRoot();
		Singleton = nullptr;
	}
}

void USQLiteManagerObject::CreateDatabase(const FString* Name, const FString* Filename, bool RelativeToGameContentDirectory)
{
	FString ActualFilename = *Filename;

	if (RelativeToGameContentDirectory)
	{
		ActualFilename = FPaths::ProjectContentDir() + *Filename;
	}
	// create [8/26/2021 admin]
	bool Ret = OpenDatabase(Name, &ActualFilename);
	if (!Ret)
	{
		LOGSQLITE(Error, *FString::Printf(TEXT("CreateDatabase - Cannot be created DB file: '%s' "), *ActualFilename));
	}
}

USQLiteManagerObject* USQLiteManagerObject::GetSQLiteManagerInstance()
{
	if (Singleton == nullptr)
	{
		Singleton = NewObject<USQLiteManagerObject>();
		Singleton->AddToRoot();
	}
	return Singleton;
}

bool USQLiteManagerObject::RegisterDatabase(FString Name, FString Filename, bool RelativeToGameContentDirectory)
{
	FString actualFilename = Filename;

	if (RelativeToGameContentDirectory)
	{
		actualFilename = FPaths::ProjectContentDir() + Filename;
	}
	if (!IsValidDatabase(&Name, &Filename, RelativeToGameContentDirectory,true))
	{
		FString message = "Unable to add database '" + actualFilename + "', it is not valid (problems opening it)!";
		LOGSQLITE(Warning, *message);
		CreateDatabase(&Name, &Filename, RelativeToGameContentDirectory);
	}

	FString successMessage = "Registered SQLite database '" + actualFilename + "' successfully.";
	LOGSQLITE(Verbose, *successMessage);
	return true;
}

bool USQLiteManagerObject::IsDatabaseRegistered(FString DatabaseName)
{
	return Databases.Contains(DatabaseName);
}

FSQLiteTable USQLiteManagerObject::CreateTable(const FString DatabaseName, const FString TableName, const TArray<FSQLiteTableField> Fields, const FSQLitePrimaryKey PK)
{
	FSQLiteTable Table;
	Table.DatabaseName = DatabaseName;
	Table.TableName = TableName;
	Table.Fields = Fields;
	Table.PK = PK;

	FString Query = "";
	Query += "CREATE TABLE IF NOT EXISTS ";
	Query += TableName;
	Query += "(";

	bool singlePrimaryKeyExists = false;

	for (const FSQLiteTableField& field : Fields)
	{
		if (field.ResultStr.Len() > 2) {

			if (field.ResultStr.Contains("PRIMARY KEY")) {
				singlePrimaryKeyExists = true;
			}

			Query += field.ResultStr + ", ";

		}

	}

	if (singlePrimaryKeyExists) {
		Query = Query.Left(Query.Len() - 2);

		Query += ");";
	}
	else {
		if (PK.ResultStr.Len() > 2) {
			Query += " " + PK.ResultStr + " ";
		}
		else {
			Query = Query.Left(Query.Len() - 2);
		}

		Query += ");";
	}

	//LOGSQLITE(Warning, *query);

	Table.Created = ExecSql(DatabaseName, Query);

	return Table;
}

bool USQLiteManagerObject::CreateIndexes(const FString DatabaseName, const FString TableName, const TArray<FSQLiteIndex> Indexes)
{
	bool IdxCrSts = true;

	for (const FSQLiteIndex& Idx : Indexes)
	{
		if (Idx.ResultStr.Len() > 2) {
			FString Query = Idx.ResultStr.Replace(TEXT("$$$TABLE_NAME$$$"), *TableName);

			//LOGSQLITE(Warning, *query);

			IdxCrSts = ExecSql(DatabaseName, Query);
			if (!IdxCrSts) {
				//LOGSQLITE(Warning, TEXT("ExecSql break"));
				break;
			}
		}

	}

	return IdxCrSts;
}

bool USQLiteManagerObject::CreateIndex(const FString DatabaseName, const FString TableName, const FSQLiteIndex Index)
{
	bool IdxCrSts = true;

	FString Query = Index.ResultStr.Replace(TEXT("$$$TABLE_NAME$$$"), *TableName);

	//LOGSQLITE(Warning, *query);

	IdxCrSts = ExecSql(DatabaseName, Query);

	return IdxCrSts;
}

bool USQLiteManagerObject::DropIndex(const FString DatabaseName, const FString IndexName)
{
	bool IdxCrSts = true;

	FString Query = "DROP INDEX " + IndexName;

	//LOGSQLITE(Warning, *query);

	IdxCrSts = ExecSql(DatabaseName, Query);

	return IdxCrSts;
}

bool USQLiteManagerObject::DropTable(const FString DatabaseName, const FString TableName)
{
	bool IdxCrSts = true;


	FString Query = "DROP TABLE " + TableName;

	//LOGSQLITE(Warning, *query);

	IdxCrSts = ExecSql(DatabaseName, Query);

	return IdxCrSts;
}

bool USQLiteManagerObject::TruncateTable(const FString DatabaseName, const FString TableName)
{
	bool IdxCrSts = true;


	FString Query = "DELETE FROM " + TableName + ";";

	//LOGSQLITE(Warning, *query);

	IdxCrSts = ExecSql(DatabaseName, Query);

	return IdxCrSts;
}

bool USQLiteManagerObject::IsTableExists(const FString DatabaseName, const FString TableName)
{
	if (!IsDatabaseRegistered(DatabaseName))
	{
		LOGSQLITE(Error, *FString::Printf(TEXT("IsTableExists - invalid database '%s'"), *DatabaseName));
		return false;
	}
	FString Query = "SELECT * FROM sqlite_master WHERE type='table' AND name='" + TableName + "';";

	FSQLitePreparedStatement SQLitePreparedStatement;

	bool bOpenDB = PrepareStatement(&DatabaseName, &Query, SQLitePreparedStatement);

	bool bTableExists = false;
	if (bOpenDB)
	{
		bool bExeSucceed = Databases[DatabaseName]->Execute(*Query);

		if (SQLitePreparedStatement.IsValid())
		{
			// Initialize records from compiled query
			FSQLiteResultSet SQLiteResultSet = FSQLiteResultSet(MoveTemp(SQLitePreparedStatement));
			TArray<SQLiteResultValue> ResultRows;
			for (FDataBaseRecordSet::TIterator Iterator(&SQLiteResultSet); Iterator; ++Iterator)
			{
				bTableExists = true;
				break;
			}
		}
	}
	
	
	return bTableExists;
}

bool USQLiteManagerObject::ExecSql(const FString DatabaseName, const FString Query)
{
	TSharedRef<FSQLiteDatabase> SQLiteCoreDatabase = Databases[DatabaseName];
	bool RetStatus = false;
	if (SQLiteCoreDatabase->Execute(*Query))
	{
		RetStatus = true;
	}
	else
	{
		LOGSQLITE(Error, *FString::Printf(TEXT("ExecSql - Query Exec Failed: ErrorMsg:%s"), *SQLiteCoreDatabase->GetLastError()));
	}
	return RetStatus;
}

void USQLiteManagerObject::BeginDestroy()
{
	for (auto &DBInfo : Databases)
	{
		if (!DBInfo.Value->Close())
		{
			LOGSQLITE(Error, *FString::Printf(TEXT("BeginDestroy - close DB file Failed: %s ErrorMsg:%s"), *DBInfo.Key,*DBInfo.Value->GetLastError()));
		}
	}
	Super::BeginDestroy();
}

bool USQLiteManagerObject::InsertRowsIntoTable(const FString DatabaseName, const FString TableName, const TArray<FSQLiteTableRowSimulator> RowsOfFields)
{
	bool bSucceed = true;
	for (FSQLiteTableRowSimulator Row : RowsOfFields) 
	{
		FString Query = "INSERT INTO " + TableName + " (";
		for (FSQLiteTableField Field : Row.RowsOfFields) 
		{
			Query += Field.FieldName + ", ";
		}

		Query = Query.Left(Query.Len() - 2);

		Query = Query + ") VALUES (";
		for (FSQLiteTableField field : Row.RowsOfFields) 
		{
			if (field.FieldType.Equals(TEXT("TEXT"))) 
			{
				Query = Query + "'" + field.FieldValue + "', ";
			}
			else 
			{
				Query = Query + field.FieldValue + ", ";
			}
		}

		Query = Query.Left(Query.Len() - 2);
		Query = Query + ");";

		//LOGSQLITE(Warning, *query);
		if (!ExecSql(DatabaseName, Query))
		{
			bSucceed = false;
		}

	}
	return bSucceed;
}

bool USQLiteManagerObject::Vacuum(const FString DatabaseName)
{
	bool IdxCrSts = true;


	FString Query = "VACUUM; ";

	//LOGSQLITE(Warning, *query);

	IdxCrSts = ExecSql(DatabaseName, Query);

	return IdxCrSts;
}

bool USQLiteManagerObject::GetDataIntoObject(const FString& DatabaseName, const FString& Query, UObject* ObjectToPopulate)
{
	//////////////////////////////////////////////////////////////////////////
	// Check input validness.
	//////////////////////////////////////////////////////////////////////////

	if (ObjectToPopulate == NULL)
	{
		LOGSQLITE(Error, TEXT("ObjectToPopulate needs to be set to get any results!"));
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// Validate the database
	//////////////////////////////////////////////////////////////////////////

	if (!IsDatabaseRegistered(DatabaseName))
	{
		LOGSQLITE(Error, *FString::Printf(TEXT("Unable to get data into object, invalid database '%s'"), *DatabaseName));
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// Get the results
	//////////////////////////////////////////////////////////////////////////

	SQLiteQueryResult queryResult = RunQueryAndGetResults(DatabaseName, Query);

	if (queryResult.Success && queryResult.Results.Num() > 0)
	{
		AssignResultsToObjectProperties(queryResult.Results[0], ObjectToPopulate);
		return true;
	}
	else if (!queryResult.Success)
	{
		LOGSQLITE(Error, *FString::Printf(TEXT("Query resulted in an error: '%s'"), *queryResult.ErrorMessage));
		return false;
	}
	else if (queryResult.Results.Num() == 0)
	{
		LOGSQLITE(Error, TEXT("Query returned zero rows, no data to assign to object properties."));
		return false;
	}

	return false;
}

bool USQLiteManagerObject::GetDataIntoObjectBP(const FSQLiteDatabaseReference& DataSource, TArray<FString> Fields, FSQLiteQueryFinalizedQuery Query, UObject* ObjectToPopulate)
{
	//////////////////////////////////////////////////////////////////////////
	// Check input validness.
	//////////////////////////////////////////////////////////////////////////

	if (ObjectToPopulate == NULL)
	{
		LOGSQLITE(Error, TEXT("ObjectToPopulate needs to be set to get any results!"));
		return false;
	}

	if (DataSource.Tables.Num() == 0)
	{
		LOGSQLITE(Error, TEXT("The query needs the table name!"));
		return false;
	}

	if (Fields.Num() == 0)
	{
		LOGSQLITE(Error, TEXT("The query needs fields! You may use * to get all fields."));
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// Validate the database
	//////////////////////////////////////////////////////////////////////////

	if (!IsDatabaseRegistered(DataSource.DatabaseName))
	{
		LOGSQLITE(Error, TEXT("Unable to get data to object, database validation failed!"));
		return false;
	}


	//////////////////////////////////////////////////////////////////////////
	// Get the results
	//////////////////////////////////////////////////////////////////////////

	FString constructedQuery = ConstructQuery(DataSource.Tables, Fields, Query, 1, 0);

	SQLiteQueryResult queryResult = RunQueryAndGetResults(DataSource.DatabaseName, constructedQuery);

	if (queryResult.Success && queryResult.Results.Num() > 0)
	{
		AssignResultsToObjectProperties(queryResult.Results[0], ObjectToPopulate);
		return true;
	}
	else if (!queryResult.Success)
	{
		LOGSQLITE(Error, *FString::Printf(TEXT("Query resulted in an error: '%s'"), *queryResult.ErrorMessage));
		return false;
	}
	else if (queryResult.Results.Num() == 0)
	{
		LOGSQLITE(Error, TEXT("Query returned zero rows, no data to assign to object properties."));
		return false;
	}

	return false;
}

FSQLiteQueryResult USQLiteManagerObject::GetData(const FString& DatabaseName, const FString& Query)
{
	FSQLiteQueryResult Result;

	SQLiteQueryResult QueryResult = RunQueryAndGetResults(DatabaseName, Query);
	Result.Success = QueryResult.Success;
	Result.ErrorMessage = QueryResult.ErrorMessage;

	for (auto row : QueryResult.Results)
	{
		FSQLiteQueryResultRow outRow;
		for (auto field : row.Fields)
		{
			FSQLiteKeyValuePair outField;
			outField.Key = field.Name;
			outField.Value = field.ToString();

			outRow.Fields.Add(outField);
		}
		Result.ResultRows.Add(outRow);
	}

	return Result;
}

FSQLiteQueryResult USQLiteManagerObject::GetDataBP(const FSQLiteDatabaseReference& DataSource, TArray<FString> Fields, FSQLiteQueryFinalizedQuery Query, int32 MaxResults /*= -1*/, int32 ResultOffset /*= 0*/)
{

	FSQLiteQueryResult Result;

	//////////////////////////////////////////////////////////////////////////
	// Check input validness.
	//////////////////////////////////////////////////////////////////////////

	if (DataSource.Tables.Num() == 0)
	{
		LOGSQLITE(Error, TEXT("The query needs at least one table name!"));
		Result.Success = false;
		Result.ErrorMessage = TEXT("No table given");
		return Result;
	}

	if (Fields.Num() == 0)
	{
		LOGSQLITE(Error, TEXT("The query needs fields! You can use * to get all fields."));
		Result.Success = false;
		Result.ErrorMessage = TEXT("No fields given");
		return Result;
	}

	FString constructedQuery = ConstructQuery(DataSource.Tables, Fields, Query, MaxResults, ResultOffset);

	return GetData(DataSource.DatabaseName, constructedQuery);
}

bool USQLiteManagerObject::IsValidDatabase(const FString* Name, const FString* Filename, bool RelativeToGameContentDirectory, bool TestByOpening)
{
	FString ActualFilename = *Filename;

	if (RelativeToGameContentDirectory)
	{
		ActualFilename = FPaths::ProjectContentDir() + *Filename;
	}
	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*ActualFilename))
	{
		if (TestByOpening)
		{
			return OpenDatabase(Name, &ActualFilename);
		}
		else
		{
			return true;
		}
	}
	else
	{
		LOGSQLITE(Warning, *FString::Printf(TEXT("file not found:%s"), *ActualFilename));
	}

	return false;
}

bool USQLiteManagerObject::OpenDatabase(const FString* Name, const FString* ActualFilename)
{
	if (IsDatabaseRegistered(*Name))
	{
		FString message = "Database '" + *ActualFilename + "' is already registered, skipping.";
		LOGSQLITE(Warning, *message);
		return true;
	}
	 TSharedRef<FSQLiteDatabase> SQLiteCoreDatabase = MakeShared<FSQLiteDatabase>();
	
	/*bool Ret = SQLiteCoreDatabase->Open(**ActualFilename,ESQLiteDatabaseOpenMode::ReadWriteCreateSerialized);*/
	 bool Ret = SQLiteCoreDatabase->Open(**ActualFilename, ESQLiteDatabaseOpenMode::ReadWriteCreate);
	if (!Ret)
	{
		LOGSQLITE(Error, *FString::Printf(TEXT("OpenDatabase - Cannot be opened or created DB file: '%s' "), **ActualFilename));
	}
	else
	{
		Databases.Add(*Name,SQLiteCoreDatabase);
	}
	return Ret;
}

SQLiteQueryResult USQLiteManagerObject::RunQueryAndGetResults(FString DatabaseName, FString Query)
{
	SQLiteQueryResult QueryResult;
	if (!IsDatabaseRegistered(DatabaseName))
	{
		LOGSQLITE(Error, *FString::Printf(TEXT("Unable to get data into object, invalid database '%s'"), *DatabaseName));
		return QueryResult;
	}

	FSQLitePreparedStatement SQLitePreparedStatement;

	bool bOpenDB = PrepareStatement(&DatabaseName, &Query, SQLitePreparedStatement);
	if (bOpenDB)
	{
		
		bool bExeSucceed = Databases[DatabaseName]->Execute(*Query);
		//////////////////////////////////////////////////////////////////////////
		// Get and assign the data
		//////////////////////////////////////////////////////////////////////////
		if (SQLitePreparedStatement.IsValid())
		{
			// Initialize records from compiled query
			FSQLiteResultSet SQLiteResultSet = FSQLiteResultSet(MoveTemp(SQLitePreparedStatement));
			TArray<SQLiteResultValue> ResultRows;
			for (FDataBaseRecordSet::TIterator Iterator(&SQLiteResultSet); Iterator; ++Iterator)
			{
				FDataBaseRecordSet* DataBaseRecordSet = *Iterator;
				SQLiteResultValue Row;
				LOGSQLITE(Verbose, TEXT("Query returned a result row."));
				for (FDatabaseColumnInfo& ColumnInfo : DataBaseRecordSet->GetColumnNames())
				{
					SQLiteResultField ResultField;
					ResultField.Name = ColumnInfo.ColumnName;
					switch (ColumnInfo.DataType)
					{
					case DBT_FLOAT:
					{
						ResultField.Type = SQLiteResultValueTypes::Float;
						ResultField.DoubleValue = DataBaseRecordSet->GetFloat(*ColumnInfo.ColumnName);
					}
					break;
					case DBT_INT:
					{
						ResultField.Type = SQLiteResultValueTypes::Integer;
						ResultField.IntValue = DataBaseRecordSet->GetInt(*ColumnInfo.ColumnName);
					}
					break;
					case DBT_STRING:
					{
						ResultField.Type = SQLiteResultValueTypes::Text;
						ResultField.StringValue = DataBaseRecordSet->GetString(*ColumnInfo.ColumnName);
					}
					break;
					case DBT_UNKOWN:
					default:
					{
						ResultField.Type = SQLiteResultValueTypes::UnsupportedValueType;
					}
					break;
					}

					if (ResultField.Type != SQLiteResultValueTypes::UnsupportedValueType)
					{
						Row.Fields.Add(ResultField);
					}
				}

				ResultRows.Add(Row);
			}

			QueryResult.Results = ResultRows;
			QueryResult.Success = true;
		}
	}
	return QueryResult;
}

bool USQLiteManagerObject::PrepareStatement(const FString* DatabaseName, const FString* Query, FSQLitePreparedStatement &SQLitePreparedStatement)
{
	bool bOpen = false;
	TSharedRef<FSQLiteDatabase> SQLiteCoreDatabase = Databases[*DatabaseName];
	if (!SQLiteCoreDatabase->IsValid())
	{
		LOGSQLITE(Error, *FString::Printf(TEXT("PrepareStatement - Not Open DB Failed: %s"), DatabaseName));
		return bOpen;
	}

	bOpen = true;
	SQLitePreparedStatement = SQLiteCoreDatabase->PrepareStatement(**Query);
	
	return bOpen;
}

void USQLiteManagerObject::AssignResultsToObjectProperties(const SQLiteResultValue& ResultValue, UObject* ObjectToPopulate)
{
	auto propertyMap = CollectProperties(ObjectToPopulate);
	for (SQLiteResultField field : ResultValue.Fields)
	{
		if (propertyMap.Contains(field.Name))
		{
			FProperty* targetProperty = propertyMap[field.Name];

			if (field.Type == SQLiteResultValueTypes::Integer)
			{
				FInt64Property* int64prop = NULL;
				FIntProperty* int32prop = NULL;
				FInt16Property* int16prop = NULL;
				FInt8Property* int8prop = NULL;
				FBoolProperty* boolProp = NULL;

				if ((int64prop = CastField<FInt64Property>(targetProperty)) != NULL)
				{
					int64prop->SetPropertyValue_InContainer(ObjectToPopulate, field.IntValue);
					LOGSQLITE(Verbose, *FString::Printf(TEXT("Property '%s' was set to '%d'"), *field.Name, field.IntValue));
				}
				else if ((int32prop = CastField<FIntProperty>(targetProperty)) != NULL)
				{
					int32prop->SetPropertyValue_InContainer(ObjectToPopulate, (int32)field.IntValue);
					LOGSQLITE(Verbose, *FString::Printf(TEXT("Property '%s' was set to '%d'"), *field.Name, field.IntValue));
				}
				else if ((int16prop = CastField<FInt16Property>(targetProperty)) != NULL)
				{
					int16prop->SetPropertyValue_InContainer(ObjectToPopulate, (int16)field.IntValue);
					LOGSQLITE(Verbose, *FString::Printf(TEXT("Property '%s' was set to '%d'"), *field.Name, field.IntValue));
				}
				else if ((int8prop = CastField<FInt8Property>(targetProperty)) != NULL)
				{
					int8prop->SetPropertyValue_InContainer(ObjectToPopulate, (int8)field.IntValue);
					LOGSQLITE(Verbose, *FString::Printf(TEXT("Property '%s' was set to '%d'"), *field.Name, field.IntValue));
				}
				else if ((boolProp = CastField<FBoolProperty>(targetProperty)) != NULL)
				{
					boolProp->SetPropertyValue_InContainer(ObjectToPopulate, field.IntValue > 0);
					LOGSQLITE(Verbose, *FString::Printf(TEXT("Property '%s' was set to '%d'"), *field.Name, field.IntValue));
				}
			}

			else if (field.Type == SQLiteResultValueTypes::Float)
			{
				FDoubleProperty* doubleProp = NULL;
				FFloatProperty* floatProp = NULL;
				if ((doubleProp = CastField<FDoubleProperty>(targetProperty)) != NULL)
				{
					doubleProp->SetPropertyValue_InContainer(ObjectToPopulate, field.DoubleValue);
					LOGSQLITE(Verbose, *FString::Printf(TEXT("Property '%s' was set to '%f'"), *field.Name, field.DoubleValue));
				}
				else if ((floatProp = CastField<FFloatProperty>(targetProperty)) != NULL)
				{
					floatProp->SetPropertyValue_InContainer(ObjectToPopulate, (float)field.DoubleValue);
					LOGSQLITE(Verbose, *FString::Printf(TEXT("Property '%s' was set to '%f'"), *field.Name, field.DoubleValue));
				}
			}

			else if (field.Type == SQLiteResultValueTypes::Text)
			{
				FStrProperty* strProp = NULL;
				if ((strProp = CastField<FStrProperty>(targetProperty)) != NULL)
				{
					strProp->SetPropertyValue_InContainer(ObjectToPopulate, field.StringValue);
					LOGSQLITE(Verbose, *FString::Printf(TEXT("Property '%s' was set to '%s'"), *field.Name, *field.StringValue.Mid(0, 64)));
				}
			}

		}
	}
}

TMap<FString, FProperty*> USQLiteManagerObject::CollectProperties(UObject* SourceObject)
{
	UClass* SourceObjectClass = SourceObject->GetClass();
	TMap<FString, FProperty*> Props;
	for (TFieldIterator<FProperty > PropIt(SourceObjectClass, EFieldIteratorFlags::SuperClassFlags::IncludeSuper);
		PropIt; ++PropIt)
	{
		Props.Add(*PropIt->GetNameCPP(), *PropIt);
	}

	return Props;
}

FString USQLiteManagerObject::ConstructQuery(TArray<FString> Tables, TArray<FString> Fields, FSQLiteQueryFinalizedQuery QueryObject, int32 MaxResults /*= -1*/, int32 ResultOffset /*= 0*/)
{
	FString FieldString;
	for (int32 i = 0; i < Fields.Num(); i++)
	{
		FieldString.Append(Fields[i] + (i < Fields.Num() - 1 ? "," : ""));
	}

	FString TableList = FString::Join(Tables, TEXT(","));
	TArray<FString> AllQueryParams;

	AllQueryParams.Add(FString::Printf(TEXT("SELECT %s FROM %s"), *FieldString, *TableList));

	if (QueryObject.Query.Len() > 0)
	{
		AllQueryParams.Add(FString::Printf(TEXT("WHERE %s"), *QueryObject.Query));
	}

	if (MaxResults >= 0)
	{
		AllQueryParams.Add(FString::Printf(TEXT("LIMIT %i"), MaxResults));
	}

	if (ResultOffset > 0)
	{
		AllQueryParams.Add(FString::Printf(TEXT("OFFSET %i"), ResultOffset));
	}

	FString FinalQuery = FString::Join(AllQueryParams, TEXT(" "));
	return FinalQuery;
}

