// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DekeyInteractableInterface.h"
#include "man.generated.h"

UCLASS()
class MYPROJECT1024_API Aman : public ACharacter, public IDekeyInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	Aman();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USkeletalMeshComponent* CustomMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		UAnimSequence* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		UAnimSequence* RidingAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		FVector Ridinglocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		FRotator RidingRotator;

	virtual void OnInteract(AActor* InteractingActor) override;

};
