// Fill out your copyright notice in the Description page of Project Settings.

#include "man.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Public/DirectorProxy.h"
#include "Public/MyPlayerController.h"
#include "Public/MyPlayerCameraManager.h"
#include "Camera/CameraComponent.h"

// Sets default values
Aman::Aman()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
    GetCapsuleComponent()->SetGenerateOverlapEvents(true);

    BodyCollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyCollision"));
    BodyCollisionComponent->SetupAttachment(GetMesh(), "BodySocket");
    BodyCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BodyCollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
    BodyCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    BodyCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Overlap);
    BodyCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &Aman::OnFogReceived);

    //hand collision
    //打击有父类角色类判别,暂时绑在角色上好一点
    HandCollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HandCollision"));
    HandCollisionComponent->SetupAttachment(GetMesh(), "HandSocket");
    HandCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly); 
    HandCollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); 
    HandCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); 
    HandCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Overlap); 

    HandCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &Aman::OnHandHit);

    //weapon
    LanternMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Lantern"));
    LanternMesh->SetupAttachment(GetMesh(), "HandSocket");

    /*LanternCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LanternMesh"));
    LanternCollision->SetupAttachment(LanternMesh);

    LanternCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    LanternCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
    LanternCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    LanternCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Overlap);
    LanternCollision->OnComponentBeginOverlap.AddDynamic(this, &Aman::OnHandHit);*/

    LightCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LightCollision"));
    LightCollision->SetupAttachment(LanternMesh);

    LightCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    LightCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
    LightCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    LightCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Overlap);
    LightCollision->OnComponentBeginOverlap.AddDynamic(this, &Aman::OnLightHit);


    Ridinglocation = FVector(0.0f, 0.0f, 70.0f);
    RidingRotator = FRotator(0.0f, 0.0f, -90.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
    GetCharacterMovement()->bOrientRotationToMovement = false; 
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); 

    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f; 
    CameraBoom->bUsePawnControlRotation = true; 

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false; 

    //注意有没有注释掉参数初始化...
    delta = 0.01;
    RotateSpeed = 10;
    MoveSpeed = 3;
    StandSpeed = 3;
    CrouchSpeed = 1;

    IsMoving = false;
    IsCrouching = false;

    //hook
    bIsHookShotActive = false;
    BestHookPoint_tem = nullptr;
    HookSpeed = 1000.0f;

    SpeakTimer = 0;
    MaxSpeakTime = 10.0f;

    InteractTimer = 10.0f;

}

