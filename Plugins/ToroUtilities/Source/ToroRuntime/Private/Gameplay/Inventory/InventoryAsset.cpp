// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Inventory/InventoryAsset.h"

UInventoryAsset::UInventoryAsset()
	: AssetType(EInvAssetType::Item), ItemType(EInvItemType::QuestItem), bReusableKey(true)
{
}

uint8 UInventoryAsset::GetStackLimit() const
{
	return AssetType == EInvAssetType::Archive || ItemType == EInvItemType::Equipment ? 1 : 128;
}

bool UInventoryAsset::IsValidEquipment() const
{
	return ItemType == EInvItemType::Equipment && !Equipment.IsNull();
}

bool UInventoryAsset::IsValidArchive() const
{
	return AssetType == EInvAssetType::Archive && !ContentText.IsEmptyOrWhitespace();
}

#if WITH_EDITOR
FText UInventoryAsset::GetDescription() const
{
	return FText::Format(INVTEXT("Type: {0}"), AssetType == EInvAssetType::Item
		? UEnum::GetDisplayValueAsText(ItemType) : INVTEXT("Archive"));
}
#endif