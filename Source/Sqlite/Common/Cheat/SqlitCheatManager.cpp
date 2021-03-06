// Fill out your copyright notice in the Description page of Project Settings.


#include "SqlitCheatManager.h"
#include "Runtime/PakFile/Public/IPlatformFilePak.h"
#include "Runtime/Core/Public/HAL/PlatformFilemanager.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Misc/PackageName.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Common/Runtime/SqliteGameInstance.h"
#include "Log.h"
#include "Common/Runtime/SqliteCharacter.h"
#include "Common/Runtime/SqlitPlayerController.h"
#include "Engine/NetConnection.h"

#include "JsonReaderExtension.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "ProfilingDebugging/ABTesting.h"
#if PLATFORM_ANDROID
#include "Android/AndroidPlatformHttp.h"
#else
#include "Windows/WindowsPlatformHttp.h"
#endif
#include "Misc/MessageDialog.h"
#include "Blueprint/UserWidget.h"

#include "TestEventThread.h"
#include "Common/DownLoad/DownloadManager.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/Level.h"

void USqlitCheatManager::TestMountTestMapPak()
{
	FString PakPath = FString::Printf(TEXT("%s/DownLoad/PAKs/TestPak/TestMapPak/TestMapPak_Win_TestPak.pak"), *FPaths::ProjectDir());
	IPlatformFile* InnerPlatform = FPlatformFileManager::Get().FindPlatformFile(FPakPlatformFile::GetTypeName());
	FPakPlatformFile* PakPlatformFile = (FPakPlatformFile*)InnerPlatform;
	FString InPath = FString::Printf(TEXT("%sTestMapPak/"), *FPaths::ProjectContentDir());

	bool bSuccessed = PakPlatformFile->Mount(*PakPath, 0, *InPath);
	if (bSuccessed)
	{
		UE_LOG(LogTemp, Display, TEXT("Mount successed: %s"), *PakPath);
	}
	
}

void USqlitCheatManager::TestMountTestMapPakUseCustomPakFile()
{
	FString PakPath = FString::Printf(TEXT("%s/DownLoad/PAKs/TestPakByCustomPakFile/TestMapPak/TestMapPak_Win_TestPakByCustomPakFile.pak"), *FPaths::ProjectDir());
	USqliteGameInstance* SqliteGameInstance = Cast<USqliteGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!SqliteGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("SqliteGameInstance is invalid"));
		return;
	}
	
	FPakPlatformFile* PakPlatformFile = SqliteGameInstance->GetClientPakPlatformFile();
	FString InPath = FString::Printf(TEXT("%sTestMapPak/"), *FPaths::ProjectContentDir());

	bool bSuccessed = PakPlatformFile->Mount(*PakPath, 1000, *InPath);
	if (bSuccessed)
	{
		UE_LOG(LogTemp, Display, TEXT("Mount successed: %s"), *PakPath);
	}
}

