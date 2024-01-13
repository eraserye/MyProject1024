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
    if (GEngine)
    {
        int32 MyKey = 2;
        float TimeToDisplay = 3.0f;
        FColor TextColor = FColor::Red;
        FString VectorString = FString::Printf(TEXT("tick"));

        FString Message = VectorString;

        GEngine->AddOnScreenDebugMessage(MyKey, TimeToDisplay, TextColor, Message);
    }

    if (GEngine && Controller != nullptr)
    {
        int32 MyKey = 1;
        float TimeToDisplay = 3.0f;
        FColor TextColor = FColor::Red;
        FString VectorString = FString::Printf(TEXT("have con "));

        FString Message = VectorString;

        GEngine->AddOnScreenDebugMessage(MyKey, TimeToDisplay, TextColor, Message);
    }
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

void Aman::OnInteract(AActor* InteractingActor)
{
    FVector Start = GetActorLocation();
    FVector ForwardVector = GetActorForwardVector();
    FVector End = ((ForwardVector * 1000.f) + Start);
    DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);
    if (InteractingActor)
    {
        DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);
        AttachToActor(InteractingActor, FAttachmentTransformRules::KeepWorldTransform);

        SetActorRelativeLocation(Ridinglocation);
        //CustomMesh->SetWorldRotation(RidingRotator);
        SetActorRelativeRotation(RidingRotator);
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        GetCharacterMovement()->GravityScale = 0;

        /*if (RidingAnimation)
        {
            if (CustomMesh && RidingAnimation)
            {
                CustomMesh->PlayAnimation(RidingAnimation, true);
            }
        }*/
    }
}

