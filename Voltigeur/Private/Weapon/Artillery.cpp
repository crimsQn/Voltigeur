// Fill out your copyright notice in the Description page of Project Settings.

#include "Voltigeur.h"
#include "Artillery.h"
#include "Engine.h"
#include "Projectile.h"


void AArtillery::ProjectileFire()
{
	Super::ProjectileFire();

	if (ProjectileClass != NULL)
	{
		//MFLoc muzzle flash location in skeleton
		FVector MFLoc = WeaponMesh->GetSocketLocation("Weapon_Socket");
		FRotator MFRot = WeaponMesh->GetSocketRotation("Weapon_Socket") + FRotator(0.f, 90.f, 0.f); //adjustment needed or proj flies sideways 
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		AProjectile const* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, MFLoc, MFRot, SpawnParams);
		if (Projectile)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("Projectile Spawned"));
		}
	}

}
