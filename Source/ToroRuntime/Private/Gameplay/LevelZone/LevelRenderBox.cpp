// Copyright (C) RedCraft86. All Rights Reserved.

#include "LevelZone/LevelRenderBox.h"
#include "EngineUtils.h"

ALevelRenderBox::ALevelRenderBox()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.05f;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	CubeComponent = CreateDefaultSubobject<UStaticMeshComponent>("CubeComponent");
	CubeComponent->SetupAttachment(RootComponent);
	
	CubeComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CubeComponent->SetStaticMesh(LoadObject<UStaticMesh>(nullptr,
		TEXT("/Engine/BasicShapes/Cube.Cube")));
#if WITH_EDITOR
	CubeComponent->SetMaterial(0, LoadObject<UMaterialInterface>(nullptr,
		TEXT("/ToroUtilities/Assets/Procedural/M_ProcBarrier_01.M_ProcBarrier_01")));
	CubeComponent->SetOverlayMaterial(LoadObject<UMaterialInterface>(nullptr,
		TEXT("/ToroUtilities/Assets/Procedural/M_ProcBarrier_02.M_ProcBarrier_02")));
#endif
	
	bStartWithCollision = false;
	ClearMaterial = LoadObject<UMaterialInterface>(nullptr,
		TEXT("/ToroUtilities/Assets/M_Clear.M_Clear"));
}

#if WITH_EDITOR
void ALevelRenderBox::FindTargets()
{
	if (FindTag.IsNone())
	{
		return;
	}

	FVector Origin, Extent;
	GetActorBounds(false, Origin, Extent, false);
	const FBox Box(Origin - Extent, Origin + Extent);
	
	for (const TObjectPtr<AActor> Actor : TActorRange<AActor>(GetWorld()))
	{
		if (Box.IsInside(Actor->GetActorLocation()) && Actor->ActorHasTag(FindTag))
		{
			Targets.Add(Actor);
		}
	}
}
#endif

void ALevelRenderBox::BeginPlay()
{
	CullComponents.Empty(Targets.Num());
	for (auto It = Targets.CreateIterator(); It; ++It)
	{
		if (const AActor* Actor = It->LoadSynchronous())
		{
			if (UCullingComponent* Comp = UCullingComponent::Get(Actor))
			{
				CullComponents.Add(Comp);
				It.RemoveCurrent();
			}
		}
		else
		{
			It.RemoveCurrent();
		}
	}
	Super::BeginPlay();

	CubeComponent->SetOverlayMaterial(nullptr);
	CubeComponent->SetMaterial(0, ClearMaterial);
}

void ALevelRenderBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!HasActorBegunPlay() || !IsEnabled())
	{
		return;
	}

	const bool bRendered = WasRecentlyRendered();
	for (TSoftObjectPtr<AActor>& Actor : Targets)
	{
		if (AActor* ActorPtr = Actor.LoadSynchronous())
		{
			ActorPtr->SetActorHiddenInGame(!bRendered);
		}
	}
	for (TWeakObjectPtr<UCullingComponent>& Comp : CullComponents)
	{
		if (Comp.IsValid())
		{
			bRendered ? Comp->AddRenderRequest(this) : Comp->RemoveRenderRequest(this); 
		}
	}
}

