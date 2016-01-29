// Fill out your copyright notice in the Description page of Project Settings.

#include "Voltigeur.h"
#include "Pistol.h"
#include "Projectile.h"
#include "Engine.h"

APistol::APistol()
{
	WeaponConfig.Name = TEXT("Pistol");
	WeaponConfig.MaxAmmo = 10.f;
	WeaponConfig.TimeBetweenShots = 0.8f;
	WeaponConfig.ShotCost = 1; //one bullet per shot
	WeaponConfig.WeaponRange = 69.f;
	WeaponConfig.Damage = 50.f;
}

void APistol::ProjectileFire()
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



