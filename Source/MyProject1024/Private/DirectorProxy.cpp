// Fill out your copyright notice in the Description page of Project Settings.


#include "DirectorProxy.h"
#include "Camera/CameraComponent.h"

// Sets default values
ADirectorProxy::ADirectorProxy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	RootComponent = BoxCollider;

	CameraPath = CreateDefaultSubobject<USplineComponent>(TEXT("CameraPath"));
	CameraPath->SetupAttachment(RootComponent);
	//CameraPath->AddSplinePoint(FVector(0.f, 0.f, 0.f), ESplineCoordinateSpace::Local);
	//CameraPath->AddSplinePoint(FVector(100.f, 0.f, 0.f), ESplineCoordinateSpace::Local);
	CameraPath->SetClosedLoop(false);

	Path = CreateDefaultSubobject<USplineComponent>(TEXT("Path"));
	Path->SetupAttachment(RootComponent);
	Path->SetClosedLoop(false);

	CameraPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CameraPoint"));
	CameraPoint->SetupAttachment(RootComponent);

	CameraPoint->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	CameraPoint->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(RootComponent); 
	FollowCamera->bUsePawnControlRotation = false;

	SequencePath = CreateDefaultSubobject<USplineComponent>(TEXT("SequencePath"));
	SequencePath->SetupAttachment(RootComponent);
	SequencePath->SetClosedLoop(false);

	SequencePoint = CreateDefaultSubobject<USceneComponent>(TEXT("SequencePoint"));
	SequencePoint->SetupAttachment(RootComponent);
	SequencePoint->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	SequencePoint->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	SequencyTarget = CreateDefaultSubobject<USceneComponent>(TEXT("SequencyTarget"));
	SequencyTarget->SetupAttachment(RootComponent);
	SequencyTarget->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	SequencyTarget->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));



}

void ADirectorProxy::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FVector CameraPos = CameraPath->GetLocationAtTime(PathT, ESplineCoordinateSpace::World);
	//look at pawn
	FVector ViewCenter = Path->GetLocationAtTime(PathT, ESplineCoordinateSpace::World);
	FVector ViewDirection = (ViewCenter - CameraPos).GetSafeNormal();
	FRotator Rotator = ViewDirection.Rotation();

	FollowCamera->SetWorldLocation(CameraPos);
	FollowCamera->SetWorldRotation(Rotator);
}

// Called when the game starts or when spawned
void ADirectorProxy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADirectorProxy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

