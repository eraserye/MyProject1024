// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AICharacter.h"
#include "MyAIController.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EndMainAction.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT1024_API UEndMainAction : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UEndMainAction();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
