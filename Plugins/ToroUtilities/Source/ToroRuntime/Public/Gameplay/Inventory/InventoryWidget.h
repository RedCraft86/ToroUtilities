// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroRuntime.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "DataTypes/GameInputConfig.h"
#include "UserInterface/ExprTextBlock.h"
#include "UserInterface/ToroManagedWidget.h"
#include "InventoryWidget.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UInventorySlotWidget(const FObjectInitializer& ObjectInitializer)
		: UUserWidget(ObjectInitializer)
	{}

	UFUNCTION(BlueprintPure, Category = InventorySlot)
		UInventoryAsset* GetAsset() const { return Asset; }

	UFUNCTION(BlueprintImplementableEvent)
		void SetSelectState(const bool bSelected);
	virtual void SetSelectState_Implementation(const bool bSelected) {}

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> IconImage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> SelectButton;

	UPROPERTY(Transient)
		TObjectPtr<UInventoryAsset> Asset;

	TObjectPtr<class UInventoryWidget> ParentUI;
	
	UFUNCTION() void OnClicked();

	virtual void InitializeWidget(UInventoryWidget* InWidget, UInventoryAsset* InAsset);
};

UCLASS(Abstract)
class TORORUNTIME_API UInventoryItemWidget final : public UInventorySlotWidget
{
	GENERATED_BODY()

public:

	UInventoryItemWidget(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{}

	void InitItem(UInventoryWidget* InWidget, UInventoryAsset* InAsset, const uint8 Amount, const bool bEquipped);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> AmountText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UWidget> EquipVisual;
};

UCLASS(Abstract)
class TORORUNTIME_API UInventoryArchiveWidget final : public UInventorySlotWidget
{
	GENERATED_BODY()

public:

	UInventoryArchiveWidget(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{}

	void InitArchive(UInventoryWidget* InWidget, UInventoryAsset* InAsset);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> NameText;
};

UCLASS(Abstract)
class TORORUNTIME_API UInventoryWidget final : public UToroManagedWidget
{
	GENERATED_BODY()

public:

	UInventoryWidget(const FObjectInitializer& ObjectInitializer);

	void UpdateWidget();
	void SetSelected(UInventorySlotWidget* Widget);
	
	virtual void PushWidget() override;
	virtual void PopWidget() override;

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ItemButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ArchiveButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UWidgetSwitcher> TabSwitcher;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> ItemContainer;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> ArchiveContainer;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> ThumbnailImage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> LabelText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UExprTextBlock> DescText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> EquipButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> EquipText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UWidget> ArchivePanel;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UExprTextBlock> ArchiveText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> TabAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> InfoAnim;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
		TSubclassOf<UInventoryItemWidget> ItemSlotClass;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
		TSubclassOf<UInventoryArchiveWidget> ArchiveSlotClass;

	UPROPERTY(Transient)
		TMap<TObjectPtr<UInventoryAsset>, TObjectPtr<UInventorySlotWidget>> SlotWidgets;

	bool bArchiveTab;
	FGameInputConfig InputConfig;
	TObjectPtr<class UInventoryManager> Manager;

	TWeakObjectPtr<UInventoryAsset> SelectedItem;
	TWeakObjectPtr<UInventoryAsset> SelectedArchive;

	UFUNCTION() void OnEquipButton();
	UFUNCTION() void OnItemTab() { GoToTab(false); }
	UFUNCTION() void OnArchiveTab() { GoToTab(true); }

	void UpdateInfo();
	void UpdateSlots();
	void GoToTab(const bool bToArchive);

	virtual void InitWidget(APlayerController* Controller) override;
	virtual bool CanCreateWidget(const UObject* ContextObject) const override
	{
		return !UToroSettings::Get()->IsOnMap(ContextObject, EToroMapType::MainMenu);
	}
};
