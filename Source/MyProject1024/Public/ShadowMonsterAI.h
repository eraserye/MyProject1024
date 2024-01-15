// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AICharacter.h"
#include "../man.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "ShadowMonsterAI.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT1024_API AShadowMonsterAI : public AAICharacter
{
	GENERATED_BODY()
	
public:
	AShadowMonsterAI();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Chase();
	void EndChase();

	void Attack();
	void EndAttack();

	void Pray();
	void EndPray();

	UFUNCTION(BlueprintCallable)
		void OnHitReceived(UPrimitiveComponent* HitComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

	//用main action函数做接口并不好
	//virtual void MainAction() override;
	//virtual void EndMainAction() override;

	UPROPERTY(BlueprintReadWrite, Category = "animation category")
		bool IsChasing;
	UPROPERTY(BlueprintReadWrite, Category = "animation category")
		bool IsAttacking;
	UPROPERTY(BlueprintReadWrite, Category = "animation category")
		bool IsPraying;
	UPROPERTY(BlueprintReadWrite, Category = "animation category")
		float TurnAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* BodyCollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Collection")
		UMaterialParameterCollection* CollectionReference;

protected:
	FVector VanishStartPoint;
	FVector VanishCurPoint;

	UMaterialParameterCollectionInstance* CollectionInstance;

	void BeHit(AActor* OtherActor, UPrimitiveComponent* OtherComp);
	bool IsStartHit;

	float VanishTimer;
	const float MAXVanishTimer = 5.0f;

	UMaterialInstanceDynamic* MatInst;
};
