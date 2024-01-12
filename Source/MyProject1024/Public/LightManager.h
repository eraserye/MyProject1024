// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SplineComponent.h"
#include "LightManager.generated.h"

UCLASS()
class MYPROJECT1024_API ALightManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky")
		USkyAtmosphereComponent* DaySky;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky")
		USkyAtmosphereComponent* NightSky;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky")
		USkyAtmosphereComponent* CurrentSky;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky")
		UDirectionalLightComponent* DayDirLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky")
		UDirectionalLightComponent* NightDirLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky")
		UDirectionalLightComponent* CurrentDirLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky")
		USkyLightComponent* DaySkyLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky")
		USkyLightComponent* NightSkyLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky")
		USkyLightComponent* CurrentSkyLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky")
		UExponentialHeightFogComponent* DayFog;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky")
		UExponentialHeightFogComponent* NightFog;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky")
		UExponentialHeightFogComponent* CurrentFog;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sky")
		USplineComponent* Path;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void LerpAtmo(float Time);
	void LerpDirLight(float Time);
	void LerpSkyLight(float Time);
	void LerpFog(float Time);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
