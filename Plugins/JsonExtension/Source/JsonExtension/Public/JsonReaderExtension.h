// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Serialization/JsonReader.h"

class FJsonStringReaderExtension
	: public TJsonReader<TCHAR>
{
public:

	static TSharedRef<FJsonStringReaderExtension> Create(const FString& JsonString)
	{
		return MakeShareable(new FJsonStringReaderExtension(JsonString));
	}

	static TSharedRef<FJsonStringReaderExtension> Create(FString&& JsonString)
	{
		return MakeShareable(new FJsonStringReaderExtension(MoveTemp(JsonString)));
	}

public:

	virtual ~FJsonStringReaderExtension() = default;
	void UpdateJsonString(const FString& JsonString)
	{
		Content = JsonString;
		InitReader();
		FinishedReadingRootObject = false;
	}

protected:

	/**
	 * Parses a string containing Json information.
	 *
	 * @param JsonString The Json string to parse.
	 */
	FJsonStringReaderExtension(const FString& JsonString)
		: Content(JsonString)
		, Reader(nullptr)
	{
		InitReader();
	}

	/**
	 * Parses a string containing Json information.
	 *
	 * @param JsonString The Json string to parse.
	 */
	FJsonStringReaderExtension(FString&& JsonString)
		: Content(MoveTemp(JsonString))
		, Reader(nullptr)
	{
		InitReader();
	}

	FORCEINLINE void InitReader()
	{
		if (Content.IsEmpty())
		{
			return;
		}
		Reader = MakeUnique<FBufferReader>((void*)*Content, Content.Len() * sizeof(TCHAR), false);
		check(Reader.IsValid());

		Stream = Reader.Get();
	}

protected:
	FString Content;
	TUniquePtr<FBufferReader> Reader;
};

template <class CharType = TCHAR>
class TJsonReaderFactoryExtension
{
public:

	static TSharedRef<TJsonReader<TCHAR>> Create(const FString& JsonString)
	{
		return FJsonStringReaderExtension::Create(JsonString);
	}

	static TSharedRef<TJsonReader<TCHAR>> Create(FString&& JsonString)
	{
		return FJsonStringReaderExtension::Create(MoveTemp(JsonString));
	}
};
