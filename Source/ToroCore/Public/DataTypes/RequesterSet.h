// Copyright (C) 2026 Tayzar Linn. Licensed under LGPL-3.0, see project LICENSE file.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "RequesterSet.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FRequestSetSingleDelegateBP, const bool, bChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRequestSetMultiDelegateBP, const bool, bChanged);

/**
 * A utility struct used to manage a set of "requests" represented as UObjects.
 * Uses the TWeakObjectPtr to not interfere with the Garbage Collector.
 * Useful for systems where multiple things can affect one value.
 * (i.e. Pause Game by Requests, Show UI by Requests, etc.)
 */
USTRUCT(BlueprintType)
struct TOROCORE_API FRequesterSet final
{
	GENERATED_BODY()

private:

	UPROPERTY(Transient)
		TSet<TWeakObjectPtr<const UObject>> Requests;

	void BroadcastUpdate() const
	{
		const bool bState = HasRequests();
		OnRequestChanged.Broadcast(bState);
		OnRequestChangedBP.Broadcast(bState);
	}

public:

	UPROPERTY()
	FRequestSetMultiDelegateBP OnRequestChangedBP;

	DECLARE_MULTICAST_DELEGATE_OneParam(FRequestSetDelegate, const bool);
	FRequestSetDelegate OnRequestChanged;

	/**
	 * Scans the set for invalid/garbage-collected objects and removes them.
	 * @return true if any null/stale pointers were removed and an update was broadcasted.
	 */
	bool CleanupNulls()
	{
		bool bChanged = false;
		for (auto It = Requests.CreateIterator(); It; ++It)
		{
			if (!It->IsValid())
			{
				It.RemoveCurrent();
				bChanged = true;
			}
		}

		if (bChanged)
		{
			BroadcastUpdate();
		}

		return bChanged;
	}

	/**
	 * Adds a unique requester to the set and triggers a brodcast if the state changes.
	 * @param Requester The object making the request or some object to hold the state.
	 */
	void AddRequest(const UObject* Requester)
	{
		if (Requester && !Requests.Contains(Requester))
		{
			Requests.Add(Requester);
			if (!CleanupNulls())
			{
				BroadcastUpdate();
			}
		}
	}

	/**
	 * Removes a specific requester from the set and triggers a broadcast if the state changes.
	 * @param Requester The object making the request or some object that was holding the state.
	 */
	void RemoveRequest(const UObject* Requester)
	{
		if (Requester && Requests.Remove(Requester) > 0)
		{
			if (!CleanupNulls())
			{
				BroadcastUpdate();
			}
		}
	}

	/**
	 * Removes all requesters and triggers a broadcast if the state changes.
	 */
	void ClearRequests()
	{
		if (!Requests.IsEmpty())
		{
			Requests.Empty();
			BroadcastUpdate();
		}
	}

	/** 
	 * Returns true if there is at least one valid requester in the set. 
	 */
	bool HasRequests() const
	{
		return !Requests.IsEmpty();
	}
};

/**
 * Blueprint function wrappers for RequesterSet struct functions
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API URequesterSetLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Scans the set for invalid/garbage-collected objects and removes them.
	 * @param InSet The requester set to modify.
	 * @return true if any null/stale pointers were removed and an update was broadcasted.
	 */
	UFUNCTION(BlueprintCallable, Category = RequesterSet)
	static bool CleanupNulls(UPARAM(ref) FRequesterSet& InSet)
	{
		return InSet.CleanupNulls();
	}

	/**
	 * Adds a unique requester to the set and triggers a brodcast if the state changes.
	 * @param InSet The requester set to modify.
	 * @param Requester The object making the request or some object to hold the state.
	 */
	UFUNCTION(BlueprintCallable, Category = RequesterSet)
	static void AddRequest(UPARAM(ref) FRequesterSet& InSet, const UObject* Requester)
	{
		InSet.AddRequest(Requester);
	}

	/**
	 * Removes a specific requester from the set and triggers a broadcast if the state changes.
	 * @param InSet The requester set to modify.
	 * @param Requester The object making the request or some object that was holding the state.
	 */
	UFUNCTION(BlueprintCallable, Category = RequesterSet)
	static void RemoveRequest(UPARAM(ref) FRequesterSet& InSet, const UObject* Requester)
	{
		InSet.RemoveRequest(Requester);
	}

	/**
	 * Removes all requesters and triggers a broadcast if the state changes.
	 * @param InSet The requester set to modify.
	 */
	UFUNCTION(BlueprintCallable, Category = RequesterSet)
	static void ClearRequests(UPARAM(ref) FRequesterSet& InSet)
	{
		InSet.ClearRequests();
	}

	/** 
	 * Returns true if there is at least one valid requester in the set. 
	 * @param InSet The requester set to check.
	 */
	UFUNCTION(BlueprintPure, Category = RequesterSet)
	static bool HasRequests(const FRequesterSet& InSet)
	{
		return InSet.HasRequests();
	}

	/** 
	 * Binds a single dynamic delegate to the set's update event. 
	 * @param InSet The requester set to bind to.
	 */
	UFUNCTION(BlueprintCallable, Category = RequesterSet)
	static void BindOnRequestChanged(UPARAM(ref) FRequesterSet& InSet, const FRequestSetSingleDelegateBP& InDelegate)
	{
		if (InDelegate.IsBound())
		{
			InSet.OnRequestChangedBP.Add(InDelegate);
		}
	}

	/** 
	 * Unbinds a single dynamic delegate from the set's update event. 
	 * @param InSet The requester set to unbind from.
	 */
	UFUNCTION(BlueprintCallable, Category = RequesterSet)
	static void UnbindOnRequestChanged(UPARAM(ref) FRequesterSet& InSet, const FRequestSetSingleDelegateBP& InDelegate)
	{
		if (InDelegate.IsBound())
		{
			InSet.OnRequestChangedBP.Remove(InDelegate);
		}
	}
};