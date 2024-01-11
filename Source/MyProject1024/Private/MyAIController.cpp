// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "born.h"
#include "../horse_spline_view.h"
#include "AICharacter.h"

AMyAIController::AMyAIController()
{
    BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));

    RandomActionThresholdValue = 0.01;

    
    UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

    // 创建视觉感知配置
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

void AMyAIController::Tick(float DeltaTime) {
    UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
    //main action
    //rotate
    if (BlackboardComp && BlackboardComp->GetKeyID("RunMainAction") != FBlackboard::InvalidKey)
    {
        RunMainAction = BlackboardComp->GetValueAsBool("RunMainAction");
    }
    if (RunMainAction) {
        FVector TargetDir = TargetLocation - Cast<ACharacter>(GetPawn())->GetActorLocation();
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
        if (Aborn* born = Cast<Aborn>(GetPawn())) {
            born->TurnAngle = TurnAngle;
        }
    }
    if (!RunMainAction && !RunRandomAction) {
        RunMainAction = true;
        float Radius = 400.0f;
        float RandomAngle = FMath::FRandRange(-PI / 2.0f, PI / 2.0f);
        FVector CharacterLoc = GetPawn()->GetActorLocation();
        FRotator ActorRotation = GetPawn()->GetActorRotation();
        //FRotator ActorYaw(0, ActorRotation.Yaw, 0);
        FVector ActorForward = FRotationMatrix(ActorRotation).GetUnitAxis(EAxis::X);
        FVector ActorRight = FRotationMatrix(ActorRotation).GetUnitAxis(EAxis::Y);
        TargetLocation = Radius*ActorForward*cos(RandomAngle)+ Radius * ActorRight * sin(RandomAngle) + CharacterLoc;
        if (BlackboardComp && BlackboardComp->GetKeyID("TargetLoc") != FBlackboard::InvalidKey)
        {
            Blackboard->SetValueAsVector("TargetLoc", TargetLocation);
        }
        if (BlackboardComp && BlackboardComp->GetKeyID("RunMainAction") != FBlackboard::InvalidKey)
        {
            Blackboard->SetValueAsBool("RunMainAction", true);
        }
    }

    //random action
    float RandomValue = FMath::FRandRange(0.0f, 5.0f);
    //要避免多次设置 因为回重复进入分支,导致状态不正确结束 因此加入wait node
    if (RandomValue < RandomActionThresholdValue && !RunRandomAction)
    {
        RunRandomAction = true;
        RunMainAction = false;
        ActionTime = 4.0f;
        if (BlackboardComp && BlackboardComp->GetKeyID("RunRandomAction") != FBlackboard::InvalidKey)
        {
            Blackboard->SetValueAsBool("RunRandomAction", true);
        }
        //因为每帧从黑板获取值,记得设置
        if (BlackboardComp && BlackboardComp->GetKeyID("RunMainAction") != FBlackboard::InvalidKey)
        {
            Blackboard->SetValueAsBool("RunMainAction", false);
        }
    }

    //random action timer
    if (RunRandomAction) {
        ActionTime -= DeltaTime;
        if (ActionTime < 0) {
            RunRandomAction = false;
            if (BlackboardComp && BlackboardComp->GetKeyID("RunRandomAction") != FBlackboard::InvalidKey)
            {
                Blackboard->SetValueAsBool("RunRandomAction", false);
            }
        }
    }

    //main action is always executed,unless occupied by random action
}

void AMyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
    if (BlackboardComp && BehaviorTree && BehaviorTree->BlackboardAsset)
    {
        BlackboardComp->InitializeBlackboard(*(BehaviorTree->BlackboardAsset));
        BehaviorTreeComp->StartTree(*BehaviorTree);
    }

    if (BlackboardComp && BlackboardComp->GetKeyID("TargetLoc") != FBlackboard::InvalidKey)
    {
        Blackboard->SetValueAsVector("TargetLoc", FVector(0.0f,0.0f,0.0f));
    }
    if (BlackboardComp && BlackboardComp->GetKeyID("HasLoc") != FBlackboard::InvalidKey)
    {
        Blackboard->SetValueAsBool("HasLoc", false);
    }
}

void AMyAIController::BeginPlay()
{
    Super::BeginPlay();

    if (AIPerceptionComponent)
    {
        AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AMyAIController::OnPerceptionUpdated);
    }

    float DeltaTime = 1 / 60;
    if (GetWorld()) {
        
        //无法成功设置计时器,先用tick
        //GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMyAIController::PerFrameUpdate, DeltaTime, true);
    }
}

void AMyAIController::OnPerceptionUpdated(const TArray<AActor*>& DetectedActors)
{
    
}

void AMyAIController::PerFrameUpdate()
{
    float RandomValue = FMath::FRandRange(0.0f, 1.0f);
    if (GEngine)
    {
        int32 MyKey = 5;
        float TimeToDisplay = 3.0f;
        FColor TextColor = FColor::Red;
        FString VectorString = FString::Printf(TEXT("wait random actiom!"));

        FString Message = VectorString;

        GEngine->AddOnScreenDebugMessage(MyKey, TimeToDisplay, TextColor, Message);
    }
    if (RandomValue > RandomActionThresholdValue) 
    {
        if (GEngine)
        {
            int32 MyKey = 6;
            float TimeToDisplay = 3.0f;
            FColor TextColor = FColor::Red;
            FString VectorString = FString::Printf(TEXT("start random action!"));

            FString Message = VectorString;

            GEngine->AddOnScreenDebugMessage(MyKey, TimeToDisplay, TextColor, Message);
        }
        RunRandomAction = true;
        ActionTime = 5.0f;
        UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
        if (BlackboardComp && BlackboardComp->GetKeyID("RunRandomAction") != FBlackboard::InvalidKey)
        {
            Blackboard->SetValueAsBool("RunRandomAction", true);
        }
    }

    //random action timer
    if (RunRandomAction) {
        float DeltaTime = 1.0f / 60.0f;
        ActionTime -= DeltaTime;
        if (ActionTime < 0) {
            RunRandomAction = false;
            UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
            if (BlackboardComp && BlackboardComp->GetKeyID("RunRandomAction") != FBlackboard::InvalidKey)
            {
                Blackboard->SetValueAsBool("RunRandomAction", false);
            }
        }
    }
}