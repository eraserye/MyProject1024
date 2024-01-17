// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowMonsterAI.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Canvas.h"
#include "GameFramework/HUD.h"
#include "CanvasItem.h"

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
	//BodyCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AShadowMonsterAI::OnHitReceived);


	//make sure the function order is right
	ActionFunctions.Add(std::bind(&AShadowMonsterAI::Pray, this));

	EndActionFunctions.Add(std::bind(&AShadowMonsterAI::EndPray, this));

	GetCharacterMovement()->MaxWalkSpeed = 100.0f;

	IsStartHit = false;

	VanishTimer = 1.0f;
	MAXVanishTimer = 2.0f;
	AnimPlayRate = 1.0f;
}

void AShadowMonsterAI::BeginPlay()
{
	Super::BeginPlay();
	if (CollectionReference) {
		CollectionInstance = GetWorld()->GetParameterCollectionInstance(CollectionReference);
	}

	USkeletalMeshComponent* MeshComp = GetMesh();
	MatInst = Cast<UMaterialInstanceDynamic>(MeshComp->GetMaterial(0));
	if (!MatInst){
		MatInst = UMaterialInstanceDynamic::Create(MeshComp->GetMaterial(0), this);
		MeshComp->SetMaterial(0, MatInst);
	}
	else {
		
	}
}

void AShadowMonsterAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappingComponents;
	BodyCollisionComponent->GetOverlappingComponents(OverlappingComponents);
	
	bool HasHit = false;
	for (UPrimitiveComponent* Component : OverlappingComponents)
	{
		if (Aman* man = Cast<Aman>(Component->GetOwner())) {
			BeHit(man, Component);
			HasHit = true;
			break;
		}
	}
	//if (!HasHit) {
	//	IsStartHit = false;
	//}

	if (VanishTimer > 0) {
		VanishTimer -= DeltaTime;
		if (VanishTimer <= 0) {
			Destroy();
		}
		if (MatInst) {
			MatInst->SetScalarParameterValue(FName("VanishTime"), VanishTimer/ MAXVanishTimer);
		}
	}
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
	Aman* man = Cast<Aman>(OtherActor);
	if (man && OtherActor != this && OtherComp)
	{
		
		FVector Center = OtherComp->Bounds.Origin;
		if (SweepResult.Time == 0.0f) {
			VanishStartPoint = Center;
		}
		VanishCurPoint = Center;

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		FVector2D VanishCurPointScreen;
		FVector2D VanishStartPointScreen;
		bool bIsOnScreen = false;
		if (PlayerController)
		{
			bIsOnScreen = UGameplayStatics::ProjectWorldToScreen(PlayerController, VanishStartPoint, VanishStartPointScreen)
				&& UGameplayStatics::ProjectWorldToScreen(PlayerController, VanishCurPoint, VanishCurPointScreen);
		}

		if (CollectionInstance && bIsOnScreen)
		{
			if (GEngine)
			{
				int32 MyKey = 7;
				float TimeToDisplay = 3.0f;
				FColor TextColor = FColor::Red;
				FString VectorString = FString::Printf(TEXT("Time: %f screen pos : start %f %f cur %f %f"), SweepResult.Time,
					VanishStartPointScreen.X, VanishStartPointScreen.Y, VanishCurPointScreen.X, VanishCurPointScreen.Y);

				FString Message = VectorString;

				GEngine->AddOnScreenDebugMessage(MyKey, TimeToDisplay, TextColor, Message);
			}
			CollectionInstance->SetVectorParameterValue(TEXT("CollisionStartPoint"), FVector(VanishStartPointScreen,1.0f));
			CollectionInstance->SetVectorParameterValue(TEXT("CollisionCurPoint"), FVector(VanishCurPointScreen,1.0f));
		}
	}
}

void AShadowMonsterAI::BeHit(AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
	FVector Center = OtherComp->Bounds.Origin;
	if (!IsStartHit) {
		VanishStartPoint = Center;
		IsStartHit = true;
		VanishTimer = MAXVanishTimer;
		AnimPlayRate = 0.0f;
	}
	VanishCurPoint = Center;

	if (MatInst) {
		MatInst->SetVectorParameterValue(FName("CollisionStartPoint"), FVector(VanishStartPoint));
		MatInst->SetVectorParameterValue(FName("CollisionCurPoint"), FVector(VanishCurPoint));
	}

	//APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	//FVector2D VanishCurPointScreen;
	//FVector2D VanishStartPointScreen;
	//CollectionInstance->SetVectorParameterValue(TEXT("CollisionStartPoint"), FVector(VanishStartPoint));
	//CollectionInstance->SetVectorParameterValue(TEXT("CollisionCurPoint"), FVector(VanishCurPoint));
	/*bool bIsOnScreen = false;
	if (PlayerController)
	{
		bIsOnScreen = UGameplayStatics::ProjectWorldToScreen(PlayerController, VanishStartPoint, VanishStartPointScreen)
			&& UGameplayStatics::ProjectWorldToScreen(PlayerController, VanishCurPoint, VanishCurPointScreen);
	}

	if (CollectionInstance && bIsOnScreen)
	{
		if (GEngine)
		{
			int32 MyKey = 7;
			float TimeToDisplay = 3.0f;
			FColor TextColor = FColor::Red;
			FString VectorString = FString::Printf(TEXT("screen pos : start %f %f cur %f %f"),
				VanishStartPointScreen.X, VanishStartPointScreen.Y, VanishCurPointScreen.X, VanishCurPointScreen.Y);

			FString Message = VectorString;

			GEngine->AddOnScreenDebugMessage(MyKey, TimeToDisplay, TextColor, Message);
		}
		CollectionInstance->SetVectorParameterValue(TEXT("CollisionStartPoint"), FVector(VanishStartPointScreen, 1.0f));
		CollectionInstance->SetVectorParameterValue(TEXT("CollisionCurPoint"), FVector(VanishCurPointScreen, 1.0f));
	}*/
}