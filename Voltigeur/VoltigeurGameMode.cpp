// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Voltigeur.h"
#include "VoltigeurGameMode.h"
#include "ControllerPawn.h"
#include "VoltigeurPlayerController.h"

AVoltigeurGameMode::AVoltigeurGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AVoltigeurPlayerController::StaticClass();
	DefaultPawnClass = AControllerPawn::StaticClass();
}