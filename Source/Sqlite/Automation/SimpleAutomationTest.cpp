#include "Misc/AutomationTest.h"
#include "Log.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSimpleAutomationTestInfo,
	"Sqlit.SimpleAutomationTest.Test",
	/*EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter)*/
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
	bool FSimpleAutomationTestInfo::RunTest(const FString& Parameters)
{
	ULog::Info(TEXT("Sqlit.SimpleAutomationTest.Test!!!!"));
	return true;
}