// Called when the game starts or when spawned
void Aman::BeginPlay()
{
	Super::BeginPlay();

    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

// Called every frame
void Aman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    /*if (GEngine)
    {
        int32 MyKey = 2;
        float TimeToDisplay = 3.0f;
        FColor TextColor = FColor::Red;
        FString VectorString = FString::Printf(TEXT("tick"));

        FString Message = VectorString;

        GEngine->AddOnScreenDebugMessage(MyKey, TimeToDisplay, TextColor, Message);
    }*/

    //钩锁点检测
    TArray<AActor*> FoundActors;
    FVector CharacterLocation = GetActorLocation();
    float DetectionRadius = 1000.0f;

    //获取所有钩锁类
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AhookPoint::StaticClass(), FoundActors);

    FVector ForwardVector = GetActorForwardVector();
    TArray<AActor*> VisibleActors;
    float AngleThreshold = 45.0f; 

    //获取符合条件
    for (AActor* Actor : FoundActors)
    {
        FVector DirectionToHookPoint = Actor->GetActorLocation() - CharacterLocation;
        DirectionToHookPoint.Normalize();
        float Angle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(ForwardVector, DirectionToHookPoint)));

        FVector HookPointLocation = Actor->GetActorLocation();
        //for tolerance
        FVector ViewDirection = (HookPointLocation - CharacterLocation).GetSafeNormal();
        HookPointLocation -= ViewDirection * 50;
        FHitResult HitResult;
        if (Angle <= AngleThreshold
            && !GetWorld()->LineTraceSingleByChannel(HitResult, CharacterLocation, HookPointLocation, ECC_Visibility))
        {
            VisibleActors.Add(Actor);
        }
    }

    AActor* BestHookPoint = nullptr;
    float BestDistance = FLT_MAX;
    float BestAngle = FLT_MAX;

    //获得最佳点
    for (AActor* Actor : VisibleActors)
    {
        FVector DirectionToHookPoint = Actor->GetActorLocation() - CharacterLocation;
        float Distance = DirectionToHookPoint.Size();
        DirectionToHookPoint.Normalize();
        float Angle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(ForwardVector, DirectionToHookPoint)));

        if (Angle < BestAngle || (Angle == BestAngle && Distance < BestDistance))
        {
            BestHookPoint = Actor;
            BestDistance = Distance;
            BestAngle = Angle;
        }
    }

    if (BestHookPoint_tem) {
        //去除上一帧
        if (Cast<AhookPoint>(BestHookPoint_tem)->WidgetInstance)
        {
            Cast<AhookPoint>(BestHookPoint_tem)->RemoveMyWidgetFromViewport();
        }
    }

    if (BestHookPoint != nullptr)
    {
        Cast<AhookPoint>(BestHookPoint)->ShowHookUI(GetController<APlayerController>());
        showUI = true;
    }

    BestHookPoint_tem = BestHookPoint;

    //hook shot
    if (bIsHookShotActive)
    {
        FVector CurrentLocation = GetActorLocation();
        FVector Direction = (HookShotTarget - CurrentLocation).GetSafeNormal();
        FVector NewLocation = CurrentLocation + Direction * HookSpeed * DeltaTime;

        if (FVector::Dist(NewLocation, HookShotTarget) < 100.0f) // 这个值也可以调整
        {
            bIsHookShotActive = false;
            GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            GetCharacterMovement()->GravityScale = 1;
            ApplyInitialFallVelocity();
        }
        else
        {
            SetActorLocation(NewLocation);
        }
    }

    if (SpeakTimer > 0) {
        SpeakTimer -= DeltaTime;
        if (SpeakTimer < 0) {
            IsSpeaking = false;
            if (CurInteractingActor)
            {
                AttachToActor(CurInteractingActor, FAttachmentTransformRules::KeepWorldTransform);

                SetActorRelativeLocation(Ridinglocation);
                SetActorRelativeRotation(RidingRotator);
                GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
                GetCharacterMovement()->GravityScale = 0;
                IsRidding = true;
            }
        }
    }
    
}

void Aman::ApplyInitialFallVelocity()
{
    FVector CurrentLocation = GetActorLocation();
    FVector Direction = (HookShotTarget - CurrentLocation).GetSafeNormal();
    FVector FallVelocity = Direction * FallSpeed;
    GetCharacterMovement()->Velocity = FallVelocity;

}

// Called to bind functionality to input
void Aman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
        
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &Aman::Move);

        EnhancedInputComponent->BindAction(ClimbAction, ETriggerEvent::Triggered, this, &Aman::Climb);

        EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &Aman::Interact);
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &Aman::CrouchSwitch);

        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &Aman::Look); 

        EnhancedInputComponent->BindAction(HitAction, ETriggerEvent::Started, this, &Aman::Hit);

        EnhancedInputComponent->BindAction(HookShotAction, ETriggerEvent::Triggered, this, &Aman::HookShot);

        EnhancedInputComponent->BindAction(LightAction, ETriggerEvent::Triggered, this, &Aman::Light);

        EnhancedInputComponent->BindAction(LightAction, ETriggerEvent::Completed, this, &Aman::EndLight);
    }

}

