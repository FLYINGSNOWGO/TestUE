/**
 * Copyright 2019 Neil Fang. All Rights Reserved.
 *
 * Animated Texture from GIF file
 *
 * Created by Neil Fang
 * GitHub: https://github.com/neil3d/UAnimatedTexture5
 *
*/


#include "SVGToSplineThumbnailRenderer.h"
#include "SVGToSplineActor.h"

#include "CanvasTypes.h"	// Engine
#include "CanvasItem.h"	// Engine
#include "Engine/Texture2D.h"	// Engine
#include "ThumbnailRendering/ThumbnailManager.h"	// UnrealEd

#include "Misc/FileHelper.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"

namespace SVGToSplineThumbnailHelper
{
	//根据文件后缀返回对应IImageWrapper
	TSharedPtr<IImageWrapper> GetImageWrapperByExtention(const FString InImagePath)
	{
		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		if (InImagePath.EndsWith(".png"))
		{
			return ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
		}
		else if (InImagePath.EndsWith(".jpg") || InImagePath.EndsWith(".jpeg"))
		{
			return ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
		}
		else if (InImagePath.EndsWith(".bmp"))
		{
			return ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP);
		}
		else if (InImagePath.EndsWith(".ico"))
		{
			return ImageWrapperModule.CreateImageWrapper(EImageFormat::ICO);
		}
		else if (InImagePath.EndsWith(".exr"))
		{
			return ImageWrapperModule.CreateImageWrapper(EImageFormat::EXR);
		}
		else if (InImagePath.EndsWith(".icns"))
		{
			return ImageWrapperModule.CreateImageWrapper(EImageFormat::ICNS);
		}
		return nullptr;
	}

	UTexture2D* LoadTexture2D(const FString& ImagePath, bool& IsValid, int32& OutWidth, int32& OutHeight)
	{
		UTexture2D* Texture = nullptr;
		IsValid = false;
		if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*ImagePath))
		{
			return nullptr;
		}
		TArray<uint8> CompressedData;
		if (!FFileHelper::LoadFileToArray(CompressedData, *ImagePath))
		{
			return nullptr;
		}
		TSharedPtr<IImageWrapper> ImageWrapper = GetImageWrapperByExtention(ImagePath);
		if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(CompressedData.GetData(), CompressedData.Num()))
		{
			TArray<uint8> UncompressedRGBA;
			if (ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, UncompressedRGBA))
			{
				Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_R8G8B8A8);
				if (Texture != nullptr)
				{
					IsValid = true;
					OutWidth = ImageWrapper->GetWidth();
					OutHeight = ImageWrapper->GetHeight();
					void* TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
					FMemory::Memcpy(TextureData, UncompressedRGBA.GetData(), UncompressedRGBA.Num());
					Texture->GetPlatformData()->Mips[0].BulkData.Unlock();
					Texture->UpdateResource();
				}
			}
		}
		return Texture;

	}
};

void USVGToSplineThumbnailRenderer::Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* Viewport, FCanvas* Canvas, bool bAdditionalViewFamily)
{
	USVGToSpline* SVGToSplineActor = Cast<USVGToSpline>(Object);
	if (SVGToSplineActor != nullptr)
	{
		 //If support transparentcy, draw a checkerboard background first.
		UTexture2D* Icon = nullptr;
		SVGToSplineActor->GetIconTexture(Icon);
		if (!Icon)
		{
			FString IconPath = FPaths::ProjectPluginsDir() / TEXT("SVGToSpline/Resources/EditorIcon.png");
			bool IsValid = false;
			int32 OutWidth;
			int32 OutHeight;
			Icon = SVGToSplineThumbnailHelper::LoadTexture2D(IconPath, IsValid, OutWidth, OutHeight);
		}
		const int32 CheckerDensity = 8;
		//FString IconPath = FPaths::ProjectPluginsDir() / TEXT("SVGToSpline/Resources/EditorIcon.png");
		UTexture2D* Checker = UThumbnailManager::Get().CheckerboardTexture;
		Canvas->DrawTile(
			0.0f, 0.0f, Width, Height,							// Dimensions
			0.0f, 0.0f, CheckerDensity, CheckerDensity,			// UVs
			FLinearColor::White, Checker->GetResource());			// Tint & Texture

		// Use A canvas tile item to draw
		FCanvasTileItem CanvasTile(FVector2D(X, Y), Icon->GetResource(), FVector2D(Width, Height), FLinearColor::White);
		CanvasTile.BlendMode = SE_BLEND_Opaque;
		CanvasTile.Draw(Canvas);
	}
}