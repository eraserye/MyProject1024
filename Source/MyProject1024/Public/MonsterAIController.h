// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyAIController.h"
#include "AICharacter.h"
#include "../horse_spline_view.h"
#include "../man.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT1024_API AMonsterAIController : public AMyAIController
{
	GENERATED_BODY()
	
public:
	AMonsterAIController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YourCategory")
	float AfterHitSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YourCategory")
		float AttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YourCategory")
		float AttackMoveSpeed;

protected:
	UFUNCTION(BlueprintCallable)
	void OnPerceptionUpdated(const TArray<AActor*>& DetectedActors);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		UAISenseConfig_Sight* SightConfig;

	virtual void RunningMainAction() override;

	virtual void StartMainAction() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	//之后改成有行进路径的角色或者带tag
	ACharacter* TargetPlayer;
	FVector LastLoc;

	bool BeHit;

	bool StartVanish;

};
