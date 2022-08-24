//SimpleAutomationTest.cpp
#include "Misc/AutomationTest.h"
#include "DownloadManager.h"

using FDownloadFileEntry = FDownloadManager::FDownloadFileEntry;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FResourcesDownloaderAutoTest,
	"ResourcesDownloader.TestDownLoad",
	/*EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter)*/
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
	bool FResourcesDownloaderAutoTest::RunTest(const FString& Parameters)
{
	UE_LOG(LogDownloadManager, Log, TEXT("FResourcesDownloaderAutoTest::RunTest"));
	FDownloadFileEntry DownloadFileEntry;
	DownloadFileEntry.RelativeUrl = TEXT("https://172.16.23.85:32480/smartmap/rod/storage/file/mapfile_2022_3ee4f20d84d14c4e92c09e2dbe7c61d2.pak");
	DownloadFileEntry.FileName = TEXT("TestFile.pak");
	DownloadFileEntry.RemoteMd5 = TEXT("1e0bcb91a7a01aea5b696907e0b93140");
	DownloadFileEntry.FileSize = 24087;
	DownloadFileEntry.FileFullPath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("AutoDownload/TestFile.pak"));
	UE_LOG(LogDownloadManager, Log, TEXT("FResourcesDownloaderAutoTest::RunTest DownloadFileEntry.FileFullPath:%s"), *DownloadFileEntry.FileFullPath);
	FDownloadManager::Get()->DownLoad(DownloadFileEntry);
	return true;
}