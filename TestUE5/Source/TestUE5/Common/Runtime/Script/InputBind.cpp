#include "InputBind.h"
UInputBind::UInputBind():LastAxisValue(0)
{
	
}

UInputBind::~UInputBind()
{
	//GEngine->AddOnScreenDebugMessage(1, 10, FColor::Cyan, TEXT("~UInputBind::~UInputBind"));
	UE_LOG(LogTemp, Warning,TEXT("UInputBind::~UInputBind ")); 
}

void UInputBind::BindAction(UInputComponent* inputComponent, EInputBindType bindType, const FString& bindKey, EInputEvent bindIE /*= EInputEvent::IE_Pressed*/)
{
	if (bindType == EInputBindType::BindAction)
	{
		inputComponent->BindAction(*bindKey,bindIE,this,&UInputBind::OnAction);
	}
	else if (bindType == EInputBindType::BindAxis)
	{
		inputComponent->BindAxis(*bindKey,this, &UInputBind::OnAxis);
	}
	else if (bindType == EInputBindType::BindTouch)
	{
		inputComponent->BindTouch(bindIE,this, &UInputBind::OnTouch);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(" ERROR  UInputBind::BindAction %s"), bindType)
	}
	
}


void UInputBind::OnAxis(float dt)
{
	
	if (dt != 0.f)
	{
		InputeDelegateBindAxis.ExecuteIfBound(dt);
	}
	//LastAxisValue = dt;
}

void UInputBind::OnAction()
{
	 InputeDelegateBindAction.ExecuteIfBound();
}

void UInputBind::OnTouch(ETouchIndex::Type fingerIndex, FVector location)
{
	InputeDelegateBindTouch.ExecuteIfBound(fingerIndex,location);
}

void UInputBind::RmoveAllBind()
{
	InputeDelegateBindAction.Clear();
	InputeDelegateBindAxis.Clear();
	InputeDelegateBindTouch.Clear();
}