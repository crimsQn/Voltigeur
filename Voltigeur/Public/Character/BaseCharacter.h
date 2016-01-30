// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

/************************************************************************/
/* Forward Declarations                                                 */
/************************************************************************/
class AWeapon;
class ARangedWeapon;
class AMeleeWeapon;


/*State of character's friendlyness to Player. This will eventually determine
AI behaviors*/
UENUM(BlueprintType)
enum class EFriendlyState : uint8
{
	EPlayer			UMETA(DisplayName = "Player"),
	EFriendly		UMETA(DisplayName = "Friendly"),
	ENeutral		UMETA(DisplayName = "Neutral"),
	EHostile		UMETA(DisplayName = "Hostile")
};

/*Basic inventory struct for weapons*/
struct FWeaponSlot
{
	//const uint8 BAREHANDS = 0;
	const uint8 RIFLE = 0;
	const uint8 PISTOL = 1;
	const uint8 MELEE = 2; //blade or tools

	const uint8 INVENTORY_SIZE = 3;
	uint8 CurrentlyEquippedSlot;
};

USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Character Status", meta = (AllowPrivateAccess = "true"))
	float health; //Character's health
	//TODO float EyeRange;
	//TODO implement stat system
};

UCLASS()
class VOLTIGEUR_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Set Friendly State
	void SetFriendlyState(EFriendlyState NewState);
	// Getter for Friendly State
	FORCEINLINE	EFriendlyState GetFriendlyState() const { return CurrentFriendlyState; }

	//Target enemy unit
	void SetTarget(ACharacter* const Enemy);
	FORCEINLINE	ACharacter* GetTarget() const { return Target; }

	//interlocutor is one that this character is interacting with
	void SetInterlocutor(ACharacter* const character);
	FORCEINLINE ACharacter* GetInterlocutor() const { return Interlocutor; }

	//TODO Need to use generic Weapon
	//Current Weapon
	FORCEINLINE AWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

protected:
	/*****************Camera Settings*****************/
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	void SetupCameraSettings();

	/****Default Top-Down Settings****/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float ZOOM_INCREMENT = 70.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float MAX_TOPDOWN_BOOM_LENGTH = 1000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float MIN_TOPDOWN_BOOM_LENGTH = 700.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float DEFAULT_ARM_LENGTH = 900.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	FRotator DEFAULT_CAMERA_ROT = FRotator(-50.f, 0.f, 0.f);

	/****Default Third-Person Settings****/
	bool OnThirdPersonCam; //checks if zoom level exceeds min bounds of Top-Down camera, then go to third person cam

	//Cam Rotation
	float MIN_THIRDP_CAM_ROT_PITCH = -90.f; //at this pitch angle, cam is facing char's back
	float MAX_THIRDP_CAM_ROT_PITCH = -50.f; //also default angle of top down cam
	FRotator CAM_THIRDP_ROT_INC = FRotator(10.f, 0, 0); // increment how the cam rotates: 4 MUST match with Zoom Increment!
	//float CAM_THIRDP_ROT_INC = 10.f;

	//Cam Length
	float MIN_THIRDP_BOOM_LENGTH = 400.f;
	float MAX_THIRDP_BOOM_LENGTH = 700.f;
	float ZOOM_THIRDP_INC = 50.f; //in ThirdPerson Cam, there is 4 zoom increments : Must match with Cam Rot Inc
	
	void ZoomIn();
	void ZoomOut();
	void ChangeView();

	/*****************Interaction Settings***************/
	UPROPERTY(VisibleAnywhere, Category = "Character Status", meta = (AllowPrivateAccess = "false"))
	ACharacter* Target; //the character that this unit is targeting
	
	UPROPERTY(VisibleAnywhere, Category = "Character Status", meta = (AllowPrivateAccess = "false"))
	ACharacter* Interlocutor; //the character this unit is interacting/talking to

	/******************Character Data Settings*****************/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Status", meta = (AllowPrivateAccess = "true"))
	EFriendlyState CurrentFriendlyState; //hostile to player, etc...

	UPROPERTY(EditDefaultsOnly, Category = "Character Status")
	FCharacterData CharacterData;


	/******************Collision Settings**********************/
	/*
	@param OtherActor if other actor touches this CollisionComp
	@param OtherComp seam component? I'm not sure...
	*/
	UFUNCTION()
	void OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
	void SetupCollisionSettings();

	//If an actor collides such as weapon trigger pickup
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Collision", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionComp;

	/******************Weapon Interaction Settings*************/
	void SetupCharacterSettings();

	UPROPERTY(EditDefaultsOnly, Category = DefaultInventory)
	TSubclassOf<AWeapon> WeaponSpawn;

	/*Equipments*/
	TArray<AWeapon*> WeaponInventory; //index 0 is nullptr which is fist
	void ProcessWeaponPickup(AWeapon *Weapon);
	FWeaponSlot WeaponSlot; //struct to organize slot # enumerations
	class AWeapon* CurrentWeapon; //initiated to NULL which means bare-hands 
	void NextWeapon();
	void PrevWeapon();
	void EquipWeapon(AWeapon *Weapon);
	void GiveDefaultWeapon();

	void Aim(); //TODO need to implement



};
