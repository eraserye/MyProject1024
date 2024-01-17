// Fill out your copyright notice in the Description page of Project Settings.


#include "FishSwarm.h"

#include "MyProject1024/man.h"
#include "Physics/ImmediatePhysics/ImmediatePhysicsShared/ImmediatePhysicsCore.h"

// Sets default values
AFishSwarm::AFishSwarm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// 创建并设置触发器组件
	BeginSwarmCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	// Pool=CreateDefaultSubobject<TArray<AFish*>>()
	RootComponent = BeginSwarmCollisionComponent;

	// 绑定事件
	BeginSwarmCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFishSwarm::OnPlayerEnterTriggerBox);
	BeginSwarmCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AFishSwarm::OnPlayerLeaveTriggerBox);


	
	
}

void AFishSwarm::ReturnFish(AFish* Fish)
{
	Fish->SetActorHiddenInGame(true);
	Fish->SetActorEnableCollision(false);
	Fish->bIsDead=true;
	AvailiblePool.Add(Fish);
}

AFish* AFishSwarm::GetFish()
{
	if (AvailiblePool.Num() > 0)
	{
		AFish* Fish = AvailiblePool.Pop();
		Fish->SetActorHiddenInGame(false);
		Fish->SetActorEnableCollision(true);
		Fish->bIsDead=false;
		InitializeFish(Fish);
		return Fish;
	}
	return nullptr;
}

void AFishSwarm::InitializePool()
{
	UWorld* WorldContext=GetWorld();
	for(int i=0;i<FishSpawnRate;i++)
	{
		// if (GEngine)
		// {
		// 	int32 MyKey = 7;
		// 	float TimeToDisplay = 3.0f;
		// 	FColor TextColor = FColor::Red;
		// 	FString VectorString = FString::Printf(TEXT("initial %d"),i);
		//
		// 	FString Message = VectorString;
		//
		// 	GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, TextColor, Message);
		// }
		AFish* NewFish = WorldContext->SpawnActor<AFish>(FishClass);
		// 将鱼附加到Swarm Actor
		NewFish->AttachToComponent(this->RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		NewFish->SetActorEnableCollision(false);
		NewFish->SetActorHiddenInGame(true);
		NewFish->bIsDead=true;
		Pool.Add(NewFish);
		AvailiblePool.Add(NewFish);
	}
}

void AFishSwarm::InitializeFish(AFish* Fish)
{
	FVector SpawnPos=FVector(FMath::RandRange(BBoxMin.X,BBoxMax.X),FMath::RandRange(BBoxMin.Y,BBoxMax.Y),FMath::RandRange(BBoxMin.Z,BBoxMax.Z));
	FVector SpawnVelocity=StartVelocity+FVector(FMath::RandRange(-VelocityRange.X,VelocityRange.X),FMath::RandRange(-VelocityRange.Y,VelocityRange.Y),FMath::RandRange(-VelocityRange.Z,VelocityRange.Z));
	
	Fish->Restart(SpawnVelocity,SpawnPos);
}

FVector AFishSwarm::CalcForce(float Timer)
{
	if (ForceCurve)
	{
		float CurveValue = ForceCurve->GetFloatValue(Timer);
		return CurveValue*ForceDir;
	}else
	{
		return 0.2*ForceDir;
	}
}

void AFishSwarm::UpdateEachFish(float DeltaTime)
{
	AFish* Temp;
	if (bIsSpawn)
	{
		if (GEngine)
		{
			int32 MyKey = 7;
			float TimeToDisplay = 3.0f;
			FColor TextColor = FColor::Red;
			FString VectorString = FString::Printf(TEXT("get fish"));
			
			FString Message = VectorString;
			
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, TextColor, Message);
		}
		if(FMath::FRandRange(0.0,1.0f)<SpawnSpeed)	GetFish();
	}
	// GetFish();
	for (int i=0;i<Pool.Num();i++)
	{
		Temp=Pool[i];
		
		if (!Temp->bIsDead)
		{
			
			Temp->LifeTime+=DeltaTime;
			if (Temp->LifeTime>TotLifeTime)
			{
				ReturnFish(Temp);
			}else
			{
				// Test:

				float StopForce=1.f;
				// float StopForce=0;
				FVector F=CalcForce(Temp->LifeTime);
				Temp->AddForce(F+FVector(0.0f,0.0f,0.0f+StopForce*Gravity),DeltaTime);
				
			}
		}else 
		{
			
			
		}
	}
}

void AFishSwarm::OnPlayerEnterTriggerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	
	Aman* man = Cast<Aman>(OtherActor);
	if (man && OtherActor != this && OtherComp)
	{
		if (GEngine)
		{
			int32 MyKey = 7;
			float TimeToDisplay = 3.0f;
			FColor TextColor = FColor::Red;
			FString VectorString = FString::Printf(TEXT("enter!"));

			FString Message = VectorString;

			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, TextColor, Message);
		}
		bIsSpawn=true;
	}
	

}

void AFishSwarm::OnPlayerLeaveTriggerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	
	Aman* man = Cast<Aman>(OtherActor);
	if (man && OtherActor != this && OtherComp)
	{
		if (GEngine)
		{
			int32 MyKey = 7;
			float TimeToDisplay = 3.0f;
			FColor TextColor = FColor::Red;
			FString VectorString = FString::Printf(TEXT("leave!"));

			FString Message = VectorString;

			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, TextColor, Message);
		}
		bIsSpawn=false;
	}
}

// Called when the game starts or when spawned
void AFishSwarm::BeginPlay()
{
	Super::BeginPlay();
	InitializePool();
}

// Called every frame
void AFishSwarm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateEachFish(DeltaTime);
	
}

