// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "TimerManager.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT1024_API AMyAIController : public AAIController
{
	GENERATED_BODY()

public:
    AMyAIController();

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBehaviorTree* BehaviorTree;

    virtual void Tick(float DeltaTime) override;


protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UAIPerceptionComponent* AIPerceptionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UAISenseConfig_Sight* SightConfig;

    UBehaviorTreeComponent* BehaviorTreeComp;

    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;

    UFUNCTION(BlueprintCallable)
    virtual void OnPerceptionUpdated(const TArray<AActor*>& DetectedActors);

    virtual void PerFrameUpdate();

    float RandomActionThresholdValue;

    FTimerHandle TimerHandle;

    float ActionTime;

    bool RunRandomAction;
    bool RunMainAction;

    FVector TargetLocation;

	
};