void Aman::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    bool IsActorRotation = true;
    if (Controller != nullptr)
    {
        const FRotator ActorRotation = GetActorRotation();
        const FRotator ActorYaw(0, ActorRotation.Yaw, 0);
        const FVector ActorForward = FRotationMatrix(ActorYaw).GetUnitAxis(EAxis::X);
        const FVector ActorRight = FRotationMatrix(ActorYaw).GetUnitAxis(EAxis::Y);

        AActor* CameraManager = nullptr;
        AMyPlayerController* PC = GetController<AMyPlayerController>();
        if (PC && Cast<AMyPlayerCameraManager>(PC->PlayerCameraManager)) {
            CameraManager = Cast<AMyPlayerCameraManager>(PC->PlayerCameraManager);
        }
        FRotator CameraRotation;
        if (CameraManager) {
            CameraRotation = CameraManager->GetActorRotation();
        }
        else {
            CameraRotation = FollowCamera->GetComponentRotation();
        }
        FRotator CameraYaw(0, CameraRotation.Yaw, 0);
        FVector ForwardDirection = FRotationMatrix(CameraYaw).GetUnitAxis(EAxis::X);
        FVector RightDirection = FRotationMatrix(CameraYaw).GetUnitAxis(EAxis::Y);


        const FVector MoveDirection = ActorForward;

        const FVector RotateForward = ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X;
        AddMovementInput(ActorForward, MoveSpeed);

        SetActorRotation(FMath::RInterpTo(GetActorRotation(), RotateForward.Rotation(), delta, RotateSpeed));
        TurnAngle = RotateForward.Rotation().Yaw - GetActorRotation().Yaw;
        if (TurnAngle > 180) {
            TurnAngle = TurnAngle - 360;
        }
        else if (TurnAngle < -180) {
            TurnAngle += 360;
        }

        IsMoving = true;

        /*if (SpeedLevel == 0) {
            SpeedLevel = 1;
        }*/
    }
}

void Aman::Interact(const FInputActionValue& Value) {

}

void Aman::Climb(const FInputActionValue& Value) {

}

void Aman::CrouchSwitch(const FInputActionValue& Value) {
    IsCrouching = !IsCrouching;
    MoveSpeed = IsCrouching ? CrouchSpeed : StandSpeed;
    bool c = CanCrouch();
    c = GetCharacterMovement()->CanEverCrouch();
    if (GEngine)
    {
        int32 MyKey = 0;
        float TimeToDisplay = 0.5f;
        FColor TextColor = FColor::Red;
        FString VectorString = FString::Printf(TEXT("can crouch %d"),int(c));

        FString Message = VectorString;

        GEngine->AddOnScreenDebugMessage(MyKey, TimeToDisplay, TextColor, Message);
    }
    IsCrouching ? Crouch() : UnCrouch();
}

void Aman::Look(const FInputActionValue& Value)
{
    // input is a Vector2D
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // add yaw and pitch input to controller
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void Aman::HookShot(const FInputActionValue& Value)
{
    if (BestHookPoint_tem)
    {
        GetCharacterMovement()->GravityScale = 0;
        bIsHookShotActive = true;
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        HookShotTarget = Cast<AhookPoint>(BestHookPoint_tem)->GetTargetPosition();
    }
}

void Aman::Hit(const FInputActionValue& Value)
{
    IsHitting = true;
}

void Aman::Light(const FInputActionValue& Value)
{
    IsLighting = true;
}

void Aman::EndLight(const FInputActionValue& Value)
{
    IsLighting = false;
}

void Aman::OnInteract(AActor* InteractingActor)
{
    SpeakTimer = InteractTimer;
    IsSpeaking = true;
    CurInteractingActor = InteractingActor;
}

void Aman::OnHandHit(UPrimitiveComponent* HitComponent,
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void Aman::OnLightHit(UPrimitiveComponent* HitComponent,
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void Aman::OnFogReceived(UPrimitiveComponent* HitComponent,
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}


