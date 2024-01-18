// Fill out your copyright notice in the Description page of Project Settings.


#include "horse_spline_view.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "hookPoint.h"
#include "Particles/ParticleSystemComponent.h"
#include "ParticleEmitterInstances.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Public/DirectorProxy.h"
#include "Public/MyPlayerController.h"
#include "Public/MyPlayerCameraManager.h"


// Sets default values
Ahorse_spline_view::Ahorse_spline_view()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &Ahorse_spline_view::OnOverlapBegin);

	//注意有没有注释掉参数初始化...
	delta = 0.01;
	RotateSpeed = 10;
	MoveSpeed = 3;
	Tolerance = 1000;

	RaceStep = 1;
	RaceMAX = 6;

	IsMoving = false;

	RadiusDetection = 500;
	bDebug = false;

	InteractMode = "RayDetermine";
	bIsHookShotActive = false;

	BestHookPoint_tem = nullptr;

	HookSpeed = 1000.0f;
	FallSpeed = 1000.0f;

	RaceTimer = 0.0f;

}

// Called when the game starts or when spawned
void Ahorse_spline_view::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	//AnimPlayRate = 0;

}

// Called every frame
void Ahorse_spline_view::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> FoundActors;
	FVector CharacterLocation = GetActorLocation();
	float DetectionRadius = 1000.0f; 

	//获取所有钩锁类
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AhookPoint::StaticClass(), FoundActors);

	FVector ForwardVector = GetActorForwardVector();
	TArray<AActor*> VisibleActors;
	float AngleThreshold = 45.0f; // 你可以调整这个值

	//获取符合条件
	for (AActor* Actor : FoundActors)
	{
		FVector DirectionToHookPoint = Actor->GetActorLocation() - CharacterLocation;
		DirectionToHookPoint.Normalize();
		float Angle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(ForwardVector, DirectionToHookPoint)));

		FVector HookPointLocation = Actor->GetActorLocation();
		FHitResult HitResult;
		if (Angle <= AngleThreshold 
			&& !GetWorld()->LineTraceSingleByChannel(HitResult, CharacterLocation, HookPointLocation, ECC_Visibility))
		{
			VisibleActors.Add(Actor);
		}
	}

	AActor* BestHookPoint = nullptr;
	float BestDistance = FLT_MAX;
	float BestAngle = FLT_MAX;

	//获得最佳点
	for (AActor* Actor : VisibleActors)
	{
		FVector DirectionToHookPoint = Actor->GetActorLocation() - CharacterLocation;
		float Distance = DirectionToHookPoint.Size();
		DirectionToHookPoint.Normalize();
		float Angle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(ForwardVector, DirectionToHookPoint)));

		if (Angle < BestAngle || (Angle == BestAngle && Distance < BestDistance))
		{
			BestHookPoint = Actor;
			BestDistance = Distance;
			BestAngle = Angle;
		}
	}

	

	if (BestHookPoint_tem) {
		//去除上一帧
		if (Cast<AhookPoint>(BestHookPoint_tem)->WidgetInstance)
		{
			Cast<AhookPoint>(BestHookPoint_tem)->RemoveMyWidgetFromViewport();
		}
	}

	if (BestHookPoint != nullptr)
	{
		Cast<AhookPoint>(BestHookPoint)->ShowHookUI(GetController<APlayerController>());
		BestHookPoint_tem = BestHookPoint;
		showUI = true;
	}
	
	//hook shot
	if (bIsHookShotActive)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector Direction = (HookShotTarget - CurrentLocation).GetSafeNormal();
		FVector NewLocation = CurrentLocation + Direction * HookSpeed * DeltaTime;

		if (FVector::Dist(NewLocation, HookShotTarget) < 100.0f) // 这个值也可以调整
		{
			bIsHookShotActive = false;
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			GetCharacterMovement()->GravityScale = 1;
			ApplyInitialFallVelocity();
		}
		else
		{
			SetActorLocation(NewLocation);
		}
	}

	if (RaceTimer > 0) {
		RaceTimer -= DeltaTime;
		if (RaceTimer < 0) {
			SpeedLevel -= 1;
			if (SpeedLevel > 1) {
				RaceTimer = 2.0f;
			}
		}
	}
}

