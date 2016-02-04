// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/************************************************************************/
/* Forward Declarations                                                 */
/************************************************************************/
class AWeapon;

/**
 * Inventory includes a container which holds currently equipped items.
 * And other items in character's storage.
 * Rifle (Primary) = 0
 * Pistol (Secondary) = 1
 * Melee (Tertiary) = 2
 * TODO: User can toggle each weapon class to switch to different weapons in the class.
 * For instance if "keyboard: 1" is bound as Rifle, User can toggle by hitting 1 between different
 * Rifle objects if there are multiples.
 * For now, it is a simple array assuming that there is only one weapon per weapon class.
 */
class VOLTIGEUR_API Inventory
{
public:
	Inventory();
	~Inventory();

	void AddWeapon(AWeapon* NewWeapon);
	bool RemoveWeapon(AWeapon* Weapon); //removes weapon if there is a match
	AWeapon* GetNextWeaponClass(int32 current_weapon_index); //traverses to next weapon class
	AWeapon* GetPrevWeaponClass(int32 current_weapon_index); //traverses to previous weapon class
	AWeapon* GetMelee(); //this is default weapon
	AWeapon* GetWeaponInArray(int32 index);
	void SetWeaponInArray(int32 index, AWeapon* NewWeapon);
	inline int32 GetNumWeapons() const { return NumWeapons; }


private:
	/************************************************************************/
	/*    Class Variables                                                   */
	/************************************************************************/
	TArray<AWeapon*> WeaponInventory; //for wielding weapon items. Not ones stored
	const int32 DEFAULT_INVENTORY_SIZE = 3;
	const int32 PRIMARY_WEAPON_INDEX = 0; //Rifles
	const int32	SECONDARY_WEAPON_INDEX = 1; //Pistol
	const int32 TERTIARY_WEAPON_INDEX = 2; //Melee and Fist
	int32 NumWeapons; //number of weapons stored in the weapon inventory

	/************************************************************************/
	/*    Functions                                                         */
	/************************************************************************/

};
