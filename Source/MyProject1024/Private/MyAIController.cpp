// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "born.h"
#include "../horse_spline_view.h"
#include "AICharacter.h"

AMyAIController::AMyAIController()
{
    BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));

    RandomActionThresholdValue = 0.01;

    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

    RandomActionTime = 4.0f;

    MainActionPriority = 0;
    RandomActionPriority = 0;
}

//在ai controller子类中tick可以是固定的
void AMyAIController::Tick(float DeltaTime) {
    if (BlackboardComp && BlackboardComp->GetKeyID("RunMainAction") != FBlackboard::InvalidKey)
    {
        RunMainAction = BlackboardComp->GetValueAsBool("RunMainAction");
    }

    //main action is always true
    bool bCanSeize = true;
    if (MainActionPriority > RandomActionPriority) {
        bCanSeize = !RunMainAction;
    }

    //random action
    float RandomValue = FMath::FRandRange(0.0f, 5.0f);
    //要避免多次设置 因为回重复进入分支,导致状态不正确结束 因此加入wait node
    if (RandomValue < RandomActionThresholdValue && !RunRandomAction && bCanSeize)
    {
        RunRandomAction = true;
        RunMainAction = false;
        ActionTime = RandomActionTime;
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

    //随机行为的优先级高 确保两者互斥 或同时为false 因为RunningMainAction是并行的
    RunMainAction = false ^ RunRandomAction ? false : RunMainAction;

    if (RunMainAction) {
        RunningMainAction();
    }
    if (!RunMainAction && !RunRandomAction) {
        StartMainAction();
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
}

//根据目标位置进行移动和旋转
void AMyAIController::RunningMainAction() {
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

//main action have no end
//初始化主要行为的参数并开始主要行为
void AMyAIController::StartMainAction() {
    RunMainAction = true;
    float Radius = 400.0f;
    float RandomAngle = FMath::FRandRange(-PI / 2.0f, PI / 2.0f);
    FVector CharacterLoc = GetPawn()->GetActorLocation();
    FRotator ActorRotation = GetPawn()->GetActorRotation();
    //FRotator ActorYaw(0, ActorRotation.Yaw, 0);
    FVector ActorForward = FRotationMatrix(ActorRotation).GetUnitAxis(EAxis::X);
    FVector ActorRight = FRotationMatrix(ActorRotation).GetUnitAxis(EAxis::Y);
    TargetLocation = Radius * ActorForward * cos(RandomAngle) + Radius * ActorRight * sin(RandomAngle) + CharacterLoc;
    if (BlackboardComp && BlackboardComp->GetKeyID("TargetLoc") != FBlackboard::InvalidKey)
    {
        Blackboard->SetValueAsVector("TargetLoc", TargetLocation);
    }
    if (BlackboardComp && BlackboardComp->GetKeyID("RunMainAction") != FBlackboard::InvalidKey)
    {
        Blackboard->SetValueAsBool("RunMainAction", true);
    }
}


void AMyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
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

    float DeltaTime = 1 / 60;
    if (GetWorld()) {
        
        //无法成功设置计时器,先用tick
        //GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMyAIController::PerFrameUpdate, DeltaTime, true);
    }
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
            if (BlackboardComp && BlackboardComp->GetKeyID("RunRandomAction") != FBlackboard::InvalidKey)
            {
                Blackboard->SetValueAsBool("RunRandomAction", false);
            }
        }
    }
}