// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "WorldActionList.h"
#include "Actors/ToroActor.h"
#include "WorldActionActor.generated.h"

/**
 * A simple actor that holds FWorldActionList for generic and simple usage.
 */
UCLASS(MinimalAPI, NotBlueprintable, BlueprintType)
class AWorldActionActor final : public AToroActor
{
	GENERATED_BODY()

public:

	AWorldActionActor()
	{
		bEnableAutoLODGeneration = false; // Include Actor in HLOD option
		bStartWithCollision = false;

		SetCanBeDamaged(false);
	}

	/** Executes all actions within the provided ActionList. */
	UFUNCTION(BlueprintCallable, Category = WorldActions)
	TORORUNTIME_API void ExecuteActions()
	{
		if (IsEnabled(this))
		{
			Actions.Execute(this);
		}
	}

	TORORUNTIME_API const FWorldActionList& GetActionList() const
	{
		return Actions;
	}

protected:

	/** The collection of polymorphic actions. Use the '+' button in the editor to select specific action types. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		FWorldActionList Actions;

	virtual void EnableStateChanged(const bool bState) override {}

	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		Actions.Initialize(this);
	}
};