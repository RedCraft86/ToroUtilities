// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "CommonTextBlock.h"
#include "CommonButtonBase.h"
#include "CommonLabeledButton.generated.h"

UCLASS(NotBlueprintable, BlueprintType, PrioritizeCategories = (Label, Appearance))
class TOROCORE_API UCommonLabeledButton final : public UCommonButtonBase
{
	GENERATED_BODY()

public:

	UCommonLabeledButton(const FObjectInitializer& ObjectInit);

	UFUNCTION(BlueprintCallable, Category = "Button|Label", DisplayName = "Set Label Text")
		void SetLabelContentText(const FText InText);

	UFUNCTION(BlueprintPure, Category = "Button|Label", DisplayName = "Get Label Text")
		const FText& GetLabelContentText() const { return LabelText; }

	UFUNCTION(BlueprintCallable, Category = "Button|Label")
		void SetLabelColor(const FLinearColor& InColor);

	UFUNCTION(BlueprintPure, Category = "Button|Label")
		const FLinearColor& GetLabelColor() const { return LabelShadowColor; }

	UFUNCTION(BlueprintCallable, Category = "Button|Label")
		void SetLabelStyle(const TSubclassOf<UCommonTextStyle> InStyle);

	UFUNCTION(BlueprintPure, Category = "Button|Label")
	    TSubclassOf<UCommonTextStyle> GetLabelStyle() const { return LabelStyle; }

	UFUNCTION(BlueprintCallable, Category = "Button|Label|Font")
	    void SetLabelFont(const FSlateFontInfo& InFont);

	UFUNCTION(BlueprintPure, Category = "Button|Label|Font")
	    const FSlateFontInfo& GetLabelFont() const { return LabelFont; }

	UFUNCTION(BlueprintCallable, Category = "Button|Label|Font")
	    void SetLabelStrikeBrush(const FSlateBrush& InBrush);

	UFUNCTION(BlueprintPure, Category = "Button|Label|Font")
	    const FSlateBrush& GetLabelStrikeBrush() const { return LabelStrikeBrush; }

	UFUNCTION(BlueprintCallable, Category = "Button|Label|Font")
	    void SetLabelShadowOffset(const FVector2D& InOffset);

	UFUNCTION(BlueprintPure, Category = "Button|Label|Font")
	    const FVector2D& GetLabelShadowOffset() const { return LabelShadowOffset; }

	UFUNCTION(BlueprintCallable, Category = "Button|Label|Font")
	    void SetLabelShadowColor(const FLinearColor& InColor);

	UFUNCTION(BlueprintPure, Category = "Button|Label|Font")
	    const FLinearColor& GetLabelShadowColor() const { return LabelShadowColor; }

	UFUNCTION(BlueprintCallable, Category = "Button|Label|Font")
	    void SetLabelTransformPolicy(const ETextTransformPolicy InPolicy);

	UFUNCTION(BlueprintPure, Category = "Button|Label|Font")
	    ETextTransformPolicy GetLabelTransformPolicy() const { return LabelTransformPolicy; }

	UFUNCTION(BlueprintCallable, Category = "Button|Label|Font")
	    void SetLabelJustification(const ETextJustify::Type InJustification);

	UFUNCTION(BlueprintPure, Category = "Button|Label|Font")
	    ETextJustify::Type GetLabelJustification() const { return LabelJustification; }

	UFUNCTION(BlueprintCallable, Category = "Button|Label|Wrapping")
	    void SetLabelAutoWrapText(const bool bInAutoWrap);

	UFUNCTION(BlueprintPure, Category = "Button|Label|Wrapping")
	    bool GetLabelAutoWrapText() const { return bLabelAutoWrapText; }

	UFUNCTION(BlueprintCallable, Category = "Button|Label|Wrapping")
	    void SetLabelWrapTextAt(const float InWrapAt);

	UFUNCTION(BlueprintPure, Category = "Button|Label|Wrapping")
	    float GetLabelWrapTextAt() const { return LabelWrapTextAt; }

	UFUNCTION(BlueprintCallable, Category = "Button|Label|Wrapping")
	    void SetLabelWrappingPolicy(const ETextWrappingPolicy InPolicy);

