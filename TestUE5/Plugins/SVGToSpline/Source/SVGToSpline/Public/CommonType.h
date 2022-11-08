#pragma once
#include "CoreMinimal.h"
#include "Templates/UniquePtr.h"
#include "CommonType.generated.h"

class USVGToSpline;

UENUM(BlueprintType)
enum class EMotionType : uint8
{
	MoveTo,
	LineTo,
	CubicTo,
	Close
};

UENUM(BlueprintType)
enum class EMotionStatus : uint8
{
	OK,
	PathSegmentError,
	PathError,
	PathsError
};

USTRUCT(BlueprintType)
struct FMotionData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		EMotionType MotionType;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		TArray<FVector2D> Datas;
};

USTRUCT(BlueprintType)
struct FMotionPath
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		FVector2D StartPoint;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		TArray<FMotionData> MotionDatas;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		FVector2D EndPoint;

};

//USTRUCT()
//struct FSVGTask
//{
//	GENERATED_BODY()
//	UPROPERTY()
//		TObjectPtr<USVGToSpline> SVGToSpline;
//}

DECLARE_DELEGATE_TwoParams(FNotifyUpdateMotionPoint, const EMotionType, const FVector2D&);
DECLARE_DELEGATE_TwoParams(FNotifyCompleteCmd, const EMotionStatus, const FVector2D&);