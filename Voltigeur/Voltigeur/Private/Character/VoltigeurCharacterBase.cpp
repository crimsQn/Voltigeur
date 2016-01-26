// Fill out your copyright notice in the Description page of Project Settings.

#include "Voltigeur.h"
#include "VoltigeurCharacterBase.h"
#include "Engine.h"
#include "Engine/Blueprint.h"


// Sets default values
AVoltigeurCharacterBase::AVoltigeurCharacterBase()
{
	/*Clean up code for all the default settings*/
	SetDefaultSettings();
	SetCharacterSettings();
	SetCollisionSettings();
	
}

void AVoltigeurCharacterBase::SetDefaultSettings()
{
	/*Default Settings*/
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//	PrimaryActorTick.bCanEverTick = true;

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

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

void AVoltigeurCharacterBase::SetCharacterSettings()
{
	//Check out USTRUCT FCharacterData in .h fileV
	CurrentCharacterData.health = 100.f;
	Target = nullptr; //no enemy targeting
	Interlocutor = nullptr; // not interacting with anyone
	CurrentFriendlyState = EFriendlyState::ENeutral; //Neutral to player by default

	/*Inventory*/
	WeaponInventory.SetNum(WeaponSlot.InventorySize, false);

	CurrentWeapon = NULL; //NULL value is bare-hands

	/*From WeaponEssential Tutorial 1*/
//	static ConstructorHelpers::FObjectFinder<UBlueprint> WeaponBP(TEXT("Blueprint'/Game/Blueprint/Weapon/Weapon_BP.Weapon_BP'"));
//	WeaponSpawn = NULL; //make sure it is cleared
//	if (WeaponBP.Succeeded())
//	{
//		WeaponSpawn = (UClass*)WeaponBP.Object->GeneratedClass;
//	}

}

void AVoltigeurCharacterBase::SetCollisionSettings()
{
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	CollisionComp->AttachParent = GetRootComponent();

	/*Weapon Pickup Collision Configs*/
	//When an item enters the collision comp, call OnCollision
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AVoltigeurCharacterBase::OnCollision);
}

// Called when the game starts or when spawned
void AVoltigeurCharacterBase::BeginPlay()
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
void AVoltigeurCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVoltigeurCharacterBase::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Pistol", IE_Pressed, this, &AVoltigeurCharacterBase::EquipPistol);
	InputComponent->BindAction("Rifle", IE_Pressed, this, &AVoltigeurCharacterBase::EquipRifle);
	InputComponent->BindAction("Artillery", IE_Pressed, this, &AVoltigeurCharacterBase::EquipArtillery);

 	/**Camera Settings**/
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &AVoltigeurCharacterBase::ZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &AVoltigeurCharacterBase::ZoomOut);
	InputComponent->BindAction("ChangeView", IE_Pressed, this, &AVoltigeurCharacterBase::ChangeView);
}

void AVoltigeurCharacterBase::SetTarget(ACharacter* const Enemy)
{
	Target = Enemy;
}

void AVoltigeurCharacterBase::SetInterlocutor(ACharacter* const character)
{
	Interlocutor = character;
}

// Wields the weapon on the character
//void AVoltigeurCharacterBase::Wield()
//{
/* TODO
	uint8* CurrentSlotNo = &WeaponSlot.CurrentEquippedSlot;

	//TODO Math check
	switch (CurrentSlotNo)
	{
		case WeaponSlot.BareHands :
			CurrentSlotNo++;
			break;
		case WeaponSlot.Pistol :
			CurrentSlotNo++;
			break;
		case WeaponSlot.Rifle :
			CurrentSlotNo++;
			break;
		case WeaponSlot.Throwable :
			CurrentSlotNo++;
			break;
		case WeaponSlot.Blade :
			CurrentSlotNo = WeaponSlot.BareHands;
			break;
	}
*/ //TODO

	/*Who is making spawn happen*/

/* //TODO

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(WeaponSpawn, SpawnParams);
	if (Spawner)
	{
		Spawner->AttachRootComponentTo(GetMesh(), "Weapon_Socket", EAttachLocation::SnapToTarget);
		CurrentWeapon = Spawner;
	}
	
	//Attach to mesh

	//Wield animation
}


//void AVoltigeurCharacterBase::Attack()
//{
//	if (Target == nullptr)
//	{
//		return;
//	}
//	CurrentWeapon->Fire();
	//Check pawn if valid Character
	//Get Weapon damage
	//Get health from pawn
	//Do attack check such as if attack is successful
	//inflict damage to this character or the enemy character
}
*/ //TODO

void AVoltigeurCharacterBase::OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	/*Object collided with OtherActor a ... ?*/
	APistol *Pistol = Cast<APistol>(OtherActor);
	if (Pistol)
	{
		WeaponInventory[WeaponSlot.Pistol] = Pistol->GetClass();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Picked up " + Pistol->WeaponConfig.Name);
		Pistol->Destroy(); //after picked up destroy what's on ground
	}
	ARifle *Rifle = Cast<ARifle>(OtherActor);
	if (Rifle)
	{
		WeaponInventory[WeaponSlot.Rifle] = Rifle->GetClass();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Picked up " + Rifle->WeaponConfig.Name);
		Rifle->Destroy(); //after picked up destroy what's on ground
	}
	AArtillery *Artillery = Cast<AArtillery>(OtherActor);
	if (Artillery)
	{
		WeaponInventory[WeaponSlot.Artillery] = Artillery->GetClass();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Picked up " + Artillery->WeaponConfig.Name);
		Artillery->Destroy(); //after picked up destroy what's on ground
	}
}

