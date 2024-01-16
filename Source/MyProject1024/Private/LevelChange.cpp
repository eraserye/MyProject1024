// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelChange.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelChange::ALevelChange()
{
	
	PrimaryActorTick.bCanEverTick = true;

	// 创建触发器组件并设置为根组件
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	// 绑定触发器事件
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ALevelChange::OnPlayerEnterTriggerBox);

}

// Called when the game starts or when spawned
void ALevelChange::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelChange::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelChange::OnPlayerEnterTriggerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			UGameplayStatics::OpenLevel(World, TargetLevelName);
		}
	}
}

