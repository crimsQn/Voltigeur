// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Voltigeur.h"
#include "VoltigeurPlayerController.h"
#include "VoltigeurGameMode.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine.h"
#include "VoltigeurCharacterBase.h"

AVoltigeurPlayerController::AVoltigeurPlayerController()
{
	//Default Settings
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::CardinalCross;
	//End of Default Settinsg

}

void AVoltigeurPlayerController::PlayerTick(float DeltaTime) //every frame stuff
{
	Super::PlayerTick(DeltaTime);

}

void AVoltigeurPlayerController::SetupInputComponent()
{
	/*********Movement*************/
	//Set up game play key bindings
	Super::SetupInputComponent();

	//LMB which moves pawn or selects objects
	//When LMB is released, 'order' is executed every frame in PlayerTick
	InputComponent->BindAction("MoveOrInteract", IE_Released, this, &AVoltigeurPlayerController::CheckUnderMouse);

	//Double-Click to interact
	//implement deselect

	//Scroll between WeaponInventory
//	InputComponent->BindAction("CycleWeaponInventory", IE_Pressed, this, &AVoltigeurPlayerController::CycleWeaponInventory);

}

//Step 1 of "Select"
void AVoltigeurPlayerController::CheckUnderMouse()
{
	FHitResult Hit; //is whatever was under the mouse cursor right now
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Pawn), false, Hit);

	//Hit is an character 
	if (Hit.GetActor() && Hit.GetActor()->IsA(ACharacter::StaticClass()))
	{
		//TODO delete
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Selecting a Character"));
		AVoltigeurPlayerController::ProcessSelectedCharacter(Hit);
	}
	// else process movement
	else if (Hit.bBlockingHit)
	{
		AVoltigeurPlayerController::GoToLocation(Hit);
	}
}

//Step 2 of "Select"
void AVoltigeurPlayerController::ProcessSelectedCharacter(FHitResult Hit)
{
	//Check if Pawn is a default spanning pawn or if player is currently controlling a character
	if (GetPawn() && GetPawn()->IsA(AControllerPawn::StaticClass()))
	{
		//Process Selection in a different function for code to be legible
		AVoltigeurPlayerController::ProcessSelectionPawn(Hit);
		return;
	}

	//TODO change cast to generic base character
	AVoltigeurCharacterBase* SelectedVoltigeur = Cast<AVoltigeurCharacterBase>(Hit.GetActor());
	AVoltigeurCharacterBase* PlayerVoltigeur = Cast<AVoltigeurCharacterBase>(GetPawn());

	//if Hit is a Voltigeur unit
	if (SelectedVoltigeur && PlayerVoltigeur)
	{
		EFriendlyState SelectedCharFriendlyState = SelectedVoltigeur->GetFriendlyState();

		if (SelectedCharFriendlyState == EFriendlyState::EPlayer)
		{
			//TODO need to reset animation before Un-possessing the character
			//Un-possess current pawn
			AVoltigeurPlayerController::UnPossess();
			//Switch control to this character
			this->Possess(SelectedVoltigeur);
			//TODO reset camera??
		}
		//Check if unit under cursor is hostile
		else if (SelectedCharFriendlyState == EFriendlyState::EHostile)
		{
			//Check if player character's target is valid
			if (PlayerVoltigeur->GetTarget() != SelectedVoltigeur)
			{
				//Set target
				PlayerVoltigeur->SetTarget(SelectedVoltigeur);
				//TODO Delete
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Targetting"));
				//Rotate controlled pawn to enemy target
				//TODO change DeltaTime for animation
				AVoltigeurPlayerController::FaceSelected(SelectedVoltigeur->GetActorLocation(), 0.f);
			}
			else //Valid target, then attack
			{
				//Check if Character is facing enemy
				AVoltigeurPlayerController::FaceSelected(SelectedVoltigeur->GetActorLocation(), 0.f);
				//Check if weapon is equipped
				if (PlayerVoltigeur->GetCurrentWeapon() != NULL)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Weapon Fired"));
					PlayerVoltigeur->GetCurrentWeapon()->Fire();
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No Weapon Equipped: (Should Replace with bare-fist)"));
				}
			}
		}
		else //Neutral or friendly units
		{
			//Make sure player's target is null
			PlayerVoltigeur->SetTarget(nullptr);
			//TODO Delete
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Un-Targetting"));
			//Rotate controlled pawn to enemy target
			//TODO change DeltaTime for animation
			AVoltigeurPlayerController::FaceSelected(SelectedVoltigeur->GetActorLocation(), 0.f);
		}
		//else select that character if out of range, interact or move to actor if in range

	}
	// TODO else peasant unit for example		else

}

void AVoltigeurPlayerController::ProcessSelectionPawn(FHitResult Hit)
{
	AVoltigeurCharacterBase* SelectedVoltigeur = Cast<AVoltigeurCharacterBase>(Hit.GetActor());

	//if Selected Character is a Friendly, then possess it
	if (SelectedVoltigeur && SelectedVoltigeur->GetFriendlyState() == EFriendlyState::EPlayer)
	{
		//save this controller pawn in a pointer
		ControllerPawnPtr = Cast<AControllerPawn>(this->GetPawn());
		this->UnPossess();
		this->Possess(SelectedVoltigeur);
	}

	//TODO else show Character Detail HUD
}

/*
//Step 2 of "Select"
void AVoltigeurPlayerController::ProcessSelectedCharacter(FHitResult Hit)
{
	//TODO change cast to generic base character
	AVoltigeurCharacterBase* SelectedVoltigeur = Cast<AVoltigeurCharacterBase>(Hit.GetActor());

	//if Hit is a Voltigeur unit
	if (SelectedVoltigeur && SelectedVoltigeur->GetFriendlyState() == EFriendlyState::EPlayer)
	{
		//TODO need to reset animation before Un-possessing the character
		//Un-possess current pawn
		AVoltigeurPlayerController::UnPossess();
		//Switch control to this character
		this->Possess(SelectedVoltigeur);
		//Switch PlayerController to CharacterController
		AVoltigeurPlayerController CharacterController = AVoltigeurPlayerController::StaticClass();
		//TODO reset camera??
	}
}
*/

void AVoltigeurPlayerController::GoToLocation(FHitResult Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("In Go To Location"));

	FVector loc = Hit.ImpactPoint;

	APawn* const Pawn = GetPawn();
	if (Pawn)
	{
		UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
		float const Distance = FVector::Dist(loc, Pawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if (NavSys && (Distance > 120.0f))
		{
			NavSys->SimpleMoveToLocation(this, loc);
		}
	}
}

//Rotates Pawn so it faces target's location
void AVoltigeurPlayerController::FaceSelected(FVector TargetLoc, float DeltaTime)
{
	//TODO DELETE
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("In face direction"));
	//Check if Target Location is same has current except Z-value
	FVector ActorLoc = GetPawn()->GetActorLocation();
	if (ActorLoc != TargetLoc)
	{
		GetPawn()->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(ActorLoc, TargetLoc));
	}
}