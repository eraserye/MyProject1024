// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AICharacter.h"
#include "MyAIController.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RandomAction.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT1024_API URandomAction : public UBTTaskNode
{
	GENERATED_BODY()

public:
	URandomAction();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
