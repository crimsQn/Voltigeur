// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Voltigeur.h"
#include "VoltigeurPlayerController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine.h"
#include "VoltigeurCharacterBase.h"

AVoltigeurPlayerController::AVoltigeurPlayerController()
{
	//Default Settings
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::CardinalCross;
	//End of Default Settings
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

	//Scroll between WeaponInventory
	InputComponent->BindAction("CycleWeaponInventory", IE_Pressed, this, &AVoltigeurPlayerController::CycleWeaponInventory);
}

//Step 1 of "Select"
void AVoltigeurPlayerController::CheckUnderMouse()
{
	FHitResult Hit; //is whatever was under the mouse cursor right now
					//TODO Create trace type, instead of converting?
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Pawn), false, Hit);

	//Hit is an character 
	if (Hit.GetActor() && Hit.GetActor()->IsA(ACharacter::StaticClass()))
	{
		//TODO delete
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Selecting a Character"));
		AVoltigeurPlayerController::ProcessSelectedCharacter(Hit);
	}
	else if (Hit.bBlockingHit)
	{
		AVoltigeurPlayerController::GoToLocation(Hit);
	}
}

//Step 2 of "Select"
void AVoltigeurPlayerController::ProcessSelectedCharacter(FHitResult Hit)
{
	//TODO change cast to generic base character
	AVoltigeurCharacterBase* SelectedVoltigeur = Cast<AVoltigeurCharacterBase>(Hit.GetActor());
	AVoltigeurCharacterBase* PlayerVoltigeur = Cast<AVoltigeurCharacterBase>(GetPawn());

	//if Hit is a Voltigeur unit
	if (SelectedVoltigeur && PlayerVoltigeur)
	{
		EFriendlyState SelectedCharFriendlyState = SelectedVoltigeur->GetFriendlyState();

		//Check if unit under cursor is hostile
		if (SelectedCharFriendlyState == EFriendlyState::EHostile)
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
		else if (SelectedCharFriendlyState == EFriendlyState::EPlayer)
		{
			//TODO need to reset animation before Un-possessing the character
			//Un-possess current pawn
			AVoltigeurPlayerController::UnPossess();
			//Switch control to this character
			this->Possess(SelectedVoltigeur);
			//TODO reset camera??
		}
		else //Neutral or friendly units
		{
			//Make sure player's target is null
			PlayerVoltigeur->SetTarget(nullptr);
			//TODO Delete
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Un-Targetting"));
			//Rotate controlled pawn to enemy target
			//TODO change DeltaTime for animation
			AVoltigeurPlayerController::FaceSelected(SelectedVoltigeur->GetActorLocation(), 1.f);
		}
		//else select that character if out of range, interact or move to actor if in range

	}
	// TODO else peasant unit for example		else

}

//Rotates Pawn so it faces target's location
void AVoltigeurPlayerController::FaceSelected(FVector targetloc, float DeltaTime)
{
	//Check for valid pawn
	if (GetPawn() == nullptr)
	{
		return;
	}
	//Check if Target Location is same has current except Z-value
	FVector ActorLoc = GetPawn()->GetActorLocation();
	if (ActorLoc.X == targetloc.X && ActorLoc.Y == targetloc.Y)
	{
		return;
	}
	else
	{
		FVector Direction = targetloc - ActorLoc;
		FRotator NewRotation = Direction.Rotation();
		//Rotation to face target is only affected by yaw
		NewRotation.Yaw = FRotator::ClampAxis(NewRotation.Yaw);

		//Update mesh
		GetPawn()->FaceRotation(NewRotation, DeltaTime);
	}
}


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

void AVoltigeurPlayerController::CycleWeaponInventory()
{
	APawn* const Pawn = GetPawn();
	if (Pawn)
	{
		
	}
}