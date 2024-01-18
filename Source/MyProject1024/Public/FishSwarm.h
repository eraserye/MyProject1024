// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Fish.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "FishSwarm.generated.h"

UCLASS()
class MYPROJECT1024_API AFishSwarm : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFishSwarm();

	// 鱼actor
	UPROPERTY(EditAnywhere, Category = "FishSwarm")
	TSubclassOf<class AFish> FishClass;

	// 生成的鱼的数量
	UPROPERTY(EditAnywhere, Category = "FishSwarm")
	int32 FishSpawnRate;

	// 进入该collision后开始进行鱼群运动,离开后停止
	UPROPERTY(EditAnywhere, Category = "FishSwarm")
	UBoxComponent* BeginSwarmCollisionComponent;
	

	// 生成的速度（概率）
	UPROPERTY(EditAnywhere,Category="FishSwarm")
	float SpawnSpeed;

	// 鱼平均速度
	UPROPERTY(EditAnywhere,Category="FishSwarm")
	FVector StartVelocity;

	// 鱼速度扰动
	UPROPERTY(EditAnywhere,Category="FishSwarm")
	FVector VelocityRange;

	UPROPERTY(EditAnywhere,Category="FishSwarm")
	FVector BBoxMin;
	UPROPERTY(EditAnywhere,Category="FishSwarm")
	FVector BBoxMax;

	// 力曲线
	UPROPERTY(EditAnywhere, Category = "Movement")
	UCurveFloat* ForceCurve;

	// 重力（先别管阻力了、或者也用curve）
	UPROPERTY(EditAnywhere,Category="Movement")
	float Gravity;

	UPROPERTY(EditAnywhere,Category="Movement")
	FVector ForceDir;

	//每只鱼的生存时间
	UPROPERTY(EditAnywhere,Category="FishSwarm")
	float TotLifeTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		TSubclassOf<UUserWidget> HookWidgetClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<AFish*> Pool;

	TArray<AFish*> AvailiblePool;

	// 在离开范围后不再生成新的
	bool bIsSpawn;

	void ReturnFish(AFish* Fish);

	AFish* GetFish();

	void InitializePool();

	void InitializeFish(AFish* Fish);

	FVector CalcForce(float Timer);

	void UpdateEachFish(float DeltaTime);
	
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnPlayerEnterTriggerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnPlayerLeaveTriggerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	

};
