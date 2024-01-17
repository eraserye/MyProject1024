// Fill out your copyright notice in the Description page of Project Settings.


#include "LightManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALightManager::ALightManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DaySky = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("DaySky"));
	NightSky = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("NightSky"));
	CurrentSky = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("CurrentSky"));

    NightSky->SetupAttachment(RootComponent);
    DaySky->SetupAttachment(RootComponent);
    CurrentSky->SetupAttachment(RootComponent);

    DayDirLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("DayDirLight"));
    NightDirLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("NightDirLight"));
    CurrentDirLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("CurrentDirLight"));

    DayDirLight->SetupAttachment(RootComponent);
    NightDirLight->SetupAttachment(RootComponent);
    CurrentDirLight->SetupAttachment(RootComponent);


    DaySkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("DaySkyLight"));
    NightSkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("NightSkyLight"));
    CurrentSkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("CurrentSkyLight"));

    NightSkyLight->SetupAttachment(RootComponent);
    DaySkyLight->SetupAttachment(RootComponent);
    CurrentSkyLight->SetupAttachment(RootComponent);



    DayFog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("DayFog"));
    NightFog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("NightFog"));
    CurrentFog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("CurrentFog"));

    NightFog->SetupAttachment(RootComponent);
    DayFog->SetupAttachment(RootComponent);
    CurrentFog->SetupAttachment(RootComponent);


    //DayFog->bEnableVolumetricFog = true;
    //NightFog->bEnableVolumetricFog = true;

	Path = CreateDefaultSubobject<USplineComponent>(TEXT("Path"));
	Path->SetupAttachment(RootComponent);
	Path->SetClosedLoop(false);

}

// Called when the game starts or when spawned
void ALightManager::BeginPlay()
{
	Super::BeginPlay();


    DaySky->SetVisibility(false, true);
    NightSky->SetVisibility(false, true);
    CurrentSky->SetVisibility(true, true);
    DaySky->SetHiddenInGame(true, true);
    NightSky->SetHiddenInGame(true, true);
    CurrentSky->SetHiddenInGame(false, true);


    DayDirLight->SetVisibility(false, true);
    NightDirLight->SetVisibility(false, true);
    CurrentDirLight->SetVisibility(true, true);
    DayDirLight->SetHiddenInGame(true, true);
    NightDirLight->SetHiddenInGame(true, true);
    CurrentDirLight->SetHiddenInGame(false, true);

    DaySkyLight->SetVisibility(false, true);
    NightSkyLight->SetVisibility(false, true);
    CurrentSkyLight->SetVisibility(true, true);
    DaySkyLight->SetHiddenInGame(true, true);
    NightSkyLight->SetHiddenInGame(true, true);
    CurrentSkyLight->SetHiddenInGame(false, true);

    DayFog->SetVisibility(false, true);
    NightFog->SetVisibility(false, true);
    CurrentFog->SetVisibility(true, true);
    DayFog->SetHiddenInGame(true, true);
    NightFog->SetHiddenInGame(true, true);
    CurrentFog->SetHiddenInGame(false, true);
	
}

// Called every frame
void ALightManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FVector PawnLocation;
    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0)) {
        PawnLocation = PlayerController->GetPawn()->GetActorLocation();
    }
	float PathKey = Path->FindInputKeyClosestToWorldLocation(PawnLocation);
	int PointNum = Path->GetNumberOfSplinePoints();
	float Time = PathKey / (PointNum-1);
    Time = FMath::Clamp(Time, 0, 1);


	//interplo
    LerpAtmo(Time);
    LerpDirLight(Time);
    LerpSkyLight(Time);
    LerpFog(Time);
}

void ALightManager::LerpAtmo(float Time) {
    Time = Time < 0.75 ? 0.0 : (Time - 0.75) / 0.25;
    if (DaySky && NightSky)
    {
        float DayMultiScattering = DaySky->MultiScatteringFactor;
        float NightMultiScattering = NightSky->MultiScatteringFactor;

        float DayRayleighScale = DaySky->RayleighScatteringScale;
        float NightRayleighScale = NightSky->RayleighScatteringScale;

        FLinearColor DayRayleighScattering = DaySky->RayleighScattering;
        FLinearColor NightRayleighScattering = NightSky->RayleighScattering;

        float DayMieScale = DaySky->MieScatteringScale;
        float NightMieScale = NightSky->MieScatteringScale;

        FLinearColor DayMieScattering = DaySky->MieScattering;
        FLinearColor NightMieScattering = NightSky->MieScattering;

        float InterpMultiScattering = FMath::Lerp(DayMultiScattering, NightMultiScattering, Time);
        float InterpRayleighScale = FMath::Lerp(DayRayleighScale, NightRayleighScale, Time);
        FLinearColor InterpRayleighScattering = FLinearColor::LerpUsingHSV(DayRayleighScattering, NightRayleighScattering, Time);
        float InterpMieScale = FMath::Lerp(DayMieScale, NightMieScale, Time);
        FLinearColor InterpMieScattering = FLinearColor::LerpUsingHSV(DayMieScattering, NightMieScattering, Time);

        CurrentSky->SetMultiScatteringFactor(InterpMultiScattering);
        CurrentSky->SetRayleighScatteringScale(InterpRayleighScale);
        CurrentSky->SetRayleighScattering(InterpRayleighScattering);
        CurrentSky->SetMieScatteringScale(InterpMieScale);
        CurrentSky->SetMieScattering(InterpMieScattering);
    }
}

