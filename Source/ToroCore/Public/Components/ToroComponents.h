// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "ToroComponents.generated.h"

/**
 * An enhanced base class for non-spatial Actor Components.
 * Adds a "ConstructionScript" event that fires during editor property changes and before begin play,
 * allowing for logic that executes both in-editor and at runtime when the actor is spawned.
 */
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TOROCORE_API UToroActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UToroActorComponent();

	/** 
	 * Event called during editor property changes and before begin play. 
	 * Useful for initializing component state based on user-editable properties.
	 */
	UFUNCTION(BlueprintImplementableEvent, meta = (ForceAsFunction = true))
		void ConstructionScript();

protected:

	virtual void OnConstruction() { ConstructionScript(); }

	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};

/**
 * An enhanced base class for spatial Scene Components.
 * Adds a "ConstructionScript" event that fires during editor property changes and before begin play,
 * allowing for logic that executes both in-editor and at runtime when the actor is spawned.
 */
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TOROCORE_API UToroSceneComponent : public USceneComponent
{
	GENERATED_BODY()

public:

	UToroSceneComponent();

	/** 
	 * Event called during editor property changes and before begin play. 
	 * Useful for initializing component state based on user-editable properties.
	 */
	UFUNCTION(BlueprintImplementableEvent, meta = (ForceAsFunction = true))
		void ConstructionScript();

protected:

	virtual void OnConstruction() { ConstructionScript(); }

	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
