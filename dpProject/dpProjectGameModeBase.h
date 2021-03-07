// Copyright Epic Games, Inc. All Rights Reserved.

/////////////////////////////////////////////////////////////////////////////
//	Class:
//		AdpProjectGameModeBase
//	Description:
//		Game mode derived class. It takes the input provided through keyboard
//      and set it as useful values for table and chairs manipulation.
//	SourceFiles: 
//		dpProjectGameModeBase.cpp
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <algorithm>
#include "GameFramework/PlayerController.h"
#include "dpProjectGameModeBase.generated.h"


UCLASS()
class DPPROJECT_API AdpProjectGameModeBase : public AGameModeBase
{
	GENERATED_UCLASS_BODY()
public:

	virtual void BeginPlay() override;
	// Mouse delta along X and Y axis
	float DeltaX = 0;
	float DeltaY = 0;

private:
	//////////////////////////////////
	// Methods to bind the action with
	//////////////////////////////////
	void ExitGame();
	void MoveMouse();
	void StopMouse();
};
