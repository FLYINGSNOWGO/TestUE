/**
 * Copyright 2019 Neil Fang. All Rights Reserved.
 *
 * Animated Texture from GIF file
 *
 * Created by Neil Fang
 * GitHub: https://github.com/neil3d/UAnimatedTexture5
 *
*/

#include "SVGToSplineModule.h"

#define LOCTEXT_NAMESPACE "FSVGToSplineModule"

void FSVGToSplineModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FSVGToSplineModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
DEFINE_LOG_CATEGORY(LogSVGToSpline);
IMPLEMENT_MODULE(FSVGToSplineModule, SVGToSpline)