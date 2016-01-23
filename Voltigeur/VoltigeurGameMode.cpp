// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Voltigeur.h"
#include "VoltigeurGameMode.h"
#include "VoltigeurPlayerController.h"
// #include "VoltigeurCharacter.h"

AVoltigeurGameMode::AVoltigeurGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AVoltigeurPlayerController::StaticClass();

}