// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon.h"
#include "Artillery.generated.h"

/**
 * 
 */
UCLASS()
class VOLTIGEUR_API AArtillery : public AWeapon
{
	GENERATED_BODY()


	virtual void ProjectileFire() override;
	
};
