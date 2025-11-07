// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Inventory/InventoryItem.h"
#if WITH_EDITOR
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#endif

AInventoryItem::AInventoryItem()
	: Item(nullptr), Amount(1), RespawnTime(0), LabelFormat(INVTEXT("{Item} x{Amount}"))
{
	PrimaryActorTick.bCanEverTick = false;
}

void AInventoryItem::RespawnItem()
{
	GetWorldTimerManager().ClearTimer(RespawnTimer);
	if (!IsEnabled())
	{
		SetEnabled(true);
	}
}

bool AInventoryItem::GetInteractInfo_Implementation(const FHitResult& Hit, FInteractionInfo& Info)
{
	return InventoryManager && Item && Amount != 0
		&& Super::GetInteractInfo_Implementation(Hit, Info);
}

void AInventoryItem::OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& Hit)
{
	if (InventoryManager && IsEnabled())
	{
		InventoryManager->AddItem(Item, Amount);
		Super::OnBeginInteract_Implementation(Player, Hit);
		SetEnabled(false);

		if (RespawnTime > 0)
		{
			GetWorldTimerManager().SetTimer(RespawnTimer, this,
			   &AInventoryItem::RespawnItem, RespawnTime * 60.0f, false);
		}
	}
}

void AInventoryItem::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		InventoryManager = UInventoryManager::Get(this);
	});
}

#if WITH_EDITOR
void AInventoryItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (Item && Item->AssetType != EInvAssetType::Item)
	{
		FNotificationInfo Info(FText::Format(
			INVTEXT("\"{0}\" is not an Item"),
			FText::FromString(GetNameSafe(Item)))
		);
		Info.ExpireDuration = 1.0f;
		FSlateNotificationManager::Get().AddNotification(Info);
		Item = nullptr;
	}

	if (LabelFormat.IsEmptyOrWhitespace())
	{
		LabelFormat = INVTEXT("{Item} x{Amount}");
	}
	
	if (Item && Amount != 0)
	{
		Interaction.Label = FText::FormatNamed(LabelFormat,
		   TEXT("Item"), Item->DisplayName,
		   TEXT("Amount"), Amount
		);
	}
	else
	{
		Interaction.Label = INVTEXT("INVALID");
	}
}
#endif