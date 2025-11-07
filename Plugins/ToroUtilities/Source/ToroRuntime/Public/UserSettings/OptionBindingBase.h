// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroUserSettings.h"
#include "Framework/ToroGameInstance.h"
#include "StructUtils/InstancedStruct.h"
#include "OptionBindingBase.generated.h"

UENUM(BlueprintType)
enum class EUserOptionImpact : uint8
{
	None,
	Low,
	Medium,
	High,
	Varies
};

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Option Bindings", meta = (Hidden))
struct TORORUNTIME_API FOptionBindingBase
{
	GENERATED_BODY()

	virtual ~FOptionBindingBase() = default;
	FOptionBindingBase(): Impact(EUserOptionImpact::None)
	{
		Name = INVTEXT("Invalid Option");
		Tooltip = INVTEXT("This option is not configured properly!");
	}

	UPROPERTY(EditAnywhere, Category = Option)
		FText Name;

	UPROPERTY(EditAnywhere, Category = Option)
		FText Tooltip;

	UPROPERTY(EditAnywhere, Category = Option)
		EUserOptionImpact Impact;

	FText GetImpactText() const
	{
		switch (Impact)
		{
			case EUserOptionImpact::Low:	return INVTEXT("Low");
			case EUserOptionImpact::Medium:	return INVTEXT("Medium");
			case EUserOptionImpact::High:	return INVTEXT("High");
			case EUserOptionImpact::Varies:	return INVTEXT("Varies by Scenario");
			default:						return FText::GetEmpty();
		}
	}

	virtual FText GetFormattedTooltip() const
	{
		FTextBuilder Builder;
		Builder.AppendLine(Tooltip);

		const FText ImpactText = GetImpactText();
		if (!ImpactText.IsEmptyOrWhitespace())
		{
			Builder.AppendLineFormat(INVTEXT("\nImpact: {0}"), ImpactText);
		}
		
		return Builder.ToText();
	}

	virtual bool ShouldHide() const { return !Settings; }
	virtual void InitBinding() { Settings = UToroUserSettings::Get(); }

	UToroUserSettings* GetSettings() const { return Settings; }
	UToroGameInstance* GetGameInstance() const
	{
		return Settings ? Cast<UToroGameInstance>(Settings->GetGameInstance()) : nullptr;
	}

private:

	TObjectPtr<UToroUserSettings> Settings;
};

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Toggles", meta = (Hidden))
struct TORORUNTIME_API FToggleOptionBinding : public FOptionBindingBase
{
	GENERATED_BODY()

	FToggleOptionBinding() {}
	virtual bool GetValue() const { return false; }
	virtual void SetValue(const bool InValue) {}
};

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Slider", meta = (Hidden))
struct TORORUNTIME_API FSliderOptionBinding : public FOptionBindingBase
{
	GENERATED_BODY()

	FSliderOptionBinding(): bConstantUpdate(false), SliderRange(0, 100), MaxDecimals(0) {}

	UPROPERTY(EditAnywhere, Category = Option, meta = (DisplayPriority = 1))
		bool bConstantUpdate;

	UPROPERTY(EditAnywhere, Category = Option, meta = (DisplayPriority = 1))
		FVector2D SliderRange;

	UPROPERTY(EditAnywhere, Category = Option, meta = (DisplayPriority = 1))
		uint8 MaxDecimals;

	virtual float GetValue() const { return 0.0f; }
	virtual void SetValue(const float InValue) {}
};

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Swappers", meta = (Hidden))
struct TORORUNTIME_API FSwapperOptionBinding : public FOptionBindingBase
{
	GENERATED_BODY()

	FSwapperOptionBinding(): Options(LMHEC) {}

	UPROPERTY(EditAnywhere, Category = Option, meta = (DisplayPriority = 1))
		TArray<FName> Options;

	UPROPERTY(EditAnywhere, Category = Option, meta = (DisplayPriority = 1, GetOptions = "Options"))
		TMap<FName, FText> OptionTooltips;

	virtual FText GetFormattedTooltip() const override
	{
		if (OptionTooltips.IsEmpty())
		{
			return Super::GetFormattedTooltip();
		}

		FTextBuilder Builder;
		Builder.AppendLine(Tooltip);

		const uint8 Idx = GetValue();
		if (Options.IsValidIndex(Idx) && OptionTooltips.Contains(Options[Idx]))
		{
			Builder.AppendLineFormat(INVTEXT("\n{0}: {1}"),
				FText::FromName(Options[Idx]), OptionTooltips.FindRef(Options[Idx]));
		}

		const FText ImpactText = GetImpactText();
		if (!ImpactText.IsEmptyOrWhitespace())
		{
			Builder.AppendLineFormat(INVTEXT("\n{0}"), ImpactText);
		}
		
		return Builder.ToText();
	}

	virtual TArray<FName> GetOptions() const { return Options; }
	virtual uint8 GetValue() const { return 0; }
	virtual void SetValue(const uint8 InValue) {}

	static inline TArray<FName> OLMH = {"Off", "Low", "Medium", "High"};
	static inline TArray<FName> LMHEC = {"Low", "Medium", "High", "Epic", "Cinematic"};
};

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Dropdowns", meta = (Hidden))
struct TORORUNTIME_API FDropdownOptionBinding : public FOptionBindingBase
{
	GENERATED_BODY()

	FDropdownOptionBinding() {}

	UPROPERTY(EditAnywhere, Category = Option, meta = (DisplayPriority = 1))
		TArray<FString> Options;

	UPROPERTY(EditAnywhere, Category = Option, meta = (DisplayPriority = 1, GetOptions = "Options"))
		TMap<FString, FText> OptionTooltips;

	virtual FText GetFormattedTooltip() const override
	{
		if (OptionTooltips.IsEmpty())
		{
			return Super::GetFormattedTooltip();
		}

		FTextBuilder Builder;
		Builder.AppendLine(Tooltip);

		const FString Val = GetValue();
		if (Options.Contains(Val) && OptionTooltips.Contains(Val))
		{
			Builder.AppendLineFormat(INVTEXT("\n{0}: {1}"),
				FText::FromString(Val), OptionTooltips.FindRef(Val));
		}

		const FText ImpactText = GetImpactText();
		if (!ImpactText.IsEmptyOrWhitespace())
		{
			Builder.AppendLineFormat(INVTEXT("\n{0}"), ImpactText);
		}
		
		return Builder.ToText();
	}

	virtual TArray<FString> GetOptions() const { return {}; }
	virtual FString GetValue() const { return TEXT(""); }
	virtual void SetValue(const FString InValue) {}
};