// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "CommonUserWidget.h"
#include "UserSettings/Providers/UserSettingsProvider.h"
#include "SettingRowBase.generated.h"

class UCommonButtonStyle;
class UCommonBorderStyle;
class UCommonTextStyle;
class UCommonBorder;
class UHorizontalBox;
class UCommonTextBlock;
class UCommonButtonBase;

UCLASS(Abstract, NotBlueprintable, BlueprintType)
class TORORUNTIME_API USettingRowBase : public UCommonUserWidget
{
	GENERATED_BODY()

public:

	USettingRowBase(const FObjectInitializer& ObjectInit);

	/** Info and access provider used for this setting row. */
	UPROPERTY(EditAnywhere, Category = Setting, meta = (ShowTreeView))
		TInstancedStruct<FUserSettingsProviderBase> Provider;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOptionHoveredDelegate, const TInstancedStruct<FUserSettingsProviderBase>&)
	FOptionHoveredDelegate OnHovered;

protected:

	/** Style of the Background. */
	UPROPERTY(EditAnywhere, Category = Appearance)
		TSubclassOf<UCommonBorderStyle> BorderStyle;

	/** Style of the Label Text. */
	UPROPERTY(EditAnywhere, Category = Appearance)
		TSubclassOf<UCommonTextStyle> LabelStyle;

	/** Style of the Revert Button. */
	UPROPERTY(EditAnywhere, Category = Appearance)
		TSubclassOf<UCommonButtonStyle> RevertStyle;

	UPROPERTY(Transient, DuplicateTransient, TextExportTransient)
		TObjectPtr<UCommonBorder> Background;

	UPROPERTY(Transient, DuplicateTransient, TextExportTransient)
		TObjectPtr<UHorizontalBox> RowContainer;

	UPROPERTY(Transient, DuplicateTransient, TextExportTransient)
		TObjectPtr<UCommonTextBlock> RowLabel;

	UPROPERTY(Transient, DuplicateTransient, TextExportTransient)
		TObjectPtr<UHorizontalBox> ContentContainer;

	UPROPERTY(Transient, DuplicateTransient, TextExportTransient)
		TObjectPtr<UCommonButtonBase> RevertButton;

	UPROPERTY(Transient)
		TObjectPtr<const UScriptStruct> AllowedStruct;

	virtual void ConstructSettings(UHorizontalBox* Container) {}

	virtual bool Initialize() override;
	virtual void SynchronizeProperties() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const override;
#endif
};
