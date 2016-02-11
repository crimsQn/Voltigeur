// Fill out your copyright notice in the Description page of Project Settings.

#include "Voltigeur.h"
#include "MeleeWeapon.h"
#include "BaseCharacter.h"

AMeleeWeapon::AMeleeWeapon()
{
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->AttachTo(RootComponent);

	//Animation
	Anim_IsAttacking = false;
}


void AMeleeWeapon::Attack()
{
	//if MyPawn
	if (MyPawn)
	{
		//if Target and valid range
		if (MyPawn->GetTarget())
		{
			//Check valid attacking distance
			FVector TargetLoc = MyPawn->GetTarget()->GetActorLocation();
			FVector PawnLoc = MyPawn->GetActorLocation();
			if (FVector::Dist(PawnLoc, TargetLoc) <= WeaponConfig.WeaponRange)
			{
				/*Execute Animation*/
				Anim_IsAttacking = true;

				/*inflict damage*/
				MyPawn->GetTarget()->InflictDamage(WeaponConfig.Damage);
			}
			else
			{
				//TODO Delete
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("Target Out of Range: ") + FString::SanitizeFloat(WeaponConfig.WeaponRange));
			}
		}
	}
}