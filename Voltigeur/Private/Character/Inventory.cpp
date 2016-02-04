// Fill out your copyright notice in the Description page of Project Settings.

#include "Voltigeur.h"
#include "Inventory.h"
#include "Weapon.h"

Inventory::Inventory()
{

	WeaponInventory.SetNum(DEFAULT_INVENTORY_SIZE);
	NumWeapons = 0;


}

Inventory::~Inventory()
{
	WeaponInventory.Empty();
}

void Inventory::AddWeapon(AWeapon* NewWeapon)
{
	//input validation
	if (NewWeapon != NULL)
	{
		int32 WeaponTypeIndex = NewWeapon->WeaponConfig.CategoryNum;
		WeaponInventory.Insert(NewWeapon, WeaponTypeIndex);
		NumWeapons++;
	}
}

bool Inventory::RemoveWeapon(AWeapon* Weapon)
{
	int32 NumOfItems = WeaponInventory.RemoveSingle(Weapon);
	NumWeapons--;
	return NumOfItems == 1 ? true : false;
}

AWeapon* Inventory::GetNextWeaponClass(int32 current_weapon_index)
{
	AWeapon* Weapon = NULL;
	//Check if one increment of Current Weapon index will exceed array length
	if (current_weapon_index + 1 >= DEFAULT_INVENTORY_SIZE)
		current_weapon_index = 0; //reset current weapon index to 0
	else
		current_weapon_index++; //start from next index

	//find next weapon
	//TODO Logic: There is always a barefist weapon in the last index of the array
	for (int32 i = current_weapon_index; i < WeaponInventory.Num(); i++)
	{
		//if index contains a valid weapon
		if (WeaponInventory[i] != NULL)
		{
			//TODO
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Black, TEXT("Found Next Weapon"));
			//set return weapon
			Weapon = WeaponInventory[i];
			return Weapon;
		}
	}
	return Weapon;
}

AWeapon* Inventory::GetPrevWeaponClass(int32 current_weapon_index)
{
	AWeapon* Weapon = NULL;
	//Check if previous index can be below 0, reset index to array length -1
	if (current_weapon_index < 1) current_weapon_index = WeaponInventory.Num();

	//find previous weapon
	for (int32 i = current_weapon_index - 1; i >= 0; i--)
	{
		//if index contains a valid weapon
		if (WeaponInventory[i] != NULL)
		{
			//set return weapon
			Weapon = WeaponInventory[i];
			return Weapon;
		}
	}
	return Weapon;
}

AWeapon* Inventory::GetMelee()
{
	AWeapon* Weapon = NULL;
	Cast<AWeapon>(WeaponInventory[TERTIARY_WEAPON_INDEX]) != NULL ? 
		Weapon = WeaponInventory[TERTIARY_WEAPON_INDEX] : Weapon = NULL; //error
	return Weapon;
}

AWeapon* Inventory::GetWeaponInArray(int32 index)
{
	//input validation
	if (index >= 0 && index < WeaponInventory.Num())
	{
		return WeaponInventory[index];
	}
	return nullptr; //error
}

void Inventory::SetWeaponInArray(int32 index, AWeapon* NewWeapon)
{
	//input validation
	if (index >= 0 && index < WeaponInventory.Num() && NewWeapon != NULL)
	{
		WeaponInventory[index] = NewWeapon;
	}
}