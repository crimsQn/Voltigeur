// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/RangedWeapon.h"
#include "Pistol.generated.h"

/**
 * 
 */
UCLASS()
class VOLTIGEUR_API APistol : public ARangedWeapon
{
	GENERATED_BODY()
	
public:
	APistol();

	virtual void ProjectileFire() override;
	
	//FORCEINLINE int32 GetWeaponPriority() { return WeaponConfig.Priority; }
};
