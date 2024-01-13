// Fill out your copyright notice in the Description page of Project Settings.


#include "GuideController.h"

AGuideController::AGuideController() :AMyAIController()
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
}

//move towards target location
//set rotation
//�뿪����ʱ��Ҫ����ǰ�� ������loc,������loc����һ��������Ϊ
void AGuideController::RunningMainAction() {
    FVector TargetDir = TargetLoc - Cast<ACharacter>(GetPawn())->GetActorLocation();
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
    //if (Aborn* born = Cast<Aborn>(GetPawn())) {
    //    born->TurnAngle = TurnAngle;
    //}
}

//ע��guide��Ŀ�� ������Ϊ
//��������һ���ȴ�/תͷ��ǰ����Ϊ
void AGuideController::StartMainAction() {
    if (TargetPlayer) {
        RunMainAction = true;
        USplineComponent* PlayerPath = TargetPlayer->CurPath;
        float PathKey = PlayerPath->FindInputKeyClosestToWorldLocation(TargetPlayer->GetActorLocation());
        float MaxKey = PlayerPath->GetNumberOfSplinePoints() - 1;
        float KeyOffset = PathKey + 5.0f > MaxKey ? MaxKey : PathKey + 5.0f;
        TargetLoc = PlayerPath->GetLocationAtSplineInputKey(KeyOffset, ESplineCoordinateSpace::World);
        if (BlackboardComp && BlackboardComp->GetKeyID("TargetLoc") != FBlackboard::InvalidKey)
        {
            Blackboard->SetValueAsVector("TargetLoc", TargetLoc);
        }
        if (BlackboardComp && BlackboardComp->GetKeyID("HasLoc") != FBlackboard::InvalidKey)
        {
            Blackboard->SetValueAsBool("HasLoc", true);
        }
        if (BlackboardComp && BlackboardComp->GetKeyID("RunMainAction") != FBlackboard::InvalidKey)
        {
            Blackboard->SetValueAsBool("RunMainAction", true);
        }
    }
    else {
        //set start main actionTimer ��Ҫ��? ����Ҫ ��wait
    }
}

void AGuideController::BeginPlay()
{
    Super::BeginPlay();

    if (AIPerceptionComponent)
    {
        AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AGuideController::OnPerceptionUpdated);
    }
}

void AGuideController::OnPerceptionUpdated(const TArray<AActor*>& DetectedActors)
{
    for (AActor* Actor : DetectedActors)
    {
        //if (Actor->IsA(Ahorse_spline_view::StaticClass()))
        if (Ahorse_spline_view* Player = Cast<Ahorse_spline_view>(Actor))
        {
            if (USplineComponent* PlayerPath = Player->CurPath) {
                TargetPlayer = Player;
            }
            break;
        }
    }
    //�뿪����ʱ������� ����һֱ����
    TargetPlayer = nullptr;
}