// Copyright (C) RedCraft86. All Rights Reserved.

#include "Actors/TeleportPoint.h"
#include "Actors/ToroCharacter.h"
#include "Kismet/GameplayStatics.h"
#if WITH_EDITORONLY_DATA
#include "DrawDebugHelpers.h"
#include "Components/BillboardComponent.h"
#include "Components/ArrowComponent.h"
#endif

ATeleportPoint::ATeleportPoint()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	bIsSpatiallyLoaded = false;
	bIsMainWorldOnly = true;

	SetHidden(true);

#if WITH_EDITOR
	if (Icon = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Sprite")); Icon)
	{
		Icon->SetHiddenInGame(true);
		Icon->Sprite = LoadObject<UTexture2D>(nullptr, TEXT("/Engine/EditorResources/S_TargetPoint.S_TargetPoint"));
		Icon->bIsScreenSizeScaled = true;
		Icon->SetRelativeScale3D_Direct(FVector(0.5f, 0.5f, 0.5f));
		Icon->SetupAttachment(GetRootComponent());
	}

	if (Arrow = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow")); Arrow)
	{
		Arrow->SetHiddenInGame(true);
		Arrow->ArrowSize = 0.5f;
		Arrow->bTreatAsASprite = true;
		Arrow->bIsScreenSizeScaled = true;
		Arrow->ArrowColor = FColor(150, 200, 255);
		Arrow->SetupAttachment(Icon ? Icon : GetRootComponent());
		if (Icon)
		{
			Arrow->SetRelativeScale3D(FVector::OneVector / Icon->GetRelativeScale3D());
		}
	}
#endif
}

void ATeleportPoint::TeleportActor(AActor* TargetActor) const
{
	if (!TargetActor) return;
	if (AToroCharacter* AsCharacter = Cast<AToroCharacter>(TargetActor))
	{
		AsCharacter->Teleport(GetActorLocation(), GetActorRotation());
	}
	else
	{
		TargetActor->SetActorLocation(GetActorLocation());
		TargetActor->SetActorRotation(GetActorRotation());
	}
}

void ATeleportPoint::TeleportPlayer(const int32 PlayerIndex) const
{
	TeleportActor(UGameplayStatics::GetPlayerCharacter(this, PlayerIndex));
}

#if WITH_EDITOR
void ATeleportPoint::OffsetFromFloor()
{
	const FVector Start = GetActorLocation();
	const FVector End = Start - FVector(0, 0, 10000.0f);

	FHitResult Hit;
	const FCollisionQueryParams Params(NAME_None, false, this);
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, Params))
	{
		DrawDebugLine(GetWorld(), Start, Hit.Location, FColor::Green, false, 2.0f, 0, 1);
		DrawDebugPoint(GetWorld(), Hit.Location, 10.0f, FColor::Red, false, 2.0f);
		const float DistanceToFloor = Start.Z - Hit.ImpactPoint.Z;
		if (!FMath::IsNearlyEqual(DistanceToFloor, DesiredOffset))
		{
			const float NeededOffset = DesiredOffset - DistanceToFloor;
			SetActorLocation(Start + FVector(0, 0, NeededOffset));
		}
	}
}

void ATeleportPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!bDidOffset)
	{
		bDidOffset = true;
		OffsetFromFloor();
	}

	SetActorScale3D(FVector::OneVector);
	if (FRotator Rotation = GetActorRotation(); !FMath::IsNearlyZero(Rotation.Roll))
	{
		Rotation.Roll = 0.0f;
		SetActorRotation(Rotation);
	}
}
#endif
