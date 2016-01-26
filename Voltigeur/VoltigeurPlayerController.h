// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/PlayerController.h"
#include "ControllerPawn.h"
#include "VoltigeurPlayerController.generated.h"

/**
* Controller when player is possessing a Character
*/
UCLASS()
class VOLTIGEUR_API AVoltigeurPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AVoltigeurPlayerController();

protected:
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	//LMB
	/*Mouse cursor*/
	void CheckUnderMouse();

	/*When Player is controlling a character*/
	void ProcessSelectedCharacter(FHitResult Hit); //when player is controlling a character

	/*When player is controlling a default pawn*/
	void ProcessSelectionPawn(FHitResult Hit);

	/*Pointer to Controller Pawn*/
	class AControllerPawn* ControllerPawnPtr;

	void Select(AActor* Selected); //can select character or objects like 'sign-post'
	void GoToLocation(FHitResult Hit);

	void FaceSelected(FVector TargetLoc, float DeltaTime); //after Pawn targets, make pawn face target

														   //	void Target(ACharacter* Enemy); //for characters
														   //TODO need to implement CTRL + RMB when wielding artillery or splash weapon
	void Target(FVector* TargetArea); //for artillery, get Artillery's splash radius

	void CycleWeaponInventory(); //use X to cycle weapon inventory

};





