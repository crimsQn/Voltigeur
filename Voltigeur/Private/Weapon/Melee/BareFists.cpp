// Fill out your copyright notice in the Description page of Project Settings.

#include "Voltigeur.h"
#include "BareFists.h"
#include "Engine.h"
#include "BaseCharacter.h"

ABareFists::ABareFists()
{
	WeaponConfig.Name = "Bare Fists";
	WeaponConfig.WeaponRange = 80.f;
	WeaponConfig.Damage = 10.f;
	WeaponConfig.CategoryNum = 2;
	WeaponConfig.SerialNum = 0; //TODO replace with HashKey
	WeaponConfig.Caliber = 0.00f;
}

void ABareFists::Attack()
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
			if (FVector::Dist(PawnLoc, TargetLoc) > WeaponConfig.WeaponRange)
			{
				IsPunching = true; //TODO start animation
				//inflict damage
				MyPawn->GetTarget()->InflictDamage(WeaponConfig.Damage);
			}
			else
			{
				//TODO Delete
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("Target Out of Range"));
			}
		}
	}
}
