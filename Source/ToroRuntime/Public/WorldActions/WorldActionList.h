// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "WorldActionBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WorldActionList.generated.h"

/**
 * A list of inlined WorldAction objects that can be easily initialized and executed through this struct.
 */
USTRUCT(BlueprintType)
struct TORORUNTIME_API FWorldActionList final
{
	GENERATED_BODY()

private:
#if WITH_EDITOR
	friend class FWorldActionListDetails;
#endif

	UPROPERTY(EditAnywhere, Instanced, Category = Actions)
		TArray<TObjectPtr<UWorldActionBase>> Actions;

public:

	FWorldActionList() {}

	void Execute(const UObject* WorldContext) const;
	void Initialize(const UObject* WorldContext) const;
	void ForEachAction(const TFunction<void(UWorldActionBase*)>& Func) const;

	const TArray<TObjectPtr<UWorldActionBase>>& GetActions() const { return Actions; }
};

/**
 * Blueprint function wrappers for FWorldActionList
 */
UCLASS()
class TORORUNTIME_API UWorldActionLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = WorldActions, meta = (DefaultToSelf = "ContextObject"))
	static void ExecuteActions(const FWorldActionList& ActionList, const UObject* ContextObject)
	{
		ActionList.Execute(ContextObject);
	}

	UFUNCTION(BlueprintCallable, Category = WorldActions, meta = (DefaultToSelf = "ContextObject"))
	static void InitializeActions(const FWorldActionList& ActionList, const UObject* ContextObject)
	{
		ActionList.Initialize(ContextObject);
	}

	UFUNCTION(BlueprintPure, Category = WorldActions)
	static TArray<UWorldActionBase*> GetActions(const FWorldActionList& ActionList)
	{
		return ActionList.GetActions();
	}
};