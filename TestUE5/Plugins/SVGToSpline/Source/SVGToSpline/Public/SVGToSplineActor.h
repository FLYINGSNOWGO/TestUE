/**
 * Copyright 2022 CarlZhou. All Rights Reserved.
 *
 * SVGToSpline
 *
 * Created by CarlZhou
 *
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CommonType.h"
#include "SVGToSplineActor.generated.h"


/**
 * SVGToSplineActor
 * @see class UTexture2D
 */
UCLASS(BlueprintType, Category = SVGToSpline)
class SVGTOSPLINE_API USVGToSpline : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
		FString SourceFile;
	UPROPERTY()
	FString OriginalFile;
	UPROPERTY()
		UTexture2D* Icon;
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		TArray<FMotionPath> MotionPaths;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		FVector2D CurPoint;
public:	// UObject Interface.
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	void GetIconTexture(UTexture2D* &OutTexture);
	bool ReadSVG(const FString& InSVGFile, TArray<FString>& OutClipPath);
	bool GetDataByFile(const FString& InSVGFile);
	bool UpdateData();
#endif // WITH_EDITOR

public:
	//~ Begin UObject Interface.
	virtual void Serialize(FArchive& Ar) override;
	//~ End UObject Interface.
	void ImportFile(const FString& Filename);

};
