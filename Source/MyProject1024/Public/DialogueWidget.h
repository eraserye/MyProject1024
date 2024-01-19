// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "DialogueWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT1024_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
        void UpdateText(FText NewText);

protected:
    UPROPERTY(meta = (BindWidget))
        UTextBlock* DialogueText; 
	
};
