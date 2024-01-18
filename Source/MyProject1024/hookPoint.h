// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "Components/StaticMeshComponent.h"
#include "hookPoint.generated.h"

UCLASS()
class MYPROJECT1024_API AhookPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AhookPoint();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

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

	/*UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* CubeComponent;*/

private:
	bool IsAdd;

};
