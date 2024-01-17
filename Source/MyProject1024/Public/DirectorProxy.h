// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "DirectorProxy.generated.h"

UCLASS()
class MYPROJECT1024_API ADirectorProxy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADirectorProxy();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	UBoxComponent* BoxCollider;

	//camera attributes
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "camera")
	float FieldOfView;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "camera")
	float SpringArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "camera")
	float AspectRatio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "camera")
	FVector SocketOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "camera")
    bool ConfigCamera;

	//path
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "camera")
	USplineComponent* CameraPath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "camera")
	USplineComponent* Path;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "camera")
	FVector Offset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "camera")
	bool UsePathCamera;

	//for adjustment
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "camera")
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "camera", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float PathT;


	//static
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "camera")
	class UCameraComponent* CameraPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "camera")
	bool UseStaticCamera;

	//sequence 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "camera")
	USplineComponent* SequencePath;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "camera")
	USceneComponent* SequencePoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "camera", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float SequenceTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "camera")
	USceneComponent* SequencyTarget;

};
