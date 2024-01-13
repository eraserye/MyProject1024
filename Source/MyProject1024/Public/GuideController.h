// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyAIController.h"
#include "born.h"
#include "../horse_spline_view.h"
#include "AICharacter.h"
#include "GuideController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT1024_API AGuideController : public AMyAIController
{
	GENERATED_BODY()

public:
	AGuideController();

protected:
	UFUNCTION(BlueprintCallable)
	void OnPerceptionUpdated(const TArray<AActor*>& DetectedActors) ;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		UAISenseConfig_Sight* SightConfig;

	virtual void RunningMainAction() override;

	virtual void StartMainAction() override;

	virtual void BeginPlay() override;

	//之后改成有行进路径的角色或者带tag
	Ahorse_spline_view* TargetPlayer;

	FVector TargetLoc;
	
};
