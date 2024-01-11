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
                if (GEngine)
                {
                    int32 MyKey = 1; 
                    float TimeToDisplay = 5.0f; 
                    FColor TextColor = FColor::Red;
                    FString VectorString = FString::Printf(TEXT("slot!"));

                    FString Message = VectorString;

                    GEngine->AddOnScreenDebugMessage(MyKey, TimeToDisplay, TextColor, Message);
                }
                Slot->SetPosition(ScreenPosition);
            }

            WidgetInstance = hookWidget;
            //Widget->RemoveFromViewport();

            if (GEngine)
            {
                int32 MyKey = 2; 
                float TimeToDisplay = 5.0f; 
                FColor TextColor = FColor::Red; 
                FString VectorString = FString::Printf(TEXT("Vector2D: X=%f, Y=%f"), ScreenPosition.X, ScreenPosition.Y);

                FString Message = VectorString;

                GEngine->AddOnScreenDebugMessage(MyKey, TimeToDisplay, TextColor, Message);
            }
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

        if (GEngine)
        {
            int32 MyKey = 3; // 一个唯一的键值用于标识消息，如果多次调用该方法并使用相同的键，新的消息将覆盖旧的消息
            float TimeToDisplay = 5.0f; // 消息显示的时间（秒）
            FColor TextColor = FColor::Red; // 消息的颜色
            FString VectorString = FString::Printf(TEXT("RemoveMyWidgetFromViewports"));

            FString Message = VectorString;

            GEngine->AddOnScreenDebugMessage(MyKey, TimeToDisplay, TextColor, Message);
        }
    }
}


FVector AhookPoint::GetTargetPosition() {
    return GetActorLocation() + targetOffset;
}

