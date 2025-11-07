// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/Button.h"
#include "Components/SpinBox.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboBoxString.h"
#include "UserSettings/OptionBindingBase.h"
#include "SettingRowWidgets.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UToroSettingRow : public UUserWidget
{
	GENERATED_BODY()

public:

	UToroSettingRow(const FObjectInitializer& ObjectInitializer)
		: UUserWidget(ObjectInitializer), bHideWhenDisabled(false)
	{}

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> LabelText;

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bHideWhenDisabled;

	virtual void OnRefreshUI() {}
	virtual FText GetLabelText() { return INVTEXT("Invalid Option"); }

	void SetDisabled(const bool bDisabled)
	{
		SetIsEnabled(!bDisabled);
		SetVisibility(bHideWhenDisabled && bDisabled
			? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}
	void OnSettingsUpdated(const ESettingApplyType Type)
	{
		if (Type == ESettingApplyType::UI)
		{
			OnRefreshUI();
		}
	}

	virtual void NativeConstruct() override
	{
		Super::NativeConstruct();
		OnRefreshUI();
	}
	virtual void NativeOnInitialized() override
	{
		Super::NativeOnInitialized();
		if (UToroUserSettings* UserSettings = UToroUserSettings::Get())
		{
			UserSettings->OnSettingsUpdated.AddUObject(this, &UToroSettingRow::OnSettingsUpdated);
		}
	}
#if WITH_EDITOR
	virtual void NativePreConstruct() override
	{
		Super::NativePreConstruct();
		if (LabelText) LabelText->SetText(GetLabelText());
	}
#endif
};

UCLASS(Abstract)
class TORORUNTIME_API UToggleSettingRow : public UToroSettingRow
{
	GENERATED_BODY()

public:

	UToggleSettingRow(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ToggleButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> ToggleAnim;

	UPROPERTY(EditAnywhere, Category = Settings, NoClear, meta = (ExcludeBaseStruct, HideViewOptions))
		TInstancedStruct<FToggleOptionBinding> Binding;

	bool bState;

	UFUNCTION() void ToggleClicked();
	void SyncVisualState(const bool bImmediate);

	virtual void OnRefreshUI() override;
	virtual FText GetLabelText() override;
	virtual void NativeConstruct() override;
};

UCLASS(Abstract)
class TORORUNTIME_API USliderSettingRow : public UToroSettingRow
{
	GENERATED_BODY()

public:

	USliderSettingRow(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USpinBox> SliderBox;

	UPROPERTY(EditAnywhere, Category = Settings, NoClear, meta = (ExcludeBaseStruct, HideViewOptions))
		TInstancedStruct<FSliderOptionBinding> Binding;

	bool bOverriding;

	UFUNCTION() void OnSliderEnd(const float InValue) { SetValue(InValue); }
	UFUNCTION() void OnSliderDynamic(const float InValue) { SetValue(InValue); }
	UFUNCTION() void OnSliderCommit(const float InValue, ETextCommit::Type CommitType) { SetValue(InValue); }
	void SetValue(const float InValue);

	virtual void OnRefreshUI() override;
	virtual FText GetLabelText() override;
	virtual void NativeConstruct() override;
};

UCLASS(Abstract)
class TORORUNTIME_API USwapperSettingRow : public UToroSettingRow
{
	GENERATED_BODY()

public:

	USwapperSettingRow(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> RightButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> LeftButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> OptionLabel;

	UPROPERTY(EditAnywhere, Category = Settings, NoClear, meta = (ExcludeBaseStruct, HideViewOptions))
		TInstancedStruct<FSwapperOptionBinding> Binding;

	uint8 OptionIdx;
	TArray<FName> Options;

	UFUNCTION() void RightArrow();
	UFUNCTION() void LeftArrow();
	void SyncVisualOption();

	virtual void OnRefreshUI() override;
	virtual FText GetLabelText() override;
	virtual void NativeConstruct() override;
};

UCLASS(Abstract)
class TORORUNTIME_API UDropdownSettingRow : public UToroSettingRow
{
	GENERATED_BODY()

public:

	UDropdownSettingRow(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UComboBoxString> DropdownBox;

	UPROPERTY(EditAnywhere, Category = Settings, NoClear, meta = (ExcludeBaseStruct, HideViewOptions))
		TInstancedStruct<FDropdownOptionBinding> Binding;

	bool bOverriding;

	UFUNCTION() void OptionPicked(const FString Option, ESelectInfo::Type Info);

	virtual void OnRefreshUI() override;
	virtual FText GetLabelText() override;
	virtual void NativeConstruct() override;
};
