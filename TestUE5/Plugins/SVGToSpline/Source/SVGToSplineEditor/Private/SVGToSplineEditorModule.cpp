/**
 * Copyright 2019 Neil Fang. All Rights Reserved.
 *
 * Animated Texture from GIF file
 *
 * Created by Neil Fang
 * GitHub: https://github.com/neil3d/USVGToSpline5
 *
*/

#include "SVGToSplineEditorModule.h"
#include "SVGToSplineThumbnailRenderer.h"
#include "SVGToSplineActor.h"
#include "Misc/CoreDelegates.h"	// Core
#include "ThumbnailRendering/ThumbnailManager.h"	// UnrealEd

#define LOCTEXT_NAMESPACE "FSVGToSplineEditorModule"

void FSVGToSplineEditorModule::StartupModule()
{
	FCoreDelegates::OnPostEngineInit.AddRaw(this, &FSVGToSplineEditorModule::OnPostEngineInit);
}

void FSVGToSplineEditorModule::OnPostEngineInit()
{
	UThumbnailManager::Get().RegisterCustomRenderer(USVGToSpline::StaticClass(), USVGToSplineThumbnailRenderer::StaticClass());
}

void FSVGToSplineEditorModule::ShutdownModule()
{
	if (UObjectInitialized())
	{
		UThumbnailManager::Get().UnregisterCustomRenderer(USVGToSpline::StaticClass());
	}
}

#undef LOCTEXT_NAMESPACE

DEFINE_LOG_CATEGORY(LogSVGToSplineEditor);
IMPLEMENT_MODULE(FSVGToSplineEditorModule, SVGToSplineEditor)