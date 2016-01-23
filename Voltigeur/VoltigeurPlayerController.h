// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "VoltigeurPlayerController.generated.h"

UCLASS()
class AVoltigeurPlayerController : public APlayerController
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
	void ProcessSelectedCharacter(FHitResult Hit);
	void Select(AActor* Selected); //can select character or objects like 'sign-post'
	void GoToLocation(FHitResult Hit);

	void FaceSelected(FVector targetloc, float DeltaTime); //after Pawn targets, make pawn face target

														   //	void Target(ACharacter* Enemy); //for characters
														   //TODO need to implement CTRL + RMB when wielding artillery or splash weapon
	void Target(FVector* TargetArea); //for artillery, get Artillery's splash radius

	void CycleWeaponInventory(); //use X to cycle weapon inventory
};


