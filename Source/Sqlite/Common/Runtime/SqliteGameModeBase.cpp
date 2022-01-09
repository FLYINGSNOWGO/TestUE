// Fill out your copyright notice in the Description page of Project Settings.


#include "SqliteGameModeBase.h"
#include "SQLiteManagerObject.h"
#include "SQLiteManagerBPLibrary.h"
#include "Runtime/Core/Public/Async/Async.h"
#include "Runtime/CoreUObject/Public/Misc/PackageName.h"
#include "Runtime/PakFile/Public/IPlatformFilePak.h"
#include "Runtime/Core/Public/HAL/PlatformFilemanager.h"
#include "Runtime/Core/Public/Misc/CoreDelegates.h"

void ASqliteGameModeBase::BeginPlay()
{
	SQLiteManager = USQLiteManagerObject::GetSQLiteManagerInstance();
}

void ASqliteGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	bUseSeamlessTravel = true;
	Super::InitGame(MapName,Options,ErrorMessage);
}

void ASqliteGameModeBase::RegisterDB()
{
	FString DBName = TEXT("TestDatabase");
	if (!SQLiteManager->IsDatabaseRegistered(DBName))
	{
		SQLiteManager->RegisterDatabase(DBName, "Database/TestDatabase.db", true);
	}
}

