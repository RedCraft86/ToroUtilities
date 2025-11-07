// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserSettings/ImageFidelity.h"
#include "UserSettings/OptionBindingBase.h"
#include "ResolutionBindings.generated.h"

USTRUCT(BlueprintType, DisplayName = "Resolution")
struct TORORUNTIME_API FResolutionBindings : public FDropdownOptionBinding
{
	GENERATED_BODY()

	FResolutionBindings();

	virtual FString GetValue() const override;
	virtual void SetValue(const FString InValue) override;
	virtual TArray<FString> GetOptions() const override { return Options; }

protected:

	FString OldOption;

	virtual void InitBinding() override;

	void RevertValue() const;
	void ApplyInternal(const FIntPoint& Resolution) const;
	static FIntPoint DecomposeResolution(const FString& Option);
};

USTRUCT(BlueprintType, DisplayName = "Borderless")
struct TORORUNTIME_API FBorderlessBinding : public FToggleOptionBinding
{
	GENERATED_BODY()

	FBorderlessBinding();

	virtual bool GetValue() const override;
	virtual void SetValue(const bool InValue) override;

protected:

	bool bOldValue = false;
	void RevertValue() const;
	void ApplyInternal(const bool bBorderless) const;
};

USTRUCT(BlueprintType, DisplayName = "Resolution Scale")
struct TORORUNTIME_API FResolutionScaleBinding : public FSliderOptionBinding
{
	GENERATED_BODY()

	FResolutionScaleBinding();

	virtual float GetValue() const override { return GetSettings()->GetResolutionPercent(); }
	virtual void SetValue(const float InValue) override { GetSettings()->SetResolutionPercent(InValue); }
	virtual bool ShouldHide() const override
	{
		return Super::ShouldHide() || ImageFidelity::IsUsingAnyExternalUpscaler();
	}
};