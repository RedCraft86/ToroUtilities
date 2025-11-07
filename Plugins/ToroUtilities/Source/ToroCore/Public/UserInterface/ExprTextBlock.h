// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Widgets/ExpressiveTextRendererWidget.h"
#include "ExprTextBlock.generated.h"

USTRUCT(BlueprintType)
struct TOROCORE_API FExpressiveTextData
{
	GENERATED_BODY()

	FExpressiveTextData();
	int64 CalcChecksum() const;
	FExpressiveText GetExpressiveText();
	void SetText(const FText& InText, const bool bUseFieldsFromAsset = false);
	void SetTextFields(const FExpressiveTextFields& InFields);
	void SetTextAsset(UExpressiveTextAsset* InAsset);
	
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ExpressiveText, meta = (AllowPrivateAccess = true))
		bool bUseAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ExpressiveText, meta = (EditCondition = "!bUseAsset", EditConditionHides, AllowPrivateAccess = true, ShowOnlyInnerProperties))
		FExpressiveTextFields TextFields;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ExpressiveText, meta = (EditCondition = "bUseAsset", EditConditionHides, AllowPrivateAccess = true))
		TObjectPtr<UExpressiveTextAsset> TextAsset;
};

UCLASS(DisplayName = "Expressive Text")
class TOROCORE_API UExprTextBlock final : public UExpressiveTextRendererWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Content, meta = (ShowOnlyInnerProperties))
		FExpressiveTextData ExpressiveText;

	UFUNCTION(BlueprintCallable, Category = "Expressive Text Block")
		void SetText(const FText& InText, const bool bUseFieldsFromAsset = true);

	UFUNCTION(BlueprintCallable, Category = "Expressive Text Block")
		void SetTextFields(const FExpressiveTextFields& InFields);

	UFUNCTION(BlueprintCallable, Category = "Expressive Text Block")
		void SetTextAsset(UExpressiveTextAsset* InAsset);

	/* Run with tick to update any size changes to this text */
	UFUNCTION(BlueprintCallable, Category = "Expressive Text Block")
		void UpdateSize();
	
private:

	int64 CachedChecksum = 0;
	FVector2D CurrentSize{0.0f};
	FVector2D CachedSize{0.0f};

	void UpdateText();
	bool IsCacheInvalid() const;
	virtual void SynchronizeProperties() override;
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override { return NSLOCTEXT("UMG", "Common", "Common"); }
	virtual void OnCreationFromPalette() override
	{
		SetText(NSLOCTEXT("ToroUtilities", "ExprTextBlock", "[32rr](Expressive Text)"));
		SetVisibility(ESlateVisibility::HitTestInvisible);
	}
#endif

public: // Statics for FExpressiveTextData

	UFUNCTION(BlueprintCallable, Category = ExpressiveTextData, DisplayName = "Get Expressive Text")
	static FExpressiveText GetExpressiveTextData(UPARAM(ref) FExpressiveTextData& InData)
	{
		return InData.GetExpressiveText();
	}

	UFUNCTION(BlueprintCallable, Category = ExpressiveTextData, DisplayName = "Set Text")
	static void SetTextData(UPARAM(ref) FExpressiveTextData& InData, const FText InText, const bool bUseFieldsFromAsset = false)
	{
		InData.SetText(InText, bUseFieldsFromAsset);
	}

	UFUNCTION(BlueprintCallable, Category = ExpressiveTextData, DisplayName = "Set Text Fields")
	static void SetTextFieldsData(UPARAM(ref) FExpressiveTextData& InData, const FExpressiveTextFields& InFields)
	{
		InData.SetTextFields(InFields);
	}
	
	UFUNCTION(BlueprintCallable, Category = ExpressiveTextData, DisplayName = "Set Text Asset")
	static void SetTextAssetData(UPARAM(ref) FExpressiveTextData& InData, UExpressiveTextAsset* InAsset)
	{
		InData.SetTextAsset(InAsset);
	}
};