void USqlitCheatManager::TestUseTestMapPakByUseCustomPakFile()
{
	FString LongPackageName = TEXT("/Game/TestMapPak/NewMap.NewMap");

	UWorld* PersistentWorld = GetWorld();
	if (!PersistentWorld)
	{
		UE_LOG(LogTemp, Fatal, TEXT("DynamicAddStreamingLevel Faild,PersistentWorld is invaild!!!"));
		return;
	}
	const FString ShortPackageName = FPackageName::GetShortName(LongPackageName);
	UE_LOG(LogTemp, Display, TEXT("ShortPackageName: %s"), *ShortPackageName);
	const FString PackagePath = FPackageName::GetLongPackagePath(LongPackageName);
	UE_LOG(LogTemp, Display, TEXT("PackagePath: %s"), *PackagePath);
	FString UniqueLevelPackageName = PackagePath + TEXT("/") + PersistentWorld->StreamingLevelsPrefix + ShortPackageName;
	UE_LOG(LogTemp, Display, TEXT("UniqueLevelPackageName: %s"), *UniqueLevelPackageName);
	ULevelStreamingDynamic* StreamingLevel = NewObject<ULevelStreamingDynamic>(PersistentWorld, ULevelStreamingDynamic::StaticClass(), NAME_None, RF_Transient, NULL);
	StreamingLevel->SetWorldAssetByPackageName(FName(*UniqueLevelPackageName));
	StreamingLevel->LevelColor = FColor::MakeRandomColor();
	StreamingLevel->LevelTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector);
	StreamingLevel->PackageNameToLoad = FName(*LongPackageName);

	/////////////////////////////////////
	StreamingLevel->SetShouldBeLoaded(false);
	StreamingLevel->SetShouldBeVisible(false);
	StreamingLevel->bShouldBlockOnLoad = false;
	/////////////////////////////////////

	PersistentWorld->AddUniqueStreamingLevel(StreamingLevel);

	FLatentActionInfo actionInfo;
	actionInfo.CallbackTarget = this;
	actionInfo.ExecutionFunction = FName(TEXT("OnClientStreamingLevelLoadedByUseCustomPakFile"));
	actionInfo.Linkage = 0;
	actionInfo.UUID = this->GetUniqueID();// FGuid::NewGuid();
	//UE_LOG(LogClientREPC, Log, TEXT("--Client to async loadin sub level ,level name is :%s--"), *_MapName);
	UGameplayStatics::LoadStreamLevel(PersistentWorld, FName(TEXT("NewMap")), true, false, actionInfo);
}

void USqlitCheatManager::TestUseTestMapPak()
{
	FString LongPackageName = TEXT("/Game/TestMapPak/NewMap.NewMap");

	UWorld* PersistentWorld = GetWorld();
	if (!PersistentWorld)
	{
		UE_LOG(LogTemp, Fatal, TEXT("DynamicAddStreamingLevel Faild,PersistentWorld is invaild!!!"));
		return;
	}
	const FString ShortPackageName = FPackageName::GetShortName(LongPackageName);
	UE_LOG(LogTemp, Display, TEXT("ShortPackageName: %s"), *ShortPackageName);
	const FString PackagePath = FPackageName::GetLongPackagePath(LongPackageName);
	UE_LOG(LogTemp, Display, TEXT("PackagePath: %s"), *PackagePath);
	FString UniqueLevelPackageName = PackagePath + TEXT("/") + PersistentWorld->StreamingLevelsPrefix + ShortPackageName;
	UE_LOG(LogTemp, Display, TEXT("UniqueLevelPackageName: %s"), *UniqueLevelPackageName);
	ULevelStreamingDynamic* StreamingLevel = NewObject<ULevelStreamingDynamic>(PersistentWorld, ULevelStreamingDynamic::StaticClass(), NAME_None, RF_Transient, NULL);
	StreamingLevel->SetWorldAssetByPackageName(FName(*UniqueLevelPackageName));
	StreamingLevel->LevelColor = FColor::MakeRandomColor();
	StreamingLevel->LevelTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector);
	StreamingLevel->PackageNameToLoad = FName(*LongPackageName);

	///////////////////////////////////////
	StreamingLevel->SetShouldBeLoaded(false);
	StreamingLevel->SetShouldBeVisible(false);
	StreamingLevel->bShouldBlockOnLoad = false;
	///////////////////////////////////////
	// ??????????????????????????????load?????????????????????????????????load?????? [11/17/2021 CarlZhou]
	PersistentWorld->AddStreamingLevel(StreamingLevel);

	//FLatentActionInfo actionInfo;
	//actionInfo.CallbackTarget = this;
	//actionInfo.ExecutionFunction = FName(TEXT("OnClientStreamingLevelLoaded"));
	//actionInfo.Linkage = 0;
	//actionInfo.UUID = this->GetUniqueID();// FGuid::NewGuid();
	////UE_LOG(LogClientREPC, Log, TEXT("--Client to async loadin sub level ,level name is :%s--"), *_MapName);
	//UGameplayStatics::LoadStreamLevel(PersistentWorld, FName(TEXT("NewMap")), true, false, actionInfo);
}

