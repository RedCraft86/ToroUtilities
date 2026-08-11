// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "Components/Editor/EmptyVisualComponent.h"
#if WITH_EDITOR
#include "Components/BillboardComponent.h"
#endif

UEmptyVisualComponent::UEmptyVisualComponent()
{
#if WITH_EDITOR
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PrimaryComponentTick.TickInterval = 0.5f;
	bTickInEditor = true;
#else
	PrimaryComponentTick.bCanEverTick = false;
#endif

#if WITH_EDITOR
	SetVisibility(true);
	SetIsVisualizationComponent(true);
	bVisualizeComponent = true;
#else
	SetVisibility(false, true);
#endif
}

#if WITH_EDITOR
void UEmptyVisualComponent::BeginPlay()
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

void UEmptyVisualComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	if (SpriteComponent)
	{
		SpriteComponent->SetVisibility(false);

		UTexture2D* SpriteImage = Sprite.LoadSynchronous();
		if (!IsVisible() || !SpriteImage)
		{
			return;
		}

		bool bShowVisual = true;
		TArray<USceneComponent*> SceneComponents;
		GetOwner()->GetComponents<USceneComponent>(SceneComponents);
		for (const USceneComponent* Component : SceneComponents)
		{
			if (Component == this || Component == SpriteComponent || IgnoreComponents.Contains(Component))
			{
				continue;
			}

			if (Component->bVisualizeComponent || Component->IsA<UPrimitiveComponent>())
			{
				bShowVisual = false;
				break;
			}
		}

		if (bShowVisual)
		{
			SpriteComponent->SetVisibility(true);
			SpriteComponent->SetRelativeScale3D(FVector(Scale));
			if (SpriteComponent->Sprite != SpriteImage)
			{
				SpriteComponent->SetSprite(SpriteImage);
			}
		}
	}
}

bool UEmptyVisualComponent::CanAttachAsChild(const USceneComponent* ChildComponent, FName SocketName) const
{
	return ChildComponent == SpriteComponent;
}
#endif
