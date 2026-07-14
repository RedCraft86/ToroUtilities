// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "CommonUserWidget.h"
#include "UserSettings/Providers/UserSettingsProvider.h"
#include "ToroSettingRow.generated.h"

class UCommonTextBlock;
class UCommonButtonBase;

UCLASS(Abstract, NotBlueprintable, BlueprintType)
class TORORUNTIME_API UToroSettingRowBase : public UCommonUserWidget
{
	GENERATED_BODY()

public:

	UToroSettingRowBase(const FObjectInitializer& ObjectInit);

	/** Info and access provider used for this setting row. */
	UPROPERTY(EditAnywhere, Category = Setting, meta = (ShowTreeView))
		TInstancedStruct<FUserSettingsProviderBase> Provider;

protected:

	/** Label for the setting name. REQUIREMENT: A CommonTextBlock named 'SettingLabel'. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (BindWidget))
		TObjectPtr<UCommonTextBlock> SettingLabel;

	/** Label for the setting name. REQUIREMENT: A CommonButton named 'RevertButton'. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (BindWidget))
		TObjectPtr<UCommonButtonBase> RevertButton;

	UPROPERTY(Transient)
		TObjectPtr<const UScriptStruct> AllowedStruct;

	void OnResetClicked();
	void OnSettingsApplied(const UToroGameUserSettings* Settings, const EUserSettingApplyType Type);
	void CheckResettability() const;

	virtual void UpdateSettingRow() {}

	virtual void NativeConstruct() override;
	virtual void SynchronizeProperties() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const override;
#endif
};

UCLASS(Abstract, Blueprintable, BlueprintType)
class TORORUNTIME_API UToroSettingRow_Toggle final : public UToroSettingRowBase
{
	GENERATED_BODY()

public:

	UToroSettingRow_Toggle(const FObjectInitializer& ObjectInit);

protected:

	/** Button for the toggle itself. REQUIREMENT: A CommonButton named 'ToggleButton'. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (BindWidget))
		TObjectPtr<UCommonButtonBase> ToggleButton;

	/** Animation for the toggle where Start = Off and End = On. REQUIREMENT: A WidgetAnimation named 'ToggleAnim'. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> ToggleAnim;

	void OnToggleClicked();

	virtual void UpdateSettingRow() override;
	virtual void NativeConstruct() override;
};

UCLASS(Abstract, Blueprintable, BlueprintType)
class TORORUNTIME_API UToroSettingRow_Slider final : public UToroSettingRowBase
{
	GENERATED_BODY()

public:

	UToroSettingRow_Slider(const FObjectInitializer& ObjectInit);

protected:

	/** Spin box for the toggle itself. REQUIREMENT: A SpinBox named 'SpinSlider'. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (BindWidget))
		TObjectPtr<class USpinBox> SpinSlider;

	UFUNCTION() void OnValueChanged(float InValue);
	UFUNCTION() void OnValueCommitted(float InValue, ETextCommit::Type CommitMethod);

	virtual void UpdateSettingRow() override;
	virtual void NativeConstruct() override;
};

UCLASS(Abstract, Blueprintable, BlueprintType)
class TORORUNTIME_API UToroSettingRow_Swapper final : public UToroSettingRowBase
{
	GENERATED_BODY()

public:

	UToroSettingRow_Swapper(const FObjectInitializer& ObjectInit);

protected:

	/** Left button for the swapper. REQUIREMENT: A CommonButton named 'LeftButton'. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (BindWidget))
		TObjectPtr<UCommonButtonBase> LeftButton;

	/** Right button for the swapper. REQUIREMENT: A CommonButton named 'RightButton'. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (BindWidget))
		TObjectPtr<UCommonButtonBase> RightButton;

	/** Option label for the swapper. REQUIREMENT: A CommonTextBlock named 'OptionLabel'. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (BindWidget))
		TObjectPtr<UCommonTextBlock> OptionLabel;

	void OnLeftButtonClicked();
	void OnRightButtonClicked();

	virtual void UpdateSettingRow() override;
	virtual void NativeConstruct() override;
};

UCLASS(Abstract, Blueprintable, BlueprintType)
class TORORUNTIME_API UToroSettingRow_Selector final : public UToroSettingRowBase
{
	GENERATED_BODY()

public:

	UToroSettingRow_Selector(const FObjectInitializer& ObjectInit);

protected:

	/** Combo box that is the core of the selector. REQUIREMENT: A ComboBoxString named 'SelectorBox'. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (BindWidget))
		TObjectPtr<class UComboBoxString> SelectorBox;

	UFUNCTION() void OnValueSelected(FString SelectedItem, ESelectInfo::Type SelectionType);

	virtual void UpdateSettingRow() override;
	virtual void NativeConstruct() override;
};