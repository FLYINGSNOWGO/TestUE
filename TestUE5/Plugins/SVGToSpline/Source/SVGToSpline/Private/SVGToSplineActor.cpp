/**
 * Copyright 2019 Neil Fang. All Rights Reserved.
 *
 * Animated Texture from GIF file
 *
 * Created by Neil Fang
 * GitHub: https://github.com/neil3d/UAnimatedTexture5
 *
*/


#include "SVGToSplineActor.h"
#include "SVGToSplineModule.h"

#include "lunasvg.h"
#include "tinyxml2.h"
#include "layoutcontext.h"

#include <algorithm>

namespace SVGToSplineHelper
{
	
};


#if WITH_EDITOR

#include "IImageWrapper.h"
#include "IImageWrapperModule.h"

void USVGToSpline::GetIconTexture(UTexture2D* &OutTexture)
{
	if (Icon)
	{
		OutTexture = Icon;
		return;
	}
	std::string filename = TCHAR_TO_UTF8(*SourceFile);
	std::uint32_t Width = 256, Height = 256;
	std::uint32_t bgColor = 0x00000000;

	auto Document = lunasvg::Document::loadFromFile(filename);
	if (!Document)
	{
		UE_LOG(LogSVGToSpline, Error, TEXT("[GetTextureBySourceFile] loadFromFile faild;Filename:%s"), *SourceFile);
		return;
	}

	auto Bitmap = Document->renderToBitmap(Width, Height, bgColor);
	if (!Bitmap.valid())
	{
		UE_LOG(LogSVGToSpline, Error, TEXT("[GetTextureBySourceFile] renderToBitmap faild;Filename:%s"), *SourceFile);
		return;
	}
	OutTexture = UTexture2D::CreateTransient(Width, Height, PF_R8G8B8A8);
	if (OutTexture != nullptr)
	{
		void* TextureData = OutTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
		FMemory::Memcpy(TextureData, Bitmap.data(), Width* Height* 4);
		OutTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
		OutTexture->UpdateResource();
	}
	Icon = OutTexture;
}

bool USVGToSpline::ReadSVG(const FString& InSVGFile, TArray<FString>& OutClipPath)
{
	UE_LOG(LogSVGToSpline, Log, TEXT("[ReadSVG] Satrt"));
	tinyxml2::XMLDocument Doc;

	if (const auto& ErrorCode = Doc.LoadFile(TCHAR_TO_UTF8(*InSVGFile)) != tinyxml2::XMLError::XML_SUCCESS)
	{
		UE_LOG(LogSVGToSpline, Error, TEXT("[ReadSVG] load svg file faild; ErrorCode:%d;File:%s"), ErrorCode, *InSVGFile);
		return false;
	}

	for (tinyxml2::XMLNode* Node = Doc.FirstChildElement("svg")->FirstChild();
		Node;
		Node = Node->NextSibling())
	{
		UE_LOG(LogSVGToSpline, Log, TEXT("[ReadSVG] Node Value:%s"), UTF8_TO_TCHAR(Node->Value()));
		if (const tinyxml2::XMLElement* Element = Node->ToElement())
		{
			const char* Style = "failed";
			if (Element->QueryStringAttribute("style", &Style) == tinyxml2::XMLError::XML_SUCCESS)
			{
				UE_LOG(LogSVGToSpline, Log, TEXT("[ReadSVG] Element Style Value:%s"), UTF8_TO_TCHAR(Style));
				const char* AnimalPath = "failed";
				if (Element->QueryStringAttribute("d", &AnimalPath) == tinyxml2::XMLError::XML_SUCCESS)
				{
					UE_LOG(LogSVGToSpline, Log, TEXT("[ReadSVG] Element AnimalPath Value:%s"), UTF8_TO_TCHAR(AnimalPath));
				}
			}
		}
	}

	return true;
}


