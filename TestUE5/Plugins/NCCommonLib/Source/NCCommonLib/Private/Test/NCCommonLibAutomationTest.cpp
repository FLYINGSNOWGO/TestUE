
#include "NCCommonLib.h"
#include "Misc/AutomationTest.h"
#include "NCCommonUtils.h"

#if WITH_EDITOR

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNCCommonLibAutomationTestInfo,
	"NCCommonLib.NCCommonUtils.StringToArrayChar",
	/*EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter)*/
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
	bool FNCCommonLibAutomationTestInfo::RunTest(const FString& Parameters)
{
	UE_LOG_NCCOMMONLIB_AUTOMATIONTEST(Log, TEXT("Start StringToArrayChar Test"));
	FString TestStr = TEXT("a b c d 1 5 a");
	TArray<const char*> OutAnsiStr;
	NPNCCommonUtils::StringToArrayChar(TestStr, TEXT(" "), OutAnsiStr);
	UE_LOG_NCCOMMONLIB_AUTOMATIONTEST(Log, TEXT("End StringToArrayChar Test"));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNCCommonLibAutomationTestTLexToStringInfo,
	"NCCommonLib.NCCommonUtils.TLexToString",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
	bool FNCCommonLibAutomationTestTLexToStringInfo::RunTest(const FString& Parameters)
{
	enum class ETestEnum : uint32
	{
		None,
		Enum1,
		Enum2,
		Count
	};
	auto LexToString = [](ETestEnum EnumVal) {
		static const TCHAR* Strings[] =
		{
			TEXT("ETestEnum::None"),
			TEXT("ETestEnum::Enum1"),
			TEXT("ETestEnum::Enum2")
		};
		return NPNCCommonUtils::TLexToString(EnumVal, Strings);
	};

	FString EnumValStr = LexToString(ETestEnum::Enum1);
	UE_LOG_NCCOMMONLIB_AUTOMATIONTEST(Log, TEXT("NPNCCommonUtils::TLexToString: %s"), *EnumValStr);
	return !EnumValStr.IsEmpty();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNCCommonLibAutomationTestCompressedStringInfo,
	"NCCommonLib.NCCommonUtils.CompressedString",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
	bool FNCCommonLibAutomationTestCompressedStringInfo::RunTest(const FString& Parameters)
{
	/*FString TestStr = TEXT("A three-dimensional scene is normally never band-limited when rendered with point	\
		samples.Edges of triangles, shadow boundaries, and other phenomena produce a					\
		signal that changes discontinuously and so produces frequencies that are infinite[252].			\
		Also, no matter how closely packed the samples are, objects can still be small enough			\
		that they do not get sampled at all.Thus, it is impossible to entirely avoid aliasing			\
		problems when using point samples to render a scene, and we almost always use point				\
		sampling.However, at times it is possible to know when a signal is band - limited.				\
		One example is when a texture is applied to a surface.It is possible to compute the				\
		frequency of the texture samples compared to the sampling rate of the pixel.If this				\
		frequency is lower than the Nyquist limit, then no special action is needed to properly			\
		sample the texture.If the frequency is too high, then a variety of algorithms are used			\
		to band - limit the texture");*/

	FString TestStr = TEXT("0.0,0.0,0.0,0.0,0.0,0.0,0.0,-0.163661,0.368486,-0.13123,-0.995739,0.653029,-0.224906,-0.082709,0.0,-1.570796,1.570796,-0.000243,1.570796,-0.0,0.000607,0.066851,0.000187,0.698324,0.698324,0.698337,-0.528612,0.000607,0.000607,0.000607,0.000585,0.0,0.0,0.0,0.0");

	TArray<uint8> CompressedDataTestStr;
	NPNCCommonUtils::CompressedString(TestStr, CompressedDataTestStr, ECompressionFlags::COMPRESS_BiasSpeed);

	FString SavePath = FPaths::ProjectSavedDir() / TEXT("TestStr");
	return FFileHelper::SaveArrayToFile(CompressedDataTestStr, *SavePath);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNCCommonLibAutomationTestReadCompressedStringInfo,
	"NCCommonLib.NCCommonUtils.ReadCompressedString",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
	bool FNCCommonLibAutomationTestReadCompressedStringInfo::RunTest(const FString& Parameters)
{
	FString SavePath = FPaths::ProjectSavedDir() / TEXT("TestStr");

	//Load the Compressed data array
	TArray<uint8> CompressedData;
	if (!FFileHelper::LoadFileToArray(CompressedData, *SavePath))
	{
		UE_LOG_NCCOMMONLIB_AUTOMATIONTEST(Error, TEXT("NPNCCommonUtils::ReadCompressedString: Invalid File:%s"), *SavePath);
		return false;
	}

	FString TestStr;
	NPNCCommonUtils::ReadCompressedString(CompressedData, TestStr, ECompressionFlags::COMPRESS_BiasSpeed);

	UE_LOG_NCCOMMONLIB_AUTOMATIONTEST(Log, TEXT("NPNCCommonUtils::ReadCompressedString: TestStr:%s"), *TestStr);
	return true;
}
#endif
