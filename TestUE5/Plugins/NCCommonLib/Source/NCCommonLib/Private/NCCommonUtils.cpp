// Fill out your copyright notice in the Description page of Project Settings.


#include "NCCommonUtils.h"
#include "NCCommonLib.h"
#include "Serialization/BufferArchive.h"
#include "Serialization/ArchiveSaveCompressedProxy.h"
#include "Serialization/ArchiveLoadCompressedProxy.h"

void NPNCCommonUtils::StringToArrayChar(const FString& InString, const TCHAR* InDelim, TArray<const char*>& OutAnsiStr)
{
	TArray<FString> StrArr;
	const TCHAR* TagDelims[1] = { InDelim };
	InString.ParseIntoArray(StrArr, TagDelims, 1);

	TArray<FTCHARToUTF8> StrBuffer;
	StrBuffer.Reserve(StrArr.Num());
	OutAnsiStr.Reserve(StrArr.Num());

	for (int TagIndex = 0; TagIndex < StrArr.Num(); ++TagIndex)
	{
		const FString& Str = StrArr[TagIndex];
		FTCHARToUTF8& Converter = StrBuffer.Emplace_GetRef(*Str);
		OutAnsiStr.Emplace(Converter.Get());
	}
}

void NPNCCommonUtils::CompressedString(FString& InStr, TArray<uint8>& OutCompressedData, const ECompressionFlags InCompressionFlags)
{
	FBufferArchive ToBinary;
	ToBinary << InStr;
	if (ToBinary.Num() < 0)
		return;
	const FName CompressionFormat = NAME_Oodle;
	FArchiveSaveCompressedProxy Compressor =
		FArchiveSaveCompressedProxy(OutCompressedData, CompressionFormat, InCompressionFlags);

	Compressor << ToBinary;
	Compressor.Flush();

	UE_LOG(LogNCCommonUtils, Log, TEXT("CompressedString: Befor Compresse:%d, After Compresse:%d "), ToBinary.Num(), OutCompressedData.Num());
}

void NPNCCommonUtils::ReadCompressedString(const TArray<uint8>& InCompressedData, FString& OutStr, const ECompressionFlags InCompressionFlags)
{
	// Decompress File 
	FArchiveLoadCompressedProxy Decompressor =
		FArchiveLoadCompressedProxy(InCompressedData, NAME_Oodle, InCompressionFlags);

	//Decompression Error?
	if (Decompressor.GetError())
	{
		UE_LOG(LogNCCommonUtils, Error, TEXT("ReadCompressedString: FArchiveLoadCompressedProxy Error: Data Was Not Compressed"));
		return;
	}

	//Decompress
	FBufferArchive DecompressedBinaryArray;
	Decompressor << DecompressedBinaryArray;

	OutStr.Empty();

	FMemoryReader FromBinary = FMemoryReader(DecompressedBinaryArray, true); //true, free data after done
	FromBinary.Seek(0);
	FromBinary << OutStr;
	Decompressor.FlushCache();
	FromBinary.FlushCache();

	// Empty & Close Buffer 
	DecompressedBinaryArray.Empty();
	DecompressedBinaryArray.Close();
}
