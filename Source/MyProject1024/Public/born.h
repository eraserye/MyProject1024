// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AICharacter.h"
#include "born.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT1024_API Aborn : public AAICharacter
{
	GENERATED_BODY()

protected:

public:
	Aborn();
	void Eat();
	void EndEat();
	void LookAround();
	void EndLookAround();

	virtual void MainAction() override;
	virtual void EndMainAction() override;

	UPROPERTY(BlueprintReadWrite, Category = "animation category")
	bool IsEating;
	UPROPERTY(BlueprintReadWrite, Category = "animation category")
	bool IsLooking;
	UPROPERTY(BlueprintReadWrite, Category = "animation category")
	bool IsWalking;
	UPROPERTY(BlueprintReadWrite, Category = "animation category")
	float TurnAngle;

	
};