void Ahorse_spline_view::ApplyInitialFallVelocity()
{
	FVector CurrentLocation = GetActorLocation();
	FVector Direction = (HookShotTarget - CurrentLocation).GetSafeNormal();
	FVector FallVelocity = Direction* FallSpeed;
	GetCharacterMovement()->Velocity = FallVelocity;

}

//碰撞检测
void Ahorse_spline_view::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, 
					AActor* OtherActor, UPrimitiveComponent* OtherComp, 
					int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor->IsA(ADirectorProxy::StaticClass()))
	{
		ADirectorProxy* director = Cast<ADirectorProxy>(OtherActor);
		CurPath = director->Path;
		AMyPlayerController* PC = GetController<AMyPlayerController>();
		AMyPlayerCameraManager* CameraManager = nullptr;
		if (PC) {
			CameraManager = Cast<AMyPlayerCameraManager>(PC->PlayerCameraManager);
		}
		if (CameraManager) {
			CameraManager->DirectorProxy = director;
			CameraManager->SwitchMode = true;
			//CameraManager->SequenceMode = true;
		}

		auto ConfigCamera = [this](float SpringArmLength, FVector SocketOffset,float FieldOfView,float AspectRatio) {
			//config camera boom
			CameraBoom->TargetArmLength = SpringArmLength;
			CameraBoom->SocketOffset = SocketOffset;

			//camera
			FollowCamera->FieldOfView = FieldOfView;
			FollowCamera->AspectRatio = AspectRatio;
		};
		
		if (director->ConfigCamera) {
			ConfigCamera(director->SpringArmLength, director->SocketOffset, director->FieldOfView, director->AspectRatio);
		}
		else if(CameraManager) {
			ConfigCamera(400.0f, FVector(0.0f, 0.0f, 0.0f), CameraManager->DefaultFOV, CameraManager->DefaultAspectRatio);
		}

	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void Ahorse_spline_view::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &Ahorse_spline_view::Move);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &Ahorse_spline_view::EndMove);

		EnhancedInputComponent->BindAction(RaceAction, ETriggerEvent::Started, this, &Ahorse_spline_view::Race);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &Ahorse_spline_view::Interact);
		EnhancedInputComponent->BindAction(HookShotAction, ETriggerEvent::Triggered, this, &Ahorse_spline_view::HookShot);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &Ahorse_spline_view::Look);
	}

}

void Ahorse_spline_view::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	bool IsActorRotation = true;
	if (Controller != nullptr)
	{
		const FRotator ActorRotation = GetActorRotation();
		const FRotator ActorYaw(0, ActorRotation.Yaw, 0);
		const FVector ActorForward = FRotationMatrix(ActorYaw).GetUnitAxis(EAxis::X);
		const FVector ActorRight = FRotationMatrix(ActorYaw).GetUnitAxis(EAxis::Y);

		AActor* CameraManager = nullptr;
		AMyPlayerController* PC = GetController<AMyPlayerController>();
		if (PC && Cast<AMyPlayerCameraManager>(PC->PlayerCameraManager)) {
			CameraManager = Cast<AMyPlayerCameraManager>(PC->PlayerCameraManager);
		}
		FRotator CameraRotation;
		if (CameraManager) {
			CameraRotation = CameraManager->GetActorRotation();
		}
		else {
			CameraRotation = FollowCamera->GetComponentRotation();
		}
		FRotator CameraYaw(0, CameraRotation.Yaw, 0);
		FVector ForwardDirection = FRotationMatrix(CameraYaw).GetUnitAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(CameraYaw).GetUnitAxis(EAxis::Y);


		const FVector MoveDirection = ActorForward;

		const FVector RotateForward = ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X;
		if (SpeedLevel == 0) {
			SpeedLevel = 1;
		}
		float ResSpeed = SpeedLevel * 2 - 1 + (SpeedLevel > 3 ? SpeedLevel * 4 : 0);
		AddMovementInput(ActorForward, MoveSpeed * ResSpeed);

		if (GEngine)
		{
			int32 MyKey = 5;
			float TimeToDisplay = 3.0f;
			FColor TextColor = FColor::Red;
			FString VectorString = FString::Printf(TEXT("speed level:%f res:%f"), SpeedLevel, ResSpeed);

			FString Message = VectorString;

			GEngine->AddOnScreenDebugMessage(MyKey, TimeToDisplay, TextColor, Message);
		}

		SetActorRotation(FMath::RInterpTo(GetActorRotation(), RotateForward.Rotation(), delta, RotateSpeed));
		TurnAngle = RotateForward.Rotation().Yaw - GetActorRotation().Yaw;
		if (TurnAngle > 180) {
			TurnAngle = TurnAngle - 360;
		}
		else if (TurnAngle < -180) {
			TurnAngle += 360;
		}

		IsMoving = true;
	}
}

