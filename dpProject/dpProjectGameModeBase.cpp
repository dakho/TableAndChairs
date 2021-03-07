// Copyright Epic Games, Inc. All Rights Reserved.


#include "dpProjectGameModeBase.h"


AdpProjectGameModeBase::AdpProjectGameModeBase(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {}

void AdpProjectGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	// Define UWorld pointer
	UWorld* World = GetWorld();
	// show the mouse cursor and enable clicking
	World->GetFirstPlayerController()->bShowMouseCursor = true;
	World->GetFirstPlayerController()->bEnableClickEvents = true;
	World->GetFirstPlayerController()->bEnableMouseOverEvents = true;
	// Bind the action with a corresponding function
	World->GetFirstPlayerController()->InputComponent->BindAction("ExitGame", IE_Pressed, this, &AdpProjectGameModeBase::ExitGame);
	World->GetFirstPlayerController()->InputComponent->BindAction("MouseAction", IE_Pressed, this, &AdpProjectGameModeBase::MoveMouse);
	World->GetFirstPlayerController()->InputComponent->BindAction("MouseAction", IE_Released, this, &AdpProjectGameModeBase::StopMouse);

}

// Exit game method
void AdpProjectGameModeBase::ExitGame()
{
	FGenericPlatformMisc::RequestExit(true);
}

// Move mouse method
void AdpProjectGameModeBase::MoveMouse()
{
	
	UWorld* World = GetWorld();
	// Update DeltaX and DeltaY
	World->GetFirstPlayerController()->GetInputMouseDelta(DeltaX, DeltaY);
}

// Stop mouse method
void AdpProjectGameModeBase::StopMouse()
{
	// Set deltas to zero
	DeltaX = 0;
	DeltaY = 0;
}
