// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "RequesterSet.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FRequestSetSingleDelegateBP, const bool, bChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRequestSetMultiDelegateBP, const bool, bChanged);

/**
 * A utility structure that manages a collection of "requests" from various UObjects.
 * Uses TWeakObjectPtr to ensure that if a requester is destroyed, the set can clean itself up.
 * Useful for systems like "Game Paused by Requesters", "UI Visibility Requesters", etc.
 */
USTRUCT(BlueprintType)
struct TOROCORE_API FRequesterSet final
{
	GENERATED_BODY()

	/**
	 * Adds a unique requester to the set. 
	 * Triggers a broadcast if the state changes from "No Requests" to "Has Requests".
	 * @param InRequester The object making the request.
	 */
	void AddRequest(const UObject* InRequester)
	{
		if (InRequester && !Requests.Contains(InRequester))
		{
			Requests.Add(InRequester);
			if (!CleanupNulls()) // Prevent double broadcasts
			{
				BroadcastUpdate();
			}
		}
	}

	/**
	 * Removes a specific requester from the set.
	 * Triggers a broadcast if the set becomes empty.
	 * @param InRequester The object that previously made the request.
	 */
	void RemoveRequest(const UObject* InRequester)
	{
		if (InRequester && Requests.Remove(InRequester) > 0)
		{
			BroadcastUpdate();
		}
	}

	/**
	 * Removes all requesters and triggers an update broadcast.
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
	 * Scans the set for invalid/garbage-collected objects and removes them.
	 * @return True if any null/stale pointers were removed AND brodadcast update was called.
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
	 * Returns true if there is at least one valid requester in the set. 
	 */
	bool HasRequests() const
	{
		return !Requests.IsEmpty();
	}

	UPROPERTY()
		FRequestSetMultiDelegateBP OnRequestChangedBP;

	DECLARE_MULTICAST_DELEGATE_OneParam(FRequestSetDelegate, const bool);
	FRequestSetDelegate OnRequestChanged;

private:

	TArray<TWeakObjectPtr<const UObject>> Requests;

	void BroadcastUpdate() const
	{
		const bool bState = HasRequests();
		OnRequestChanged.Broadcast(bState);
		OnRequestChangedBP.Broadcast(bState);
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

	/** Adds a requester to the provided set. */
	UFUNCTION(BlueprintCallable, Category = RequesterSet)
	static void AddRequest(UPARAM(ref) FRequesterSet& InSet, const UObject* Requester)
	{
		InSet.AddRequest(Requester);
	}

	/** Removes a requester from the provided set. */
	UFUNCTION(BlueprintCallable, Category = RequesterSet)
	static void RemoveRequest(UPARAM(ref) FRequesterSet& InSet, const UObject* Requester)
	{
		InSet.RemoveRequest(Requester);
	}

	/** Clears all requesters from the provided set. */
	UFUNCTION(BlueprintCallable, Category = RequesterSet)
	static void ClearRequests(UPARAM(ref) FRequesterSet& InSet)
	{
		InSet.ClearRequests();
	}

	/** Manually triggers a cleanup of null/stale pointers within the set. */
	UFUNCTION(BlueprintCallable, Category = RequesterSet)
	static bool CleanupNulls(UPARAM(ref) FRequesterSet& InSet)
	{
		return InSet.CleanupNulls();
	}

	/** Binds a single dynamic delegate to the set's update event. */
	UFUNCTION(BlueprintCallable, Category = RequesterSet)
	static void BindOnRequestChanged(UPARAM(ref) FRequesterSet& InSet, const FRequestSetSingleDelegateBP& InDelegate)
	{
		if (InDelegate.IsBound())
		{
			InSet.OnRequestChangedBP.Add(InDelegate);
		}
	}

	/** Unbinds a single dynamic delegate from the set's update event. */
	UFUNCTION(BlueprintCallable, Category = RequesterSet)
	static void UnbindOnRequestChanged(UPARAM(ref) FRequesterSet& InSet, const FRequestSetSingleDelegateBP& InDelegate)
	{
		if (InDelegate.IsBound())
		{
			InSet.OnRequestChangedBP.Remove(InDelegate);
		}
	}

	/** Returns true if the set contains any active requests. */
	UFUNCTION(BlueprintPure, Category = RequesterSet)
	static bool HasRequests(const FRequesterSet& InSet)
	{
		return InSet.HasRequests();
	}
};