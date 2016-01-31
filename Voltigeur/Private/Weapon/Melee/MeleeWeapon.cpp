// Fill out your copyright notice in the Description page of Project Settings.

#include "Voltigeur.h"
#include "MeleeWeapon.h"

AMeleeWeapon::AMeleeWeapon()
{
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->AttachTo(RootComponent);
}


