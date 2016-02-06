// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/Melee/MeleeWeapon.h"
#include "BareFists.generated.h"

/************************************************************************/
/* Forward Declaration                                                  */
/************************************************************************/
class ABaseCharacter;

/**
 * 
 */
UCLASS()
class VOLTIGEUR_API ABareFists : public AMeleeWeapon
{
	GENERATED_BODY()
	
public:
	ABareFists();

	UFUNCTION(BlueprintCallable, Category = "Target")
	virtual void Attack() override;
	
	//int32 Priority;

	//FORCEINLINE int32 GetWeaponPriority() { return WeaponConfig.Priority; }


};
