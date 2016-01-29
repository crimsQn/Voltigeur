// Fill out your copyright notice in the Description page of Project Settings.

#include "Voltigeur.h"
#include "BaseCharacter.h"
#include "RangedWeapon.h"
#include "Pistol.h"
#include "Rifle.h"
#include "Engine.h"
#include "Engine/Blueprint.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;




	SetupCameraSettings();
	SetupCharacterSettings();
	SetupCollisionSettings();

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	/*Who is making spawn happen*/
	/*	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(WeaponSpawn, SpawnParams);
	if (Spawner)
	{
	Spawner->AttachRootComponentTo(GetMesh(), "Weapon_Socket", EAttachLocation::SnapToTarget);
	CurrentWeapon = Spawner;
	}
	*/
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Pistol", IE_Pressed, this, &ABaseCharacter::EquipPistol);
	InputComponent->BindAction("Rifle", IE_Pressed, this, &ABaseCharacter::EquipRifle);
	InputComponent->BindAction("Melee", IE_Pressed, this, &ABaseCharacter::EquipMelee);

	/**Camera Settings**/
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ABaseCharacter::ZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &ABaseCharacter::ZoomOut);
	InputComponent->BindAction("ChangeView", IE_Pressed, this, &ABaseCharacter::ChangeView);
}

void ABaseCharacter::SetupCameraSettings()
{
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = DEFAULT_ARM_LENGTH;
	CameraBoom->RelativeRotation = DEFAULT_CAMERA_ROT;
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// third person camera toggle
	OnThirdPersonCam = false;
}

void ABaseCharacter::SetupCollisionSettings()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	CollisionComp->AttachParent = GetRootComponent();

	/*Weapon Pickup Collision Configs*/
	//When an item enters the collision comp, call OnCollision
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnCollision);
}

void ABaseCharacter::SetupCharacterSettings()
{
	//Check out USTRUCT FCharacterData in .h fileV
	CharacterData.health = 100.f;
	Target = nullptr; //no enemy targeting
	Interlocutor = nullptr; // not interacting with anyone
	CurrentFriendlyState = EFriendlyState::ENeutral; //Neutral to player by default

	/*Inventory*/
	WeaponInventory.SetNum(WeaponSlot.INVENTORY_SIZE, false);

	CurrentWeapon = NULL; //NULL value is bare-hands

}

void ABaseCharacter::ZoomOut()
{
	//Top-Down cam
	if (!OnThirdPersonCam)
	{
		const float ADJ_MAGN = CameraBoom->TargetArmLength + ZOOM_INCREMENT;
		if (ADJ_MAGN < MAX_TOPDOWN_BOOM_LENGTH)
		{
			CameraBoom->TargetArmLength = ADJ_MAGN;
		}
		else
		{
			CameraBoom->TargetArmLength = MAX_TOPDOWN_BOOM_LENGTH;
		}
	}
	else //Third Person Cam
	{
		//check current cam rotation
		//if camera did not exceed the maximum bounds, then zoom out
		if (CameraBoom->RelativeRotation.Pitch < MAX_THIRDP_CAM_ROT_PITCH)
		{
			//TODO DELETE
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("3P ZoomOut"));
			//change rotation
			//CameraBoom->RelativeRotation.Pitch += CAM_THIRDP_ROT_INC;
			FRotator NewRotation = CameraBoom->RelativeRotation.operator+=(CAM_THIRDP_ROT_INC);
			//CameraBoom->RelativeRotation = NewRotation;
			CameraBoom->SetRelativeRotation(NewRotation);
			//change boom length
			CameraBoom->TargetArmLength += ZOOM_THIRDP_INC;
			OnThirdPersonCam = true;
		}
		else //restore default Zoomed in camera in Top-Down camera
		{
			//change rotation
			//CameraBoom->RelativeRotation = DEFAULT_CAMERA_ROT;
			//change boom length
			CameraBoom->TargetArmLength = MIN_TOPDOWN_BOOM_LENGTH;
			OnThirdPersonCam = false;
		}
	}
}

void ABaseCharacter::ZoomIn()
{
	//Top-Down cam
	if (!OnThirdPersonCam)
	{
		const float ADJ_MAGN = CameraBoom->TargetArmLength - ZOOM_INCREMENT;
		if (ADJ_MAGN > MIN_TOPDOWN_BOOM_LENGTH) //Zoom in
		{
			CameraBoom->TargetArmLength = ADJ_MAGN;
			OnThirdPersonCam = false; //next time, don't go into third person cam
		}
		else
		{
			CameraBoom->TargetArmLength = MIN_TOPDOWN_BOOM_LENGTH;
			OnThirdPersonCam = true; //next time, go into third person cam
		}
	}
	else //Third-person Cam
	{
		//check current cam rotation
		//if camera is did not exceed the minimum bounds, then zoom in
		if (CameraBoom->RelativeRotation.Pitch > MIN_THIRDP_CAM_ROT_PITCH)
		{
			//TODO DELETE
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("3P ZoomIn"));
			//change rotation
			//CameraBoom->RelativeRotation.Pitch -= CAM_THIRDP_ROT_INC;
			FRotator NewRotation = CameraBoom->RelativeRotation.operator-=(CAM_THIRDP_ROT_INC);
			CameraBoom->SetRelativeRotation(NewRotation); 
			//CameraBoom->RelativeRotation = NewRotation;
			//change boom length
			CameraBoom->TargetArmLength -= ZOOM_THIRDP_INC;
		}
	}
}

