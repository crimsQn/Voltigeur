// Fill out your copyright notice in the Description page of Project Settings.

#include "Voltigeur.h"
#include "Pistol.h"
#include "Projectile.h"
#include "Engine.h"

void APistol::ProjectileFire()
{
	Super::ProjectileFire();

	if (ProjectileClass != NULL)
	{
		FVector MuzzleLoc = WeaponMesh->GetSocketLocation("Weapon_Socket");
		FRotator MuzzleRot = WeaponMesh->GetSocketRotation("Weapon_Socket") + FRotator(0.f, 90.f, 0.f); //adjustment needed or proj flies sideways 
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



