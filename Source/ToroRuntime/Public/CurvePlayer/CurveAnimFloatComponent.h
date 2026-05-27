// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "CurveAnimBaseComponent.h"
#include "CurveAnimFloatComponent.generated.h"

/** 
 * Evaluates standard floating point curves.
 */
UCLASS(NotBlueprintable, DisplayName = "Curve Player (Float)", ClassGroup = (Animation), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UCurveAnimFloatComponent final : public UCurveAnimBaseComponent
{
	GENERATED_BODY()

public:

	UCurveAnimFloatComponent();

	/** Assigns a new inline float curve and resets time range. */
	UFUNCTION(BlueprintCallable, Category = CurvePlayer)
		void SetCurve(const FInlineFloatCurve& InCurve);

	/** @return Reference to the inline float curve. */
	UFUNCTION(BlueprintPure, Category = CurvePlayer)
		const FInlineFloatCurve& GetCurve() const { return Curve; }

	/** @return The float value evaluated at the current playback time. */
	UFUNCTION(BlueprintPure, Category = CurvePlayer)
		float GetValue() const { return Curve.GetValue(CurrentTime); }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEvaluateTickDelegateBP, const float, Value, const float, Time);
	/** Fired every frame the curve updates with the current float value and time. */
	UPROPERTY(BlueprintAssignable, DisplayName = "On Evaluate Tick")
		FOnEvaluateTickDelegateBP OnEvaluateTickBP;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEvaluateTickDelegate, const float, const float);
	FOnEvaluateTickDelegate OnEvaluateTick;

	virtual bool IsValidCurve() const override { return Super::IsValidCurve() && Curve.HasAnyData(); }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		FInlineFloatCurve Curve;

	virtual void BroadcastEval() const override;
	virtual void BeginPlay() override;
};
