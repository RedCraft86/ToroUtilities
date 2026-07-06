// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "Framework/ToroPlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "ConvexVolume.h"

void ConstructQueryParams(FCollisionQueryParams& QueryParams, const APlayerCameraManager* Manager,
	const TFunction<void(FCollisionQueryParams&)>& ModifyFunc)
{
	QueryParams.AddIgnoredActor(Manager);
	QueryParams.AddIgnoredActor(Manager->GetOwningPlayerController());
	QueryParams.AddIgnoredActor(Manager->GetViewTarget());
	if (ModifyFunc)
	{
		ModifyFunc(QueryParams);
	}
}

AToroPlayerCameraManager::AToroPlayerCameraManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	bEnableAutoLODGeneration = false; // Include Actor in HLOD option

	SetCanBeDamaged(false);
}

bool AToroPlayerCameraManager::LineTraceSingleFromView(FHitResult& HitResult, const FVector& Target,
	const ECollisionChannel TraceChannel, const TFunction<void(FCollisionQueryParams&)>& ModifyParams) const
{
	FCollisionQueryParams QueryParams;
	ConstructQueryParams(QueryParams, this, ModifyParams);
	return GetWorld()->LineTraceSingleByChannel(HitResult, GetCameraLocation(), Target, TraceChannel, QueryParams);
}

bool AToroPlayerCameraManager::LineTraceMultiFromView(TArray<FHitResult>& HitResults, const FVector& Target,
	const ECollisionChannel TraceChannel, const TFunction<void(FCollisionQueryParams&)>& ModifyParams) const
{
	FCollisionQueryParams QueryParams;
	ConstructQueryParams(QueryParams, this, ModifyParams);
	return GetWorld()->LineTraceMultiByChannel(HitResults, GetCameraLocation(), Target, TraceChannel, QueryParams);
}

bool AToroPlayerCameraManager::IsActorSeen(AActor* Target, const float BoxScale, const uint8 MaxSamples) const
{
	if (!Target || BoxScale < 0.2f)
	{
		return false;
	}

	FMatrix ViewMatrix, ProjectionMatrix, ViewProjectionMatrix;
	UGameplayStatics::GetViewProjectionMatrix(GetCameraCacheView(), 
		ViewMatrix, ProjectionMatrix, ViewProjectionMatrix);

	FConvexVolume Frustum;
	GetViewFrustumBounds(Frustum, ViewProjectionMatrix, true);

	FVector Origin, Extent;
	Target->GetActorBounds(false, Origin, Extent, true);
	if (!Frustum.IntersectBox(Origin, Extent * BoxScale))
	{
		return false;
	}

	if (MaxSamples == 0)
	{
		FHitResult Hit;
		return !LineTraceSingleFromView(Hit, Origin, ECC_Visibility, [Target](FCollisionQueryParams& Params)
		{
			Params.AddIgnoredActor(Target);
		});
	}

	const FBox ActorBox = FBox::BuildAABB(Origin, Extent * BoxScale);
	for (int32 i = 0; i < MaxSamples; i++)
	{
		FHitResult Hit;
		if (!LineTraceSingleFromView(Hit, FMath::RandPointInBox(ActorBox), ECC_Visibility, 
			[Target](FCollisionQueryParams& Params)
			{
				Params.AddIgnoredActor(Target);
			}))
		{
			return true;
		}
	}

	return false;
}

void AToroPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();

	// Ensure each map starts on a black screen for any deferred loading
	SetManualCameraFade(1.0f, FLinearColor::Black, true);

	// Fixes MotionBlur and AA issues when camera is moved while paused
	GetWorld()->bIsCameraMoveableWhenPaused = true;
}

void AToroPlayerCameraManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Allows photomode-like systems to move the camera even when game is paused
	if (GetWorld()->IsPaused() && GetTickableWhenPaused())
	{
		UpdateCamera(DeltaSeconds);
	}
}
