// Copyright Epic Games, Inc. All Rights Reserved.

#include "SqliteLoadingScreen.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "MoviePlayer.h"
#include "Widgets/Images/SThrobber.h"
#include "Runtime/Engine/Classes/Engine/GameEngine.h"
#include "Runtime/Engine/Public/TimerManager.h"

// This module must be loaded "PreLoadingScreen" in the .uproject file, otherwise it will not hook in time!
struct FSqliteLoadingScreenBrush : public FSlateDynamicImageBrush, public FGCObject
{
	FSqliteLoadingScreenBrush(const FName InTextureName, const FVector2D& InImageSize)
		: FSlateDynamicImageBrush(InTextureName, InImageSize)
	{
		SetResourceObject(LoadObject<UObject>(NULL, *InTextureName.ToString()));
	}

	virtual void AddReferencedObjects(FReferenceCollector& Collector)
	{
		if (UObject* CachedResourceObject = GetResourceObject())
		{
			Collector.AddReferencedObject(CachedResourceObject);
		}
	}
};

class SSqliteLoadingScreen : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSqliteLoadingScreen) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		// Load version of the logo with text baked in, path is hardcoded because this loads very early in startup
		static const FName LoadingScreenName(TEXT("/Game/UI/T_Sqlite_TransparentLogo.T_Sqlite_TransparentLogo"));

		LoadingScreenBrush = MakeShareable(new FSqliteLoadingScreenBrush(LoadingScreenName, FVector2D(1024, 256)));
		
		FSlateBrush *BGBrush = new FSlateBrush();
		BGBrush->TintColor = FLinearColor(0.034f, 0.034f, 0.034f, 1.0f);

		ChildSlot
			[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SBorder)	
				.BorderImage(BGBrush)
			]
			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SImage)
				.Image(LoadingScreenBrush.Get())
			]
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.VAlign(VAlign_Bottom)
				.HAlign(HAlign_Right)
				.Padding(FMargin(10.0f))
				[
					SNew(SThrobber)
					.Visibility(this, &SSqliteLoadingScreen::GetLoadIndicatorVisibility)
				]
			]
		];
	}

private:
	/** Rather to show the ... indicator */
	EVisibility GetLoadIndicatorVisibility() const
	{
		bool Vis =  GetMoviePlayer()->IsLoadingFinished();
		if (Vis)
		{
			UE_LOG(LogTemp,Display,TEXT("完成loading,关闭UI"));
		}
		return GetMoviePlayer()->IsLoadingFinished() ? EVisibility::Collapsed : EVisibility::Visible;
	}
	
	/** Loading screen image brush */
	TSharedPtr<FSlateDynamicImageBrush> LoadingScreenBrush;
};

class FSqliteLoadingScreenModule : public ISqliteLoadingScreenModule
{
private:
	TWeakPtr<class SWindow> MainWindow;
	TSharedPtr<SSqliteLoadingScreen> LoadingScreen;
	TWeakPtr<SWidget> OrgContent;
	FTimerHandle TimerHandle;
public:
	virtual void StartupModule() override
	{
		// Force load for cooker reference
		LoadObject<UObject>(nullptr, TEXT("/Game/UI/T_Sqlite_TransparentLogo.T_Sqlite_TransparentLogo") );

		if (IsMoviePlayerEnabled())
		{
			//CreateScreen();
		}
	}
	
	virtual bool IsGameModule() const override
	{
		return true;
	}

	virtual void StartInGameLoadingScreen(bool bPlayUntilStopped, float PlayTime) override
	{
		/*FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = !bPlayUntilStopped;
		LoadingScreen.bWaitForManualStop = bPlayUntilStopped;
		LoadingScreen.bAllowEngineTick = bPlayUntilStopped;
		LoadingScreen.MinimumLoadingScreenDisplayTime = PlayTime;
		LoadingScreen.WidgetLoadingScreen = SNew(SSqliteLoadingScreen);
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);*/
		CreateScreen();
	}

	virtual void CreateScreen()
	{
		GEngine->AddOnScreenDebugMessage(0,3,FColor::Red,TEXT("!!!!!CreateScreen!!!!!!"));
		UGameEngine* GameEngine = Cast<UGameEngine>(GEngine);
		if (GameEngine)
		{
			TSharedPtr<class SViewport> GameViewportWidget = GameEngine->GameViewportWidget;

			LoadingScreen = SNew(SSqliteLoadingScreen);
			if (GameViewportWidget.IsValid())
			{
				OrgContent = GameViewportWidget->GetContent();
				GameViewportWidget->SetContent(LoadingScreen.ToSharedRef());
				GWorld->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=](){
					StopInGameLoadingScreen();

				}), 10.f, false);
			}
		}
	}

	void StopInGameLoadingScreen();

};

void FSqliteLoadingScreenModule::StopInGameLoadingScreen()
{
	UE_LOG(LogTemp, Display, TEXT("SqliteLoadingScreen->StopInGameLoadingScreen"));
	UGameEngine* GameEngine = Cast<UGameEngine>(GEngine);
	if (GameEngine)
	{
		TSharedPtr<class SViewport> GameViewportWidget = GameEngine->GameViewportWidget;
		if (GameViewportWidget.IsValid())
		{
			GameViewportWidget->SetContent(OrgContent.Pin());
			GWorld->GetTimerManager().ClearTimer(TimerHandle);
		}
	}
}

IMPLEMENT_GAME_MODULE(FSqliteLoadingScreenModule, SqliteLoadingScreen);