bool USVGToSpline::GetDataByFile(const FString& InSVGFile)
{
	UE_LOG(LogSVGToSpline, Log, TEXT("[GetDataByFile] Satrt"));
	std::string filename = TCHAR_TO_UTF8(*InSVGFile);
	auto Document = lunasvg::Document::loadFromFile(filename);
	if (!Document)
	{
		UE_LOG(LogSVGToSpline, Error, TEXT("[GetTextureBySourceFile] loadFromFile faild;Filename:%s"), *InSVGFile);
		return false;
	}

	auto ProcessLayoutObject = [this](const auto& LayoutObject) {
		if (!IsValid(this) || !LayoutObject.get())
			return;
		if (LayoutObject->id != lunasvg::LayoutId::Shape )
		{
			return;
		}
		lunasvg::LayoutShape* Shape = static_cast<lunasvg::LayoutShape*>(LayoutObject.get());
		if (!Shape)
			return;
		auto Cmds = Shape->path.commands();
		if (std::find(std::begin(Cmds), std::end(Cmds), lunasvg::PathCommand::Close) == std::end(Cmds))
		{
			FMotionPath MotionPath;
			auto ProcessPath = [](const auto& InPath, FMotionPath& OutMotionPath) {
				lunasvg::PathIterator It(InPath);
				while (!It.isDone())
				{
					FMotionData MotionData;
					std::array<lunasvg::Point, 3> P;
					switch (It.currentSegment(P))
					{
					case lunasvg::PathCommand::MoveTo:
					{
						OutMotionPath.StartPoint = FVector2D(P[0].x, P[0].y);
						MotionData.MotionType = EMotionType::MoveTo;
						MotionData.Datas.Add(FVector2D(P[0].x, P[0].y));
					}
						break;
					case lunasvg::PathCommand::LineTo:
					{
						MotionData.MotionType = EMotionType::LineTo;
						MotionData.Datas.Add(FVector2D(P[0].x, P[0].y));
					}
						break;
					case lunasvg::PathCommand::CubicTo:
					{
						MotionData.MotionType = EMotionType::CubicTo;
						MotionData.Datas.Add(FVector2D(P[0].x, P[0].y));
						MotionData.Datas.Add(FVector2D(P[1].x, P[1].y));
						MotionData.Datas.Add(FVector2D(P[2].x, P[2].y));
					}
						break;
					case lunasvg::PathCommand::Close:
						break;
					}
					OutMotionPath.MotionDatas.Add(MotionData);
					It.next();
				}
			};
			ProcessPath(Shape->path, MotionPath);
			auto ProcessEndPoint = [](FMotionPath& OutMotionPath) {
				int32 MaxMotionDataIndex = OutMotionPath.MotionDatas.Num() == 0 ? 0 : OutMotionPath.MotionDatas.Num() - 1;
				FMotionData LastMotionData = OutMotionPath.MotionDatas[MaxMotionDataIndex];
				int32 MaxPointIndex = LastMotionData.Datas.Num() == 0 ? 0 : LastMotionData.Datas.Num() - 1;
				//TODO:Ã»¿¼ÂÇclose
				OutMotionPath.EndPoint = LastMotionData.Datas[MaxPointIndex];
			};
			ProcessEndPoint(MotionPath);
			MotionPaths.Add(MotionPath);
		}
	};
	const auto& Children = Document->root->children;
	std::for_each(std::begin(Children), std::end(Children), ProcessLayoutObject);
	UE_LOG(LogSVGToSpline, Log, TEXT("[GetDataByFile] End"));
	return true;
}


bool USVGToSpline::UpdateData()
{
	MotionPaths.Empty();
	CurPoint = FVector2D::ZeroVector;
	if (!IFileManager::Get().Delete(*SourceFile, false, false, true))
	{
		UE_LOG(LogSVGToSpline, Error, TEXT("[UpdateData] Delete Old File:%s"), *SourceFile);
		return false;
	}
	if (IFileManager::Get().Copy(*SourceFile, *OriginalFile) != ECopyResult::COPY_OK)
	{
		UE_LOG(LogSVGToSpline, Error, TEXT("[UpdateData] Copy faild!"));
		return false;
	}
	return GetDataByFile(SourceFile);
}

void USVGToSpline::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	bool RequiresNotifyMaterials = false;
	bool ResetAnimState = false;

	FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	if (PropertyThatChanged)
	{
		const FName PropertyName = PropertyThatChanged->GetFName();

		// static const FName SupportsTransparencyName = GET_MEMBER_NAME_CHECKED(UAnimatedTexture2D, SupportsTransparency);

		// if (PropertyName == SupportsTransparencyName)
		// {
		// 	RequiresNotifyMaterials = true;
		// 	ResetAnimState = true;
		// }
	}// end of if(prop is valid)

	// if (ResetAnimState)
	// {
	// 	FrameDelay = RenderFrameToTexture();
	// 	FrameTime = 0;
	// }

	// if (RequiresNotifyMaterials)
	// 	NotifyMaterials();
}

#endif // WITH_EDITOR


void USVGToSpline::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
}

void USVGToSpline::ImportFile(const FString& Filename)
{
	OriginalFile = FPaths::ConvertRelativePathToFull(*Filename);
	UE_LOG(LogSVGToSpline, Log, TEXT("[ImportFile] Filename:%s"), *OriginalFile);
	FString ShortPakName = FPackageName::GetShortName(OriginalFile);
	SourceFile = FPaths::ProjectPluginsDir() / TEXT("SVGToSpline/Resources/Editor") / ShortPakName;
	UpdateData();
}
