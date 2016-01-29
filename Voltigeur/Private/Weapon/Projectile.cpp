// Fill out your copyright notice in the Description page of Project Settings.

#include "Voltigeur.h"
#include "Projectile.h"
#include "Engine.h"
#include "BaseCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"



// Sets default values
AProjectile::AProjectile()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(5.0f);
	//	ColisionComp->BodyInstance.SetCollisionProfileName("Projectile"); 	
	//	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnCollision);
	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp; //moving along with collision component
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = false; //rotation with velocity
	ProjectileMovement->bShouldBounce = false; //.bounce

	InitialLifeSpan = 5.f;
}

//void AProjectile::OnHit(AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
void AProjectile::OnCollision(AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	// if player char exist && player is not this && this hits everything on the scene
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		ABaseCharacter *Enemy = Cast<ABaseCharacter>(OtherActor);
		if (Enemy && Enemy->GetFriendlyState() == EFriendlyState::EHostile) //if hit is enemy character
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "ENEMY Exploded!");
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.f, GetActorLocation()); //where ever it is located, it is gonna bounce back
			Enemy->Destroy(); //he dead	
		}
		Destroy(); //destroy after hit
	}
}

