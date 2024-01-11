// Fill out your copyright notice in the Description page of Project Settings.


#include "EndAction.h"

EBTNodeResult::Type UEndAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (AMyAIController* AIController = Cast<AMyAIController>(OwnerComp.GetAIOwner()))
    {
        if (AAICharacter* ControlledCharacter = Cast<AAICharacter>(AIController->GetPawn()))
        {
            ControlledCharacter->EndAction();
            return EBTNodeResult::Succeeded;
        }
    }
    return EBTNodeResult::Failed;
}