// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Gameplay/Inventory/InventoryManager.h"
#include "UserInterface/ToroWidgetManager.h"
#include "SaveSystem/ToroGameSave.h"

void UInventoryManager::AddItem(UInventoryAsset* InItem, const uint8 Amount)
{
	uint8& Count = Items.FindOrAdd(InItem);
	Count += Amount;
	UpdateUI();
}

bool UInventoryManager::RemoveItem(UInventoryAsset* InItem, const uint8 Amount)
{
	uint8* Count = Items.Find(InItem);
	if (Count && *Count > Amount)
	{
		*Count = FMath::Max(*Count - Amount, 0);
		if (*Count == 0) Items.Remove(InItem);
		UpdateUI();
		return true;
	}
	return false;
}

bool UInventoryManager::HasItem(UInventoryAsset* InItem, const uint8 MinAmount)
{
	const uint8* Count = Items.Find(InItem);
	return Count && *Count >= MinAmount;
}

bool UInventoryManager::UseKey(UInventoryAsset* InItem)
{
	if (InItem && InItem->AssetType == EInvAssetType::Item && HasItem(InItem))
	{
		if (InItem->ItemType != EInvItemType::KeyItem || !InItem->bReusableKey)
		{
			RemoveItem(InItem);
		}
		return true;
	}
	return false;
}

void UInventoryManager::AddArchive(UInventoryAsset* InItem)
{
	if (!Archives.Contains(InItem))
	{
		Archives.Add(InItem);
	}
}

void UInventoryManager::UnEquipItem()
{
	if (Equipment.Actor)
	{
		Equipment.Actor->UnequipItem();
		Equipment = FInventoryEquipment();
		UpdateUI();
	}
}

void UInventoryManager::EquipItem(UInventoryAsset* InItem)
{
	if (Equipment.Item == InItem) return;
	if (Equipment.Actor)
	{
		UnEquipItem();
	}

	if (!InItem || !InItem->IsValidEquipment()) return;
	GetWorld()->GetTimerManager().SetTimerForNextTick([this, NewItem = InItem]()
	{
		Equipment.Actor = GetWorld()->SpawnActor<AEquipmentActor>(
			NewItem->Equipment.LoadSynchronous());
		if (Equipment.Actor)
		{
			Equipment.Item = NewItem;
			Equipment.Actor->EquipItem();
			UpdateUI();
		}
	});
}

void UInventoryManager::SetEquipmentUsage(const bool bUsing) const
{
	if (IsValid(Equipment.Actor))
	{
		Equipment.Actor->SetUseState(bUsing);
	}
}

void UInventoryManager::OpenInventory()
{
	if (UInventoryWidget* Widget = GetInventoryWidget())
	{
		Widget->PushWidget();
	}
}

void UInventoryManager::CloseInventory()
{
	if (UInventoryWidget* Widget = GetInventoryWidget())
	{
		Widget->PopWidget();
	}
}

bool UInventoryManager::IsInventoryOpen()
{
	const UInventoryWidget* Widget = GetInventoryWidget();
	return Widget && Widget->IsPushed();
}

void UInventoryManager::PullFromSave()
{
	UnEquipItem();
	if (const UToroGameSave* Save = SaveManager ? SaveManager->FindOrAddSave<UToroGameSave>() : nullptr)
	{
		Archives = Save->Archives.ToInventoryArchives();
		//Items = Save->Items.ToInventoryItems();
		// if (!Save->Equipment.IsNull())
		// {
		// 	EquipItem(TSoftObjectPtr<UInventoryAsset>(Save->Equipment).LoadSynchronous());
		// }
	}
}

void UInventoryManager::PushToSave()
{
	if (UToroGameSave* Save = SaveManager ? SaveManager->FindOrAddSave<UToroGameSave>() : nullptr)
	{
		//Save->Items = FInventoryItemSave(Items);
		Save->Archives = FInventoryArchiveSave(Archives);
		Save->Equipment = Equipment.Item.ToSoftObjectPath();
	}

	UnEquipItem();
	for (auto It = Items.CreateIterator(); It; ++It)
	{
		const UInventoryAsset* Asset = It.Key().LoadSynchronous();
		if (!Asset || Asset->bTransient)
		{
			It.RemoveCurrent();
		}
	}
}

void UInventoryManager::EnsureInventory(
	const TArray<TSoftObjectPtr<UInventoryAsset>>& InArchives,
	const TMap<TSoftObjectPtr<UInventoryAsset>, uint8>& InItems)
{
	for (const TSoftObjectPtr<UInventoryAsset>& Archive : InArchives)
	{
		if (!Archives.Contains(Archive)) Archives.Add(Archive);
	}

	for (const TPair<TSoftObjectPtr<UInventoryAsset>, uint8>& Item : InItems)
	{
		uint8& Amount = Items.FindOrAdd(Item.Key);
		Amount = FMath::Max(Amount, Item.Value);
	}
	
	const UToroGameSave* Save = SaveManager ? SaveManager->FindOrAddSave<UToroGameSave>() : nullptr;
	if (Save && !Save->Equipment.IsNull())
	{
		EquipItem(TSoftObjectPtr<UInventoryAsset>(Save->Equipment).LoadSynchronous());
	}
}

UInventoryWidget* UInventoryManager::GetInventoryWidget()
{
	if (!InventoryWidget)
	{
		InventoryWidget = AToroWidgetManager::GetWidget<UInventoryWidget>(this);
	}
	return InventoryWidget;
}

void UInventoryManager::UpdateUI()
{
	if (UInventoryWidget* Widget = GetInventoryWidget())
	{
		Widget->UpdateWidget();
	}
}

void UInventoryManager::BeginPlay()
{
	Super::BeginPlay();
	SaveManager = UToroSaveManager::Get(this);
}