void ALightManager::LerpDirLight(float Time) {
    float RotateTime = Time < 0.75 ? Time / 0.75 : (1 - Time) / 0.25;
    Time = Time < 0.75 ? 0.0 : (Time - 0.75) / 0.25;
    if (DayDirLight && NightDirLight)
    {
        float DayIntensity = DayDirLight->Intensity;
        float NightIntensity = NightDirLight->Intensity;

        FLinearColor DayLightColor = DayDirLight->LightColor;
        FLinearColor NightLightColor = NightDirLight->LightColor;

        FLinearColor DayCloudScatteredLuminanceScale = DayDirLight->CloudScatteredLuminanceScale;
        FLinearColor NightCloudScatteredLuminanceScale = NightDirLight->CloudScatteredLuminanceScale;

        float DayTemperature = DayDirLight->Temperature;
        float NightTemperature = NightDirLight->Temperature;

        FRotator  DayRotation = DayDirLight->GetComponentTransform().Rotator();
        FRotator  NightRotation = NightDirLight->GetComponentTransform().Rotator();

        FQuat DayQuat = FQuat(DayRotation);
        FQuat NightQuat = FQuat(NightRotation);



        float InterpIntensity = FMath::Lerp(DayIntensity, NightIntensity, Time);
        FLinearColor InterpLightColor = FLinearColor::LerpUsingHSV(DayLightColor, NightLightColor, Time);
        FLinearColor InterpCloudScatteredLuminanceScale = FLinearColor::LerpUsingHSV(DayCloudScatteredLuminanceScale, NightCloudScatteredLuminanceScale, Time);
        float InterpTemperature = FMath::Lerp(DayTemperature, NightTemperature, Time);
        //inter rotate
        FQuat InterpolatedQuat = FQuat::Slerp(DayQuat, NightQuat, RotateTime);
        FRotator InterpolatedRotation = InterpolatedQuat.Rotator();
        

        CurrentDirLight->SetIntensity(InterpIntensity);
        CurrentDirLight->SetLightColor(InterpLightColor);
        CurrentDirLight->CloudScatteredLuminanceScale = InterpCloudScatteredLuminanceScale;
        CurrentDirLight->SetTemperature(InterpTemperature);
        //CurrentDirLight->GetComponentTransform().Rotator() = InterpolatedRotation;
        CurrentDirLight->SetWorldRotation(InterpolatedRotation);
    }
}

void ALightManager::LerpSkyLight(float Time) {
    Time = Time < 0.75 ? 0.0 : (Time - 0.75) / 0.25;
    if (DaySkyLight && NightSkyLight)
    {
        float DayIntensity = DaySkyLight->Intensity;
        float NightIntensity = NightSkyLight->Intensity;

        FLinearColor DayLightColor = DaySkyLight->LightColor;
        FLinearColor NightLightColor = NightSkyLight->LightColor;

        float InterpIntensity = FMath::Lerp(DayIntensity, NightIntensity, Time);
        FLinearColor InterpLightColor = FLinearColor::LerpUsingHSV(DayLightColor, NightLightColor, Time);

        CurrentSkyLight->SetIntensity(InterpIntensity);
        CurrentSkyLight->SetLightColor(InterpLightColor);
    }
}

void ALightManager::LerpFog(float Time) {
    Time = Time < 0.75 ? 0.0 : (Time - 0.75) / 0.25;
    if (DaySkyLight && NightSkyLight)
    {
        float DayDensity = DayFog->FogDensity;
        float NightDensity = NightFog->FogDensity;

        float DayStartDistance = DayFog->StartDistance;
        float NightStartDistance= NightFog->StartDistance;

        float DayScatteringDistribution= DayFog->VolumetricFogScatteringDistribution;
        float NightScatteringDistribution = NightFog->VolumetricFogScatteringDistribution;

        FColor DayAlbedo = DayFog->VolumetricFogAlbedo;
        FColor NightAlbedo = NightFog->VolumetricFogAlbedo;

        float InterpIntensity = FMath::Lerp(DayDensity, NightDensity, Time);
        float InterpStartDistance = FMath::Lerp(DayStartDistance, NightStartDistance, Time);
        float InterpVolumetricFogScatteringDistribution = FMath::Lerp(DayScatteringDistribution, NightScatteringDistribution, Time);
        FLinearColor InterpAlbedo = FLinearColor::LerpUsingHSV(DayAlbedo, NightAlbedo, Time);

        CurrentFog->SetFogDensity(InterpIntensity);
        CurrentFog->SetStartDistance(InterpStartDistance);
        CurrentFog->SetVolumetricFogScatteringDistribution(InterpVolumetricFogScatteringDistribution);
        CurrentFog->SetVolumetricFogAlbedo(InterpAlbedo.ToFColorSRGB());
    }
}

