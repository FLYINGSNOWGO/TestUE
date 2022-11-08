// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
namespace NPNCCommonUtils
{
	/**
	 * String To Array Char
	 * @InString ���������ַ���
	 * @Delim �ָ��
	 * @OutAnsiStr ������
	 */
	void StringToArrayChar(const FString& InString, const TCHAR* InDelim, TArray<const char*>& OutAnsiStr);

	void CompressedString(FString& InStr, TArray<uint8>& OutCompressedData, const ECompressionFlags InCompressionFlags = ECompressionFlags::COMPRESS_BiasMemory);
	void ReadCompressedString(const TArray<uint8>& InCompressedData, FString& OutStr, const ECompressionFlags InCompressionFlags = ECompressionFlags::COMPRESS_BiasMemory);

	template<class EnumType>
	constexpr auto CastToUnderlying(EnumType Type)
	{
		static_assert(TIsEnum<EnumType>::Value, "");
		using UnderType = __underlying_type(EnumType);
		return static_cast<UnderType>(Type);
	}
	/**
	 * ��ö��ת�����ַ���
	 * @Note: ö�����ĳ�Ա������Count
	 * @EnumType ö������
	 * @Strings ��̬ö���ַ���
	 */
	template<typename EnumType, typename StringArrType>
	const TCHAR* TLexToString(EnumType E, const StringArrType& Strings)
	{
		constexpr auto Count = NPNCCommonUtils::CastToUnderlying(EnumType::Count);
		static_assert(Count == UE_ARRAY_COUNT(Strings), "");

		auto Idx = NPNCCommonUtils::CastToUnderlying(E);
		if (Idx >= 0 && Idx < Count)
		{
			return Strings[Idx];
		}
		else
		{
			return TEXT("");
		}
	}
};