void USqlitCheatManager::TestLoadMap()
{
	UWorld* PersistentWorld = GetWorld();
	if (!PersistentWorld)
	{
		UE_LOG(LogTemp, Fatal, TEXT("DynamicAddStreamingLevel Faild,PersistentWorld is invaild!!!"));
		return;
	}
	FLatentActionInfo actionInfo;
	actionInfo.CallbackTarget = this;
	actionInfo.ExecutionFunction = FName(TEXT("OnClientStreamingLevelLoaded"));
	actionInfo.Linkage = 0;
	actionInfo.UUID = this->GetUniqueID();// FGuid::NewGuid();
	//UE_LOG(LogClientREPC, Log, TEXT("--Client to async loadin sub level ,level name is :%s--"), *_MapName);
	UGameplayStatics::LoadStreamLevel(PersistentWorld, FName(TEXT("NewMap")), true, false, actionInfo);
}

void USqlitCheatManager::OnClientStreamingLevelLoaded(int32 Linkage)
{
	UE_LOG(LogTemp, Display, TEXT("USqlitCheatManager::OnClientStreamingLevelLoaded"));
}

void USqlitCheatManager::OnClientStreamingLevelLoadedByUseCustomPakFile(int32 Linkage)
{
	UE_LOG(LogTemp, Display, TEXT("USqlitCheatManager::OnClientStreamingLevelLoadedByUseCustomPakFile"));
	USqliteGameInstance* SqliteGameInstance = Cast<USqliteGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!SqliteGameInstance)
	{
		UE_LOG(LogTemp, Fatal, TEXT("SqliteGameInstance is invaild!!!"));
		return;
	}
}
#include <string>
void USqlitCheatManager::SetString(FString& OutString)
{
	std::string str = "12.23.23.11";
	//OutString = FString(str.c_str());
	OutString.Append(FString(str.c_str()));
}

void USqlitCheatManager::TestPaths()
{
	UE_LOG(LogTemp, Display, TEXT("FPaths::RootDir: %s"), *FPaths::RootDir());
	UE_LOG(LogTemp, Display, TEXT("FPaths::ProjectDir: %s"),*FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()));
	UE_LOG(LogTemp, Display, TEXT("FPaths::ProjectContentDir: %s"), *FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir()));
}

void USqlitCheatManager::TestExit()
{
	FScopeLog TestExit(TEXT("USqlitCheatManager::TestExit"));
	FPlatformMisc::RequestExit(true);
}

void USqlitCheatManager::TestInstalledGameContentDir()
{
	FString InstalledGameContentDir = FPaths::Combine(*FPaths::ProjectPersistentDownloadDir(), TEXT("InstalledContent"), FApp::GetProjectName(), TEXT("Content"), TEXT("Paks"));
	FPlatformMisc::AddAdditionalRootDirectory(FPaths::Combine(*FPaths::ProjectPersistentDownloadDir(), TEXT("InstalledContent")));

	ULog::Info(FString::Printf(TEXT("InstalledGameContentDir:%s"),*InstalledGameContentDir),ELoggingOptions::LO_Both);
	TArray<FString> PakFolders;
	FString CmdStr = FString::Printf(TEXT("-pakdir=%s"), *InstalledGameContentDir);
	ULog::Info(FString::Printf(TEXT("CmdStr:%s"), *CmdStr), ELoggingOptions::LO_Both);

	FPakPlatformFile::GetPakFolders(*CmdStr, PakFolders);
	ULog::Info(FString::Printf(TEXT("PakFolders Num:%d"), PakFolders.Num()), ELoggingOptions::LO_Both);

	for (FString& PakDir : PakFolders)
	{
		ULog::Info(FString::Printf(TEXT("PakDir:%s"), *PakDir), ELoggingOptions::LO_Both);
	}
	
	FCoreDelegates::OnMountAllPakFiles.Execute(PakFolders);
}

