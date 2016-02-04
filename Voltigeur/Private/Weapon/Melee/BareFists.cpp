// Fill out your copyright notice in the Description page of Project Settings.

#include "Voltigeur.h"
#include "BareFists.h"

ABareFists::ABareFists()
{
	WeaponConfig.Name = "Bare Fists";
	WeaponConfig.WeaponRange = 50.f;
	WeaponConfig.Damage = 10.f;
	WeaponConfig.CategoryNum = 2;
	WeaponConfig.SerialNum = 0; //TODO replace with HashKey
	WeaponConfig.Caliber = 0.00f;
}

void ABareFists::Attack()
{

}
