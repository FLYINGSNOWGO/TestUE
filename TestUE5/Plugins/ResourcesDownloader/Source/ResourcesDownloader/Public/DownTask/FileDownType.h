#pragma once
#include "CoreMinimal.h"
#include "FileDownType.generated.h"

UENUM(BlueprintType)
enum class EDownloadTaskEvent : uint8
{
    REQ_HEAD,
    RET_HEAD,
    BEGIN_DOWNLOAD,
    UPDATE_DOWNLOAD,
    END_DOWNLOAD,
    TE_ERROR
};

UENUM(BlueprintType)
enum class EGameHotUpdateState : uint8
{
    BEGIN_GameHotUpdate,
    BEGIN_GETVERSION,
    GET_VERSIONTIMEOUT,
    END_GETVERSION,
    START_DOWNLOADPASK,
    BEGIN_DOWNLOAD,
    END_DOWNLOAD,
    BEGIN_MOUNT,
    END_MOUNT,
    END_GameHotUpdate,
    HUS_ERROR
};

USTRUCT(BlueprintType)
struct FPakFileProperty
{
    GENERATED_BODY()
    FPakFileProperty(const FString& PakName, const int64 PakSize, const FString& MD5) : PakName(PakName),
        PakSize(PakSize), MD5(MD5)
    {
    }
    FPakFileProperty() = default;
    UPROPERTY();
    FString PakName;
    UPROPERTY();
    int64 PakSize;
    UPROPERTY();
    FString MD5;
    UPROPERTY();
    bool NeedUpdate = false;

    UPROPERTY();
    FString URL;

    bool operator ==(const FPakFileProperty& Other) const
    {
        return PakName.Equals(Other.PakName) && PakSize == Other.PakSize && MD5.ToLower().Equals(Other.MD5.ToLower());
    }
};

USTRUCT(BlueprintType)
struct FDownloadProgress
{
    GENERATED_BODY()

    FDownloadProgress() = default;

    FDownloadProgress(const int64 CurrentDownloadSize, const int64 TotalDownloadSize, const FString& DownSpeed,const float downloadPer) :
        CurrentDownloadSize(CurrentDownloadSize), TotalDownloadSize(TotalDownloadSize), DownloadSpeed(DownSpeed), DownloadPer(downloadPer)
    {
    }

    UPROPERTY(BlueprintReadOnly, Category = "FDownloadProgress")
    int64 CurrentDownloadSize;

    UPROPERTY(BlueprintReadOnly, Category = "FDownloadProgress")
    int64 TotalDownloadSize;

    UPROPERTY(BlueprintReadOnly, Category = "FDownloadProgress")
    FString DownloadSpeed;

    UPROPERTY(BlueprintReadOnly, Category = "FDownloadProgress")
    float DownloadPer = 0.0f;


    static FString ConvertIntToSize(int64 Size)
    {
        if (Size < 1024)
        {
            return FString::FormatAsNumber(Size).Append(TEXT("B"));
        }

        Size /= 1024;

        if (Size < 1024)
        {
            return FString::FormatAsNumber(Size).Append(TEXT("KB"));
        }

        Size /= 1024;

        return FString::FormatAsNumber(Size).Append(TEXT("MB"));
    }
};
