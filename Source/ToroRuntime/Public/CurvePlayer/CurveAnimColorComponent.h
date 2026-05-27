// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "CurveAnimBaseComponent.h"
#include "CurveAnimColorComponent.generated.h"

/** 
 * Evaluates FLinearColor curves (RGBA). 
 */
UCLASS(NotBlueprintable, DisplayName = "Curve Player (Linear Color)", ClassGroup = (Animation), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UCurveAnimColorComponent final : public UCurveAnimBaseComponent
{
	GENERATED_BODY()

public:

	UCurveAnimColorComponent();

	/** Assigns a new inline color curve and resets time range. */
	UFUNCTION(BlueprintCallable, Category = CurvePlayer)
		void SetCurve(const FInlineColorCurve& InCurve);

	/** @return Reference to the inline color curve. */
	UFUNCTION(BlueprintPure, Category = CurvePlayer)
		const FInlineColorCurve& GetCurve() const { return Curve; }

	/** @return The color value evaluated at the current playback time. */
	UFUNCTION(BlueprintPure, Category = CurvePlayer)
		FLinearColor GetValue() const { return Curve.GetValue(CurrentTime); }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEvaluateTickDelegateBP, const FLinearColor, Value, const float, Time);
	/** Fired every frame the curve updates with the current color value and time. */
	UPROPERTY(BlueprintAssignable, DisplayName = "On Evaluate Tick")
		FOnEvaluateTickDelegateBP OnEvaluateTickBP;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEvaluateTickDelegate, const FLinearColor, const float);
	FOnEvaluateTickDelegate OnEvaluateTick;

	virtual bool IsValidCurve() const override { return Super::IsValidCurve() && Curve.HasAnyData(); }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		FInlineColorCurve Curve;

	virtual void BroadcastEval() const override;
	virtual void BeginPlay() override;
};
