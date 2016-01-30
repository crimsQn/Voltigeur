// Fill out your copyright notice in the Description page of Project Settings.

#include "Voltigeur.h"
#include "RangedWeapon.h"
//#include "Projectile.h"

ARangedWeapon::ARangedWeapon()
{
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->AttachTo(RootComponent);
}

void ARangedWeapon::Attack()
{
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
	//implemented in Appropriate ranged weapon sub-classes
}