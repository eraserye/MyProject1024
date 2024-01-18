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
#include "Public/DirectorProxy.h"
#include "hookPoint.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* HandCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* BodyCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"),Category = "Weapon")
		USkeletalMeshComponent* LanternMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"),Category = "Weapon")
		UCapsuleComponent* LanternCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"),Category = "Weapon")
		UCapsuleComponent* LightCollision;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* HitAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* HookShotAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LightAction;

public:
	// Sets default values for this character's properties
	Aman();

protected:
	UFUNCTION(BlueprintCallable)
		void OnHandHit(UPrimitiveComponent* HitComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION(BlueprintCallable)
		void OnLightHit(UPrimitiveComponent* HitComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnFogReceived(UPrimitiveComponent* HitComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void EndMove(const FInputActionValue& Value);

	void CrouchSwitch(const FInputActionValue& Value);

	void Climb(const FInputActionValue& Value); 

	void Interact(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Hit(const FInputActionValue& Value);
	void EndHit(const FInputActionValue& Value);

	void HookShot(const FInputActionValue& Value);
	
	void Light(const FInputActionValue& Value);

	void EndLight(const FInputActionValue& Value);

	void ApplyInitialFallVelocity();

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
		bool IsHitting;

	UPROPERTY(BlueprintReadWrite, Category = "Your Category")
		bool IsCrouching;

	UPROPERTY(BlueprintReadWrite, Category = "Your Category")
		bool IsClimbing;

	UPROPERTY(BlueprintReadWrite, Category = "Your Category")
		bool IsLighting;

	UPROPERTY(BlueprintReadWrite, Category = "Your Category")
		bool IsRidding;

	//interact
	UPROPERTY(BlueprintReadWrite, Category = "Your Category")
		bool IsSpeaking;

	UPROPERTY(BlueprintReadWrite, Category = "Your Category")
		bool IsLeaning;

	//hook 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YourCategory")
		float HookSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YourCategory")
		float FallSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YourCategory")
		float MaxSpeakTime;

	void Riding(AActor* CurInteractingActor);

	void EndRiding();


private:
	bool bIsHookShotActive;
	FVector HookShotTarget;
	//hook point for last frame
	AActor* BestHookPoint_tem;
	bool showUI;
	float SpeakTimer;
	AActor* CurInteractingActor;


};
