// Fill out your copyright notice in the Description page of Project Settings.

#include "Voltigeur.h"
#include "BaseCharacter.h"
#include "Weapon.h"
#include "BareFists.h"
#include "Engine.h"
#include "Engine/Blueprint.h"
#include "Inventory.h"

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

	CurrentWeapon = GiveDefaultWeapon();
	InitializeInventory();

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

	/**Camera Settings**/
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ABaseCharacter::ZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &ABaseCharacter::ZoomOut);
	InputComponent->BindAction("ChangeView", IE_Pressed, this, &ABaseCharacter::ChangeView);

	/**Weapon Select**/
	InputComponent->BindAction("NextWeapon", IE_Pressed, this, &ABaseCharacter::NextWeapon);
	InputComponent->BindAction("PrevWeapon", IE_Pressed, this, &ABaseCharacter::PrevWeapon);
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

	//WeaponInventory.SetNum(3, false);

	CurrentWeapon = NULL; //NULL value is bare-hands

}

void ABaseCharacter::InitializeInventory()
{
	CharInventory = new Inventory::Inventory();

	/* Deprecated using Inventory.cpp instead
	//Spawn and give default weapon to character
	//AWeapon* DefaultWeapon = Cast<AWeapon>(GiveDefaultWeapon());
	//if (DefaultWeapon)
	//{
		WeaponInventory.SetNumUninitialized(WeaponInvConfig.INVENTORY_SIZE);
		MeleeWeaponContainer.SetNumUninitialized(1);
		MeleeWeaponContainer.Insert(CurrentWeapon, 0);

		//Add Weapon Containers
		//TODO hardcoded inventory indexes
		WeaponInventory.Insert(RifleContainer, 0);
		WeaponInventory.Insert(PistolContainer, 1);
		WeaponInventory.Insert(MeleeWeaponContainer, 2); //2 is where melee weapons are stored
		//TODO delete
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("Initialized Inventory"));
	//}
	*/
}

AWeapon* ABaseCharacter::GiveDefaultWeapon()
{

	/*
	//Give barefist to Character
	AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(WeaponSpawn);
	if (Spawner)
	{
		CurrentWeapon->SetOwningPawn(this);
		CurrentWeapon->OnEquip();
		CurrentWeaponTypeIndex = CurrentWeapon->WeaponConfig.CategoryNum;
		return Spawner;
	}
	else
	{
		return NULL;
	}
	*/

	/*TArray Implementation*/
	//AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(WeaponSpawn);
	//if (Spawner)
	//{
	//	WeaponInventory[Spawner->WeaponConfig.Priority] = Spawner;
	//	CurrentWeapon = WeaponInventory[Spawner->WeaponConfig.Priority];
	//	CurrentWeapon->SetOwningPawn(this);
	//	CurrentWeapon->OnEquip();
	//}
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

void ABaseCharacter::OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AWeapon *Weapon = Cast<AWeapon>(OtherActor);
	if (Weapon)
	{
		ProcessWeaponPickup(Weapon);
	}
}

