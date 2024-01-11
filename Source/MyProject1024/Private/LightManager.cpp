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

    DaySky->SetVisibility(false, true);
    NightSky->SetVisibility(false, true);
    CurrentSky->SetVisibility(true, true);
    DaySky->SetHiddenInGame(true, true);
    NightSky->SetHiddenInGame(true, true);
    CurrentSky->SetHiddenInGame(false, true);

    DayDirLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("DayDirLight"));
    NightDirLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("NightDirLight"));
    CurrentDirLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("CurrentDirLight"));

    DayDirLight->SetupAttachment(RootComponent);
    NightDirLight->SetupAttachment(RootComponent);
    CurrentDirLight->SetupAttachment(RootComponent);

    DayDirLight->SetVisibility(false, true);
    NightDirLight->SetVisibility(false, true);
    CurrentDirLight->SetVisibility(true, true);
    DayDirLight->SetHiddenInGame(true, true);
    NightDirLight->SetHiddenInGame(true, true);
    CurrentDirLight->SetHiddenInGame(false, true);

    DaySkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("DaySkyLight"));
    NightSkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("NightSkyLight"));
    CurrentSkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("CurrentSkyLight"));

    NightSkyLight->SetupAttachment(RootComponent);
    DaySkyLight->SetupAttachment(RootComponent);
    CurrentSkyLight->SetupAttachment(RootComponent);

    DaySkyLight->SetVisibility(false, true);
    NightSkyLight->SetVisibility(false, true);
    CurrentSkyLight->SetVisibility(true, true);
    DaySkyLight->SetHiddenInGame(true, true);
    NightSkyLight->SetHiddenInGame(true, true);
    CurrentSkyLight->SetHiddenInGame(false, true);

    DayFog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("DayFog"));
    NightFog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("NightFog"));
    CurrentFog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("CurrentFog"));

    NightFog->SetupAttachment(RootComponent);
    DayFog->SetupAttachment(RootComponent);
    CurrentFog->SetupAttachment(RootComponent);

    DayFog->SetVisibility(false, true);
    NightFog->SetVisibility(false, true);
    CurrentFog->SetVisibility(true, true);
    DayFog->SetHiddenInGame(true, true);
    NightFog->SetHiddenInGame(true, true);
    CurrentFog->SetHiddenInGame(false, true);

    DayFog->bEnableVolumetricFog = true;
    NightFog->bEnableVolumetricFog = true;

	Path = CreateDefaultSubobject<USplineComponent>(TEXT("Path"));
	Path->SetupAttachment(RootComponent);
	Path->SetClosedLoop(false);


}

// Called when the game starts or when spawned
void ALightManager::BeginPlay()
{
	Super::BeginPlay();
	
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

    if (GEngine)
    {
        int32 MyKey = 3;
        float TimeToDisplay = 0.5f;
        FColor TextColor = FColor::Red;
        FString VectorString = FString::Printf(TEXT("time %f"), Time);

        FString Message = VectorString;

        GEngine->AddOnScreenDebugMessage(MyKey, TimeToDisplay, TextColor, Message);
    }

	//interplo
    LerpAtmo(Time);
    LerpDirLight(Time);
    LerpSkyLight(Time);
    LerpFog(Time);
}

void ALightManager::LerpAtmo(float Time) {
    Time = FMath::Clamp(Time, 0, 1);
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

        float InterpMultiScattering = FMath::Lerp(DayMultiScattering, NightMultiScattering, Time);
        float InterpRayleighScale = FMath::Lerp(DayRayleighScale, NightRayleighScale, Time);
        FLinearColor InterpRayleighScattering = FLinearColor::LerpUsingHSV(DayRayleighScattering, NightRayleighScattering, Time);
        float InterpMieScale = FMath::Lerp(DayMieScale, NightMieScale, Time);

        CurrentSky->SetMultiScatteringFactor(InterpMultiScattering);
        CurrentSky->SetRayleighScatteringScale(InterpRayleighScale);
        CurrentSky->SetRayleighScattering(InterpRayleighScattering);
        CurrentSky->SetMieScatteringScale(InterpMieScale);
    }
}

void ALightManager::LerpDirLight(float Time) {
    Time = FMath::Clamp(Time, 0, 1);
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

        float InterpIntensity = FMath::Lerp(DayIntensity, NightIntensity, Time);
        FLinearColor InterpLightColor = FLinearColor::LerpUsingHSV(DayLightColor, NightLightColor, Time);
        FLinearColor InterpCloudScatteredLuminanceScale = FLinearColor::LerpUsingHSV(DayCloudScatteredLuminanceScale, NightCloudScatteredLuminanceScale, Time);
        float InterpTemperature = FMath::Lerp(DayTemperature, NightTemperature, Time);

        CurrentDirLight->SetIntensity(InterpIntensity);
        CurrentDirLight->SetLightColor(InterpLightColor);
        CurrentDirLight->CloudScatteredLuminanceScale = InterpCloudScatteredLuminanceScale;
        CurrentDirLight->SetTemperature(InterpTemperature);
    }
}

void ALightManager::LerpSkyLight(float Time) {
    Time = FMath::Clamp(Time, 0, 1);
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
    Time = FMath::Clamp(Time, 0, 1);
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

