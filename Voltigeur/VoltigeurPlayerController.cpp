// Fill out your copyright notice in the Description page of Project Settings.

#include "Voltigeur.h"
#include "VoltigeurPlayerController.h"
#include "VoltigeurGameMode.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine.h"
#include "BaseCharacter.h"
#include "VoltigeurDefaultPawn.h"
#include "RangedWeapon.h"

AVoltigeurPlayerController::AVoltigeurPlayerController()
{
	//Default Settings
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
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
	InputComponent->BindAction("MoveOrInteract", IE_Pressed, this, &AVoltigeurPlayerController::CheckUnderMouse);

	//Double-Click to interact
	//implement deselect
	InputComponent->BindAction("Deselect", IE_Pressed, this, &AVoltigeurPlayerController::DeselectToDefaultPawn);

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
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Selecting a Character"));
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
	//Check if Pawn is a default spanning pawn
	if (GetPawn() && GetPawn()->IsA(AVoltigeurDefaultPawn::StaticClass()))
	{
		//TODO DELETE it's a bit buggy
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Default Pawn Selecting a Character"));
		//Selection is processed in a dedicated function for controlling a default pawn
		ProcessSelectionPawn(Hit);
		return;
	}

	//TODO change cast to generic base character
	ABaseCharacter* SelectedChar = Cast<ABaseCharacter>(Hit.GetActor());
	ABaseCharacter* PlayerChar = Cast<ABaseCharacter>(GetPawn());

	//if valid pawn and selection
	if (SelectedChar && PlayerChar)
	{
		EFriendlyState SelectedCharFriendlyState = SelectedChar->GetFriendlyState();

		if (SelectedCharFriendlyState == EFriendlyState::EPlayer)
		{
			//TODO need to reset animation before Un-possessing the character
			//Un-possess current pawn
			AVoltigeurPlayerController::UnPossess();
			//Switch control to this character
			this->Possess(SelectedChar);
			//TODO reset camera??
		}
		//Check if unit under cursor is hostile
		else if (SelectedCharFriendlyState == EFriendlyState::EHostile)
		{
			//Check if player character's target is valid
			if (PlayerChar->GetTarget() != SelectedChar)
			{
				//Set target
				PlayerChar->SetTarget(SelectedChar);
				//TODO Delete
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Targetting"));
				//Rotate controlled pawn to enemy target
				//TODO change DeltaTime for animation
				AVoltigeurPlayerController::FaceSelected(SelectedChar->GetActorLocation(), 0.f);
			}
			else //Valid target, then attack
			{
				//Check if Character is facing enemy
				AVoltigeurPlayerController::FaceSelected(SelectedChar->GetActorLocation(), 0.f);
				//Check if weapon is equipped
				if (PlayerChar->GetCurrentWeapon() != NULL)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Weapon Fired"));
					PlayerChar->GetCurrentWeapon()->Attack();
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
			PlayerChar->SetTarget(nullptr);
			//TODO Delete
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Un-Targetting"));
			//Rotate controlled pawn to enemy target
			//TODO change DeltaTime for animation
			AVoltigeurPlayerController::FaceSelected(SelectedChar->GetActorLocation(), 0.f);
		}
		//else select that character if out of range, interact or move to actor if in range

	}
	// TODO else peasant unit for example		else

}

void AVoltigeurPlayerController::ProcessSelectionPawn(FHitResult Hit)
{
	ABaseCharacter* SelectedChar = Cast<ABaseCharacter>(Hit.GetActor());

	//if Selected Character is a Player possessible, then possess it
	if (SelectedChar && SelectedChar->GetFriendlyState() == EFriendlyState::EPlayer)
	{
		//save this controller pawn in a pointer
		ControllerPawnPtr = Cast<AVoltigeurDefaultPawn>(this->GetPawn());
		//Destroy DefaultPawn and it will respawn if Player deselects another character
		ControllerPawnPtr->DetachFromControllerPendingDestroy();
		ControllerPawnPtr->Destroy(); //destroy default pawn
		this->Possess(SelectedChar);
		this->SetPawn(SelectedChar);
	}

	//TODO else show Character Detail HUD
}

void AVoltigeurPlayerController::DeselectToDefaultPawn()
{
	//TODO Delete
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("In deselect"));
	FHitResult Hit;
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Pawn), false, Hit);
	//check if Pawn is default pawn already
	AVoltigeurDefaultPawn* defaultpawn = Cast<AVoltigeurDefaultPawn>(GetPawn());
	if (GetPawn() == NULL || defaultpawn)
	{
		return;
	}
	if (Hit.bBlockingHit)
	{
		//TODO Stop character to idle animation
		GetPawn()->TurnOff();
		this->UnPossess(); //unpossess selected character
		FVector SpawnLoc = Hit.ImpactPoint;
		FRotator SpawnRot = FRotator(0.f, 0.f, 0.f); //arbitrary value
		FActorSpawnParameters SpawnParam;
		SpawnParam.Owner = this;
		SpawnParam.Instigator = Instigator;
		AVoltigeurDefaultPawn* NewPawn = GetWorld()->SpawnActor<AVoltigeurDefaultPawn>(SpawnLoc, SpawnRot, SpawnParam);
		if (NewPawn)
		{
			this->SetPawn(NewPawn);
			this->Possess(GetPawn());
			//TODO Delete
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("deselect Success"));
		}
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

