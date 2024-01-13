// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "DekeyInteractableInterface.h"
#include "man.generated.h"

UCLASS()
class MYPROJECT1024_API Aman : public ACharacter, public IDekeyInteractableInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ClimbAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

public:
	// Sets default values for this character's properties
	Aman();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);

	void CrouchSwitch(const FInputActionValue& Value);

	void Climb(const FInputActionValue& Value); 

	void Interact(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		UAnimSequence* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		UAnimSequence* RidingAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		FVector Ridinglocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		FRotator RidingRotator;

	virtual void OnInteract(AActor* InteractingActor) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YourCategory")
		float delta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YourCategory")
		float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YourCategory")
		float RotateSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YourCategory")
		float CrouchSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YourCategory")
		float StandSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Your Category")
		float TurnAngle;

	UPROPERTY(BlueprintReadWrite, Category = "Your Category")
		bool IsMoving;

	UPROPERTY(BlueprintReadWrite, Category = "Your Category")
		bool IsCrouching;

	UPROPERTY(BlueprintReadWrite, Category = "Your Category")
		bool IsClimbing;


};
