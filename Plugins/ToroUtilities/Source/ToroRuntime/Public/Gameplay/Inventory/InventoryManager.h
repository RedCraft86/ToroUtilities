// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InventoryAsset.h"
#include "InventoryWidget.h"
#include "Helpers/ClassGetterMacros.h"
#include "Helpers/GameplayTagMacros.h"
#include "Components/ActorComponent.h"
#include "Framework/ToroPlayerState.h"
#include "SaveSystem/ToroSaveManager.h"
#include "InventoryManager.generated.h"

namespace InventoryTags
{
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(Inventory)
	TORORUNTIME_API inline bool IsValidTag(const FGameplayTag& InTag)
	{
		static FGameplayTag BaseTag = TAG_Inventory.GetTag();
		return InTag.MatchesTag(BaseTag) || InTag == BaseTag;
	}
}

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FInventoryEquipment
{
	GENERATED_BODY()

	UPROPERTY(Transient) TSoftObjectPtr<UInventoryAsset> Item;
	UPROPERTY(Transient) TObjectPtr<AEquipmentActor> Actor;

	FInventoryEquipment(): Item(nullptr), Actor(nullptr) {}
};

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UInventoryManager final : public UActorComponent
{
	GENERATED_BODY()

public:

	UInventoryManager() {}

	PLAYER_COMPONENT_GETTER(UInventoryManager, AToroPlayerState, Inventory)

	UFUNCTION(BlueprintCallable, Category = Statics, meta = (WorldContext = "ContextObject"))
	static UInventoryManager* GetInventoryManager(const UObject* ContextObject, const int32 PlayerIdx = 0)
	{
		return Get(ContextObject, PlayerIdx);
	}

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void AddItem(UInventoryAsset* InItem, const uint8 Amount = 1);

	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool RemoveItem(UInventoryAsset* InItem, const uint8 Amount = 1);

	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool HasItem(UInventoryAsset* InItem, const uint8 MinAmount = 1);

	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool UseKey(UInventoryAsset* InItem);

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void AddArchive(UInventoryAsset* InItem);

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void UnEquipItem();

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void EquipItem(UInventoryAsset* InItem);

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void SetEquipmentUsage(const bool bUsing) const;

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void OpenInventory();

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void CloseInventory();

	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool IsInventoryOpen();
	
	void PullFromSave(const FGameplayTag& Profile);
	void PushToSave(const FGameplayTag& Profile) const;
	void EnsureInventory(
		const TArray<TSoftObjectPtr<UInventoryAsset>>& InArchives,
		const TMap<TSoftObjectPtr<UInventoryAsset>, uint8>& InItems);

	const TArray<TSoftObjectPtr<UInventoryAsset>>& GetArchives() { return Archives; }
	const TMap<TSoftObjectPtr<UInventoryAsset>, uint8>& GetItems() { return Items; }
	const FInventoryEquipment& GetEquipment() { return Equipment; }

private:

	UPROPERTY(Transient) TArray<TSoftObjectPtr<UInventoryAsset>> Archives;
	UPROPERTY(Transient) TMap<TSoftObjectPtr<UInventoryAsset>, uint8> Items;
	UPROPERTY(Transient) FInventoryEquipment Equipment;

	TObjectPtr<UToroSaveManager> SaveManager;

	TObjectPtr<UInventoryWidget> InventoryWidget;
	UInventoryWidget* GetInventoryWidget();

	void UpdateUI();
	virtual void BeginPlay() override;
};
