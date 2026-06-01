// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Engine/StaticMeshActor.h"
#include "WorldActions/WorldActionActor.h"
#include "ActorActions.generated.h"

/** Sets or toggles the 'Hidden in Game' state for a collection of actors. */
UCLASS(NotBlueprintable, DisplayName = "[Actor] Change Visibility")
class TORORUNTIME_API UWorldAction_Visibility final : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_Visibility()
		: Type(EWorldActionApplyType::Set), bVisible(false)
	{}

private:

	/** Whether visibility should be set to a value or toggled relative to the current value. */
	UPROPERTY(EditAnywhere, Category = Action)
		EWorldActionApplyType Type;

	/** The visibility state to apply (only used when Type is 'Set'). */
	UPROPERTY(EditAnywhere, Category = Action, meta = (EditCondition = "Type == EWorldActionApplyType::Set", EditConditionHides))
		bool bVisible;

	/** The actors whose visibility will be modified. */
	UPROPERTY(EditAnywhere, Category = Action)
		TSet<TSoftObjectPtr<AActor>> Targets;

	virtual void OnExecute_Implementation() override;
};

/** Enables or disables collision for a collection of actors. */
UCLASS(NotBlueprintable, DisplayName = "[Actor] Enable Collision")
class TORORUNTIME_API UWorldAction_Collision final : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_Collision()
		: Type(EWorldActionApplyType::Set), bCollisionEnabled(false)
	{}

private:

	/** Whether visibility should be set to a value or toggled relative to the current value. */
	UPROPERTY(EditAnywhere, Category = Action)
		EWorldActionApplyType Type;

	/** The collision state to apply (only used when Type is 'Set'). */
	UPROPERTY(EditAnywhere, Category = Action, meta = (EditCondition = "Type == EWorldActionApplyType::Set", EditConditionHides))
		bool bCollisionEnabled;

	/** The actors whose collision state will be modified. */
	UPROPERTY(EditAnywhere, Category = Action)
		TSet<TSoftObjectPtr<AActor>> Targets;

	virtual void OnExecute_Implementation() override;
};

/** Toggles the custom 'Enabled' state for actors implementing IEnabledState. */
UCLASS(NotBlueprintable, DisplayName = "[Actor] Enabled State")
class TORORUNTIME_API UWorldAction_ActorEnabled final : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_ActorEnabled()
		: Type(EWorldActionApplyType::Set), bEnabled(false)
	{}

private:

	/** Whether visibility should be set to a value or toggled relative to the current value. */
	UPROPERTY(EditAnywhere, Category = Action)
		EWorldActionApplyType Type;

	/** The enabled state to apply (only used when Type is 'Set'). */
	UPROPERTY(EditAnywhere, Category = Action, meta = (EditCondition = "Type == EWorldActionApplyType::Set", EditConditionHides))
		bool bEnabled;

	/** Target actors. Restricted to those that implement the EnabledState interface. */
	UPROPERTY(EditAnywhere, Category = Action, meta = (MustImplement = "/Script/ToroCore.EnabledState"))
		TSet<TSoftObjectPtr<AActor>> Targets;

	virtual void OnExecute_Implementation() override;
};

/** Dynamically adds or removes gameplay tags (FName) on a collection of actors. */
UCLASS(NotBlueprintable, DisplayName = "[Actor] Apply Tags")
class TORORUNTIME_API UWorldAction_ActorTags final : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_ActorTags() {}

private:

	/** Tags to append to the targets. */
	UPROPERTY(EditAnywhere, Category = Action)
		TSet<FName> AddTags;

	/** Tags to strip from the targets. */
	UPROPERTY(EditAnywhere, Category = Action)
		TSet<FName> RemoveTags;

	/** The actors whose tags array will be modified. */
	UPROPERTY(EditAnywhere, Category = Action)
		TSet<TSoftObjectPtr<AActor>> Targets;

	virtual void OnExecute_Implementation() override;
};

/** 
 * Updates 'Custom Primitive Data' on Static Mesh Actors. 
 * This is an extremely efficient way to update material parameters without creating dynamic material instances.
 */
UCLASS(NotBlueprintable, DisplayName = "[Actor] Primitive Data")
class TORORUNTIME_API UWorldAction_PrimitiveData final : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_PrimitiveData() 
		: bOverrideDefault(true)
	{}

private:

	/** If true, the new data will overwrite current values instantly. */
	UPROPERTY(EditAnywhere, Category = Action)
		bool bOverrideDefault;

	/** 
	 * Map of Primitive Data Indices and their target values. 
	 * Key: The index of the primitive data float (0-31).
	 * Value: The new float value for the material.
	 */
	UPROPERTY(EditAnywhere, Category = Action)
		TMap<uint8, float> PrimitiveData;

	/** The actors whose primitive data will be modified. */
	UPROPERTY(EditAnywhere, Category = Action)
		TSet<TSoftObjectPtr<AStaticMeshActor>> Targets;

	virtual void OnExecute_Implementation() override;
};