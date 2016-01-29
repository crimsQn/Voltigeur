// Fill out your copyright notice in the Description page of Project Settings.

#include "Voltigeur.h"
#include "VoltigeurDefaultPawn.h"

AVoltigeurDefaultPawn::AVoltigeurDefaultPawn()
{
	AVoltigeurDefaultPawn::SetupCameraSettings();
	//TODO need to make sure collision is off to Characters

	//CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	//TODO no collision
	//CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//RootComponent = CollisionComp;

	//MovementComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComp"));
	//MovementComp->UpdatedComponent = CollisionComp; //move with collision comp
	//MovementComp->SetPlaneConstraintEnabled(true); //constraint to plane
	//MovementComp->MaxSpeed = 1800.f;


}

// Called when the game starts or when spawned
void AVoltigeurDefaultPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called to bind functionality to input
void AVoltigeurDefaultPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &AVoltigeurDefaultPawn::ZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &AVoltigeurDefaultPawn::ZoomOut);
}

void AVoltigeurDefaultPawn::SetupCameraSettings()
{
	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = DEFAULT_ARM_LENGTH;
	CameraBoom->RelativeRotation = DEFAULT_CAMERA_ROT;
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

}

void AVoltigeurDefaultPawn::ZoomOut()
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

void AVoltigeurDefaultPawn::ZoomIn()
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