// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "Components/Editor/EdShapeComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#if WITH_EDITOR
#include "Subsystems/UnrealEditorSubsystem.h"
#include "Editor.h"
#endif

UEdShapeComponent::UEdShapeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PrimaryComponentTick.TickInterval = 0.1f;
	bTickInEditor = true;

	bAutoActivate = false;
	bIsEditorOnly = true;
#if WITH_EDITORONLY_DATA
	SetIsVisualizationComponent(true);
#endif
}

#if WITH_EDITOR
void UEdShapeComponent::UpdateNavPoints()
{
	FVector CamPosition = FVector::ZeroVector;
	if (UUnrealEditorSubsystem* System = GEditor ? GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>() : nullptr)
	{
		FRotator Ignored;
		System->GetLevelViewportCameraInfo(CamPosition, Ignored);
	}

	for (TPair<FName, FWireNavPathData>& WirePath : WireNavPaths)
	{
		const int32 NumTargets = WirePath.Value.Targets.Num();
		WirePath.Value.PathPoints.Empty(NumTargets);
		for (int i = 0; i < NumTargets - 1; i++)
		{
			if (const UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this,
				TransformLocation(WirePath.Value.Targets[i]), TransformLocation(WirePath.Value.Targets[i + 1])))
			{
				WirePath.Value.PathPoints.Append(NavPath->PathPoints);
			}
		}

		WirePath.Value.FindNearestPoint(CamPosition);
	}
}

void UEdShapeComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		DestroyComponent();
	});
}

void UEdShapeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	if (!WireNavPaths.IsEmpty() && !FApp::IsGame())
	{
		UpdateNavPoints();
	}
}
#endif