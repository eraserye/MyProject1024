// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "DekeyInteractableInterface.h"
#include "Components/SplineComponent.h"
#include "Public/DirectorProxy.h"
#include "horse_spline_view.generated.h"



UCLASS()
class MYPROJECT1024_API Ahorse_spline_view : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UNiagaraComponent* ParticleSystemComponent;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* RaceAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* HookShotAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* EatAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* WindAction;
	
public:
	// Sets default values for this character's properties
	Ahorse_spline_view();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	void EndMove(const FInputActionValue& Value);

	/** Called for looking input */
	void Race(const FInputActionValue& Value);

	void HookShot(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Eat(const FInputActionValue& Value);
	void EndEat(const FInputActionValue& Value);

	void Wind(const FInputActionValue& Value);

	void ApplyInitialFallVelocity();


	void Interact(const FInputActionValue& Value);

	IDekeyInteractableInterface* DekeyInteract(const FString& Tag, const FString& Condition);

	IDekeyInteractableInterface* PerformRaycastDetection(const FString& Tag);

	IDekeyInteractableInterface* PerformRadiusDetection(const FString& Tag);

	IDekeyInteractableInterface* PerformOverlapDetection(const FString& Tag);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, 
						class AActor* OtherActor, 
						class UPrimitiveComponent* OtherComp, 
						int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YourCategory")
		float delta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YourCategory")
		float RotateSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YourCategory")
		float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YourCategory")
		float Tolerance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YourCategory")
		float RadiusDetection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YourCategory")
		float HookSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YourCategory")
		float FallSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YourCategory")
		bool bDebug;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YourCategory")
		FString InteractMode;

	UPROPERTY(BlueprintReadWrite, Category = "Your Category")
		float TurnAngle;

	UPROPERTY(BlueprintReadWrite, Category = "Your Category")
		bool IsMoving;

	UPROPERTY(BlueprintReadWrite, Category = "Your Category")
		bool IsEating;

	UPROPERTY(BlueprintReadWrite, Category = "Your Category")
		int SpeedLevel;

	UPROPERTY(BlueprintReadWrite, Category = "Your Category")
		int RaceStep;

	UPROPERTY(BlueprintReadWrite, Category = "Your Category")
		int RaceMAX;

	UPROPERTY(BlueprintReadWrite, Category = "Your Category")
		float RaceSpeed;

	USplineComponent* CurPath;

	ADirectorProxy* director;

	float SequenceTimer;

	//float AnimPlayRate;
	bool BeRiding;
	AActor* Rider;


	UMaterialParameterCollectionInstance* CollectionInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Collection")
		UMaterialParameterCollection* CollectionReference;


private:
	bool bIsHookShotActive;
	FVector HookShotTarget;
	AActor* BestHookPoint_tem;
	bool showUI;
	//ADirectorProxy* CurDirector;
	float RaceTimer;
	float InteractTimer;


};
