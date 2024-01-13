// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	void Chase();
	void EndChase();

	void Attack();
	void EndAttack();

	void Pray();
	void EndPray();

	//用main action函数做借口并不好
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

};
