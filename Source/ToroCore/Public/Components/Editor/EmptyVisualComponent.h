// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "Components/ActorComponent.h"
#include "EmptyVisualComponent.generated.h"

/**
 * Component to conditionally show an editor-only billboard if the owning actor has no other visual components.
 * @note This component is editor-only and will be removed at game runtime.
 */
UCLASS(MinimalAPI, NotBlueprintable, NotBlueprintType, ClassGroup = (Editor), meta = (BlueprintSpawnableComponent))
class UEmptyVisualComponent final : public USceneComponent
{
	GENERATED_BODY()

public:

	UEmptyVisualComponent();

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Settings)
		TSoftObjectPtr<UTexture2D> Sprite { FSoftObjectPath(TEXT("/Engine/EditorResources/EmptyActor.EmptyActor")) };

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Scale = 0.5f;

	UPROPERTY(VisibleAnywhere, Category = Settings, meta = (ClampMin = 1, UIMin = 1))
		TArray<TWeakObjectPtr<const USceneComponent>> IgnoreComponents;
#endif

#if WITH_EDITOR
private:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
	virtual bool CanAttachAsChild(const USceneComponent* ChildComponent, FName SocketName) const override;
#endif
};
