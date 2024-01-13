// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyAIController.h"
#include "AICharacter.h"
#include "../horse_spline_view.h"
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

	//֮��ĳ����н�·���Ľ�ɫ���ߴ�tag
	Ahorse_spline_view* TargetPlayer;
	FVector LastLoc;
};