#include "Misc/AutomationTest.h"
#include "SVGToSplineEditorModule.h"

#if WITH_EDITOR

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSVGToSplineEditorAutomationTestReadSVGInfo,
	"SVGToSplineEditor.ReadSVG",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
	bool FSVGToSplineEditorAutomationTestReadSVGInfo::RunTest(const FString& Parameters)
{
	UE_LOG(LogSVGToSplineEditor,Log, TEXT("[ReadSVG] Satrt"));
	UE_LOG(LogSVGToSplineEditor,Log, TEXT("[ReadSVG] End"));
	return true;
}

#endif