void USqlitCheatManager::TestPlatformFileAddress()
{
	for (IPlatformFile* ChainElement = &FPlatformFileManager::Get().GetPlatformFile(); ChainElement; ChainElement = ChainElement->GetLowerLevel())
	{
		ULog::Info(FString::Printf(TEXT("Platform File Address:%p;"), ChainElement), ELoggingOptions::LO_Both);
	}
	ULog::Info(FString::Printf(TEXT("FPlatformFileManager::Get().GetPlatformFile Address:%p;"), &FPlatformFileManager::Get().GetPlatformFile()), ELoggingOptions::LO_Both);
	ULog::Info(FString::Printf(TEXT("FPlatformFileManager::Get().GetPlatformFilePakFile Address:%p;"), FPlatformFileManager::Get().GetPlatformFile(TEXT("PakFile"))), ELoggingOptions::LO_Both);
	ULog::Info(FString::Printf(TEXT("FPlatformFileManager::Get().GetPlatformFilePakFile->GetLowerLevel Address:%p;"), FPlatformFileManager::Get().GetPlatformFile(TEXT("PakFile"))->GetLowerLevel()), ELoggingOptions::LO_Both);
	ULog::Info(FString::Printf(TEXT("FPlatformFileManager::Get().GetPlatformPhysical Address:%p;"), &IPlatformFile::GetPlatformPhysical()), ELoggingOptions::LO_Both);


	USqliteGameInstance* SqliteGameInstance = Cast<USqliteGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!SqliteGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("SqliteGameInstance is invalid"));
		return;
	}

	FPakPlatformFile* PakPlatformFile = SqliteGameInstance->GetClientPakPlatformFile();
	ULog::Info(FString::Printf(TEXT("FPlatformFileManager::Get().CustomPakFile Address:%p;"), PakPlatformFile), ELoggingOptions::LO_Both);
}

void USqlitCheatManager::TestCrashDump()
{
	IPlatformFile* ChainElement = nullptr;
	ULog::Info(FString::Printf(TEXT("Platform File Address:%p;Name:%s"), ChainElement, ChainElement->GetName()), ELoggingOptions::LO_Both);
}

void USqlitCheatManager::TestCharacterInfo()
{
	TArray<AActor*> OutAllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),ASqliteCharacter::StaticClass(), OutAllActors);
	ULog::Info(FString::Printf(TEXT("ASqliteCharacter Num:%d"), OutAllActors.Num()), ELoggingOptions::LO_Both);
	for (auto& Character : OutAllActors)
	{
		ULog::Info(FString::Printf(TEXT("ASqliteCharacter Name:%s"), *Character->GetFullName()), ELoggingOptions::LO_Both);
	}
}

void USqlitCheatManager::TestCloseNetConnection()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	if (PlayerController)
	{
		ULog::Info(FString::Printf(TEXT("Start Close Client Connection!!")),ELoggingOptions::LO_Both);
		PlayerController->GetNetConnection()->Close();
	}
	else
	{
		ULog::Error(FString::Printf(TEXT("PlayerController is invalid! Close Client Connection failed!!")), ELoggingOptions::LO_Both);
	}
}

void USqlitCheatManager::TestJsonExtension()
{
	FString JsonString = TEXT("{\"qs\":\"n\",\"form\":\"QBRE\",\"sp\":\"-1\",\"pq\":\"c++ const\",\"sc\":\"0-15\",\"sk\":\"\",\"cvid\":\"8C13355E58134813A69BFDA1EF0C7F4B\"}");
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactoryExtension<>::Create(JsonString);
	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		FString pq = TEXT("");
		if (JsonObject->TryGetStringField(TEXT("pq"), pq))
		{
			ULog::Info(FString::Printf(TEXT("pq:%s"),*pq), ELoggingOptions::LO_Both);
		}
	}
	JsonString = TEXT("{\"qs\":\"n\",\"form\":\"QBRE\",\"sp\":\"-1\",\"pq\":\"test hh\",\"sc\":\"0-15\",\"sk\":\"\",\"cvid\":\"8C13355E58134813A69BFDA1EF0C7F4B\"}");
	FJsonStringReaderExtension* JsonStringReaderExtension = static_cast<FJsonStringReaderExtension*>(&Reader.Get());
	JsonStringReaderExtension->UpdateJsonString(JsonString);
	JsonObject->Values.Empty();
	bool bDeserializeSucess = FJsonSerializer::Deserialize(Reader, JsonObject);
	if ( bDeserializeSucess && JsonObject.IsValid())
	{
		FString pq = TEXT("");
		if (JsonObject->TryGetStringField(TEXT("pq"), pq))
		{
			ULog::Info(FString::Printf(TEXT("pq:%s"), *pq), ELoggingOptions::LO_Both);
		}
	}
}

