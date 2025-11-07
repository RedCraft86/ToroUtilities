// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroEventPayload.h"
#include "Helpers/ClassGetterMacros.h"
#include "Helpers/GameplayTagMacros.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ToroEventManager.generated.h"

namespace GlobalEvents
{
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(GlobalEvent)
	TORORUNTIME_API CREATE_TAG_VERIFIER(GlobalEvent)
}

DECLARE_DYNAMIC_DELEGATE_OneParam(FGlobalEventSingle, const UToroEventPayload*, Payload);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGlobalEventMulti, const UToroEventPayload*, Payload);

UCLASS()
class TORORUNTIME_API UToroEventManager final : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UToroEventManager() {}
	
	GAME_INSTANCE_SUBSYSTEM_GETTER(UToroEventManager);

	/* Binds a delegate to a global event. The delegate will be called whenever this event is invoked. */
	UFUNCTION(BlueprintCallable, Category = GlobalEvents)
		void BindGlobalEvent(UPARAM(meta = (Categories = "GlobalEvent")) const FGameplayTag EventID, const FGlobalEventSingle& Event);

	/* Unbinds a delegate from the specified global event.
	 * If no delegates are bound, the event will be removed, and its cached payload will no longer be retained.
	 */
	UFUNCTION(BlueprintCallable, Category = GlobalEvents)
		void UnbindGlobalEvent(UPARAM(meta = (Categories = "GlobalEvent")) const FGameplayTag EventID, const FGlobalEventSingle& Event);

	/* Invokes a global event by tag. Passing in a payload is optional but required to pass the Instigator.
	 * The payload, if passed, will be cached by the manager to avoid garbage collection for the listeners.
	 * If a new payload is passed or null while having an old one, the old one will be released.
	 */
	UFUNCTION(BlueprintCallable, Category = GlobalEvents, meta = (AdvancedDisplay = "Instigator", DefaultToSelf = "Instigator"))
		void InvokeGlobalEvent(UPARAM(meta = (Categories = "GlobalEvent")) const FGameplayTag EventID, UToroEventPayload* Payload, UObject* Instigator);

	/* Completely removes the specified global event from the system.
	 * This also clears any bound delegates and releases the cached payload associated with the event.
	 */
	UFUNCTION(BlueprintCallable, Category = GlobalEvents)
		void RemoveGlobalEvent(UPARAM(meta = (Categories = "GlobalEvent")) const FGameplayTag EventID);

	/* Gets the payload associated to an event. */
	UFUNCTION(BlueprintCallable, Category = GlobalEvents)
		UToroEventPayload* GetEventPayload(const FGameplayTag& EventID);

	FGlobalEventMulti& GetEventDelegate(const FGameplayTag& EventID) { return Events.FindOrAdd(EventID); }

private:

	UPROPERTY(Transient)
		TMap<FGameplayTag, FGlobalEventMulti> Events;

	UPROPERTY(Transient)
		TMap<FGameplayTag, TObjectPtr<UToroEventPayload>> Payloads;
};
