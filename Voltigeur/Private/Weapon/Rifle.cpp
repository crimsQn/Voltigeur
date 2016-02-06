// Fill out your copyright notice in the Description page of Project Settings.

#include "Voltigeur.h"
#include "Rifle.h"

ARifle::ARifle()
{
	WeaponConfig.Name = TEXT("Rifle");
	WeaponConfig.MaxAmmo = 36;
	WeaponConfig.TimeBetweenShots = 15.f;
	WeaponConfig.ShotCost = 1; //one bullet per shot
	WeaponConfig.WeaponRange = 300.f;
	WeaponConfig.Damage = 60.f;
	WeaponConfig.CategoryNum = 0; //weapon slot number in inventory
	WeaponConfig.SerialNum = 0; //TODO replace with HashKey
	WeaponConfig.Caliber = 0.58f;
}

void ARifle::ProjectileFire()
{
	Super::ProjectileFire();

}


