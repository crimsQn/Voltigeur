// Fill out your copyright notice in the Description page of Project Settings.

#include "Voltigeur.h"
#include "BareFists.h"

ABareFists::ABareFists()
{
	WeaponConfig.Name = TEXT("Bare Fists");
	WeaponConfig.WeaponRange = 140.f;
	WeaponConfig.Damage = 25.f;
	WeaponConfig.CategoryNum = 2;
	WeaponConfig.SerialNum = 0; //TODO replace with HashKey
	WeaponConfig.Caliber = 0.00f;


}

void ABareFists::Attack()
{
	Super::Attack();

}
