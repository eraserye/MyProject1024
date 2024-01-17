// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "../horse_spline_view.h"
#include "ShadowMonsterAI.h"

AMonsterAIController::AMonsterAIController() :AMyAIController()
{
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    if (SightConfig)
    {
        SightConfig->SightRadius = 500.0f;
        SightConfig->LoseSightRadius = SightConfig->SightRadius + 50.0f;
        SightConfig->PeripheralVisionAngleDegrees = 90.0f;
        SightConfig->SetMaxAge(5.0f);

        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

        AIPerceptionComponent->ConfigureSense(*SightConfig);
    }

    MainActionPriority = 1;
    RandomActionPriority = 0;
}

void AMonsterAIController::RunningMainAction() {
    if (TargetPlayer) {
        FVector TargetLoc = TargetPlayer->GetActorLocation();
        //LastLoc = TargetLoc;
        FVector TargetDir = (TargetLoc - GetPawn()->GetActorLocation()).GetSafeNormal();
        TargetLoc = TargetLoc + 100 * TargetDir;
        float delta = 0.01;
        float RotateSpeed = 2.0f;
        GetPawn()->SetActorRotation(FMath::RInterpTo(GetPawn()->GetActorRotation(), TargetDir.Rotation(), delta, RotateSpeed));
        float TurnAngle = TargetDir.Rotation().Yaw - GetPawn()->GetActorRotation().Yaw;
        if (TurnAngle > 180) {
            TurnAngle = TurnAngle - 360;
        }
        else if (TurnAngle < -180) {
            TurnAngle += 360;
        }
        /*if (BlackboardComp && BlackboardComp->GetKeyID("TargetLoc") != FBlackboard::InvalidKey)
        {
            Blackboard->SetValueAsVector("TargetLoc", TargetLoc);
        }*/
        if (AShadowMonsterAI* Monster = Cast<AShadowMonsterAI>(GetPawn())) {
            Monster->TurnAngle = TurnAngle;
            if ((TargetLoc - GetPawn()->GetActorLocation()).Size() < AttackRadius) {
                Monster->GetCharacterMovement()->MaxWalkSpeed = AttackMoveSpeed;
                Monster->Attack();
            }
        }
        //attack
        
    }
}

void AMonsterAIController::StartMainAction() {
    if (TargetPlayer) {
        RunMainAction = true;
        /*FVector TargetLoc = TargetPlayer->GetActorLocation();
        FVector TargetDir = (TargetLoc - GetPawn()->GetActorLocation()).GetSafeNormal();
        TargetLoc = TargetLoc + 100 * TargetDir;
        if (BlackboardComp && BlackboardComp->GetKeyID("TargetLoc") != FBlackboard::InvalidKey)
        {
            Blackboard->SetValueAsVector("TargetLoc", TargetLoc);
        }*/
        if (BlackboardComp && BlackboardComp->GetKeyID("TargetActor") != FBlackboard::InvalidKey)
        {
            Blackboard->SetValueAsObject("TargetActor", TargetPlayer);
        }
        if (BlackboardComp && BlackboardComp->GetKeyID("HasLoc") != FBlackboard::InvalidKey)
        {
            Blackboard->SetValueAsBool("HasLoc", true);
        }
        if (BlackboardComp && BlackboardComp->GetKeyID("RunMainAction") != FBlackboard::InvalidKey)
        {
            Blackboard->SetValueAsBool("RunMainAction", true);
        }
        if (AShadowMonsterAI* Monster = Cast<AShadowMonsterAI>(GetPawn())) {
            Monster->Chase();
        }
    }
    else {
        //set start main actionTimer 需要吗?
    }
}

void AMonsterAIController::BeginPlay()
{
    Super::BeginPlay();

    if (AIPerceptionComponent)
    {
        AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AMonsterAIController::OnPerceptionUpdated);
    }
}


void AMonsterAIController::Tick(float DeltaTime)
{
    if (!StartVanish) {
        Super::Tick(DeltaTime);
    }

    if (Cast<AShadowMonsterAI>(GetPawn())->IsStartHit)
    {
        //中断main action并接替ai的运动控制
        BehaviorTreeComp->StopTree(); 
        FVector Direction = GetPawn()->GetActorForwardVector();
        GetPawn()->AddActorWorldOffset(Direction * AfterHitSpeed * DeltaTime);
        StartVanish = true;
    }
}

void AMonsterAIController::OnPerceptionUpdated(const TArray<AActor*>& DetectedActors)
{
    for (AActor* Actor : DetectedActors)
    {
        //if (Actor->IsA(Ahorse_spline_view::StaticClass()))
        if (Aman* Player = Cast<Aman>(Actor))
        {
            TargetPlayer = Player;
            break;
        }
    }
    //离开视线时清除对象 否则一直保留
    //TargetPlayer = nullptr;
}