void ABaseCharacter::ProcessWeaponPickup(AWeapon* Weapon)
{

	/*
	//Doublylinked list
	if (Weapon != NULL)
	{
		int32 CategoryNum = Weapon->WeaponConfig.CategoryNum;
		//Pointer to save memory
		//TArray<AWeapon*>* SubContainer = &WeaponInventory[Priority]; //this is class of weapon
		TArray<AWeapon*> SubContainer = WeaponInventory[CategoryNum]; //this is class of weapon


		int32 NewWeaponIndex = Weapon->WeaponConfig.SerialNum; //Unintialized index of new weapon
		if (SubContainer.Contains(Weapon))
		{
			//Pick up item and stash it in the subcontainer
			AWeapon* Spawner = GetWorld()->SpawnActor<AWeapon>(Weapon->GetClass());
			if (Spawner)
			{
				//NewWeaponIndex = SubContainer->AddUnique(Spawner); //new weapon added
				NewWeaponIndex = SubContainer.AddUnique(Spawner); //new weapon added
				//TODO Delete
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Black, TEXT("Just picked up a ")
					+ SubContainer[NewWeaponIndex]->WeaponConfig.Name);
			}
			Weapon->Destroy(); //already stored in the subcontainer
		}
		//if already have that item in subcontainer, 
		//so additional pickup merely increases ammo count if there is any
		else
		{
			int32 CurrentAmmo = SubContainer[NewWeaponIndex]->CurrentAmmo;
			int32 MaxAmmo = SubContainer[NewWeaponIndex]->WeaponConfig.MaxAmmo;

			//if the current weapon has room to load-up more ammo
			if (CurrentAmmo >= 0 && Weapon->CurrentAmmo <= (MaxAmmo - CurrentAmmo))
			{
				SubContainer[NewWeaponIndex]->CurrentAmmo += Weapon->CurrentAmmo;
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Added ") + Weapon->CurrentAmmo);
			}
			else //if Current ammo is greater than MaxAmmo,
			{
				//on pickup, character salvage to max Ammo and throws rest away
				SubContainer[NewWeaponIndex]->CurrentAmmo = MaxAmmo;
			}
		}
	}
	*/
	/*Array Implementation
	if (Weapon != NULL)
	{
		//if this inventory slot is empty
		if (WeaponInventory[Weapon->WeaponConfig.Priority] == NULL)
		{
			//Pick up item
			//can use TSubClass
			AWeapon* Spawner = GetWorld()->SpawnActor<AWeapon>(Weapon->GetClass());
			if (Spawner)
			{
				WeaponInventory[Spawner->WeaponConfig.Priority] = Spawner; //equivalent to Weapon but spawner checks if right subclass is spawned
																		   //TODO Delete
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("You just picked up a ")
					+ WeaponInventory[Spawner->WeaponConfig.Priority]->WeaponConfig.Name);
			}
			Weapon->Destroy(); //we already have ptr stored with Spawner
		}
		else //already have a weapon so other pickups will increase ammo count
		{
			int32 CurrentAmmo = WeaponInventory[Weapon->WeaponConfig.Priority]->CurrentAmmo;
			int32 MaxAmmo = WeaponInventory[Weapon->WeaponConfig.Priority]->WeaponConfig.MaxAmmo;

			//if the current weapon has room to load-up more ammo
			if (CurrentAmmo >= 0 && Weapon->CurrentAmmo <= (MaxAmmo - CurrentAmmo))
			{
				WeaponInventory[Weapon->WeaponConfig.Priority]->CurrentAmmo += Weapon->CurrentAmmo;
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Added ") + Weapon->CurrentAmmo);
			}
			else //if Current ammo is greater than MaxAmmo,
			{
				//on pickup, character salvage to max Ammo and throws rest away
				WeaponInventory[Weapon->WeaponConfig.Priority]->CurrentAmmo = MaxAmmo;
			}
		}
	}
	*/
}

void ABaseCharacter::NextWeapon()
{

	/*
	CurrentWeaponTypeIndex = CurrentWeapon->WeaponConfig.CategoryNum;
	//Next weapon is the next weapon type.

	//Check if current index is size of WeaponInventory array. Or next weapon index will exceed array size
	if (CurrentWeaponTypeIndex == WeaponInventory.Num())
	{
		//Select weapon from index 0
		//Check if next weapon exists, if not then equip fist
		for (int32 i = 0; i <= CurrentWeaponTypeIndex; i++)
		{
			AWeapon* Temp = GrabWeaponFromSubContainer(i);
			if (Temp) //found valid weapon
			{
				EquipWeapon(Temp);
				//TODO Delete
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("Next Weapon"));
				return;
			}
		}
	}
	else //for pistol and rifle, if next weapon is empty, then reverts to fist which is always in Melee container
	{
		for (int32 i = CurrentWeaponTypeIndex + 1; i < WeaponInventory.Num(); i++)
		{
			AWeapon* Temp = GrabWeaponFromSubContainer(i);
			if (Temp) //found valid weapon
			{
				EquipWeapon(Temp);
				//TODO Delete
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("Next Weapon"));
				return;
			}
		}
	}
*/

	/*
	int32 CurrentWeaponSlot = CurrentWeapon->WeaponConfig.Priority;
	//if CurrentSlot has at least one next slot
	if (CurrentWeaponSlot < WeaponInventory.Num() - 1)
	{
		//if next slot is empty or doesn't have a weapon
		if (WeaponInventory[CurrentWeaponSlot + 1] == NULL)
		{
			//Traverse through the entire inventory until find a valid weapon to equip
			for (int32 i = CurrentWeaponSlot; i < WeaponInventory.Num(); i++)
			{
				if (WeaponInventory[i] && WeaponInventory[i]->IsA(AWeapon::StaticClass()))
				{
					EquipWeapon(WeaponInventory[i]);
				}
			}
		}
		else
		{
			EquipWeapon(WeaponInventory[CurrentWeaponSlot + 1]);
		}
	}
	else
	{
		//Do nothing
		EquipWeapon(WeaponInventory[CurrentWeaponSlot]);
	}
	*/
}

