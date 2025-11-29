// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Interaction/InteractionMarker.h"
#include "Kismet/GameplayStatics.h"
#if WITH_EDITOR
#include "Subsystems/UnrealEditorSubsystem.h"
#endif

UInteractionMarker::UInteractionMarker()
	: MaxDistance(250.0f), ScaleSpeed(10.0f), BaseSize(1.5f)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PrimaryComponentTick.TickInterval = 0.1f;
	
	SetHiddenInGame(false);
	bIsScreenSizeScaled = true;
	OpacityMaskRefVal = 0.4f;
	ScreenSize = 0.0025f;

#if WITH_EDITOR
	bTickInEditor = true;
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
	SetWorldScale3D(BaseSize);
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		CamManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	});
}

void UInteractionMarker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!GetOwner()->IsHidden())
	{
		SetVisibility(FVector::Dist(GetComponentLocation(), GetCameraPos()) <= MaxDistance);
	}
}

