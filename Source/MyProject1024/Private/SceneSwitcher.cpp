// Fill out your copyright notice in the Description page of Project Settings.


#include "SceneSwitcher.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASceneSwitcher::ASceneSwitcher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	RootComponent = BoxCollider;

	BoxCollider->SetGenerateOverlapEvents(true);
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ASceneSwitcher::SwitchScene);

	TargetLocation = CreateDefaultSubobject<USceneComponent>(TEXT("TargetLocation"));
	TargetLocation->SetupAttachment(RootComponent);
	TargetLocation->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	TargetLocation->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

}

// Called when the game starts or when spawned
void ASceneSwitcher::BeginPlay()
{
	Super::BeginPlay();

	//manage lighting
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALightManager::StaticClass(), LightingManagers);

	for (AActor* Light : LightingManagers) {
		Light->SetActorHiddenInGame(true);
	}
	if (CurLight) {
		CurLight->SetActorHiddenInGame(false);
	}
}

// Called every frame
void ASceneSwitcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASceneSwitcher::SwitchScene(class UPrimitiveComponent* OverlappedComp,
	class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OtherActor->SetActorLocation(TargetLocation->GetComponentLocation());

	AActor* BestLightManager = nullptr;
	float BestDistance = FLT_MAX;

	//获得最佳点
	for (AActor* Actor : LightingManagers)
	{
		FVector DirectionToHookPoint = Actor->GetActorLocation() - TargetLocation->GetComponentLocation();
		float Distance = DirectionToHookPoint.Size();

		if (Distance < BestDistance)
		{
			BestDistance = Distance;
			BestLightManager = Actor;
		}
	}
	for (AActor* Light : LightingManagers) {
		Light->SetActorHiddenInGame(true);
	}
	if (BestLightManager) {
		BestLightManager->SetActorHiddenInGame(false);
	}
}



