// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserSettings/ImageFidelity.h"
#include "UserSettings/OptionBindingBase.h"
#include "FidelityBindings.generated.h"

USTRUCT(BlueprintType, DisplayName = "ImageFidelity")
struct TORORUNTIME_API FImageFidelityBindings : public FDropdownOptionBinding
{
	GENERATED_BODY()

	FImageFidelityBindings();

	virtual FString GetValue() const override;
	virtual void SetValue(const FString InValue) override;
	virtual TArray<FString> GetOptions() const override { return Options; }

protected:

	virtual void InitBinding() override;
};

USTRUCT(BlueprintType, DisplayName = "FSR Quality")
struct TORORUNTIME_API FFSRQualityBinding : public FSwapperOptionBinding
{
	GENERATED_BODY()

	FFSRQualityBinding();

	virtual uint8 GetValue() const override { return GetSettings()->GetFSRQuality(); }
	virtual void SetValue(const uint8 InValue) override { GetSettings()->SetFSRQuality(InValue); }
	virtual bool ShouldHide() const override
	{
		return Super::ShouldHide() || GetSettings()->GetImageFidelity() != EImageFidelityMode::FSR;
	}
};

USTRUCT(BlueprintType, DisplayName = "FSR Sharpness")
struct TORORUNTIME_API FFSRSharpnessBinding : public FSliderOptionBinding
{
	GENERATED_BODY()

	FFSRSharpnessBinding();

	virtual float GetValue() const override { return GetSettings()->GetFSRSharpness(); }
	virtual void SetValue(const float InValue) override { GetSettings()->SetFSRSharpness(InValue); }
	virtual bool ShouldHide() const override
	{
		return Super::ShouldHide() || GetSettings()->GetImageFidelity() != EImageFidelityMode::FSR;
	}
};

USTRUCT(BlueprintType, DisplayName = "FSR Frame Generation")
struct TORORUNTIME_API FFSRFrameGenBinding : public FToggleOptionBinding
{
	GENERATED_BODY()

	FFSRFrameGenBinding();

	virtual bool GetValue() const override { return GetSettings()->GetFSRFrameGen(); }
	virtual void SetValue(const bool InValue) override { GetSettings()->SetFSRFrameGen(InValue); }
	virtual bool ShouldHide() const override
	{
		return Super::ShouldHide() || GetSettings()->GetImageFidelity() != EImageFidelityMode::FSR;
	}
};

USTRUCT(BlueprintType, DisplayName = "XeSS Quality")
struct TORORUNTIME_API FXeSSQualityBinding : public FSwapperOptionBinding
{
	GENERATED_BODY()

	FXeSSQualityBinding();

	// XeSS's quality presets are ordered reverse of FSR's, so it needs to be flipped
	virtual uint8 GetValue() const override { return 6 - GetSettings()->GetXeSSQuality(); }
	virtual void SetValue(const uint8 InValue) override { GetSettings()->SetXeSSQuality(6 - InValue); }
	virtual bool ShouldHide() const override
	{
		return Super::ShouldHide() || GetSettings()->GetImageFidelity() != EImageFidelityMode::XeSS;
	}
};

USTRUCT(BlueprintType, DisplayName = "XeSS Frame Generation")
struct TORORUNTIME_API FXeSSFrameGenBinding : public FToggleOptionBinding
{
	GENERATED_BODY()

	FXeSSFrameGenBinding();

	virtual bool GetValue() const override { return GetSettings()->GetXeSSFrameGen(); }
	virtual void SetValue(const bool InValue) override { GetSettings()->SetXeSSFrameGen(InValue); }
	virtual bool ShouldHide() const override
	{
		return Super::ShouldHide() || !ImageFidelity::XeSS::FG::IsSupported()
			|| GetSettings()->GetImageFidelity() != EImageFidelityMode::XeSS;
	}
};

USTRUCT(BlueprintType, DisplayName = "XeSS Low Latency")
struct TORORUNTIME_API FXeSSLowLatencyBinding : public FToggleOptionBinding
{
	GENERATED_BODY()

	FXeSSLowLatencyBinding();

	virtual bool GetValue() const override { return GetSettings()->GetXeSSLowLatency(); }
	virtual void SetValue(const bool InValue) override { GetSettings()->SetXeSSLowLatency(InValue); }
	virtual bool ShouldHide() const override
	{
		return Super::ShouldHide() || !ImageFidelity::XeSS::LL::IsSupported()
			|| GetSettings()->GetImageFidelity() != EImageFidelityMode::XeSS;
	}
};

USTRUCT(BlueprintType, DisplayName = "DLSS Quality")
struct TORORUNTIME_API FDLSSQualityBinding : public FSwapperOptionBinding
{
	GENERATED_BODY()

	FDLSSQualityBinding();

	virtual uint8 GetValue() const override;
	virtual void SetValue(const uint8 InValue) override;
	virtual TArray<FName> GetOptions() const override { return AcceptedOptions; }
	virtual bool ShouldHide() const override
	{
		return Super::ShouldHide() || GetSettings()->GetImageFidelity() != EImageFidelityMode::DLSS;
	}

private:
	
	TArray<FName> AcceptedOptions;
	virtual void InitBinding() override;
};

USTRUCT(BlueprintType, DisplayName = "DLSS Frame Generation")
struct TORORUNTIME_API FDLSSFrameGenBinding : public FSwapperOptionBinding
{
	GENERATED_BODY()

	FDLSSFrameGenBinding();

	virtual uint8 GetValue() const override;
	virtual void SetValue(const uint8 InValue) override;
	virtual TArray<FName> GetOptions() const override { return AcceptedOptions; }
	virtual bool ShouldHide() const override
	{
		return Super::ShouldHide() || !ImageFidelity::DLSS::FG::IsSupported()
			|| GetSettings()->GetImageFidelity() != EImageFidelityMode::DLSS;
	}

private:
	
	TArray<FName> AcceptedOptions;
	virtual void InitBinding() override;
};

USTRUCT(BlueprintType, DisplayName = "DLSS Reflex")
struct TORORUNTIME_API FDLSSReflexBinding : public FSwapperOptionBinding
{
	GENERATED_BODY()

	FDLSSReflexBinding();

	virtual uint8 GetValue() const override;
	virtual void SetValue(const uint8 InValue) override;
	virtual TArray<FName> GetOptions() const override { return AcceptedOptions; }
	virtual bool ShouldHide() const override
	{
		return Super::ShouldHide() || !ImageFidelity::DLSS::Reflex::IsSupported()
			|| GetSettings()->GetImageFidelity() != EImageFidelityMode::DLSS;
	}

private:
	
	TArray<FName> AcceptedOptions;
	virtual void InitBinding() override;
};

USTRUCT(BlueprintType, DisplayName = "DLSS Ray Reconstruction")
struct TORORUNTIME_API FDLSSRayReconstructBinding : public FToggleOptionBinding
{
	GENERATED_BODY()

	FDLSSRayReconstructBinding();

	virtual bool GetValue() const override { return GetSettings()->GetDLSSRayReconstruct(); }
	virtual void SetValue(const bool InValue) override { GetSettings()->SetDLSSRayReconstruct(InValue); }
	virtual bool ShouldHide() const override
	{
		return Super::ShouldHide() || !ImageFidelity::DLSS::RR::IsSupported()
			|| GetSettings()->GetImageFidelity() != EImageFidelityMode::DLSS;
	}
};