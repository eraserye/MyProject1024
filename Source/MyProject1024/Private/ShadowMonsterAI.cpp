// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowMonsterAI.h"

AShadowMonsterAI::AShadowMonsterAI() :AAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	BodyCollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyCollision"));
	BodyCollisionComponent->SetupAttachment(GetMesh(), "BodySocket");
	//BodyCollisionComponent->SetupAttachment(RootComponent);
	BodyCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BodyCollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	BodyCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BodyCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Overlap);
	BodyCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AShadowMonsterAI::OnHitReceived);


	//make sure the function order is right
	ActionFunctions.Add(std::bind(&AShadowMonsterAI::Pray, this));

	EndActionFunctions.Add(std::bind(&AShadowMonsterAI::EndPray, this));

	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
}

void AShadowMonsterAI::Chase()
{
	EndAttack();
	IsChasing = true;
}

void AShadowMonsterAI::EndChase()
{
	IsChasing = false;
}

void AShadowMonsterAI::Attack()
{
	EndChase();
	IsAttacking = true;
}

void AShadowMonsterAI::EndAttack()
{
	IsAttacking = false;
}

void AShadowMonsterAI::Pray()
{
	IsPraying = true;
}

void AShadowMonsterAI::EndPray()
{
	IsPraying = false;
}

void AShadowMonsterAI::OnHitReceived(UPrimitiveComponent* HitComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if ((Aman* man = Cast<Aman>(OtherActor)) != nullptr && OtherActor != this && OtherComp)
	{
		if (GEngine)
		{
			int32 MyKey = 6;
			float TimeToDisplay = 3.0f;
			FColor TextColor = FColor::Red;
			FString VectorString = FString::Printf(TEXT("be hit"));

			FString Message = VectorString;

			GEngine->AddOnScreenDebugMessage(MyKey, TimeToDisplay, TextColor, Message);
		}

	}
}