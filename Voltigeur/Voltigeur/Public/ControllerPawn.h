// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/DefaultPawn.h"
#include "ControllerPawn.generated.h"


/*Generic Controller Pawn which has a top-down camera. Class is use to span through level*/
UCLASS()
class VOLTIGEUR_API AControllerPawn : public ADefaultPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AControllerPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

private:
	/*****CAMERA SETTINGS******/
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/*Camera settings which start as Top Down. It attaches to Pawn if valid*/
	void SetCameraSettings();
	void ZoomIn();
	void ZoomOut();
	const float ZOOM_INCREMENT = 50.f;
	const float MAX_BOOM_LENGTH = 1500.f;
	const float MIN_BOOM_LENGTH = 100.f;
	const float DEFAULT_ARM_LENGTH = 1200.f;
	const FRotator DEFAULT_CAMERA_ROT = FRotator(-50.f, 0.f, 0.f);
	
};
