// Fill out your copyright notice in the Description page of Project Settings.

#include "Voltigeur.h"
#include "Pistol.h"

APistol::APistol()
{
	WeaponConfig.Name = TEXT("Pistol");
	WeaponConfig.MaxAmmo = 10;
	WeaponConfig.TimeBetweenShots = 0.8f;
	WeaponConfig.ShotCost = 1; //one bullet per shot
	WeaponConfig.WeaponRange = 69.f;
	WeaponConfig.Damage = 50.f;
	WeaponConfig.CategoryNum = 1; //weapon slot number in inventory
	WeaponConfig.SerialNum = 0; //TODO replace with HashKey
	WeaponConfig.Caliber = 0.36f;
}

void APistol::ProjectileFire()
{
	Super::ProjectileFire();

}



