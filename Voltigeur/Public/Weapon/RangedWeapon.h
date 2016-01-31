// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/Weapon.h"
#include "RangedWeapon.generated.h"

#define TRACE_WEAPON ECC_GameTraceChannel1 //one of game's empty available channel. We named it Trace_Weapon

/************************************************************************/
/* Forward declarations                                                 */
/************************************************************************/
class AProjectile;


/************************************************************************/
/* Types of projectiles                                                 */
/************************************************************************/
UENUM(BlueprintType)
enum class EWeaponProjectile : uint8
{
	EBullet			UMETA(DisplayName = "Bullet"),
//	ESpread			UMETA(DisplayName = "Spread"),
	EProjectile		UMETA(DisplayName = "Projectile")
};

/**
 * Base ranged weapon class
 */
UCLASS()
class VOLTIGEUR_API ARangedWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARangedWeapon();

	/*Triggers the ProjectileFire for the RangedWeapon class*/
	UFUNCTION()
	virtual void Attack() override;

protected:

	/*If it's a instant hit; bullet or spread*/
	//UFUNCTION()
	//	void Instant_Fire();

	UFUNCTION()
	virtual void ProjectileFire(); //only called when using Artillery class

	UPROPERTY(EditDefaultsOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectile> ProjectileClass;

	/*Bullet, Spread, or Projectile*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config, meta = (AllowPrivateAccess = "true"))
	EWeaponProjectile ProjectileType;

	/*If the bullet hit something, then what*/
	//void ProcessInstantHit(const FHitResult &Impact, const FVector &Origin,
	//	const FVector &ShootDir, int32 RandomSeed, float ReticleSpread);

	/*Direction and point of the bullet impact*/
	const FHitResult WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo);
	
	
};
