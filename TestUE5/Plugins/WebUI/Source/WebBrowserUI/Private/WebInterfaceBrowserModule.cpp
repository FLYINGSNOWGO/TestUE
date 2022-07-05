// Engine/Source/Runtime/WebBrowser/Private/WebBrowserModule.cpp

#include "WebInterfaceBrowserModule.h"
#include "WebInterfaceBrowserLog.h"
#include "WebInterfaceBrowserSingleton.h"
#include "Misc/App.h"
#include "Misc/EngineVersion.h"
#if WITH_CEF3
#	include "CEF3Utils.h"
#	if PLATFORM_MAC
#		include "include/wrapper/cef_library_loader.h"
#		define CEF3_BIN_DIR TEXT("Binaries/ThirdParty/CEF3")
#		define CEF3_FRAMEWORK_DIR CEF3_BIN_DIR TEXT("/Mac/Chromium Embedded Framework.framework")
#		define CEF3_FRAMEWORK_EXE CEF3_FRAMEWORK_DIR TEXT("/Chromium Embedded Framework")
#	endif
#endif

DEFINE_LOG_CATEGORY(LogWebInterfaceBrowser);

static FWebInterfaceBrowserSingleton* WebBrowserSingleton = nullptr;

FWebInterfaceBrowserInitSettings::FWebInterfaceBrowserInitSettings()
	: ProductVersion(FString::Printf(TEXT("%s/%s UnrealEngine/%s Chrome/84.0.4147.38"), FApp::GetProjectName(), FApp::GetBuildVersion(), *FEngineVersion::Current().ToString()))
{
}

class FWebInterfaceBrowserModule : public IWebInterfaceBrowserModule
{
private:
	// IModuleInterface Interface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

public:
	virtual bool IsWebModuleAvailable() const override;
	virtual IWebInterfaceBrowserSingleton* GetSingleton() override;
	virtual bool CustomInitialize(const FWebInterfaceBrowserInitSettings& WebBrowserInitSettings) override;

private:
#if WITH_CEF3
	bool bLoadedCEFModule = false;
#if PLATFORM_MAC
	// Dynamically load the CEF framework library.
	CefScopedLibraryLoader *CEFLibraryLoader = nullptr;
#endif
#endif
};

IMPLEMENT_MODULE( FWebInterfaceBrowserModule, WebBrowserUI );

void FWebInterfaceBrowserModule::StartupModule()
{
#if WITH_CEF3
	bLoadedCEFModule = CEF3Utils::LoadCEF3Modules(true);
#if PLATFORM_MAC
	// Dynamically load the CEF framework library into this dylibs memory space.
	// CEF now loads function pointers at runtime so we need this to be dylib specific.
	CEFLibraryLoader = new CefScopedLibraryLoader();
	
	FString CefFrameworkPath(FPaths::Combine(*FPaths::EngineDir(), CEF3_FRAMEWORK_EXE));
	CefFrameworkPath = FPaths::ConvertRelativePathToFull(CefFrameworkPath);
	
	bool bLoaderInitialized = false;
	if (!CEFLibraryLoader->LoadInMain(TCHAR_TO_ANSI(*CefFrameworkPath)))
	{
			UE_LOG(LogWebInterfaceBrowser, Error, TEXT("Chromium loader initialization failed"));
	}
#endif // PLATFORM_MAC
#endif
}

void FWebInterfaceBrowserModule::ShutdownModule()
{
	if (WebBrowserSingleton != nullptr)
	{
		delete WebBrowserSingleton;
		WebBrowserSingleton = nullptr;
	}

#if WITH_CEF3
	CEF3Utils::UnloadCEF3Modules();
#if PLATFORM_MAC
	delete CEFLibraryLoader;
	CEFLibraryLoader = nullptr;
#endif // PLATFORM_MAC
#endif
}

bool FWebInterfaceBrowserModule::CustomInitialize(const FWebInterfaceBrowserInitSettings& WebBrowserInitSettings)
{
	if (WebBrowserSingleton == nullptr)
	{
		WebBrowserSingleton = new FWebInterfaceBrowserSingleton(WebBrowserInitSettings);
		return true;
	}
	return false;
}

IWebInterfaceBrowserSingleton* FWebInterfaceBrowserModule::GetSingleton()
{
	if (WebBrowserSingleton == nullptr)
	{
		WebBrowserSingleton = new FWebInterfaceBrowserSingleton(FWebInterfaceBrowserInitSettings());
	}
	return WebBrowserSingleton;
}


bool FWebInterfaceBrowserModule::IsWebModuleAvailable() const
{
#if WITH_CEF3
	return bLoadedCEFModule;
#else
	return true;
#endif
}