	UFUNCTION(BlueprintPure, Category = "Button|Label|Wrapping")
		ETextWrappingPolicy GetLabelWrappingPolicy() const { return LabelWrappingPolicy; }

	UFUNCTION(BlueprintCallable, Category = "Button|Label|Slot")
		void SetLabelPadding(const FMargin& InPadding);

	UFUNCTION(BlueprintPure, Category = "Button|Label|Slot")
		const FMargin& GetLabelPadding() const { return LabelPadding; }

	UFUNCTION(BlueprintCallable, Category = "Button|Label|Slot")
		void SetLabelHorizontalAlignment(const EHorizontalAlignment InAlignment);

	UFUNCTION(BlueprintPure, Category = "Button|Label|Slot")
		EHorizontalAlignment GetLabelHorizontalAlignment() const { return LabelHorizontalAlignment; }

	UFUNCTION(BlueprintCallable, Category = "Button|Label|Slot")
		void SetLabelVerticalAlignment(const EVerticalAlignment InAlignment);

	UFUNCTION(BlueprintPure, Category = "Button|Label|Slot")
		EVerticalAlignment GetLabelVerticalAlignment() const { return LabelVerticalAlignment; }

	UFUNCTION(BlueprintPure, Category = Button)
		FORCEINLINE UCommonTextBlock* GetLabelTextBlock() const { return ButtonLabel; }

protected:

	UPROPERTY(EditAnywhere, Category = "Label", DisplayName = "Text", meta = (MultiLine = true))
		FText LabelText;

	UPROPERTY(EditAnywhere, Category = "Label", DisplayName = "Text Style")
		TSubclassOf<UCommonTextStyle> LabelStyle;

	UPROPERTY(EditAnywhere, Category = "Label", DisplayName = "Text Color")
		FLinearColor LabelColor;

	UPROPERTY(EditAnywhere, Category = "Label|Font", DisplayName = "Font")
		FSlateFontInfo LabelFont;

	UPROPERTY(EditAnywhere, Category = "Label|Font", DisplayName = "Strike Brush")
		FSlateBrush LabelStrikeBrush;

	UPROPERTY(EditAnywhere, Category = "Label|Font", DisplayName = "Shadow Offset")
		FVector2D LabelShadowOffset;

	UPROPERTY(EditAnywhere, Category = "Label|Font", DisplayName = "Shadow Color")
		FLinearColor LabelShadowColor;

	UPROPERTY(EditAnywhere, Category = "Label|Font", DisplayName = "Transform Policy")
		ETextTransformPolicy LabelTransformPolicy;

	UPROPERTY(EditAnywhere, Category = "Label|Font", DisplayName = "Justification")
		TEnumAsByte<ETextJustify::Type> LabelJustification;

	UPROPERTY(EditAnywhere, Category = "Label|Wrapping", DisplayName = "Auto Wrap Text")
		bool bLabelAutoWrapText;

	UPROPERTY(EditAnywhere, Category = "Label|Wrapping", DisplayName = "Wrap Text At", meta = (ClampMin = 0.0f))
		float LabelWrapTextAt;

	UPROPERTY(EditAnywhere, Category = "Label|Wrapping", DisplayName = "Wrapping Policy")
		ETextWrappingPolicy LabelWrappingPolicy;

	UPROPERTY(EditAnywhere, Category = "Label|Slot", DisplayName = "Padding")
		FMargin LabelPadding;

	UPROPERTY(EditAnywhere, Category = "Label|Slot", DisplayName = "Horizontal Alignment")
		TEnumAsByte<EHorizontalAlignment> LabelHorizontalAlignment;

	UPROPERTY(EditAnywhere, Category = "Label|Slot", DisplayName = "Vertical Alignment")
		TEnumAsByte<EVerticalAlignment> LabelVerticalAlignment;

	UPROPERTY(Transient, DuplicateTransient, TextExportTransient)
		TObjectPtr<UCommonTextBlock> ButtonLabel;

	void ApplyLabelSettings();
	const UCommonTextStyle* GetLabelStyleCDO() const;

	virtual bool Initialize() override;
	virtual void SynchronizeProperties() override;
};
