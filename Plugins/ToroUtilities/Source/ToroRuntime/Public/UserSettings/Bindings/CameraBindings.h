// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserSettings/OptionBindingBase.h"
#include "CameraBindings.generated.h"

USTRUCT(BlueprintType, DisplayName = "Smooth Camera")
struct TORORUNTIME_API FSmoothCameraBinding : public FToggleOptionBinding
{
	GENERATED_BODY()

	FSmoothCameraBinding()
	{
		Name = INVTEXT("Smooth Camera");
		Tooltip = INVTEXT("Smooth the camera rotation and make it feel less sharp. May cause snapping when turning quickly.");
	}

	virtual bool GetValue() const override { return GetSettings()->GetSmoothCamera(); }
	virtual void SetValue(const bool InValue) override { GetSettings()->SetSmoothCamera(InValue); }
};

USTRUCT(BlueprintType, DisplayName = "Sensitivity X")
struct TORORUNTIME_API FSensitivityXBinding : public FSliderOptionBinding
{
	GENERATED_BODY()

	FSensitivityXBinding()
	{
		Name = INVTEXT("Sensitivity X");
		Tooltip = INVTEXT("The turning speed in the X direction.");
		SliderRange = {0.5f, 2.0f};
		MaxDecimals = 1;
	}

	virtual float GetValue() const override { return GetSettings()->GetRawSensitivity().X; }
	virtual void SetValue(const float InValue) override { GetSettings()->SetSensitivityX(InValue); }
};

USTRUCT(BlueprintType, DisplayName = "Invert Mouse X")
struct TORORUNTIME_API FInvertMouseXBinding : public FToggleOptionBinding
{
	GENERATED_BODY()

	FInvertMouseXBinding()
	{
		Name = INVTEXT("Invert");
		Tooltip = INVTEXT("Invert turning in the X direction.");
	}

	virtual bool GetValue() const override { return GetSettings()->GetInvertMouse().X; }
	virtual void SetValue(const bool InValue) override { GetSettings()->SetInvertMouseX(InValue); }
};

USTRUCT(BlueprintType, DisplayName = "Sensitivity Y")
struct TORORUNTIME_API FSensitivityYBinding : public FSliderOptionBinding
{
	GENERATED_BODY()

	FSensitivityYBinding()
	{
		Name = INVTEXT("Sensitivity Y");
		Tooltip = INVTEXT("The turning speed in the Y direction.");
		SliderRange = {0.5f, 2.0f};
		MaxDecimals = 1;
	}

	virtual float GetValue() const override { return GetSettings()->GetRawSensitivity().Y; }
	virtual void SetValue(const float InValue) override { GetSettings()->SetSensitivityY(InValue); }
};

USTRUCT(BlueprintType, DisplayName = "Invert Mouse Y")
struct TORORUNTIME_API FInvertMouseYBinding : public FToggleOptionBinding
{
	GENERATED_BODY()

	FInvertMouseYBinding()
	{
		Name = INVTEXT("Invert");
		Tooltip = INVTEXT("Invert turning in the Y direction.");
	}

	virtual bool GetValue() const override { return GetSettings()->GetInvertMouse().Y; }
	virtual void SetValue(const bool InValue) override { GetSettings()->SetInvertMouseY(InValue); }
};

