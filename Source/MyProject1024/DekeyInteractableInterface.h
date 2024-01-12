// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DekeyInteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDekeyInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYPROJECT1024_API IDekeyInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnInteract(AActor* InteractingActor) = 0;
};
