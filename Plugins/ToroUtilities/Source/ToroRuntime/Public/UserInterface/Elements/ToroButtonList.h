// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Blueprint/UserWidget.h"
#include "ToroButtonList.generated.h"

UCLASS(Abstract, NotPlaceable)
class TORORUNTIME_API UToroButtonListEntry final : public UUserWidget
{
	GENERATED_BODY()

	friend class UToroButtonList;

public:

	UToroButtonListEntry(const FObjectInitializer& ObjectInitializer)
		: UUserWidget(ObjectInitializer), bSelected(false)
	{}

	DECLARE_DELEGATE_OneParam(FOnClicked, UToroButtonListEntry*);
	FOnClicked OnClicked;

	void SelectButton(bool bImmediate = false);
	void DeselectButton(bool bImmediate = false);
	void SetDisplayData(const FText& InText, const FSlateFontInfo& InFont) const;

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> Button;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> Label;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SelectAnim;

	bool bSelected;

	UFUNCTION() void OnButtonClicked();
	void SyncSelectionState(const bool bImmediate);

	virtual void NativeConstruct() override;
};

UCLASS(DisplayName = "Button List", PrioritizeCategories = ButtonList)
class TORORUNTIME_API UToroButtonList final : public UVerticalBox
{
	GENERATED_BODY()

public:

	UToroButtonList(const FObjectInitializer& ObjectInitializer);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectionBP, const int32, Index);
	UPROPERTY(BlueprintAssignable, DisplayName = "On Selection") FOnSelectionBP OnSelectionBP;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelection, const int32);
	FOnSelection OnSelection;

	UFUNCTION(BlueprintPure, Category = ButtonList)
		int32 GetSelectedIndex() const { return SelectedIndex; }

protected:

	UPROPERTY(EditAnywhere, Category = ButtonList, NoClear)
		TSubclassOf<UToroButtonListEntry> EntryClass;

	UPROPERTY(EditAnywhere, Category = ButtonList)
		FSlateChildSize EntrySize;

	UPROPERTY(EditAnywhere, Category = ButtonList)
		FMargin EntryPadding;

	UPROPERTY(EditAnywhere, Category = ButtonList)
		FSlateFontInfo EntryFont;

	UPROPERTY(EditAnywhere, Category = ButtonList)
		TArray<FText> Entries;

	UPROPERTY(Transient)
		TArray<TObjectPtr<UToroButtonListEntry>> Buttons;

	int32 SelectedIndex;

	void OnSelectionMade(UToroButtonListEntry* Button);
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override { return NSLOCTEXT("UMG", "Input", "Input"); }
#endif
};
