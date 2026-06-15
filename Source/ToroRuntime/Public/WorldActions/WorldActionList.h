// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "WorldActionBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WorldActionList.generated.h"

/**
 * A container for a list of inlined, polymorphic WorldAction objects.
 * This struct allows designers to define a sequence of logic (e.g., spawn particles, play sound, update quest)
 * directly within an Actor or Data Asset details panel using the 'Instanced' pattern.
 */
USTRUCT(BlueprintType)
struct TORORUNTIME_API FWorldActionList final
{
	GENERATED_BODY()

private:
#if WITH_EDITOR
	friend class FWorldActionListDetails;
#endif

	/** The collection of polymorphic actions. Use the '+' button in the editor to select specific action types. */
	UPROPERTY(EditAnywhere, Instanced, Category = Actions)
		TArray<TObjectPtr<UWorldActionBase>> Actions;

public:

	FWorldActionList() {}

	/** Runs the 'Execute' logic for every valid action in the list. */
	void Execute(UObject* WorldContext) const;

	/** Pre-initializes all actions in the list. Useful for setting up cached references before execution. */
	void Initialize(UObject* WorldContext) const;

	/** Helper for iterating through actions with a custom lambda function. */
	void ForEachAction(const TFunction<void(UWorldActionBase*)>& Func) const;

	/** Returns a read-only reference to the internal action array. */
	const TArray<TObjectPtr<UWorldActionBase>>& GetActions() const { return Actions; }
};

/**
 * Static utility class to expose FWorldActionList functionality to Blueprint Graphs.
 */
UCLASS()
class TORORUNTIME_API UWorldActionLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** Executes all actions within the provided ActionList. */
	UFUNCTION(BlueprintCallable, Category = WorldActions, meta = (DefaultToSelf = "ContextObject"))
	static void ExecuteActions(UObject* ContextObject, const FWorldActionList& ActionList)
	{
		ActionList.Execute(ContextObject);
	}

	/** Initializes all actions within the provided ActionList. */
	UFUNCTION(BlueprintCallable, Category = WorldActions, meta = (DefaultToSelf = "ContextObject"))
	static void InitializeActions(UObject* ContextObject, const FWorldActionList& ActionList)
	{
		ActionList.Initialize(ContextObject);
	}

	/** Returns the raw array of Action objects from the list for manual iteration in Blueprints. */
	UFUNCTION(BlueprintPure, Category = WorldActions)
	static TArray<UWorldActionBase*> GetActions(const FWorldActionList& ActionList)
	{
		return ActionList.GetActions();
	}
};