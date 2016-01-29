// Fill out your copyright notice in the Description page of Project Settings.

#include "Voltigeur.h"
#include "Rifle.h"
#include "Projectile.h"
#include "Engine.h"

ARifle::ARifle()
{
	WeaponConfig.Name = TEXT("Rifle");
	WeaponConfig.MaxAmmo = 36.f;
	WeaponConfig.TimeBetweenShots = 15.f;
	WeaponConfig.ShotCost = 1; //one bullet per shot
	WeaponConfig.WeaponRange = 300.f;
	WeaponConfig.Damage = 60.f;

}

void ARifle::ProjectileFire()
{
	Super::ProjectileFire();

	if (ProjectileClass != NULL)
	{
		FVector MuzzleLoc = WeaponMesh->GetSocketLocation("MuzzleFlashSocket"); //location of socket is on weapon mesh
		FRotator MuzzleRot = WeaponMesh->GetSocketRotation("MuzzleFlashSocket"); 
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		AProjectile const* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, MuzzleLoc, MuzzleRot, SpawnParams);
		if (Projectile)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("Projectile Spawned"));
		}
	}

}


