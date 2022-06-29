// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/AI/Blackboard/SubBlackboardManager.h"
#include "BehaviorTree/BlackboardData.h"

DEFINE_LOG_CATEGORY(LogSubBlackboardManager)

void USubBlackboardManager::Register(const FName& Name, const FString& BlackboardDataPath)
{
	UObject* Obj = LoadObject<UObject>(NULL, *BlackboardDataPath);
	UClass* ObjClass = (UClass*)(Obj);
	if (ObjClass)
	{
		TObjectPtr<UBlackboardData> SubBlackboardData = NewObject<UBlackboardData>(ObjClass);
		if (!SubBlackboardData)
		{
			UE_LOG(LogSubBlackboardManager,Error,TEXT("Register error:%s create fail"),*Name.ToString());
			return;
		}
		SubBlackboardDatas.Add(Name,SubBlackboardData);
	}
}

void USubBlackboardManager::UnRegister(const FName& Name)
{
	if (!SubBlackboardDatas.Contains(Name))
	{
		UE_LOG(LogSubBlackboardManager,Error,TEXT("UnRegister error:%s don't Exist"),*Name.ToString());
		return;
	}

	SubBlackboardDatas.Remove(Name);
}

UBlackboardData* USubBlackboardManager::GetSubBlackboard(const FName& Name)
{
	if (!SubBlackboardDatas.Contains(Name))
	{
		UE_LOG(LogSubBlackboardManager,Warning,TEXT("GetSubBlackboard error:%s don't Exist"),*Name.ToString());
		return nullptr;
	}

	return SubBlackboardDatas[Name];
	
}

// FBlackboard::FKey USubBlackboardManager::GetKeyID(const FName& Name, const FName& KeyName) const
// {
// 	UBlackboardData* BlackboardAsset = GetSubBlackboard(Name);
// 	return BlackboardAsset ? BlackboardAsset->GetKeyID(KeyName) : FBlackboard::InvalidKey;
// }

// UObject* UBlackboardComponent::GetValueAsObject(const FName& KeyName) const
// {
// 	return GetValue<UBlackboardKeyType_Object>(KeyName);
// }

// UClass* UBlackboardComponent::GetValueAsClass(const FName& KeyName) const
// {
// 	return GetValue<UBlackboardKeyType_Class>(KeyName);
// }

// uint8 UBlackboardComponent::GetValueAsEnum(const FName& KeyName) const
// {
// 	return GetValue<UBlackboardKeyType_Enum>(KeyName);
// }

// int32 UBlackboardComponent::GetValueAsInt(const FName& KeyName) const
// {
// 	return GetValue<UBlackboardKeyType_Int>(KeyName);
// }

// float UBlackboardComponent::GetValueAsFloat(const FName& KeyName) const
// {
// 	return GetValue<UBlackboardKeyType_Float>(KeyName);
// }

// bool UBlackboardComponent::GetValueAsBool(const FName& KeyName) const
// {
// 	return GetValue<UBlackboardKeyType_Bool>(KeyName);
// }

// FString UBlackboardComponent::GetValueAsString(const FName& KeyName) const
// {
// 	return GetValue<UBlackboardKeyType_String>(KeyName);
// }

// FName UBlackboardComponent::GetValueAsName(const FName& KeyName) const
// {
// 	return GetValue<UBlackboardKeyType_Name>(KeyName);
// }

// FVector UBlackboardComponent::GetValueAsVector(const FName& KeyName) const
// {
// 	return GetValue<UBlackboardKeyType_Vector>(KeyName);
// }

// FRotator UBlackboardComponent::GetValueAsRotator(const FName& KeyName) const
// {
// 	return GetValue<UBlackboardKeyType_Rotator>(KeyName);
// }

// void UBlackboardComponent::SetValueAsObject(const FName& KeyName, UObject* ObjectValue)
// {
// 	const FBlackboard::FKey KeyID = GetKeyID(KeyName);
// 	SetValue<UBlackboardKeyType_Object>(KeyID, ObjectValue);
// }

// void UBlackboardComponent::SetValueAsClass(const FName& KeyName, UClass* ClassValue)
// {
// 	const FBlackboard::FKey KeyID = GetKeyID(KeyName);
// 	SetValue<UBlackboardKeyType_Class>(KeyID, ClassValue);
// }

// void UBlackboardComponent::SetValueAsEnum(const FName& KeyName, uint8 EnumValue)
// {
// 	const FBlackboard::FKey KeyID = GetKeyID(KeyName);
// 	SetValue<UBlackboardKeyType_Enum>(KeyID, EnumValue);
// }

// void UBlackboardComponent::SetValueAsInt(const FName& KeyName, int32 IntValue)
// {
// 	const FBlackboard::FKey KeyID = GetKeyID(KeyName);
// 	SetValue<UBlackboardKeyType_Int>(KeyID, IntValue);
// }

// void UBlackboardComponent::SetValueAsFloat(const FName& KeyName, float FloatValue)
// {
// 	const FBlackboard::FKey KeyID = GetKeyID(KeyName);
// 	SetValue<UBlackboardKeyType_Float>(KeyID, FloatValue);
// }

// void UBlackboardComponent::SetValueAsBool(const FName& KeyName, bool BoolValue)
// {
// 	const FBlackboard::FKey KeyID = GetKeyID(KeyName);
// 	SetValue<UBlackboardKeyType_Bool>(KeyID, BoolValue);
// }

// void UBlackboardComponent::SetValueAsString(const FName& KeyName, FString StringValue)
// {
// 	const FBlackboard::FKey KeyID = GetKeyID(KeyName);
// 	SetValue<UBlackboardKeyType_String>(KeyID, StringValue);
// }

// void UBlackboardComponent::SetValueAsName(const FName& KeyName, FName NameValue)
// {
// 	const FBlackboard::FKey KeyID = GetKeyID(KeyName);
// 	SetValue<UBlackboardKeyType_Name>(KeyID, NameValue);
// }

// void UBlackboardComponent::SetValueAsVector(const FName& KeyName, FVector VectorValue)
// {
// 	const FBlackboard::FKey KeyID = GetKeyID(KeyName);
// 	SetValue<UBlackboardKeyType_Vector>(KeyID, VectorValue);
// }

// void UBlackboardComponent::SetValueAsRotator(const FName& KeyName, FRotator RotatorValue)
// {
// 	const FBlackboard::FKey KeyID = GetKeyID(KeyName);
// 	SetValue<UBlackboardKeyType_Rotator>(KeyID, RotatorValue);
// }

// bool UBlackboardComponent::IsVectorValueSet(const FName& KeyName) const
// {
// 	const FBlackboard::FKey KeyID = GetKeyID(KeyName);
// 	return IsVectorValueSet(KeyID);
// }

// bool UBlackboardComponent::IsVectorValueSet(FBlackboard::FKey KeyID) const
// {
// 	FVector VectorValue = GetValue<UBlackboardKeyType_Vector>(KeyID);
// 	return (VectorValue != FAISystem::InvalidLocation);
// }
