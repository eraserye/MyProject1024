// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "DirectorProxy.h"
#include "MyPlayerCameraManager.generated.h"

/**
 * 
 */
UENUM()
enum CameraMode
{
	SequencePath,
	SequencePoint,
	Path,
	StaticPoint,
	Follow,
};

UCLASS()
class MYPROJECT1024_API AMyPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	AMyPlayerCameraManager();
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
	ADirectorProxy* DirectorProxy;
	bool SequenceMode;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "blend")
	float LocationThreshold;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "blend")
	float RotationThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "blend")
	TEnumAsByte<enum CameraMode> Mode;

	bool SwitchMode;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "blend")
	float BlendTime;
	float BlendTimeRemain;
	FMinimalViewInfo* PrevPOVPointer;
	FMinimalViewInfo PrevPOV;
};
