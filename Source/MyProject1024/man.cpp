// Fill out your copyright notice in the Description page of Project Settings.

#include "man.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
Aman::Aman()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
    GetCapsuleComponent()->SetGenerateOverlapEvents(true);

    CustomMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CustomMesh"));
    CustomMesh->SetupAttachment(RootComponent);

    Ridinglocation = FVector(0.0f, 0.0f, 70.0f);
    RidingRotator = FRotator(0.0f, 0.0f, -90.0f);

}

// Called when the game starts or when spawned
void Aman::BeginPlay()
{
	Super::BeginPlay();

    if (CustomMesh && IdleAnimation)
    {
        CustomMesh->PlayAnimation(IdleAnimation, true);
    }
	
}

// Called every frame
void Aman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void Aman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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

        if (RidingAnimation)
        {
            if (CustomMesh && RidingAnimation)
            {
                CustomMesh->PlayAnimation(RidingAnimation, true);
            }
        }
    }
}

