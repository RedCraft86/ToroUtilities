// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Inventory/InventoryWidget.h"
#include "Gameplay/Inventory/InventoryManager.h"
#include "UserInterface/NativeContainers.h"
#include "Framework/ToroPlayerController.h"
#include "Blueprint/WidgetTree.h"

void UInventorySlotWidget::OnClicked()
{
	ParentUI->SetSelected(this);
}

void UInventorySlotWidget::InitializeWidget(UInventoryWidget* InWidget, UInventoryAsset* InAsset)
{
	Asset = InAsset;
	ParentUI = InWidget;
	IconImage->SetBrushFromTexture(InAsset->Thumbnail.LoadSynchronous());
	SelectButton->OnClicked.AddUniqueDynamic(this, &UInventorySlotWidget::OnClicked);
}

void UInventoryItemWidget::InitItem(UInventoryWidget* InWidget, UInventoryAsset* InAsset, const uint8 Amount, const bool bEquipped)
{
	InitializeWidget(InWidget, InAsset);
	AmountText->SetText(FText::AsNumber(Amount));
	EquipVisual->SetVisibility(bEquipped ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UInventoryArchiveWidget::InitArchive(UInventoryWidget* InWidget, UInventoryAsset* InAsset)
{
	InitializeWidget(InWidget, InAsset);
	NameText->SetText(InAsset->DisplayName);
}

UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bArchiveTab(false)
{
	bAutoPush = false;
	ContainerClass = UMenuWidgetContainer::StaticClass();
}

void UInventoryWidget::UpdateWidget()
{
	if (IsPushed()) UpdateSlots();
}

void UInventoryWidget::PushWidget()
{
	Super::PushWidget();
	if (AToroPlayerController* PC = GetOwningPlayer<AToroPlayerController>())
	{
		InputConfig = PC->GetInputConfig();
		PC->SetInputConfig({EGameInputMode::GameAndUI, true,
			EMouseLockMode::LockAlways, false, this});
		PC->AddPauseRequest(this);
	}
	UpdateWidget();
}

void UInventoryWidget::PopWidget()
{
	if (AToroPlayerController* PC = GetOwningPlayer<AToroPlayerController>())
	{
		PC->SetInputConfig(InputConfig);
		PC->RemovePauseRequest(this);
	}
	Super::PopWidget();
}

void UInventoryWidget::OnEquipButton()
{
	UInventoryAsset* Asset = bArchiveTab ? nullptr : SelectedItem.Get();
	Manager->EquipItem(Manager->GetEquipment().Item == Asset ? nullptr : Asset);
	UpdateInfo();
}

void UInventoryWidget::UpdateInfo()
{
	if (!Manager) return;
	if (UInventoryAsset* Asset = bArchiveTab ? SelectedArchive.Get() : SelectedItem.Get())
	{
		ThumbnailImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		ThumbnailImage->SetBrushFromTexture(Asset->Thumbnail.LoadSynchronous());
		LabelText->SetText(Asset->DisplayName);
		DescText->SetText(Asset->Description);
		
		EquipButton->SetIsEnabled(Asset->IsValidEquipment());
		EquipButton->SetVisibility(EquipButton->GetIsEnabled()
			? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		EquipText->SetText(Manager->GetEquipment().Item == Asset
			? INVTEXT("Unequip") : INVTEXT("Equip"));

		ArchiveText->SetText(Asset->ContentText);
	}
	else
	{
		LabelText->SetText(INVTEXT("None Selected"));
		DescText->SetText(INVTEXT("No item selected!"));
		ArchiveText->SetText(INVTEXT("Select an archive to start viewing."));
		ThumbnailImage->SetVisibility(ESlateVisibility::Collapsed);
		EquipButton->SetVisibility(ESlateVisibility::Collapsed);
	}
	ArchivePanel->SetVisibility(bArchiveTab
		? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	PlayAnimation(InfoAnim);
}

void UInventoryWidget::UpdateSlots()
{
	SlotWidgets.Empty();
	ItemContainer->ClearChildren();
	if (!Manager) return;

	TArray<TPair<TObjectPtr<UInventoryAsset>, uint8>> SortedItems;
	const TMap<TSoftObjectPtr<UInventoryAsset>, uint8>& Items = Manager->GetItems();
	SortedItems.Reserve(Items.Num());

	for (const TPair<TSoftObjectPtr<UInventoryAsset>, uint8>& Item : Items)
	{
		if (!Item.Key.IsNull() && Item.Value > 0)
		{
			SortedItems.Add({Item.Key.LoadSynchronous(), Item.Value});
		}
	}

	SortedItems.Sort([](const TPair<TObjectPtr<UInventoryAsset>, uint8>& A, const TPair<TObjectPtr<UInventoryAsset>, uint8>& B)
	{
		if (A.Key->ItemType != B.Key->ItemType)
		{
			return A.Key->ItemType < B.Key->ItemType;
		}
		return A.Key->DisplayName.CompareTo(B.Key->DisplayName) < 0;
	});

	for (const TPair<TObjectPtr<UInventoryAsset>, uint8>& Item : SortedItems)
	{
		if (UInventoryItemWidget* Widget = WidgetTree->ConstructWidget<UInventoryItemWidget>(ItemSlotClass))
		{
			ItemContainer->AddChild(Widget);
			SlotWidgets.Add(Item.Key, Widget);
			Widget->InitItem(this, Item.Key, Item.Value,
				Item.Key == Manager->GetEquipment().Item);
		}
	}

	if (!SlotWidgets.Contains(SelectedItem.Get()))
	{
		SelectedItem = SortedItems.IsEmpty() ? nullptr : SortedItems[0].Key;
	}

	ArchiveContainer->ClearChildren();
	const TArray<TSoftObjectPtr<UInventoryAsset>>& Archives = Manager->GetArchives();
	for (int i = Archives.Num() - 1; i >= 0; i--)
	{
		if (Archives[i].IsNull()) continue;
		if (UInventoryArchiveWidget* Widget = WidgetTree->ConstructWidget<UInventoryArchiveWidget>(ArchiveSlotClass))
		{
			ArchiveContainer->AddChild(Widget);
			SlotWidgets.Add(Archives[i].LoadSynchronous(), Widget);
			Widget->InitArchive(this, Archives[i].LoadSynchronous());
		}
	}

	if (!SlotWidgets.Contains(SelectedArchive.Get()))
	{
		SelectedArchive = Archives.IsEmpty() ? nullptr : Archives[0].LoadSynchronous();
	}

	UpdateInfo();
	PlayAnimation(TabAnim);
}

void UInventoryWidget::GoToTab(const bool bToArchive)
{
	if (bArchiveTab != bToArchive)
	{
		bArchiveTab = bToArchive;
		TabSwitcher->SetActiveWidgetIndex(bArchiveTab ? 1 : 0);
		UpdateInfo();
	}
}

void UInventoryWidget::SetSelected(UInventorySlotWidget* Widget)
{
	if (!Widget || !Widget->GetAsset()) return;
	for (const TPair<TObjectPtr<UInventoryAsset>, TObjectPtr<UInventorySlotWidget>>& SlotInfo : SlotWidgets)
	{
		SlotInfo.Value->SetSelectState(SlotInfo.Value == Widget);
		if (SlotInfo.Value == Widget)
		{
			if (SlotInfo.Value->IsA<UInventoryItemWidget>())
			{
				SelectedItem = SlotInfo.Key;
			}
			else
			{
				SelectedArchive = SlotInfo.Key;
			}
			UpdateInfo();
		}
	}
}

void UInventoryWidget::InitWidget(APlayerController* Controller)
{
	Super::InitWidget(Controller);
	ItemButton->OnClicked.AddUniqueDynamic(this, &UInventoryWidget::OnItemTab);
	ArchiveButton->OnClicked.AddUniqueDynamic(this, &UInventoryWidget::OnArchiveTab);
	EquipButton->OnClicked.AddUniqueDynamic(this, &UInventoryWidget::OnEquipButton);
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		Manager = UInventoryManager::Get(this);
	});
}
