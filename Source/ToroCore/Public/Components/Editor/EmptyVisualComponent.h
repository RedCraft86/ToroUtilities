// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "Components/ActorComponent.h"
#include "EmptyVisualComponent.generated.h"

/**
 * Component to conditionally show an editor-only actor icon based on whether the owner has
 * any other visual indicators or if it can no longer be observed if placed into a level.
 * @note This component is editor-only and will be removed at game runtime.
 */
UCLASS(MinimalAPI, NotBlueprintable, NotBlueprintType, ClassGroup = (Editor), meta = (BlueprintSpawnableComponent))
class UEmptyVisualComponent final : public USceneComponent
{
	GENERATED_BODY()

public:

	UEmptyVisualComponent();

	UPROPERTY(EditAnywhere, Category = Settings)
		TSoftObjectPtr<UTexture2D> Sprite;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Scale;

	UPROPERTY(VisibleAnywhere, Category = Settings, meta = (ClampMin = 1, UIMin = 1))
		TArray<TWeakObjectPtr<const USceneComponent>> IgnoreComponents;

private:

	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
	virtual bool CanAttachAsChild(const USceneComponent* ChildComponent, FName SocketName) const override;
#endif
};
