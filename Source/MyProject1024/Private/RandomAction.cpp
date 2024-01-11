// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomAction.h"


URandomAction::URandomAction()
{
    // 构造函数内容
}

EBTNodeResult::Type URandomAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (AMyAIController* AIController = Cast<AMyAIController>(OwnerComp.GetAIOwner()))
    {
        if (AAICharacter* ControlledCharacter = Cast<AAICharacter>(AIController->GetPawn()))
        {
            ControlledCharacter->PerformRandomAction();
            return EBTNodeResult::Succeeded;
        }
    }
    return EBTNodeResult::Failed;
}