void USqlitCheatManager::TestTMap()
{
	static TMap<FString, FString> TestMap;
	TestMap.Add(TEXT("??"),TEXT("1"));
	ULog::Info(FString::Printf(TEXT("TestMap Num:%d"), TestMap.Num()), ELoggingOptions::LO_Both);
}

void USqlitCheatManager::TestCharacter()
{
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this,0);
	if (Character)
	{
		//Character->SetActorHiddenInGame(!Character->IsHidden());
		ULog::Info(FString::Printf(TEXT("Character is Hidden:%s"), Character->IsHidden() ? TEXT("true") : TEXT("false")), ELoggingOptions::LO_Both);
		TArray<UActorComponent*> OutActorComponents;
		Character->GetComponents(OutActorComponents);

		for (UActorComponent* ActorComponent : OutActorComponents)
		{
			if (IsValid(ActorComponent))
			{
				
				ActorComponent->SetComponentTickEnabled(!ActorComponent->IsComponentTickEnabled());
			}
		}
		Character->SetActorTickEnabled(!Character->IsActorTickEnabled());
	}
	else
	{
		ULog::Info(FString::Printf(TEXT("Character is invaild")), ELoggingOptions::LO_Both);
	}
}

void USqlitCheatManager::TestABTesting()
{
	FString Info = FABTest::StaticIsActive() ? TEXT("ABTest Is Active") : TEXT("ABTest Is not Active");
	ULog::Info(FString::Printf(TEXT("USqlitCheatManager::TestABTesting:%s"),*Info), ELoggingOptions::LO_Both);
}

void USqlitCheatManager::TestHttp(FString Url)
{
	const FString Domain = FPlatformHttp::GetUrlDomain(Url);
	ULog::Info(FString::Printf(TEXT("USqlitCheatManager::TestHttp:%s"), *Domain), ELoggingOptions::LO_Both);
}

void USqlitCheatManager::AsynShowMessageBox(FString Msg)
{
	AsyncTask(ENamedThreads::AnyThread, [Msg]() {
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(Msg));
	});
}

void USqlitCheatManager::TestAsynCreateUI()
{
	AsyncTask((ENamedThreads::Type)GSlateLoadingThreadId,[this](){
		UClass* WidgetClass = LoadClass<UUserWidget>(NULL, TEXT("WidgetBlueprint'/Game/UI/TestUI.TestUI_C'"));

		if (WidgetClass)
		{
			UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
			Widget->AddToViewport();
		}
	});
}

void USqlitCheatManager::TestStringToFString()
{
	FString Test = TEXT("");
	SetString(Test);
	ULog::Info(FString::Printf(TEXT("USqlitCheatManager::TestStringToFString:%s"), *Test), ELoggingOptions::LO_Both);
}

void USqlitCheatManager::TestArraySort()
{
	TArray<int> IntArray = {4,2,5};
	int32 InsertInt = 3;
	int32 InsertIndex = IntArray.IndexOfByPredicate([InsertInt](const int32 Element){
		return Element < InsertInt;
	});
	IntArray.InsertUninitialized(InsertIndex);
	IntArray[InsertIndex] = InsertInt;
	
	TArray<FStringFormatArg> FormatArray;
	for (auto& Item : IntArray)
	{
		FormatArray.Add(FStringFormatArg(Item));
	}
	FString LogInfo = FString::Format(TEXT("IntArray:{0},{1},{2},{3}"),FormatArray);
	ULog::Info(FString::Printf(TEXT("%s"), *LogInfo), ELoggingOptions::LO_Both);
}

