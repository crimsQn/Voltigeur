// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

/************************************************************************/
/* Forward Declarations                                                 */
/************************************************************************/
class ABaseCharacter;

/************************************************************************/
/* Weapon Data                                                          */
/************************************************************************/
USTRUCT()
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	/******************General Weapon Settings*****************/

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon General", meta = (AllowPrivateAccess = "true"))
	FString Name;

	/*Rifle is 0, Pistol 1, Melee 2*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon General", meta = (AllowPrivateAccess = "true"))
	int32 CategoryNum;

	/*Unique Array Index Num of this weapon instance: HashKey*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon General", meta = (AllowPrivateAccess = "true"))
	int32 SerialNum;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon General", meta = (AllowPrivateAccess = "true"))
	float WeaponRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon General", meta = (AllowPrivateAccess = "true"))
	float Damage;
	
	/*********************Ranged Weapon Settings*******************/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Ranged", meta = (AllowPrivateAccess = "true"))
	int32 MaxAmmo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Ranged", meta = (AllowPrivateAccess = "true"))
	int32 MaxClip;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Ranged", meta = (AllowPrivateAccess = "true"))
	float TimeBetweenShots;

	/*Shoot three bullets every fire for example*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Ranged", meta = (AllowPrivateAccess = "true"))
	int32 ShotCost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Ranged", meta = (AllowPrivateAccess = "true"))
	float WeaponSpread;

};

UCLASS()
class VOLTIGEUR_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();

	UFUNCTION()
	virtual void Attack();

	/*Weapon range, cost, etc...*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config, meta = (AllowPrivateAccess = "true"))
	FWeaponData WeaponConfig;
	
	UPROPERTY(EditDefaultsOnly, Category = Config, meta = (AllowPrivateAccess = "true"))
		int32 CurrentAmmo; //current count on Ammo

	UPROPERTY(EditDefaultsOnly, Category = Config, meta = (AllowPrivateAccess = "true"))
		int32 CurrentClip; //current count on Clip

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionComp; //so that item can be picked by a Character

	void SetOwningPawn(ABaseCharacter* NewOwner); //handy for multiplayer to see who's owning it

	void AttachToPlayer();
	void DetachFromPlayer();

	void OnEquip();
	void OnUnequip();

protected:

	/*Slide of pistol animation, muzzle, etc... instead of static mesh*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* WeaponMesh;

	ABaseCharacter* MyPawn;
};