void ABaseCharacter::PrevWeapon()
{

	/*
	CurrentWeaponTypeIndex = CurrentWeapon->WeaponConfig.CategoryNum;
	//Next weapon is the next weapon type.

	//Check if current index is 0. Or prev weapon index will exceed array bounds
	if (CurrentWeaponTypeIndex == 0)
	{
		//Select weapon from index 0
		//Check if next weapon exists, if not then equip fist.
		for (int32 i = WeaponInventory.Num(); i > 0; i--)
		{
			AWeapon* Temp = GrabWeaponFromSubContainer(i);
			if (Temp) //found valid weapon
			{
				EquipWeapon(Temp);
				//TODO Delete
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("Previous Weapon"));
				return;
			}
		}
	}
	else //for pistol and melee
	{
		for (int32 i = CurrentWeaponTypeIndex - 1; i > 0; i--)
		{
			AWeapon* Temp = GrabWeaponFromSubContainer(i);
			if (Temp) //found valid weapon
			{
				EquipWeapon(Temp);
				//TODO Delete
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("Previous Weapon"));
				return;
			}
		}
	}
	*/
	/*
	int32 CurrentWeaponSlot = CurrentWeapon->WeaponConfig.Priority;
	if (WeaponInventory[CurrentWeaponSlot]->WeaponConfig.Priority != 0)
	{
		//if previous slot is empty
		if (WeaponInventory[CurrentWeaponSlot - 1] == NULL)
		{
			for (int32 i = CurrentWeaponSlot - 1; i >= 0; i--)
			{
				if (WeaponInventory[i] && WeaponInventory[i]->IsA(AWeapon::StaticClass()))
				{
					EquipWeapon(WeaponInventory[i]);
				}
			}
		}
		else
		{
			EquipWeapon(WeaponInventory[CurrentWeaponSlot - 1]);
		}
	}
	else
	{
		//Do nothing
		EquipWeapon(WeaponInventory[CurrentWeaponSlot]);
	}
	*/
}

/*
AWeapon* ABaseCharacter::GrabWeaponFromSubContainer(int32 WeaponTypeNum)
{
	//input validation
	if (WeaponTypeNum < WeaponInventory.Num())
	{
		//check if inner container is empty
		if (WeaponInventory[WeaponTypeNum].Num() == 0)
		{
			//return NULL
			return NULL;
		}
		else
		{
			return WeaponInventory[WeaponTypeNum].Pop(true);
		}
	}
		return NULL;
}
*/

void ABaseCharacter::EquipWeapon(AWeapon* Weapon)
{


	/*
	if (CurrentWeapon != NULL)
	{
		int32 CurrWeaponTypeNum = CurrentWeapon->WeaponConfig.CategoryNum;
		int32 CurrWeaponSerialNum = CurrentWeapon->WeaponConfig.SerialNum;
		TArray<AWeapon*> TempContainer = WeaponInventory[CurrWeaponTypeNum]; //TODO Copying entire ARRAY!!!
		CurrentWeapon = TempContainer[CurrWeaponSerialNum];  //store current item to container/inventory
		CurrentWeapon->OnUnequip(); //unequip weapon currently in hand
		CurrentWeapon = Weapon;
		Weapon->SetOwningPawn(this); //this specific class owns it
		Weapon->OnEquip();
	}
	else
	{
		CurrentWeapon = Weapon;
		int32 CurrWeaponTypeNum = CurrentWeapon->WeaponConfig.CategoryNum;
		int32 CurrWeaponSerialNum = CurrentWeapon->WeaponConfig.SerialNum;
		TArray<AWeapon*> TempContainer = WeaponInventory[CurrWeaponTypeNum]; //TODO Copying entire ARRAY!!!
		CurrentWeapon = TempContainer[CurrWeaponSerialNum];  //store current item to container/inventory
		CurrentWeapon->SetOwningPawn(this);
		Weapon->OnEquip();
	}
	*/

	/*
	if (CurrentWeapon != NULL)
	{
		CurrentWeapon = WeaponInventory[CurrentWeapon->WeaponConfig.Priority]; //get item to Current Weapon
		CurrentWeapon->OnUnequip(); //unequip it
		CurrentWeapon = Weapon;
		Weapon->SetOwningPawn(this); //this specific class owns it
		Weapon->OnEquip();
	}
	else
	{
		CurrentWeapon = Weapon;
		CurrentWeapon = WeaponInventory[CurrentWeapon->WeaponConfig.Priority];
		CurrentWeapon->SetOwningPawn(this);
		Weapon->OnEquip();
	}
	*/
}