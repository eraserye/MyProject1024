// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AICharacter.h"
#include "MyAIController.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MainAction.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT1024_API UMainAction : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UMainAction();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
