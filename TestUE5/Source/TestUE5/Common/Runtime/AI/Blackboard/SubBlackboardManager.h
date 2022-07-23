// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SubBlackboardManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSubBlackboardManager, Log, All)

class UBlackboardData;

/**
 * 
 */
UCLASS(BlueprintType)
class TESTUE5_API USubBlackboardManager : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "SubBlackboardManager")
	void Register(const FName& Name, const FString& BlackboardDataPath);

	UFUNCTION(BlueprintCallable, Category = "SubBlackboardManager")
	void UnRegister(const FName& Name);

	// UFUNCTION(BlueprintCallable, Category="SubBlackboardManager")
	// UObject* GetValueAsObject(const FName& Name, const FName& KeyName) const;

	// UFUNCTION(BlueprintCallable, Category="SubBlackboardManager")
	// UClass* GetValueAsClass(const FName& Name, const FName& KeyName) const;

	// UFUNCTION(BlueprintCallable, Category="SubBlackboardManager")
	// uint8 GetValueAsEnum(const FName& Name, const FName& KeyName) const;

	// UFUNCTION(BlueprintCallable, Category="SubBlackboardManager")
	// int32 GetValueAsInt(const FName& Name, const FName& KeyName) const;

	// UFUNCTION(BlueprintCallable, Category="SubBlackboardManager")
	// float GetValueAsFloat(const FName& Name, const FName& KeyName) const;

	// UFUNCTION(BlueprintCallable, Category="SubBlackboardManager")
	// bool GetValueAsBool(const FName& Name, const FName& KeyName) const;

	// UFUNCTION(BlueprintCallable, Category="SubBlackboardManager")
	// FString GetValueAsString(const FName& Name, const FName& KeyName) const;
	
	// UFUNCTION(BlueprintCallable, Category="SubBlackboardManager")
	// FName GetValueAsName(const FName& Name, const FName& KeyName) const;

	// UFUNCTION(BlueprintCallable, Category="SubBlackboardManager")
	// FVector GetValueAsVector(const FName& Name, const FName& KeyName) const;

	// UFUNCTION(BlueprintCallable, Category="SubBlackboardManager")
	// FRotator GetValueAsRotator(const FName& Name, const FName& KeyName) const;

	// UFUNCTION(BlueprintCallable, Category="SubBlackboardManager")
	// void SetValueAsObject(const FName& Name, const FName& KeyName, UObject* ObjectValue);
	
	// UFUNCTION(BlueprintCallable, Category="SubBlackboardManager")
	// void SetValueAsClass(const FName& Name, const FName& KeyName, UClass* ClassValue);

	// UFUNCTION(BlueprintCallable, Category="SubBlackboardManager")
	// void SetValueAsEnum(const FName& Name, const FName& KeyName, uint8 EnumValue);

	// UFUNCTION(BlueprintCallable, Category="SubBlackboardManager")
	// void SetValueAsInt(const FName& Name, const FName& KeyName, int32 IntValue);

	// UFUNCTION(BlueprintCallable, Category="SubBlackboardManager")
	// void SetValueAsFloat(const FName& Name, const FName& KeyName, float FloatValue);

	// UFUNCTION(BlueprintCallable, Category="SubBlackboardManager")
	// void SetValueAsBool(const FName& Name, const FName& KeyName, bool BoolValue);

	// UFUNCTION(BlueprintCallable, Category="SubBlackboardManager")
	// void SetValueAsString(const FName& Name, const FName& KeyName, FString StringValue);

	// UFUNCTION(BlueprintCallable, Category="SubBlackboardManager")
	// void SetValueAsName(const FName& Name, const FName& KeyName, FName NameValue);

	// UFUNCTION(BlueprintCallable, Category="SubBlackboardManager")
	// void SetValueAsVector(const FName& Name, const FName& KeyName, FVector VectorValue);

	// UFUNCTION(BlueprintCallable, Category = "SubBlackboardManager")
	// void SetValueAsRotator(const FName& Name, const FName& KeyName, FRotator VectorValue);

