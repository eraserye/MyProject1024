// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyAIController.h"
#include "GuideController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT1024_API AGuideController : public AMyAIController
{
	GENERATED_BODY()

public:
	AGuideController();

protected:
	virtual void OnPerceptionUpdated(const TArray<AActor*>& DetectedActors) override;
	
};
