// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAction.h"

UMainAction::UMainAction()
{
    // 构造函数内容
}


EBTNodeResult::Type UMainAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (AMyAIController* AIController = Cast<AMyAIController>(OwnerComp.GetAIOwner()))
    {
        if (AAICharacter* ControlledCharacter = Cast<AAICharacter>(AIController->GetPawn()))
        {
            ControlledCharacter->MainAction();

            //for convience
            UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
            if (BlackboardComp && BlackboardComp->GetKeyID("TargetLoc") != FBlackboard::InvalidKey)
            {
                FVector TargetLoc = BlackboardComp->GetValueAsVector("TargetLoc");
                //AIController->MoveToLocation(TargetLoc);
            }

            return EBTNodeResult::Succeeded;
        }
    }
    return EBTNodeResult::Failed;
}