// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Interaction/InteractionMarker.h"
#include "Kismet/GameplayStatics.h"
#if WITH_EDITOR
#include "Subsystems/UnrealEditorSubsystem.h"
#endif

UInteractionMarker::UInteractionMarker()
	: MaxDistance(400.0f), ScaleSpeed(5.0f), BaseSize(0.75f), TickTime(0.1f)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PrimaryComponentTick.TickInterval = 0.05f;
	
	SetHiddenInGame(false);
	bIsScreenSizeScaled = false;

#if WITH_EDITOR
	bTickInEditor = false;
	const ConstructorHelpers::FObjectFinder<UTexture2D> SpriteFinder(
		TEXT("/ToroUtilities/Assets/T_Interactable.T_Interactable"));
	if (SpriteFinder.Succeeded())
	{
		UBillboardComponent::SetSprite(SpriteFinder.Object);
	}
#endif
}

void UInteractionMarker::ResetScale()
{
	SetWorldScale3D(BaseSize);
}

void UInteractionMarker::CheckDistance()
{
	const bool bNearby = FVector::Dist(GetComponentLocation(), GetCameraPos()) <= MaxDistance;
	SizeInterp.Target = bNearby ? BaseSize : FVector::ZeroVector;
}

FVector UInteractionMarker::GetCameraPos() const
{
	FVector Position = GetComponentLocation();
#if WITH_EDITOR
	if (!FApp::IsGame())
	{
		if (!GEditor) return Position;
		if (UUnrealEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>())
		{
			FRotator Rotation;
			Subsystem->GetLevelViewportCameraInfo(Position, Rotation);
		}
	}
	else
#endif
		if (CamManager)
		{
			Position = CamManager->GetCameraLocation();
		}

	return Position;
}

void UInteractionMarker::BeginPlay()
{
	Super::BeginPlay();
	SizeInterp.bConstant = true;
	SizeInterp.Speed = ScaleSpeed;
	SizeInterp.Target = BaseSize;
	SizeInterp.SnapToTarget();

	SetWorldScale3D(SizeInterp.Current);
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		CamManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	});
}

void UInteractionMarker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!bHiddenInGame && !GetOwner()->IsHidden())
	{
		if (TickTime < 0.0f)
		{
			TickTime = 0.1f + DeltaTime;
			CheckDistance();
		}
		TickTime -= DeltaTime;

		if (!SizeInterp.IsComplete())
		{
			SetWorldScale3D(SizeInterp.Tick(DeltaTime));
			MarkRenderStateDirty();
		}
	}
}

