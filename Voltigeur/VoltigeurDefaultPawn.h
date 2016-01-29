// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/DefaultPawn.h"
#include "VoltigeurDefaultPawn.generated.h"

/**
 * This pawn is to navigate through the map when player is not controlling any character
 */
UCLASS()
class VOLTIGEUR_API AVoltigeurDefaultPawn : public ADefaultPawn
{
	GENERATED_BODY()
	
public:
	// Sets default values for this pawn's properties
	AVoltigeurDefaultPawn();

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
	void SetupCameraSettings();
	void ZoomIn();
	void ZoomOut();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float ZOOM_INCREMENT = 80.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float MAX_BOOM_LENGTH = 2500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float MIN_BOOM_LENGTH = 300.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float DEFAULT_ARM_LENGTH = 1500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	FRotator DEFAULT_CAMERA_ROT = FRotator(-70.f, 0.f, 0.f);
	
	
};
