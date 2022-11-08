/**
 * Copyright 2019 Neil Fang. All Rights Reserved.
 *
 * Animated Texture from GIF file
 *
 * Created by Neil Fang
 * GitHub: https://github.com/neil3d/UAnimatedTexture5
 *
*/

#include "SVGToSplineFactory.h"
#include "SVGToSplineEditorModule.h"
#include "SVGToSplineActor.h"

#include "Subsystems/ImportSubsystem.h"	// UnrealEd
#include "EditorFramework/AssetImportData.h"	// Engine
#include "Editor.h"	// UnrealEd

USVGToSplineFactory::USVGToSplineFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = false;
	bEditorImport = true;
	bEditAfterNew = true;
	SupportedClass = USVGToSpline::StaticClass();
	Formats.Add(TEXT("svg;SVG(Scalable Vector Graphics)"));

	// Required to checkbefore UReimportTextureFactory
	ImportPriority = DefaultImportPriority + 1;
}

bool USVGToSplineFactory::FactoryCanImport(const FString& Filename)
{
	FString Extension = FPaths::GetExtension(Filename, true);

	return Extension == TEXT(".svg");
}


UObject* USVGToSplineFactory::FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{
	UE_LOG(LogSVGToSplineEditor, Log, TEXT("USVGToSplineFactory: Importing %s ..."), *(InName.ToString()));
	check(InClass == USVGToSpline::StaticClass());

	GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPreImport(this, InClass, InParent, InName, TEXT("svg"));

	// if the texture already exists, remember the user settings
	USVGToSpline* ExistingTexture = FindObject<USVGToSpline>(InParent, *InName.ToString());
	if (ExistingTexture) {
		// TODO: use existing object settings
	}

	// FTextureReferenceReplacer RefReplacer(ExistingTexture);

	// call parent method to create/overwrite anim texture object
	USVGToSpline* SVGToSplineActor = Cast<USVGToSpline>(
		CreateOrOverwriteAsset(InClass, InParent, InName, Flags)
		);
	if (SVGToSplineActor == nullptr) {
		UE_LOG(LogSVGToSplineEditor, Error, TEXT("Create SVGToSplineActor FAILED, Name=%s."), *(InName.ToString()));
		return nullptr;
	}

	// just copy file blob to AnimTexture object

	SVGToSplineActor->ImportFile(Filename);

	// //Replace the reference for the new texture with the existing one so that all current users still have valid references.
	// RefReplacer.Replace(AnimTexture);

	GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPostImport(this, SVGToSplineActor);

	//occurs if you import over an existing SVGToSplineActor
	SVGToSplineActor->PostEditChange();

	return SVGToSplineActor;
}

bool USVGToSplineFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames)
{
	USVGToSpline* SVGToSpline = Cast<USVGToSpline>(Obj);
	return SVGToSpline != nullptr;
}

void USVGToSplineFactory::SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths)
{
	if (!Obj || !Obj->IsA(USVGToSpline::StaticClass()))
	{
		return;
	}

	USVGToSpline* SVGToSpline = Cast<USVGToSpline>(Obj);
	if (SVGToSpline)
	{
		SVGToSpline->ImportFile(NewReimportPaths[0]);
	}
}

EReimportResult::Type USVGToSplineFactory::Reimport(UObject* Obj)
{
	if (!Obj || !Obj->IsA(USVGToSpline::StaticClass()))
	{
		return EReimportResult::Failed;
	}

	USVGToSpline* SVGToSpline = Cast<USVGToSpline>(Obj);
	if (SVGToSpline)
	{
		bool bUpdateData = SVGToSpline->UpdateData();
		return bUpdateData ? EReimportResult::Succeeded : EReimportResult::Failed;
	}
	return EReimportResult::Failed;
}

int32 USVGToSplineFactory::GetPriority() const
{
	return ImportPriority;
}
