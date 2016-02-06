// Fill out your copyright notice in the Description page of Project Settings.

#include "Voltigeur.h"
#include "RangedWeapon.h"
#include "Projectile.h"

ARangedWeapon::ARangedWeapon()
{
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->AttachTo(RootComponent);
}

void ARangedWeapon::Attack()
{
	Super::Attack();
	if (ProjectileType == EWeaponProjectile::EProjectile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Projectile"));
		ProjectileFire();
	}
}

//@param need collision query paramater: e.g. ignore particular actor when tracing? Appear in scene? etc..
const FHitResult ARangedWeapon::WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo)
{
	static FName WeaponFireTag = FName(TEXT("WeaponTrace"));

	FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator); //Instigator is who is firing the trace
																		//Want trace to be synced with the scene?
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	//Don't want interference from this weapon object when tracing
	TraceParams.AddIgnoredActor(this);

	//we need to reinitialize a new line every time we fire the weapon or line will spawn at same old loc
	FHitResult Hit(ForceInit);

	//Created game trace channel in .h file
	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, TRACE_WEAPON, TraceParams);

	return Hit;
}

void ARangedWeapon::ProjectileFire()
{
	//calculate ammo
	if (CurrentAmmo > 0) CurrentAmmo -= WeaponConfig.ShotCost;
	else ReloadAmmo();
	//implemented in Appropriate ranged weapon sub-classes
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
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("Rifle Firing"));
		}
	}
}

void ARangedWeapon::ReloadAmmo()
{
	//if have enough cartridge
	if (CurrentClip > 0)
	{
		//replenish ammo count to full
		CurrentAmmo = WeaponConfig.MaxAmmo;
		//decrement clip count
		CurrentClip--;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("No Ammo"));
	}
}