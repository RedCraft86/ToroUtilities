// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "Components/ActorComponent.h"
#include "ToroActorComponent.generated.h"

/**
 * Base component with an optional per-actor instance limit.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class TORORUNTIME_API UToroActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UToroActorComponent();

	/**
	 * Maximum number of components of this class allowed on one actor.
	 * A value of zero disables the limit.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Activation)
		uint8 MaxInstancesPerActor;

protected:

	virtual void OnRegister() override;
};