void USqlitCheatManager::SetupTestEventThread()
{
	FTestEventThread::Get();
}

void USqlitCheatManager::TestEventThreadSuspend(bool bShouldSuspend)
{
	if (bShouldSuspend)
	{
		FTestEventThread::Get()->SuspendThread();
	}
	else
	{
		FTestEventThread::Get()->ResumedThread();
	}
}

void USqlitCheatManager::TestEventThreadExit()
{
	FTestEventThread::Get()->ExitThread();
}

void USqlitCheatManager::TestFEvent()
{
	FTestEventThreadHelper::Get();
}

void USqlitCheatManager::TestGameUserSettingConfig()
{
	FString Key = TEXT("bIs2cCloudiaProject");
	bool Value = true;
	GConfig->GetBool(TEXT("/Script/Engine.GameUserSettings"), *Key, Value, GGameUserSettingsIni);
	ULog::Info(FString::Printf(TEXT("Value:%s"), Value ? TEXT("True"): TEXT("False")), ELoggingOptions::LO_Both);

	FString KeyValue = TEXT("TestString");
	FString ValueStr;
	GConfig->GetString(TEXT("/Script/Engine.GameUserSettings"), *KeyValue, ValueStr, GGameUserSettingsIni);
	ULog::Info(FString::Printf(TEXT("Value:%s"), *ValueStr), ELoggingOptions::LO_Both);
}

void USqlitCheatManager::TestGetEngineVersion()
{
	ULog::Info(FString::Printf(TEXT("Test Get Engine Version:%s"), *UKismetSystemLibrary::GetEngineVersion()), ELoggingOptions::LO_Both);
}

void USqlitCheatManager::TestDownloadManager()
{
	AsyncTask(ENamedThreads::GameThread,[](){
		FDownloadManager::FDownloadFileEntry MapEntry;
		MapEntry.FileName = TEXT("UMap_Cloudia.pak");
		// TODO:Set the total size of the download file to support progress [3/7/2022 CarlZhou]
		//MapEntry.FileSize = 
		MapEntry.FileFullPath = FPaths::ProjectSavedDir() / TEXT("UMap_Cloudia.pak");
		MapEntry.RelativeUrl = TEXT("http://10.155.254.134:20172/smartmap/rod/storage/file/mapfile_2022_8c203b90fdfc45f79bfb9482d5b7177e.pak");
		//MapEntry.RemoteMapMd5 = RemoteMapMd5;
		MapEntry.DownloadFileCompleteDelegate.BindLambda([](const FString& FileFullPath) {
			ULog::Info(FString::Printf(TEXT("USqlitCheatManager::TestDownloadManager->FileFullPath:%s"), *FileFullPath), ELoggingOptions::LO_Both);
			});
		MapEntry.DownloadFileErrorDelegate.BindLambda([=](const FString& FileName, const FDownloadManager::EDownloadErrorType ErrorType) {
			FString Info = FString::Printf(TEXT("Client Download %s failed,Reasons:%s,please contact your system administrator"),
				*FileName,
				FDownloadManager::DownloadErrorTypeToString(ErrorType));
			ULog::Info(FString::Printf(TEXT("USqlitCheatManager::TestDownloadManager->%s"), *Info), ELoggingOptions::LO_Both);

			});

		MapEntry.DownloadFileWillRetryDelegate.BindLambda([](const FString& RetryFileName) {
			ULog::Warning(FString::Printf(TEXT("USqlitCheatManager::TestDownloadManager->RetryFileName:%s"), *RetryFileName), ELoggingOptions::LO_Both);
			// TODO:Retry the download interface prompt [3/4/2022 CarlZhou]
			});

		MapEntry.DownloadProgressDelegate.BindLambda([](const FString& FileName, const int32 BytesReceived, const int32 FileSize) {
			// TODO:Progress interface display [3/4/2022 CarlZhou]
			ULog::Info(FString::Printf(TEXT("USqlitCheatManager::TestDownloadManager->FileName:%s;BytesReceived:%d "), *FileName, BytesReceived), ELoggingOptions::LO_Both);
			});

		FDownloadManager::Get()->DownLoad(MapEntry);
	});
}

