// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/Interface/PlayerControllerDelegateInterface.h"


/**
 * 
 */
class SQLITE_API FClientPlayerControllerDelegator : public FPlayerControllerDelegateInterface
{
private:
	double PreviousTime = 0.f;
	const int Interval = 1.f;
	const uint8 HeartBeatThresholdValue = 20 / Interval;
	float NetworkErroTimes = 0;
public:
	FClientPlayerControllerDelegator();
	virtual ~FClientPlayerControllerDelegator();

	virtual void CheckClientNetwork(float DeltaTime) override;

	void ConnectDSImplementation();

};
