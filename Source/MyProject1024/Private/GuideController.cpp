// Fill out your copyright notice in the Description page of Project Settings.


#include "GuideController.h"
#include "../horse_spline_view.h"

AGuideController::AGuideController() :AMyAIController()
{

}


void AGuideController::OnPerceptionUpdated(const TArray<AActor*>& DetectedActors)
{
    for (AActor* Actor : DetectedActors)
    {
        //if (Actor->IsA(Ahorse_spline_view::StaticClass()))
        if (Ahorse_spline_view* Player = Cast<Ahorse_spline_view>(Actor))
        {
            UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
            if (USplineComponent* PlayerPath = Player->CurPath) {
                float PathKey = PlayerPath->FindInputKeyClosestToWorldLocation(Player->GetActorLocation());
                float MaxKey = PlayerPath->GetNumberOfSplinePoints() - 1;
                float KeyOffset = PathKey + 5.0f > MaxKey ? MaxKey : PathKey + 5.0f;
                FVector TargetLoc = PlayerPath->GetLocationAtSplineInputKey(KeyOffset, ESplineCoordinateSpace::World);
                if (BlackboardComp && BlackboardComp->GetKeyID("TargetLoc") != FBlackboard::InvalidKey)
                {
                    Blackboard->SetValueAsVector("TargetLoc", TargetLoc);
                }
                if (BlackboardComp && BlackboardComp->GetKeyID("HasLoc") != FBlackboard::InvalidKey)
                {
                    Blackboard->SetValueAsBool("HasLoc", true);
                }
            }

            break;
        }
    }
}