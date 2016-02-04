// Fill out your copyright notice in the Description page of Project Settings.

#include "Voltigeur.h"
#include "Weapon.h"
#include "BaseCharacter.h"


// Sets default values
AWeapon::AWeapon()
{

}

void AWeapon::Attack()
{
	PlayWeaponSound(AttackSound);
	//implemented in subclasses
}

void AWeapon::SetOwningPawn(ABaseCharacter* NewOwner)
{
	if (MyPawn != NewOwner)
	{
		Instigator = NewOwner;
		MyPawn = NewOwner;
	}
}

void AWeapon::OnEquip()
{
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachToPlayer();
}

void AWeapon::OnUnequip()
{
	DetachFromPlayer();
}

void AWeapon::AttachToPlayer()
{
	if (MyPawn)
	{
		DetachFromPlayer();

		USkeletalMeshComponent *Character = MyPawn->GetMesh();
		WeaponMesh->SetHiddenInGame(false);
		WeaponMesh->AttachTo(Character, "Weapon_Socket");
	}
}

void AWeapon::DetachFromPlayer()
{
	WeaponMesh->DetachFromParent();
	WeaponMesh->SetHiddenInGame(true);
}

UAudioComponent* AWeapon::PlayWeaponSound(USoundCue *Sound)
{
	UAudioComponent* AC = NULL;
	if (Sound && MyPawn)
	{
		AC = UGameplayStatics::SpawnSoundAttached(Sound, MyPawn->GetRootComponent());
	}
	return AC;
}