private:
	UPROPERTY()
	TMap<FName, TObjectPtr<UBlackboardData>> SubBlackboardDatas;

private:
	UBlackboardData* GetSubBlackboard(const FName& Name);
	// /** @return key ID from name */
	// FBlackboard::FKey GetKeyID(const FName& KeyName) const;
	//
	// template<class TDataClass>
	// bool SetValue(const FName& KeyName, typename TDataClass::FDataType Value);
	//
	// template<class TDataClass>
	// bool SetValue(FBlackboard::FKey KeyID, typename TDataClass::FDataType Value);
	
};

// template<class TDataClass>
// bool USubBlackboardManager::SetValue(const FName& Name, FBlackboard::FKey KeyID, typename TDataClass::FDataType Value)
// {
// 	UBlackboardData* BlackboardAsset = GetSubBlackboard(Name);
// 	const FBlackboardEntry* EntryInfo = BlackboardAsset ? BlackboardAsset->GetKey(KeyID) : nullptr;
// 	if ((EntryInfo == nullptr) || (EntryInfo->KeyType == nullptr) || (EntryInfo->KeyType->GetClass() != TDataClass::StaticClass()))
// 	{
// 		return false;
// 	}
//
// 	const uint16 DataOffset = EntryInfo->KeyType->HasInstance() ? sizeof(FBlackboardInstancedKeyMemory) : 0;
// 	uint8* RawData = GetKeyRawData(KeyID) + DataOffset;
// 	if (RawData)
// 	{
// 		UBlackboardKeyType* KeyOb = EntryInfo->KeyType->HasInstance() ? KeyInstances[KeyID] : EntryInfo->KeyType;
// 		const bool bChanged = TDataClass::SetValue((TDataClass*)KeyOb, RawData, Value);
// 		if (bChanged)
// 		{
// 			NotifyObservers(KeyID);
// 			if (BlackboardAsset->HasSynchronizedKeys() && IsKeyInstanceSynced(KeyID))
// 			{
// 				UAISystem* AISystem = UAISystem::GetCurrentSafe(GetWorld());
// 				for (auto Iter = AISystem->CreateBlackboardDataToComponentsIterator(*BlackboardAsset); Iter; ++Iter)
// 				{
// 					UBlackboardComponent* OtherBlackboard = Iter.Value();
// 					if (OtherBlackboard != nullptr && ShouldSyncWithBlackboard(*OtherBlackboard))
// 					{
// 						UBlackboardData* const OtherBlackboardAsset = OtherBlackboard->GetBlackboardAsset();
// 						const FBlackboard::FKey OtherKeyID = OtherBlackboardAsset ? OtherBlackboardAsset->GetKeyID(EntryInfo->EntryName) : FBlackboard::InvalidKey;
// 						if (OtherKeyID != FBlackboard::InvalidKey)
// 						{
// 							UBlackboardKeyType* OtherKeyOb = EntryInfo->KeyType->HasInstance() ? OtherBlackboard->KeyInstances[OtherKeyID] : EntryInfo->KeyType;
// 							uint8* OtherRawData = OtherBlackboard->GetKeyRawData(OtherKeyID) + DataOffset;
//
// 							TDataClass::SetValue((TDataClass*)OtherKeyOb, OtherRawData, Value);
// 							OtherBlackboard->NotifyObservers(OtherKeyID);
// 						}
// 					}
// 				}
// 			}
// 		}
//
// 		return true;
// 	}
//
// 	return false;
// }
//
// template<class TDataClass>
// bool USubBlackboardManager::SetValue(const FName& Name, const FName& KeyName, typename TDataClass::FDataType Value)
// {
// 	const FBlackboard::FKey KeyID = GetKeyID(Name, KeyName);
// 	return SetValue<TDataClass>(Name, KeyID, Value);
// }
