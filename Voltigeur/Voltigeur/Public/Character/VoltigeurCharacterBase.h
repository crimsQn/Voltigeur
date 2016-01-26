// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Weapon.h"
#include "Pistol.h"
#include "Rifle.h"
#include "Artillery.h"
#include "VoltigeurCharacterBase.generated.h"

UENUM(BlueprintType)
enum class EFriendlyState : uint8
{
	EPlayer				UMETA(DisplayName = "Player"),
	EFriendly			UMETA(DisplayName = "Friendly"),
	ENeutral			UMETA(DisplayName = "Neutral"),
	EHostile			UMETA(DisplayName = "Hostile")
};

/*Organize weapon inventory*/
struct FWeaponSlot 
{
//	const uint8 BareHands = 0;
	const uint8 Rifle = 0;
	const uint8 Pistol = 1;
	const uint8 Blade = 2;
	const uint8	Artillery = 3;
	const uint8 InventorySize = 4;
	uint8 CurrentEquippedSlot;
};

USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_USTRUCT_BODY()

	//Variables
	UPROPERTY(EditDefaultsOnly, Category = "Character Status", meta = (AllowPrivateAccess = "true"))
	float health; //health of character


	//Functions
	float GetHealth() const { return health; }
};

UCLASS()
class VOLTIGEUR_API AVoltigeurCharacterBase : public ACharacter
{
	GENERATED_BODY()

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:
	// Sets default values for this character's properties
	AVoltigeurCharacterBase();

	/*Default inherited functions*/
	///////////////////////////////
	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	//////////////////////////////
	/*END OF DEFAULT FUNCTIONS*/

	//Friendly towards player?
	void SetFriendlyState(EFriendlyState NewState);
	FORCEINLINE	EFriendlyState GetFriendlyState() const { return CurrentFriendlyState; }

	//Target enemy unit
	void SetTarget(ACharacter* const Enemy);
	FORCEINLINE	ACharacter* GetTarget() const { return Target; }

	//interlocutor is one that this character is interacting with
	void SetInterlocutor(ACharacter* const character);
	FORCEINLINE ACharacter* GetInterlocutor() const { return Interlocutor; }

	//Current Weapon
	FORCEINLINE AWeapon* GetCurrentWeapon() const { return CurrentWeapon; }


protected:

	/*Default inherited functions*/
	///////////////////////////////
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	//////////////////////////////
	/*END OF DEFAULT FUNCTIONS*/

	/*Set default settings such as capsule size, camera, etc...*/
	void SetDefaultSettings();

	/******************Weapons********************/
	void SetCharacterSettings(); //constructor to clean code
	void Wield();
	void ReduceHealth(float Damage);

	/*TODO for now, we will just spawn the weapon on map*/
	UPROPERTY(VisibleAnywhere, Category = Spawn)
	TSubclassOf<class AWeapon> WeaponSpawn;

	//Variables
	class AWeapon* CurrentWeapon; //initiated to NULL which means bare-hands 

	void Aim();
	/***************END Weapons*******************/

	/***************Character*********************/
	UPROPERTY(EditDefaultsOnly, Category = "Character Config")
	FCharacterData CurrentCharacterData;

	UPROPERTY(VisibleAnywhere, Category = "Character Status", meta = (AllowPrivateAccess = "false"))
	ACharacter* Target; //the character that this unit is targeting
	UPROPERTY(VisibleAnywhere, Category = "Character Status", meta = (AllowPrivateAccess = "false"))
	ACharacter* Interlocutor; //the character this unit is interacting/talking to

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Friendly Faction", meta = (AllowPrivateAccess = "true"))
	EFriendlyState CurrentFriendlyState; //hostile to player, etc...


	//TODO float EyeRange;

	/***************END Character****************/

	/*************Collision & Graphics***********/
	void SetCollisionSettings();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Collision", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionComp;

	/*
	@param OtherActor if other actor touches this CollisionComp 
	@param OtherComp seam component? I'm not sure...
	*/
	UFUNCTION()
	void OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	/*Equipments*/
	TArray<TSubclassOf<AWeapon>> WeaponInventory; //index 0 is nullptr which is fist
	FWeaponSlot WeaponSlot; //struct to organize slot # enumerations

	/***Weapon Equip***/
	void EquipPistol();
	void EquipRifle();
	void EquipArtillery();

private:
	/*Camera Settings*/
	void ZoomIn();
	void ZoomOut();
	const float ZOOM_INCREMENT = 50.f;
	const float MAX_BOOM_LENGTH = 1000.f;
	const float MIN_BOOM_LENGTH = 100.f;
	const float DEFAULT_ARM_LENGTH = 900.f;
	const FRotator DEFAULT_CAMERA_ROT = FRotator(-50.f, 0.f, 0.f);
	void ChangeView();
};
