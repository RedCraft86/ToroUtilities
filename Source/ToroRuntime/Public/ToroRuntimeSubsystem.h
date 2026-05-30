// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Helpers/WorldGetter.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ToroRuntimeSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FGlobalEventSingleDelegate, const FInstancedStruct&, Payload, UObject*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGlobalEventMultiDelegate, const FInstancedStruct&, Payload, UObject*, Instigator);

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

	UFUNCTION(BlueprintCallable, Category = "Global|Flags")
		void AddGlobalMetadata(UPARAM(meta = (Categories = "Flag")) const FGameplayTag Key, const FInstancedStruct& Value);

	UFUNCTION(BlueprintCallable, Category = "Global|Flags")
		void RemoveGlobalMetadata(UPARAM(meta = (Categories = "Flag")) const FGameplayTag Key);

	UFUNCTION(BlueprintPure, Category = "Global|Flags")
		bool HasGlobalMetadata(UPARAM(meta = (Categories = "Flag")) const FGameplayTag Key) const;

	UFUNCTION(BlueprintPure, Category = "Global|Flags")
		const FInstancedStruct& GetGlobalMetadata(UPARAM(meta = (Categories = "Flag")) const FGameplayTag Key) const;

	UFUNCTION(BlueprintCallable, Category = "Global|Events")
		void BindGlobalEvent(UPARAM(meta = (Categories = "Event")) const FGameplayTag Key, const FGlobalEventSingleDelegate& Event);

	UFUNCTION(BlueprintCallable, Category = "Global|Events")
		void UnbindGlobalEvent(UPARAM(meta = (Categories = "Event")) const FGameplayTag Key, const FGlobalEventSingleDelegate& Event);

	UFUNCTION(BlueprintCallable, Category = "Global|Events", meta = (AdvancedDisplay = "Instigator", DefaultToSelf = "Instigator"))
		void InvokeGlobalEvent(UPARAM(meta = (Categories = "Event")) const FGameplayTag Key, const FInstancedStruct& Payload, UObject* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Global|Events")
		void RemoveGlobalEvent(UPARAM(meta = (Categories = "Event")) const FGameplayTag Key);

	FGlobalEventMultiDelegate* GetEventDelegate(const FGameplayTag& Key);

protected:

	UPROPERTY(Transient) TMap<FGameplayTag, FInstancedStruct> Metadata;
	UPROPERTY(Transient) TMap<FGameplayTag, FGlobalEventMultiDelegate> Events;
};
