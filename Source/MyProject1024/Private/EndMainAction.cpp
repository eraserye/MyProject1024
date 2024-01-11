// Fill out your copyright notice in the Description page of Project Settings.


#include "EndMainAction.h"

UEndMainAction::UEndMainAction()
{
    // 构造函数内容
}


EBTNodeResult::Type UEndMainAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (AMyAIController* AIController = Cast<AMyAIController>(OwnerComp.GetAIOwner()))
    {
        if (AAICharacter* ControlledCharacter = Cast<AAICharacter>(AIController->GetPawn()))
        {
            ControlledCharacter->EndMainAction();

            //for convience
            UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
            if (BlackboardComp && BlackboardComp->GetKeyID("RunMainAction") != FBlackboard::InvalidKey)
            {
                BlackboardComp->SetValueAsBool("RunMainAction", false);
            }

            return EBTNodeResult::Succeeded;
        }
    }
    return EBTNodeResult::Failed;
}