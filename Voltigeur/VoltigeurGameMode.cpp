// Fill out your copyright notice in the Description page of Project Settings.

#include "Voltigeur.h"
#include "VoltigeurGameMode.h"
#include "VoltigeurDefaultPawn.h"
#include "VoltigeurPlayerController.h"

AVoltigeurGameMode::AVoltigeurGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AVoltigeurPlayerController::StaticClass();
	DefaultPawnClass = AVoltigeurDefaultPawn::StaticClass();
}