void USqlitCheatManager::TestGetCurLevelName()
{
	UWorld* PersistentWorld = GetWorld();
	if (!PersistentWorld)
	{
		UE_LOG(LogTemp, Fatal, TEXT("DynamicAddStreamingLevel Faild,PersistentWorld is invaild!!!"));
		return;
	}
	FString LevelName = PersistentWorld->GetCurrentLevel()->GetPathName();
	if (LevelName.Contains(TEXT("Map_Client_Main")))
	{
		ULog::Info(TEXT("Contains Map_Client_Main"));
	}
	FString UrlMap = PersistentWorld->GetCurrentLevel()->URL.Map;
	FString ShortName = FPackageName::GetShortName(UrlMap);
	ULog::Info(FString::Printf(TEXT("USqlitCheatManager::TestGetCurLevelName->LevelName:%s;UrlMap:%s;ShortName:%s "), *LevelName, *UrlMap,*ShortName), ELoggingOptions::LO_Both);
}

void USqlitCheatManager::TestCheck()
{
	checkf(false, TEXT("MarkPendingKill Error: %s is rooted"), *GetPathName());
}

void USqlitCheatManager::TestRPC()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		ULog::Info(FString::Printf(TEXT("Start Call DS RPC!!")), ELoggingOptions::LO_Both);
		if (ASqlitPlayerController* SPC = Cast<ASqlitPlayerController>(PlayerController))
		{
			SPC->ServerSetStartTransform(FTransform());
		}
	}
	else
	{
		ULog::Error(FString::Printf(TEXT("PlayerController is invalid! Close Client Connection failed!!")), ELoggingOptions::LO_Both);
	}
}

void USqlitCheatManager::TestLog()
{
	ULog::Info(FString::Printf(TEXT("USqlitCheatManager::TestLog!!")), ELoggingOptions::LO_Both);
	FString Str;
	FString Str1;

	UE_LOG(LogTemp,Log,TEXT("Str:%s;Str1:%s"), *Str, *Str1);
}

void USqlitCheatManager::TestT2U()
{
	FString Str;
	FString Str1;
	std::string r = TCHAR_TO_UTF8(*Str);
	std::string r1 = TCHAR_TO_UTF8(*Str1);
}

float CVar_ShooterRepGraph_DestructionInfoMaxDist = 30000.f;
static FAutoConsoleVariableRef CVarShooterRepGraphDestructMaxDist(TEXT("ShooterRepGraph.DestructInfo.MaxDist"), CVar_ShooterRepGraph_DestructionInfoMaxDist, TEXT("Max distance (not squared) to rep destruct infos at"), ECVF_Default);

static TAutoConsoleVariable<int32> CVarDrawSpritesAsTwoSided(TEXT("r.Paper2D.Test"), 1, TEXT("test tttttttt."));

void USqlitCheatManager::TestConsoleVariable()
{
	static IConsoleVariable* CVarRPaper2DTest = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Paper2D.Test"));
	ULog::Info(FString::Printf(TEXT("USqlitCheatManager::TestConsoleVariable:CVar_ShooterRepGraph_DestructionInfoMaxDist:%f;CVarRPaper2DTest:%d"), CVar_ShooterRepGraph_DestructionInfoMaxDist, CVarRPaper2DTest->GetInt()), ELoggingOptions::LO_Both);
}

void USqlitCheatManager::TestNumber()
{
	int32 P1 = 30;
	int32 P2 = 100;
	float P3 = P1 / P2;
	float P4 = (P1 * 1.f) / P2;
	ULog::Info(FString::Printf(TEXT("USqlitCheatManager::TestNumber->P3:%f;P4:%f"), P3, P4), ELoggingOptions::LO_Both);
}