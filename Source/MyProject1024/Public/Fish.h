// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Fish.generated.h"

UCLASS()
class MYPROJECT1024_API AFish : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFish();

	bool bIsDead;

	float LifeTime;

	// AFish(FVector StartSpeed, UStaticMeshComponent* FishMesh);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* FishMesh;
    // 鱼的运动速度
    
    FVector Velocity;

	FVector Location;

    // 碰撞盒
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionComponent;
	
	void MoveFish(float DeltaTime);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void AddForce(FVector Force,float DeltaTime);

	void Restart(FVector Velocity,FVector Location);

	
	
};
