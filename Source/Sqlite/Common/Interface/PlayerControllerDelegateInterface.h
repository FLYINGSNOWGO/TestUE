/* Copyright 2015 CloudMinds Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of CloudMinds Inc. nor the names of its contributors may be
 *   used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * Author: ewen.dong@cloudminds.com
 *
 * ServerGameInstance delegator class
 *
 * Currently this is a work in progress.
 */
#pragma once

#include "CoreMinimal.h"

class ASqlitPlayerController;

class FPlayerControllerDelegateInterface
{
public:
	// Common
	void SetPlayerController(ASqlitPlayerController * InPlayerController) { SqlitPlayerController = InPlayerController; }
	ASqlitPlayerController* GetPlayerController() { return SqlitPlayerController; }

	// Server Function
	UClass* GetDefaultPawn_Implementation() { return nullptr; }

	// Client Function
	virtual void tick(float DeltaTime) {};
	virtual void CheckClientNetwork(float DeltaTime) {}
protected:
	ASqlitPlayerController* SqlitPlayerController;
};