//TODO
void ABaseCharacter::ChangeView()
{

}

void ABaseCharacter::SetFriendlyState(EFriendlyState NewState)
{
	CurrentFriendlyState = NewState;
}

void ABaseCharacter::SetTarget(ACharacter* const Enemy)
{
	Target = Enemy;
}

void ABaseCharacter::SetInterlocutor(ACharacter* const Interlocutor)
{
	this->Interlocutor = Interlocutor;
}

void ABaseCharacter::EquipPistol()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	ARangedWeapon* Spawner = GetWorld()->SpawnActor<ARangedWeapon>(WeaponInventory[WeaponSlot.PISTOL], SpawnParams);
	//if pistol is successfully spawned
	if (Spawner)
	{
		//if character has a weapon in hand
		if (CurrentWeapon != NULL)
		{
			//if found duplicate weapon in inventory and in-hand while un-equipping, destroy duplicate
			if (WeaponInventory[WeaponSlot.PISTOL] != NULL &&
				WeaponInventory[WeaponSlot.PISTOL]->GetDefaultObject<ARangedWeapon>()->WeaponConfig.Name
				== CurrentWeapon->WeaponConfig.Name)
			{
				//Free up memory of duplicated item in inventory slot
				WeaponInventory[WeaponSlot.PISTOL]->GetDefaultObject<ARangedWeapon>()->Destroy();
				WeaponInventory[WeaponSlot.PISTOL] = CurrentWeapon->GetClass();
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Put away " + CurrentWeapon->WeaponConfig.Name + " in Slot: " + FString::FromInt(WeaponSlot.PISTOL));
			}
			CurrentWeapon->Destroy();
			Spawner->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); //need to temporarily disable collision to equip
			Spawner->AttachRootComponentTo(GetMesh(), "Weapon_Socket", EAttachLocation::SnapToTarget);
			CurrentWeapon = Spawner;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Current Weapon " + CurrentWeapon->WeaponConfig.Name);
		}
		else //character is not wielding any weapon
		{
			Spawner->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); //need to temporarily disable collision to equip
			Spawner->AttachRootComponentTo(GetMesh(), "Weapon_Socket", EAttachLocation::SnapToTarget);
			CurrentWeapon = Spawner;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Current Weapon " + CurrentWeapon->WeaponConfig.Name);
		}
	}
}

void ABaseCharacter::EquipRifle()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	ARangedWeapon* Spawner = GetWorld()->SpawnActor<ARangedWeapon>(WeaponInventory[WeaponSlot.RIFLE], SpawnParams);
	//if Rifle is successfully spawned
	if (Spawner)
	{
		//if character has a weapon in hand
		if (CurrentWeapon != NULL)
		{
			//if found duplicate weapon in inventory and in-hand while un-equipping, destroy duplicate
			if (WeaponInventory[WeaponSlot.RIFLE] != NULL &&
				WeaponInventory[WeaponSlot.RIFLE]->GetDefaultObject<ARangedWeapon>()->WeaponConfig.Name
				== CurrentWeapon->WeaponConfig.Name)
			{
				//Free up memory of duplicated item in inventory slot
				WeaponInventory[WeaponSlot.RIFLE]->GetDefaultObject<ARangedWeapon>()->Destroy();
				WeaponInventory[WeaponSlot.RIFLE] = CurrentWeapon->GetClass();
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Put away " + CurrentWeapon->WeaponConfig.Name + " in Slot: " + FString::FromInt(WeaponSlot.RIFLE));
			}
			CurrentWeapon->Destroy();
			Spawner->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); //need to temporarily disable collision to equip
			Spawner->AttachRootComponentTo(GetMesh(), "Weapon_Socket", EAttachLocation::SnapToTarget);
			CurrentWeapon = Spawner;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Current Weapon " + CurrentWeapon->WeaponConfig.Name);
		}
		else //character is not wielding any weapon
		{
			Spawner->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); //need to temporarily disable collision to equip
			Spawner->AttachRootComponentTo(GetMesh(), "Weapon_Socket", EAttachLocation::SnapToTarget);
			CurrentWeapon = Spawner;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Current Weapon " + CurrentWeapon->WeaponConfig.Name);
		}
	}
}

void ABaseCharacter::EquipMelee()
{

}

void ABaseCharacter::OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	/*Object collided with OtherActor is which weapon?*/
	APistol *Pistol = Cast<APistol>(OtherActor);
	if (Pistol)
	{
		WeaponInventory[WeaponSlot.PISTOL] = Pistol->GetClass();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Picked up " + Pistol->WeaponConfig.Name);
		Pistol->Destroy(); //after picked up destroy what's on ground
	}
	ARifle *Rifle = Cast<ARifle>(OtherActor);
	if (Rifle)
	{
		WeaponInventory[WeaponSlot.RIFLE] = Rifle->GetClass();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Picked up " + Rifle->WeaponConfig.Name);
		Rifle->Destroy(); //after picked up destroy what's on ground
	}
}