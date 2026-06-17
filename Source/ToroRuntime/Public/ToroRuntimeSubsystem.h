// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Helpers/WorldGetter.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ToroRuntimeSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FGlobalEventSingleDelegate, const FInstancedStruct&, Payload, UObject*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGlobalEventMultiDelegate, const FInstancedStruct&, Payload, UObject*, Instigator);

/**
 * A global, persistent subsystem tied to the Game Instance lifetime.
 * This subsystem serves as a centralized communication hub and data store, providing:
 * 1. Global Metadata: A tag-indexed map of polymorphic data (FInstancedStruct).
 * 2. Global Event Bus: A decoupled event system using Gameplay Tags as channels.
 */
UCLASS(NotBlueprintable, BlueprintType)
class TORORUNTIME_API UToroRuntimeSubsystem final : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UToroRuntimeSubsystem() {}

	[[nodiscard]] static UToroRuntimeSubsystem* Get(const UObject* ContextObject)
	{
		const UWorld* World = FWorldGetter::Get(ContextObject);
		const UGameInstance* GI = IsValid(World) ? World->GetGameInstance() : nullptr;
		return IsValid(GI) ? GI->GetSubsystem<UToroRuntimeSubsystem>() : nullptr;
	}

	/** 
	 * Adds or updates a piece of global metadata.
	 * @param Key Unique GameplayTag identifier.
	 * @param Value The data structure to store. Supports any USTRUCT marked as BlueprintType.
	 */
	UFUNCTION(BlueprintCallable, Category = "Global|Flags")
		void AddGlobalMetadata(const FGameplayTag Key, const FInstancedStruct& Value);

	/** 
	 * Removes the metadata entry associated with the provided key. 
	 * @param Key The tag to remove from the metadata map.
	 */
	UFUNCTION(BlueprintCallable, Category = "Global|Flags")
		void RemoveGlobalMetadata(const FGameplayTag Key);

	/** 
	 * Checks if a valid metadata entry exists for the specific key.
	 * @return True if the key is present in the internal storage.
	 */
	UFUNCTION(BlueprintPure, Category = "Global|Flags")
		bool HasGlobalMetadata(const FGameplayTag Key) const;

	/** 
	 * Retrieves the stored metadata for a key.
	 * @note In Blueprints, use 'Break InstancedStruct' to access the underlying data.
	 * @return A reference to the stored FInstancedStruct. Returns an empty struct if the key is missing.
	 */
	UFUNCTION(BlueprintPure, Category = "Global|Flags")
		const FInstancedStruct& GetGlobalMetadata(const FGameplayTag Key) const;

	/** 
	 * Binds a Blueprint delegate to a global event channel.
	 * @param Key Unique event identifier.
	 * @param Event The delegate to trigger when this event is invoked.
	 */
	UFUNCTION(BlueprintCallable, Category = "Global|Events")
		void BindGlobalEvent(const FGameplayTag Key, const FGlobalEventSingleDelegate& Event);

	/** 
	 * Unbinds a previously bound delegate from a global event channel.
	 * If no delegates are present for the event, it is removed from the event map.
	 * @param Key The specific event tag to unbind from.
	 */
	UFUNCTION(BlueprintCallable, Category = "Global|Events")
		void UnbindGlobalEvent(const FGameplayTag Key, const FGlobalEventSingleDelegate& Event);

	/** 
	 * Fires a global event, notifying all listeners bound to the specified channel.
	 * @param Key The event tag to invoke.
	 * @param Payload Optional data passed to all listeners (e.g., damage amounts, update counts).
	 * @param Instigator The object that triggered the event (defaults to the calling object).
	 */
	UFUNCTION(BlueprintCallable, Category = "Global|Events", meta = (AdvancedDisplay = "Instigator", DefaultToSelf = "Instigator"))
		void InvokeGlobalEvent(const FGameplayTag Key, const FInstancedStruct& Payload, UObject* Instigator);

	/** 
	 * Clears all listeners and removes the event from the event map.
	 */
	UFUNCTION(BlueprintCallable, Category = "Global|Events")
		void RemoveGlobalEvent(const FGameplayTag Key);

	FGlobalEventMultiDelegate* GetEventDelegate(const FGameplayTag& Key);

protected:

	UPROPERTY(Transient) TMap<FGameplayTag, FInstancedStruct> Metadata;
	UPROPERTY(Transient) TMap<FGameplayTag, FGlobalEventMultiDelegate> Events;
};