void AVoltigeurCharacterBase::EquipPistol()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(WeaponInventory[WeaponSlot.Pistol], SpawnParams);
	if (Spawner) //if successfully spawn pistol
	{
		if (CurrentWeapon != NULL) //if character is weapon in hand
		{
			for (int32 i = 0; i < WeaponInventory.Num(); i++)
			{
				//if found duplicate weapon in inventory and in-hand while un-equipping, then remove it
				if (WeaponInventory[i] != NULL && WeaponInventory[i]->GetDefaultObject<AWeapon>()->WeaponConfig.Name
					== CurrentWeapon->WeaponConfig.Name)
				{
					//Free up memory of duplicated item in inventory slot
					WeaponInventory[i]->GetDefaultObject<AWeapon>()->Destroy();
//					WeaponInventory[i] = NULL;
					WeaponInventory[i] = CurrentWeapon->GetClass();
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Put away " + CurrentWeapon->WeaponConfig.Name + " in Slot: " + FString::FromInt(i));
				}
			}
			CurrentWeapon->Destroy();
			Spawner->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Spawner->AttachRootComponentTo(GetMesh(), "Weapon_Socket", EAttachLocation::SnapToTarget);
			CurrentWeapon = Spawner;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Current Weapon " + CurrentWeapon->WeaponConfig.Name);
		}
		else
		{
			Spawner->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Spawner->AttachRootComponentTo(GetMesh(), "Weapon_Socket", EAttachLocation::SnapToTarget);
			CurrentWeapon = Spawner;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Current Weapon " + CurrentWeapon->WeaponConfig.Name);
		}
	}
}

void AVoltigeurCharacterBase::EquipRifle()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(WeaponInventory[WeaponSlot.Rifle], SpawnParams);
	if (Spawner) //if successfully spawn weapon 
	{
		if (CurrentWeapon != NULL) //if character is weapon in hand
		{
			for (int32 i = 0; i < WeaponInventory.Num(); i++)
			{
				//if found duplicate weapon in inventory and in-hand while un-equipping, then remove it
				if (WeaponInventory[i] != NULL && WeaponInventory[i]->GetDefaultObject<AWeapon>()->WeaponConfig.Name
					== CurrentWeapon->WeaponConfig.Name)
				{
					//Free up memory of duplicated item in inventory slot
					WeaponInventory[i]->GetDefaultObject<AWeapon>()->Destroy();
					WeaponInventory[i] = CurrentWeapon->GetClass();
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Put away " + CurrentWeapon->WeaponConfig.Name + " in Slot: " + FString::FromInt(i));
				}
			}
			CurrentWeapon->Destroy();
			Spawner->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Spawner->AttachRootComponentTo(GetMesh(), "Weapon_Socket", EAttachLocation::SnapToTarget);
			CurrentWeapon = Spawner;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Current Weapon " + CurrentWeapon->WeaponConfig.Name);
		}
		else
		{
			Spawner->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Spawner->AttachRootComponentTo(GetMesh(), "Weapon_Socket", EAttachLocation::SnapToTarget);
			CurrentWeapon = Spawner;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Current Weapon " + CurrentWeapon->WeaponConfig.Name);
		}
	}
}

void AVoltigeurCharacterBase::EquipArtillery()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(WeaponInventory[WeaponSlot.Artillery], SpawnParams);
	if (Spawner) //if successfully spawn weapon 
	{
		if (CurrentWeapon != NULL) //if character is weapon in hand
		{
			for (int32 i = 0; i < WeaponInventory.Num(); i++)
			{
				//if found duplicate weapon in inventory and in-hand while un-equipping, then remove it
				if (WeaponInventory[i] != NULL && WeaponInventory[i]->GetDefaultObject<AWeapon>()->WeaponConfig.Name
					== CurrentWeapon->WeaponConfig.Name)
				{
					//Free up memory of duplicated item in inventory slot
					WeaponInventory[i]->GetDefaultObject<AWeapon>()->Destroy();
					WeaponInventory[i] = CurrentWeapon->GetClass();
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Put away " + CurrentWeapon->WeaponConfig.Name + " in Slot: " + FString::FromInt(i));
				}
			}
			CurrentWeapon->Destroy();
			Spawner->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Spawner->AttachRootComponentTo(GetMesh(), "Weapon_Socket", EAttachLocation::SnapToTarget);
			CurrentWeapon = Spawner;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Current Weapon " + CurrentWeapon->WeaponConfig.Name);
		}
		else
		{
			Spawner->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Spawner->AttachRootComponentTo(GetMesh(), "Weapon_Socket", EAttachLocation::SnapToTarget);
			CurrentWeapon = Spawner;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Current Weapon " + CurrentWeapon->WeaponConfig.Name);
		}
	}
}

void AVoltigeurCharacterBase::ZoomIn()
{
	const float ADJ_MAGN = CameraBoom->TargetArmLength + ZOOM_INCREMENT;
	if (ADJ_MAGN < MAX_BOOM_LENGTH)
	{
		CameraBoom->TargetArmLength = ADJ_MAGN;
	}
	else
	{
		CameraBoom->TargetArmLength = MAX_BOOM_LENGTH;
	}
}

void AVoltigeurCharacterBase::ZoomOut()
{
	const float ADJ_MAGN = CameraBoom->TargetArmLength - ZOOM_INCREMENT;
	if (ADJ_MAGN > MIN_BOOM_LENGTH)
	{
		CameraBoom->TargetArmLength = ADJ_MAGN;
	}
	else
	{
		CameraBoom->TargetArmLength = MIN_BOOM_LENGTH;
	}
}

void AVoltigeurCharacterBase::ChangeView()
{

}

void AVoltigeurCharacterBase::Aim()
{

}