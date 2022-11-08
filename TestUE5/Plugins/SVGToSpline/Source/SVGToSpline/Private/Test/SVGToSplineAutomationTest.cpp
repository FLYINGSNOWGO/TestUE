#include "Misc/AutomationTest.h"
#include "SVGToSplineModule.h"
#include "tinyxml2.h"
#include "SVGToSplineActor.h"

#if WITH_EDITOR

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSVGToSplineAutomationTestReadSVGInfo,
	"SVGToSpline.ReadSVG",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
	bool FSVGToSplineAutomationTestReadSVGInfo::RunTest(const FString& Parameters)
{
	UE_LOG(LogSVGToSpline,Log, TEXT("[ReadSVG] Satrt"));
	USVGToSpline* SVGToSpline = NewObject<USVGToSpline>();
	SVGToSpline->AddToRoot();
	FString SVGFile = FPaths::ProjectDir() / TEXT("SVG/200131.svg");
	// FString SVGFile = FPaths::ProjectSavedDir() / TEXT("SVG/CrashContext.xml");
	TArray<FString> ClipPaths;
	bool bTestSuccess = SVGToSpline->ReadSVG(SVGFile,ClipPaths);
	UE_LOG(LogSVGToSpline,Log, TEXT("[ReadSVG] End"));
	SVGToSpline->RemoveFromRoot();
	return bTestSuccess;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSVGToSplineAutomationTestGetDataByFileInfo,
	"SVGToSpline.GetDataByFileTest",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
	bool FSVGToSplineAutomationTestGetDataByFileInfo::RunTest(const FString& Parameters)
{
	UE_LOG(LogSVGToSpline, Log, TEXT("[GetDataByFileTest] Satrt"));
	USVGToSpline* SVGToSpline = NewObject<USVGToSpline>();
	SVGToSpline->AddToRoot();
	FString SVGFile = FPaths::ProjectDir() / TEXT("SVG/200131.svg");
	
	bool bTestSuccess = SVGToSpline->GetDataByFile(SVGFile);
	UE_LOG(LogSVGToSpline, Log, TEXT("[GetDataByFileTest] End"));
	SVGToSpline->RemoveFromRoot();
	return bTestSuccess;
}

#endif
