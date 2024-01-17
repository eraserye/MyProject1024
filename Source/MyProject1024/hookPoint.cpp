// Fill out your copyright notice in the Description page of Project Settings.


#include "hookPoint.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "hookUI.h"
#include "Components/Image.h"

// Sets default values
AhookPoint::AhookPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    IsAdd = false;
    WidgetInstance = nullptr;
    targetOffset = FVector(0.0f, 0.0f, 90.0f);

}

// Called when the game starts or when spawned
void AhookPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AhookPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AhookPoint::ShowHookUI(APlayerController* PlayerController)
{
    FVector2D ScreenPosition;
    if (PlayerController && PlayerController->ProjectWorldLocationToScreen(GetActorLocation(), ScreenPosition))
    {
        if (UhookUI* hookWidget = CreateWidget<UhookUI>(GetWorld(), HookWidgetClass))
        {
            hookWidget->AddToViewport();
            UImage* hookImage = hookWidget->hookImage;

            UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(hookImage->Slot);
            if (Slot)
            {
                
                Slot->SetPosition(ScreenPosition);
            }

            WidgetInstance = hookWidget;
            //Widget->RemoveFromViewport();

        }
    }
}

void AhookPoint::RemoveMyWidgetFromViewport()
{
    if (WidgetInstance)
    {
        WidgetInstance->RemoveFromViewport();
        //WidgetInstance->RemoveFromParent();
        WidgetInstance = nullptr; 

    }
}


FVector AhookPoint::GetTargetPosition() {
    return GetActorLocation() + targetOffset;
}

