// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "InputBind.generated.h"

DECLARE_DYNAMIC_DELEGATE(FInputeDelegateBindAction);
DECLARE_DYNAMIC_DELEGATE_OneParam(FInputeDelegateBindAxis, float, dt);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FInputeDelegateBindTouch, int32, touchIndex, FVector,location);



UENUM(BlueprintType, meta = (ScriptName = "InputBindType"))
enum EInputBindType
{
	BindAction,
	BindAxis,
	BindTouch
};

/**
 *
 */
UCLASS()
class TESTUE5_API UInputBind : public UObject
{
    GENERATED_BODY()
    float LastAxisValue;
public:
    UInputBind();
    virtual ~UInputBind();
public:
	UPROPERTY()
    FInputeDelegateBindAction InputeDelegateBindAction;
    UPROPERTY()
    FInputeDelegateBindAxis InputeDelegateBindAxis;
    UPROPERTY()
    FInputeDelegateBindTouch InputeDelegateBindTouch;
    UFUNCTION()
    void BindAction(UInputComponent* inputComponent, EInputBindType bindType,const FString& bindKey, EInputEvent bindIE = EInputEvent::IE_Pressed);

    void OnAction();
    void OnAxis(float dt);
    void OnTouch(ETouchIndex::Type fingerIndex, FVector location);
    UFUNCTION()
    void RmoveAllBind();

};