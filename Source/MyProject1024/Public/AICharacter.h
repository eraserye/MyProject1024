// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

UCLASS()
class MYPROJECT1024_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//typedef void (AAICharacter::* ActionFunction)();
	TArray<std::function<void()>> ActionFunctions;
	TArray<std::function<void()>> EndActionFunctions;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PerformRandomAction();
	//如何声明一个纯虚函数?
	virtual void MainAction();
	virtual void EndMainAction();
	void EndAction();
	int CurActionIndex;

};
