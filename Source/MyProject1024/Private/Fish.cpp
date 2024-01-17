// Fill out your copyright notice in the Description page of Project Settings.


#include "Fish.h"

// Sets default values
AFish::AFish()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 创建网格体组件
	FishMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FishMesh"));
	RootComponent = FishMesh;

	// 初始化变量
	Velocity =FVector(1.0f,0.0f,0.0f);

	// 创建碰撞体
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(FishMesh);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFish::OnBeginOverlap);
}




// Called when the game starts or when spawned
void AFish::BeginPlay()
{
	Super::BeginPlay();
	LifeTime=0;
}

void AFish::Restart(FVector StartVelocity,FVector StartLocation)
{
	this->Velocity=StartVelocity;
	this->Location=StartLocation;
	// if (GEngine)
	// {
	// 	int32 MyKey = 7;
	// 	float TimeToDisplay = 3.0f;
	// 	FColor TextColor = FColor::Red;
	// 	FString VectorString = FString::Printf(TEXT("start Location %f %f %f"),Location.X,Location.Y,Location.Z);
	//
	// 	FString Message = VectorString;
	//
	// 	GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, TextColor, Message);
	// }
	SetActorRelativeLocation(Location);
	this->LifeTime=0.0f;
}

void AFish::MoveFish(float DeltaTime)
{
	Location =Location+ (Velocity * DeltaTime);
	// if (GEngine)
	// {
	// 	int32 MyKey = 7;
	// 	float TimeToDisplay = 3.0f;
	// 	FColor TextColor = FColor::Yellow;
	// 	FString VectorString = FString::Printf(TEXT("Location %f %f %f, Velocity %f %f %f"),Location.X,Location.Y,Location.Z,Velocity.X,Velocity.Y,Velocity.Z);
	//
	// 	FString Message = VectorString;
	//
	// 	GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, TextColor, Message);
	// }
	SetActorRelativeLocation(Location);
	// Direction = velocity dir
	if (Velocity.SizeSquared() > KINDA_SMALL_NUMBER)
	{
		FRotator NewRotation = Velocity.Rotation();
		SetActorRelativeRotation(NewRotation);
	}
	
}



// Called every frame
void AFish::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveFish(DeltaTime);
	LifeTime+=DeltaTime;
	
}



void AFish::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO
}

void AFish::AddForce(FVector Force,float DeltaTime)
{
	Velocity+=Force*DeltaTime;
	
}

