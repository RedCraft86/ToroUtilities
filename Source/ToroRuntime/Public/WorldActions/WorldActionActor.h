// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "WorldActionList.h"
#include "Actors/ToroActor.h"
#include "WorldActionActor.generated.h"

/**
 * A simple actor that holds FWorldActionList for generic and simple usage.
 */
UCLASS(NotBlueprintable, BlueprintType)
class TORORUNTIME_API AWorldActionActor final : public AToroActor
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
	void ExecuteActions() const
	{
		if (IsEnabled(this))
		{
			Actions.Execute(this);
		}
	}

	const FWorldActionList& GetActionList() const { return Actions; }

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