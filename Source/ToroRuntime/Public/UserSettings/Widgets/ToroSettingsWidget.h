// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "AsyncMessageHandle.h"
#include "NativeGameplayTags.h"
#include "CommonActivatableWidget.h"
#include "ToroSettingsWidget.generated.h"

class UCommonTextBlock;
class UCommonButtonBase;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_SettingRowHover)

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FSettingRowDescriptor final
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Setting)
		FText DisplayName;

	UPROPERTY(EditAnywhere, Category = Setting)
		FText Description;

	UPROPERTY(EditAnywhere, Category = Setting)
		FText Performance;

	FSettingRowDescriptor()
		: DisplayName(FText::GetEmpty()), Description(FText::GetEmpty()), Performance(FText::GetEmpty())
	{}

	FSettingRowDescriptor(const FText& InDisplayName, const FText& InDescription, const FText& InPerformance)
		: DisplayName(InDisplayName), Description(InDescription), Performance(InPerformance)
	{}
};

UCLASS(Abstract, Blueprintable, BlueprintType)
class TORORUNTIME_API UToroSettingsWidget final : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:

	UToroSettingsWidget(const FObjectInitializer& ObjectInit);

protected:

	/** Label for the setting info. REQUIREMENT: A CommonTextBlock named 'SettingLabel'. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (BindWidget))
		TObjectPtr<UCommonTextBlock> SettingLabel;

	/** Description for the setting info. REQUIREMENT: A CommonTextBlock named 'SettingDesc'. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (BindWidget))
		TObjectPtr<UCommonTextBlock> SettingDesc;

	/** Performance Cost for the setting info. REQUIREMENT: A CommonTextBlock named 'SettingCost'. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (BindWidget))
		TObjectPtr<UCommonTextBlock> SettingCost;

	/** Button to apply, save, and exit. REQUIREMENT: A CommonButton named 'ExitButton'. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (BindWidget))
		TObjectPtr<UCommonButtonBase> ExitButton;

	/** Button to automatically adjust scalability. REQUIREMENT: A CommonButton named 'AutoAdjustButton'. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (BindWidget))
		TObjectPtr<UCommonButtonBase> AutoAdjustButton;

	FAsyncMessageHandle HoverListenerHandle;

	void OnExitClicked();
	void OnAutoAdjustClicked();
	void OnRowHovered(const struct FAsyncMessage& Message);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
