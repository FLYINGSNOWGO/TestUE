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

const TCHAR* MotionTypeToString(const EMotionType MotionType)
{
	switch (MotionType)
	{
		case EMotionType::MoveTo:
			return TEXT("MoveTo");
		case EMotionType::LineTo:
			return	TEXT("LineTo");
		case EMotionType::CubicTo:
			return	TEXT("CubicTo");
		case EMotionType::Close:
			return TEXT("Close");
	}
	return TEXT("ErrorType");
}

UENUM(BlueprintType)
enum class EMotionStatus : uint8
{
	OK,
	PathSegmentError,
	PathError,
	PathsError
};
const TCHAR* MotionStatusToString(const EMotionStatus MotionStatus)
{
	switch (MotionStatus)
	{
	case EMotionStatus::OK:
		return TEXT("OK");
	case EMotionStatus::PathSegmentError:
		return	TEXT("PathSegmentError");
	case EMotionStatus::PathError:
		return	TEXT("PathError");
	case EMotionStatus::PathsError:
		return TEXT("PathsError");
	}
	return TEXT("ErrorType");
}

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

DECLARE_DELEGATE_ThreeParams(FNotifyUpdateMotionPoint, const EMotionType, const FVector2D&, const FVector2D&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FNotifyCompleteCmd, const EMotionStatus, const FVector2D&);