// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Miscellaneous/TeleportPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/ToroCharacter.h"
#include "Engine/World.h"
#if WITH_EDITOR
#include "DrawDebugHelpers.h"
#include "Components/BillboardComponent.h"
#include "Components/ArrowComponent.h"
#endif

ATeleportPoint::ATeleportPoint()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

#if WITH_EDITOR
	if (UBillboardComponent* Sprite = GetSpriteComponent())
	{
		Sprite->bIsEditorOnly = true;
		Sprite->SetIsVisualizationComponent(true);
		Sprite->SetRelativeScale3D_Direct(FVector(0.5f, 0.5f, 0.5f));
	}

	if (UArrowComponent* Arrow = GetArrowComponent())
	{
		Arrow->ArrowSize = 1.0f;
		Arrow->bIsEditorOnly = true;
		Arrow->bTreatAsASprite = true;
		Arrow->SetIsVisualizationComponent(true);
	}
#endif

#if WITH_EDITORONLY_DATA
	bIsMainWorldOnly = true;
	bIsSpatiallyLoaded = false;
#endif

	bEnableAutoLODGeneration = false; // Include Actor in HLOD option

	SetCanBeDamaged(false);
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
		TargetActor->TeleportTo(GetActorLocation(), GetActorRotation(), false, true);
	}
}

void ATeleportPoint::TeleportPlayer(const int32 PlayerIndex) const
{
	TeleportActor(UGameplayStatics::GetPlayerPawn(this, PlayerIndex));
}

#if WITH_EDITOR
void ATeleportPoint::OffsetFromFloor()
{
	const FVector CurrentPos = GetActorLocation();
	const FVector End = CurrentPos - FVector(0, 0, 10000.0f);

	FHitResult Hit;
	const FCollisionQueryParams Params(NAME_None, false, this);
	if (GetWorld()->LineTraceSingleByChannel(Hit, CurrentPos, End, ECC_WorldStatic, Params))
	{
		DrawDebugLine(GetWorld(), CurrentPos, Hit.Location, FColor::Green, false, 2.0f, 0, 1);
		DrawDebugPoint(GetWorld(), Hit.Location, 10.0f, FColor::Red, false, 2.0f);
		const float FloorDistance = CurrentPos.Z - Hit.ImpactPoint.Z;
		if (!FMath::IsNearlyEqual(FloorDistance, FloorOffset))
		{
			const float NeededOffset = FloorOffset - FloorDistance;
			SetActorLocation(CurrentPos + FVector(0, 0, NeededOffset));
			DrawDebugLine(GetWorld(), CurrentPos, 
				CurrentPos + FVector(0, 0, NeededOffset), 
				FColor::Yellow, false, 2.0f, 0, 1
			);
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