// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerCameraManager.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"

AMyPlayerCameraManager::AMyPlayerCameraManager() {
	LocationThreshold = 1;
	RotationThreshold = 1;
	Mode = Follow;
	PrevPOVPointer = nullptr;
	BlendTime = 3.0f;
}

void AMyPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) {
	FMinimalViewInfo OrigPOV = OutVT.POV;
	TEnumAsByte<enum CameraMode> OrigMode = Mode;

	OutVT.POV.FOV = DefaultFOV;
	OutVT.POV.OrthoWidth = DefaultOrthoWidth;
	OutVT.POV.AspectRatio = DefaultAspectRatio;
	OutVT.POV.bConstrainAspectRatio = bDefaultConstrainAspectRatio;
	OutVT.POV.bUseFieldOfViewForLOD = true;
	OutVT.POV.ProjectionMode = bIsOrthographic ? ECameraProjectionMode::Orthographic : ECameraProjectionMode::Perspective;
	OutVT.POV.PostProcessSettings.SetBaseValues();
	OutVT.POV.PostProcessBlendWeight = 1.0f;
	OutVT.POV.PreviousViewTransform.Reset();

	/*if (GEngine)
	{
		int32 MyKey = 3;
		float TimeToDisplay = 0.5f;
		FColor TextColor = FColor::Red;
		FString VectorString = FString::Printf(TEXT("fov %f  as %f"), DefaultFOV, DefaultAspectRatio);

		FString Message = VectorString;

		GEngine->AddOnScreenDebugMessage(MyKey, TimeToDisplay, TextColor, Message);
	}*/

	if (SequenceMode && DirectorProxy->SequencePath) {
		DirectorProxy->SequenceTime += DeltaTime;
		if (DirectorProxy->SequenceTime > 1) {
			DirectorProxy->SequenceTime = 0;
		}
		FVector CameraPos = DirectorProxy->SequencePath->GetLocationAtTime(DirectorProxy->SequenceTime, ESplineCoordinateSpace::World);
		FVector ViewDirection = DirectorProxy->SequencyTarget->GetComponentLocation() - CameraPos;
		FRotator Rotator = ViewDirection.Rotation();

		OutVT.POV.Location = CameraPos;
		OutVT.POV.Rotation = Rotator;
		Mode = SequencePath;
	}
	else if (SequenceMode && DirectorProxy->SequencePoint) {
		FVector CameraPos = DirectorProxy->SequencePoint->GetComponentLocation();
		FVector ViewDirection = DirectorProxy->SequencyTarget->GetComponentLocation() - CameraPos;
		FRotator Rotator = ViewDirection.Rotation();

		OutVT.POV.Location = CameraPos;
		OutVT.POV.Rotation = Rotator;
		Mode = SequencePoint;
	}
	else if (DirectorProxy && DirectorProxy->UsePathCamera) {
		//preset path
		//FVector PathPos = DirectorProxy->Path->FindLocationClosestToWorldLocation(pos);
		FVector PawnLocation = OutVT.Target->GetActorLocation();
		float PathKey = DirectorProxy->Path->FindInputKeyClosestToWorldLocation(PawnLocation);
		FVector CameraPos = DirectorProxy->CameraPath->GetLocationAtSplineInputKey(PathKey,ESplineCoordinateSpace::World);
		//look at pawn
		FVector ViewCenter = PawnLocation + DirectorProxy->Offset;
		FVector ViewDirection = (ViewCenter - CameraPos).GetSafeNormal();
		FRotator Rotator = ViewDirection.Rotation();

		OutVT.POV.Location = CameraPos;
		OutVT.POV.Rotation = Rotator;

		Mode = Path;
	}
	else if (DirectorProxy && DirectorProxy->UseStaticCamera) {
		//static shot
		FVector CameraPos = DirectorProxy->CameraPoint->GetComponentLocation();
		FVector ViewDirection = DirectorProxy->CameraPoint->GetForwardVector();
		FRotator Rotator = ViewDirection.Rotation();

		OutVT.POV.Location = CameraPos;
		OutVT.POV.Rotation = Rotator;
		Mode = StaticPoint;
	}
	else {
		if (ACameraActor* CamActor = Cast<ACameraActor>(OutVT.Target))
		{
			// Viewing through a camera actor.
			CamActor->GetCameraComponent()->GetCameraView(DeltaTime, OutVT.POV);
		}
		else {
			UpdateViewTargetInternal(OutVT, DeltaTime);
		}
		Mode = Follow;
	}
	if (SwitchMode) {
		SwitchMode = false;
		PrevPOV = OrigPOV;
		PrevPOVPointer = &PrevPOV;
		BlendTimeRemain = 3.0f;

	}
	if (PrevPOVPointer) {
		BlendTimeRemain -= DeltaTime;
		if (BlendTimeRemain < 0) {
			PrevPOVPointer = nullptr;
		}
		else {
			float DurationPct = (BlendTime - BlendTimeRemain) / BlendTime;
			float Pct = FMath::CubicInterp(0.f, 0.f, 1.f, 0.f, DurationPct);
			
			//OutVT.POV.Location = FMath::Lerp(PrevPOV.Location, OutVT.POV.Location, Pct);
			OutVT.POV.BlendViewInfo(PrevPOV, 1-Pct);
		}
	}
	//APlayerCameraManager::UpdateViewTarget(OutVT, DeltaTime);
    //blend view
	/*if (((OrigPOV.Location - OutVT.POV.Location).Size() > LocationThreshold 
		&& (OrigPOV.Location - OutVT.POV.Location).Size() < 2000))
	{
		OutVT.POV.Location = OrigPOV.Location + (OutVT.POV.Location - OrigPOV.Location).GetSafeNormal()* LocationThreshold;

	}

	if (OrigPOV.Rotation.Quaternion().AngularDistance(OutVT.POV.Rotation.Quaternion()) > RotationThreshold
		&& (OrigPOV.Rotation).Quaternion().AngularDistance(OutVT.POV.Rotation.Quaternion()) < 100) 
	{
		FQuat Q1 = OutVT.POV.Rotation.Quaternion();
		FQuat Q2 = OrigPOV.Rotation.Quaternion();
		float Alpha = FMath::Clamp(0.01, 0.0f, 1.0f); 
		FQuat Q3 = FQuat::Slerp(Q2, Q1, Alpha);
		OutVT.POV.Rotation = Q3.Rotator();
	}*/

	bool bDoNotApplyModifiers = false;

	if (!bDoNotApplyModifiers || bAlwaysApplyModifiers)
	{
		// Apply camera modifiers at the end (view shakes for example)
		ApplyCameraModifiers(DeltaTime, OutVT.POV);
	}

	// Synchronize the actor with the view target results
	SetActorLocationAndRotation(OutVT.POV.Location, OutVT.POV.Rotation, false);

	UpdateCameraLensEffects(OutVT);
}

