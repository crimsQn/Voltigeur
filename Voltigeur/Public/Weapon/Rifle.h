// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/RangedWeapon.h"
#include "Rifle.generated.h"

/**
 * 
 */
UCLASS()
class VOLTIGEUR_API ARifle : public ARangedWeapon
{
	GENERATED_BODY()
	

public:
	virtual void ProjectileFire() override;
	
	
};
