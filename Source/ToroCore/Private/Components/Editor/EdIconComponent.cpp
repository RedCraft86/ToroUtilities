// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "Components/Editor/EdIconComponent.h"
#include "TimerManager.h"

UEdIconComponent::UEdIconComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PrimaryComponentTick.TickInterval = 0.25f;
	bTickInEditor = true;
	SetHiddenInGame(true);

	bAutoActivate = false;
	bIsEditorOnly = true;
#if WITH_EDITORONLY_DATA
	SetIsVisualizationComponent(true);
#endif

	UpdateSize();
	UpdateIcon();
}

#if WITH_EDITOR
void UEdIconComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		DestroyComponent();
	});
}

void UEdIconComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	if (MaxComps <= 0 || FApp::IsGame())
	{
		SetVisibility(false);
		return;
	}

	uint8 Count = 0;
	TArray<USceneComponent*> Components;
	GetOwner()->GetComponents<USceneComponent>(Components);
	for (const USceneComponent* Comp : Components)
	{
		if (Comp && Comp != this && (Comp->IsA<UPrimitiveComponent>() || Comp->bVisualizeComponent))
		{
			if (++Count > MaxComps) break;
		}
	}

	SetVisibility(Count < MaxComps);
}
#endif