void ASqliteGameModeBase::CreateTable()
{

	FString DBName = TEXT("TestDatabase");
	if (!SQLiteManager->IsDatabaseRegistered(DBName))
	{
		GEngine->AddOnScreenDebugMessage(0, 3, FColor::Red, *FString::Printf(TEXT("CreateTable - %s:Database not registered"), *DBName));
		return;
	}

	FString TBName = TEXT("Actors");
	if (!SQLiteManager->IsTableExists(DBName, TBName))
	{
		TArray<FSQLiteTableField> Fields;

		FSQLiteTableField TBField = USQLiteManagerBPLibrary::SQLiteINTEGER(TEXT("Id"), true, true, true);
		Fields.Add(TBField);

		TBField = USQLiteManagerBPLibrary::SQLiteTEXT(TEXT("Name"), false, false);
		Fields.Add(TBField);

		TBField = USQLiteManagerBPLibrary::SQLiteINTEGER(TEXT("Age"), false, false, false);
		Fields.Add(TBField);

		TBField = USQLiteManagerBPLibrary::SQLiteTEXT(TEXT("Gender"), false, false);
		Fields.Add(TBField);

		TBField = USQLiteManagerBPLibrary::SQLiteREAL(TEXT("Height"), false, false);
		Fields.Add(TBField);

		FSQLitePrimaryKey PK;
		PK.ResultStr = TEXT("Name");

		FSQLiteTable ActorsTable = SQLiteManager->CreateTable(DBName, TBName, Fields, PK);
		if (ActorsTable.Created)
		{
			GEngine->AddOnScreenDebugMessage(0, 3, FColor::Black, TEXT("Actors Table create successed"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(0, 3, FColor::Red, TEXT("Actors Table create failed"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(0, 3, FColor::Red, TEXT("Actors Table already exists"));
	}

}

void ASqliteGameModeBase::AddDataToActorsTable(FString NameVar, int32 AgeVar, FString GenderVar, float HeightVar)
{
	FString DBName = TEXT("TestDatabase");
	FString TBName = TEXT("Actors");

	if (!SQLiteManager->IsDatabaseRegistered(DBName))
	{
		GEngine->AddOnScreenDebugMessage(0, 3, FColor::Red, *FString::Printf(TEXT("AddDataToActorsTable - %s:Database not registered"), *DBName));
		return;
	}

	/*if (!SQLiteManager->IsTableExists(DBName, TBName))
	{
		GEngine->AddOnScreenDebugMessage(0, 3, FColor::Red, TEXT("AddDataToActorsTable - %s:Table does not exist"), *TBName);
		return;
	}*/
	TArray<FSQLiteTableRowSimulator> rowsOfFields;

	FSQLiteTableRowSimulator TableRow;

	FSQLiteTableField TableField = USQLiteManagerBPLibrary::SQLiteTEXT(TEXT("Name"), true, true);
	TableField.FieldValue = NameVar;
	TableRow.RowsOfFields.Add(TableField);

	TableField = USQLiteManagerBPLibrary::SQLiteINTEGER(TEXT("Age"), false, false, false);
	TableField.FieldValue = FString::Printf(TEXT("%d"), AgeVar);
	TableRow.RowsOfFields.Add(TableField);

	TableField = USQLiteManagerBPLibrary::SQLiteTEXT(TEXT("Gender"), true, true);
	TableField.FieldValue = GenderVar;
	TableRow.RowsOfFields.Add(TableField);

	TableField = USQLiteManagerBPLibrary::SQLiteREAL(TEXT("Height"), false, false);
	TableField.FieldValue = FString::Printf(TEXT("%f"), HeightVar);
	TableRow.RowsOfFields.Add(TableField);

	rowsOfFields.Add(TableRow);

	SQLiteManager->InsertRowsIntoTable(DBName, TBName, rowsOfFields);
}

void ASqliteGameModeBase::GetMyStats(FString ActorName)
{
	FString DBName = TEXT("TestDatabase");

	/*if (!SQLiteManager->IsDatabaseRegistered(DBName))
	{
		GEngine->AddOnScreenDebugMessage(0, 3, FColor::Red, TEXT("GetMyStats - %s:Database not registered"), *DBName);
		return;
	}*/

	FSQLiteQueryResult QueryResult = SQLiteManager->GetData(DBName, FString::Printf(TEXT("SELECT Name, Age, Gender, Height FROM Actors WHERE Name = \"%s\""), *ActorName));

	FString Info;
	for (FSQLiteQueryResultRow &QueryResultRow : QueryResult.ResultRows)
	{
		for (FSQLiteKeyValuePair &KeyValuePair : QueryResultRow.Fields)
		{
			Info += FString::Printf(TEXT("%s: %s;"),*KeyValuePair.Key,*KeyValuePair.Value);
		}
	}
	GEngine->AddOnScreenDebugMessage(0,3,FColor::Black, *Info);
	UE_LOG(LogTemp, Display, TEXT("%s"),*Info);
}

void ASqliteGameModeBase::TestAsynRead()
{
	AsyncTask(ENamedThreads::AnyThread, [this]() {
		UE_LOG(LogTemp,Display, TEXT("线程1：执行读取数据"));
		//AddDataToActorsTable(TEXT("张三"), 20, TEXT("男"), 175.f);
		GetMyStats(TEXT("Bruce Willis"));
	});
	AsyncTask(ENamedThreads::AnyThread, [this]() {
		UE_LOG(LogTemp, Display, TEXT("线程2：执行读取数据"));
		GetMyStats(TEXT("CC"));
	});
}

void ASqliteGameModeBase::TestAsynReadWrite()
{
	AsyncTask(ENamedThreads::AnyThread, [this]() {
		UE_LOG(LogTemp, Display, TEXT("线程1：执行插入数据"));
		AddDataToActorsTable(TEXT("张三"), 20, TEXT("男"), 175.f);
		//GetMyStats(TEXT("Bruce Willis"));
	});
	AsyncTask(ENamedThreads::AnyThread, [this]() {
		UE_LOG(LogTemp, Display, TEXT("线程2：执行读取数据"));
		GetMyStats(TEXT("CC"));
	});
}

void ASqliteGameModeBase::TestNew()
{
	new(Table) FTestNew(32, TEXT("Test"));
	new(Table) FTestNew(64, TEXT("Test1"));

	FString Info;
	for (FTestNew& Value : Table)
	{
		Info += FString::Printf(TEXT("V1:%d;V2:%s "),Value.V1,*Value.V2);
	}
	UE_LOG(LogTemp, Display, TEXT("Info:【%s】"),*Info);
	Table.Empty();
}

FString ASqliteGameModeBase::TestCreateMD5FromFile(FString FilePath)
{
	FMD5Hash Hash = FMD5Hash::HashFile(*FilePath);
	return LexToString(Hash);
}

FString ASqliteGameModeBase::TestGetShortPakName(FString FilePath)
{
	FString ShortPakName = FPackageName::GetShortName(FilePath);
	UE_LOG(LogTemp,Display,TEXT("[%s]short pak name:%s"), *FilePath,*ShortPakName);
	return ShortPakName;
}

FString ASqliteGameModeBase::TestGetFileName(FString FilePath)
{
	int32 LastSlashIndex;
	FilePath.FindLastChar(*TEXT("/"), LastSlashIndex);
	FString PakName = FilePath.RightChop(LastSlashIndex + 1);
	UE_LOG(LogTemp, Display, TEXT("[%s]pak name:%s"), *FilePath, *PakName);
	return PakName;
}

void ASqliteGameModeBase::TestTimer()
{
	UE_LOG(LogTemp, Display, TEXT("ASqliteGameModeBase::TestTimer"));
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]() {
		GetWorldTimerManager().ClearTimer(TimerHandle);
		UE_LOG(LogTemp, Display, TEXT("10s after ASqliteGameModeBase::TestTimer"));
	}), 0.5f, false, 10.0f);
}

bool ASqliteGameModeBase::IsMountedDefaultPaks()
{
	FPakPlatformFile* PakPlatformFile = (FPakPlatformFile*)(FPlatformFileManager::Get().FindPlatformFile(FPakPlatformFile::GetTypeName()));
	if (PakPlatformFile)
	{
		TArray<FString> MountedFilePaks;
		PakPlatformFile->GetMountedPakFilenames(MountedFilePaks);
		for (auto &PakFile : MountedFilePaks)
		{
			FString DefaultPaksPath = TEXT("Content/Paks");
			if (PakFile.Contains(DefaultPaksPath))
			{
				return true;
			}
		}
	}
	return false;
}

void ASqliteGameModeBase::TestMountPak()
{
	UE_LOG(LogTemp,Display,TEXT("ProjectPersistentDownloadDir:%s"),*FPaths::ProjectPersistentDownloadDir());
	FString PakPath = FString::Printf(TEXT("%s/DownLoad/PAKs/TestPak/Blueprint/Blueprint_Win_TestPak.pak"), *FPaths::ProjectContentDir());
	FCLPakFileVisitor Visitor;
	FCoreDelegates::NewFileAddedDelegate.AddUObject(this, &ASqliteGameModeBase::OnNewFileAddedDelegate);
	bool bSuccess = FCoreDelegates::OnMountPak.Execute(PakPath, 0, (IPlatformFile::FDirectoryVisitor*)&Visitor);
	if (bSuccess)
	{
		for (auto &File : Visitor.Files)
		{
			UE_LOG(LogTemp,Display,TEXT("file in Pak:%s"),*File);
		}
		
		IPlatformFile* InnerPlatform = FPlatformFileManager::Get().FindPlatformFile(FPakPlatformFile::GetTypeName());
		FPakFile* Pak = new FPakFile(InnerPlatform, *PakPath, false);
		if (Pak)
		{
			UE_LOG(LogTemp,Display,TEXT("[%s]->GetMountPoint: %s"),*PakPath,*Pak->GetMountPoint());
			//delete Pak;
			Pak = nullptr;
		}
		FPakPlatformFile* PakPlatformFile = (FPakPlatformFile*)InnerPlatform;
		TArray<FString> MountedFilePaks;
		PakPlatformFile->GetMountedPakFilenames(MountedFilePaks);
		for (auto &PakFile : MountedFilePaks)
		{
			UE_LOG(LogTemp,Display,TEXT("Mounted PakFile: %s"),*PakFile);
		}
		FString DefaultPakPath = MountedFilePaks[0];
		Pak = new FPakFile(InnerPlatform, *DefaultPakPath, false);
		if (Pak)
		{
			UE_LOG(LogTemp, Display, TEXT("[%s]->GetMountPoint: %s full:%s"), *DefaultPakPath, *Pak->GetMountPoint(), *FPaths::ConvertRelativePathToFull(Pak->GetMountPoint()));
			
			//delete Pak;
			Pak = nullptr;
		}
	}
}

void ASqliteGameModeBase::TestMountPak1()
{
	FString PakPath = FString::Printf(TEXT("%s/DownLoad/PAKs/TestPak/Blueprint/Blueprint_Win_TestPak.pak"), *FPaths::ProjectContentDir());
	IPlatformFile* InnerPlatform = FPlatformFileManager::Get().FindPlatformFile(FPakPlatformFile::GetTypeName());
	FPakPlatformFile* PakPlatformFile = (FPakPlatformFile*)InnerPlatform;
	FString InPath = FString::Printf(TEXT("%sBlueprint/"), *FPaths::ProjectContentDir());
	bool bSuccessed = PakPlatformFile->Mount(*PakPath,0, *InPath);
	if (bSuccessed)
	{
		IPlatformFile* LowerLevel = PakPlatformFile->GetLowerLevel();
		FPakFile* Pak = new FPakFile(LowerLevel, *PakPath, false);
		if (Pak)
		{
			UE_LOG(LogTemp, Display, TEXT("[%s]->GetMountPoint: %s"), *PakPath, *Pak->GetMountPoint());
			//delete Pak;
			Pak = nullptr;
		}

	}
}

void ASqliteGameModeBase::CreateBPActorNew()
{
	FString TempName = TEXT("/Game/Blueprint/TestActor.TestActor_C");
	UClass* ObjClass = StaticLoadClass(AActor::StaticClass(), NULL, *TempName);
	if (ObjClass)
	{
		FTransform Transform;
		Transform.SetLocation(FVector(50.f, 50.f, 90.f));
		GetWorld()->SpawnActor(ObjClass, &Transform);
	}
}

void ASqliteGameModeBase::TestCreateBPActor()
{
	FString TempName = TEXT("/Game/Blueprint/TestActor.TestActor_C");
	UObject* Obj = LoadObject<UObject>(NULL, *TempName);
	UClass* ObjClass = (UClass*)(Obj);
	if (ObjClass)
	{
		FTransform Transform;
		Transform.SetLocation(FVector(50.f, 50.f, 90.f));
		GetWorld()->SpawnActor(ObjClass, &Transform);
	}
}

void ASqliteGameModeBase::OnNewFileAddedDelegate(const FString& FilePath)
{
	UE_LOG(LogTemp, Display, TEXT("FilePath:%s"), *FilePath); 
	if (FilePath.EndsWith(TEXT("uasset")))
	{
		FString TempName = TEXT("/Game/Blueprint/TestActor.TestActor");
		FString PakPath = FString::Printf(TEXT("/Game/DownLoad/PAKs/TestPak/Blueprint/Blueprint_Win_TestPak.pak"), *FPaths::ProjectContentDir());
		UPackage * AssetPackage = CreatePackage(nullptr, *PakPath);
		UBlueprint* Blueprint = LoadObject<UBlueprint>(AssetPackage, *TempName);
		if (Blueprint)
		{
			UClass* ObjClass = (UClass*)(Blueprint->GeneratedClass);
			if (ObjClass)
			{
				FTransform Transform;
				Transform.SetLocation(FVector(50.f, 50.f, 90.f));
				GetWorld()->SpawnActor(ObjClass, &Transform);
			}
		}
	}
	
}