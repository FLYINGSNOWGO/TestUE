#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TaskInfo.generated.h"

USTRUCT(BlueprintType)
struct RESOURCESDOWNLOADER_API FTaskInfo
{
    GENERATED_BODY()

    FTaskInfo() : FileSize(0), CurrentSize(0), DownloadSize(0), TotalSize(0), GUID(FGuid::NewGuid())
    {
    }

    FString FileName;

    FString URL;

    int64 FileSize;

    int64 CurrentSize;

    int64 DownloadSize;

    int64 TotalSize;

    FGuid GUID;
};
