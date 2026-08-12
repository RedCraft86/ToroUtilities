// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "Components/Editor/WireShapeComponent.h"
#if WITH_EDITOR
#include "Libraries/ToroCameraLibrary.h"
#endif

UWireShapeComponent::UWireShapeComponent()
{
#if WITH_EDITOR
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PrimaryComponentTick.TickInterval = 0.1f;
	bTickInEditor = true;

	SetIsVisualizationComponent(true);
#else
	PrimaryComponentTick.bCanEverTick = false;
#endif
}

#if WITH_EDITOR
void UWireShapeComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
	GetWorld()->GetTimerManager().SetTimerForNextTick([WeakThis = TWeakObjectPtr(this)]()
	{
		if (WeakThis.IsValid())
		{
			WeakThis->DestroyComponent();
		}
	});
}

void UWireShapeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	if (!WireNavPaths.IsEmpty() && !FApp::IsGame())
	{
		for (TPair<FName, FWireNavPathData>& NavPath : WireNavPaths)
		{
			NavPath.Value.GeneratePathPoints(GetOwner());
			NavPath.Value.FindNearestPoint(UToroCameraLibrary::GetViewTransform(this));
		}
	}
}
#endif