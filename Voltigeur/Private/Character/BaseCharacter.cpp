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
void ABaseCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

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

	  /*From WeaponEssential Tutorial 1*/
	 //	static ConstructorHelpers::FObjectFinder<UBlueprint> WeaponBP(TEXT("Blueprint'/Game/Blueprint/Weapon/Weapon_BP.Weapon_BP'"));
	  //	WeaponSpawn = NULL; //make sure it is cleared
	 //	if (WeaponBP.Succeeded())
	 //	{
	//		WeaponSpawn = (UClass*)WeaponBP.Object->GeneratedClass;
  //	}
}

void ABaseCharacter::ZoomIn()
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

void ABaseCharacter::ZoomOut()
{
	const float ADJ_MAGN = CameraBoom->TargetArmLength - ZOOM_INCREMENT;
	if (ADJ_MAGN > MIN_TOPDOWN_BOOM_LENGTH)
	{
		CameraBoom->TargetArmLength = ADJ_MAGN;
	}
	else
	{
		CameraBoom->TargetArmLength = MIN_TOPDOWN_BOOM_LENGTH;
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
			for (int32 i = 0; i < WeaponInventory.Num(); i++)
			{
				//if found duplicate weapon in inventory and in-hand while un-equipping, destroy duplicate
				if (WeaponInventory[i] != NULL && WeaponInventory[i]->GetDefaultObject<ARangedWeapon>()->WeaponConfig.Name
					== CurrentWeapon->WeaponConfig.Name)
				{
					//Free up memory of duplicated item in inventory slot
					WeaponInventory[i]->GetDefaultObject<ARangedWeapon>()->Destroy();
					WeaponInventory[i] = CurrentWeapon->GetClass();
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Put away " + CurrentWeapon->WeaponConfig.Name + " in Slot: " + FString::FromInt(i));
				}
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
	//if pistol is successfully spawned
	if (Spawner)
	{
		//if character has a weapon in hand
		if (CurrentWeapon != NULL)
		{
			for (int32 i = 0; i < WeaponInventory.Num(); i++)
			{
				//if found duplicate weapon in inventory and in-hand while un-equipping, destroy duplicate
				if (WeaponInventory[i] != NULL && WeaponInventory[i]->GetDefaultObject<ARangedWeapon>()->WeaponConfig.Name
					== CurrentWeapon->WeaponConfig.Name)
				{
					//Free up memory of duplicated item in inventory slot
					WeaponInventory[i]->GetDefaultObject<ARangedWeapon>()->Destroy();
					WeaponInventory[i] = CurrentWeapon->GetClass();
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Put away " + CurrentWeapon->WeaponConfig.Name + " in Slot: " + FString::FromInt(i));
				}
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