// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "hookPoint.generated.h"

UCLASS()
class MYPROJECT1024_API AhookPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AhookPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ShowHookUI(APlayerController* PlayerController);
	void RemoveMyWidgetFromViewport();
	FVector GetTargetPosition();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		TSubclassOf<UUserWidget> HookWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Properties")
		FVector  targetOffset;

	UPROPERTY()
		UUserWidget* WidgetInstance;

private:
	bool IsAdd;

};
