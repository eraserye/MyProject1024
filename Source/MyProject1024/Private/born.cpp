// Fill out your copyright notice in the Description page of Project Settings.


#include "born.h"
#include "GameFramework/CharacterMovementComponent.h"

Aborn::Aborn()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//make sure the function order is right
	ActionFunctions.Add(std::bind(&Aborn::Eat, this));
	ActionFunctions.Add(std::bind(&Aborn::LookAround, this));

	EndActionFunctions.Add(std::bind(&Aborn::EndEat, this));
	EndActionFunctions.Add(std::bind(&Aborn::EndLookAround, this));

	Mesh = GetMesh();
	if (Mesh) {
		AnimInstance = Mesh->GetAnimInstance();
	}

	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
}

void Aborn::Eat() 
{
	IsEating = true;
}

void Aborn::EndEat()
{
	IsEating = false;
}

void Aborn::LookAround()
{
	IsLooking = true;
}

void Aborn::EndLookAround()
{
	IsLooking = false;
}

void Aborn::MainAction()
{
	Super::MainAction();
	IsWalking = true;
}

void Aborn::EndMainAction()
{
	IsWalking = false;
}