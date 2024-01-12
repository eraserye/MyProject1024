// Fill out your copyright notice in the Description page of Project Settings.


#include "AIRabbit.h"

// Sets default values
AAIRabbit::AAIRabbit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ActionFunctions.Add(std::bind(&AAIRabbit::Jump, this));
	ActionFunctions.Add(std::bind(&AAIRabbit::LookAround, this));

}

// Called when the game starts or when spawned
void AAIRabbit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAIRabbit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAIRabbit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAIRabbit::Jump()
{

}

void AAIRabbit::LookAround()
{

}