void Ahorse_spline_view::EndMove(const FInputActionValue& Value) {
	IsMoving = false;
	TurnAngle = 0;
	SpeedLevel = 0;
}

void Ahorse_spline_view::Race(const FInputActionValue& Value)
{
	FVector2D AxisVector = Value.Get<FVector2D>();
	if (SpeedLevel != 0) {
		SpeedLevel += 1;
	}
	if (SpeedLevel > 5) {
		SpeedLevel = 5;
	}
	RaceTimer = 2.0f;
}

void Ahorse_spline_view::HookShot(const FInputActionValue& Value)
{
	if (BestHookPoint_tem)
	{
		GetCharacterMovement()->GravityScale = 0;
		bIsHookShotActive = true;
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		HookShotTarget = Cast<AhookPoint>(BestHookPoint_tem)->GetTargetPosition();
	}
}

void Ahorse_spline_view::Interact(const FInputActionValue& Value)
{
	DekeyInteract("Dekey", InteractMode);
}

void Ahorse_spline_view::DekeyInteract(const FString& Tag,const FString& Condition)
{
	if (Condition == "RayDetermine")
	{
		PerformRaycastDetection(Tag);
	}
	else if (Condition == "RadiusDetermine")
	{
		PerformRadiusDetection(Tag);
	}
	else if (Condition == "OverlapDetermine")
	{
		PerformOverlapDetection(Tag);
	}
}

void Ahorse_spline_view::PerformRaycastDetection(const FString& Tag)
{
	FVector Start = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector(); 
	FVector End = ((ForwardVector * 1000.f) + Start); 
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); 

	// 执行射线检测
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldDynamic/*ECC_Visibility*/, CollisionParams))
	{
		// 如果射线击中了某个物体
		AActor* HitActor = HitResult.GetActor();
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);
		if (HitActor != nullptr && HitActor->Tags.Contains(FName(*Tag)))
		{
			IDekeyInteractableInterface* Interactable = Cast<IDekeyInteractableInterface>(HitActor);
			if (Interactable)
			{
				Interactable->OnInteract(this);
			}
		}
	}

	if (bDebug)
	{
		//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);
	}
}

void Ahorse_spline_view::PerformRadiusDetection(const FString& Tag)
{
	// 获取角色周围的所有物体
	TArray<FOverlapResult> OverlapResults;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(RadiusDetection); // 设置检测半径

	// 执行重叠检测
	if (GetWorld()->OverlapMultiByChannel(OverlapResults, GetActorLocation(), FQuat::Identity, ECC_WorldStatic, CollisionShape))
	{
		for (auto& Result : OverlapResults)
		{
			AActor* OverlappedActor = Result.GetActor();
			if (OverlappedActor && OverlappedActor->Tags.Contains(FName(*Tag)))
			{
				IDekeyInteractableInterface* Interactable = Cast<IDekeyInteractableInterface>(OverlappedActor);
				if (Interactable)
				{
					Interactable->OnInteract(this);
				}
			}
		}
	}
}

void Ahorse_spline_view::PerformOverlapDetection(const FString& Tag)
{
	UPrimitiveComponent* CollisionComponent = GetCapsuleComponent();
	if (!CollisionComponent) return;

	TArray<AActor*> OverlappedActors;

	CollisionComponent->GetOverlappingActors(OverlappedActors);

	for (AActor* Actor : OverlappedActors)
	{
		if (Actor && Actor->Tags.Contains(FName(*Tag)))
		{
			IDekeyInteractableInterface* Interactable = Cast<IDekeyInteractableInterface>(Actor);
			if (Interactable)
			{
				Interactable->OnInteract(this);
			}
		}
	}
}

void Ahorse_spline_view::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

