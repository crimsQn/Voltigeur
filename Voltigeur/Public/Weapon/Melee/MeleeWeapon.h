// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/Weapon.h"
#include "MeleeWeapon.generated.h"

/************************************************************************/
/* Types of projectiles                                                 */
/************************************************************************/
UENUM(BlueprintType)
enum class EWeaponDamageType : uint8
{
	ESlash			UMETA(DisplayName = "Slash"),
	EBlunt			UMETA(DisplayName = "Blunt"),
	EPierce			UMETA(DisplayName = "Pierce")
};

/**
 * 
 */
UCLASS()
class VOLTIGEUR_API AMeleeWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	AMeleeWeapon();

	virtual void Attack() override; 

protected:

};
