// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "CurveAnimBaseComponent.h"
#include "CurveAnimVectorComponent.generated.h"
	 
/** 
 * Evaluates FVector curves (XYZ).
 */
UCLASS(NotBlueprintable, DisplayName = "Curve Player (Vector)", ClassGroup = (Animation), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UCurveAnimVectorComponent final : public UCurveAnimBaseComponent
{
	GENERATED_BODY()

public:

	UCurveAnimVectorComponent();

	/** Assigns a new inline vector curve and resets time range. */
	UFUNCTION(BlueprintCallable, Category = CurvePlayer)
		void SetCurve(const FInlineVectorCurve& InCurve);

	/** @return Reference to the inline vector curve. */
	UFUNCTION(BlueprintPure, Category = CurvePlayer)
		const FInlineVectorCurve& GetCurve() const { return Curve; }

	/** @return The vector value evaluated at the current playback time. */
	UFUNCTION(BlueprintPure, Category = CurvePlayer)
		FVector GetValue() const { return Curve.GetValue(CurrentTime); }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEvaluateTickDelegateBP, const FVector, Value, const float, Time);
	/** Fired every frame the curve updates with the current vector value and time. */
	UPROPERTY(BlueprintAssignable, DisplayName = "On Evaluate Tick")
		FOnEvaluateTickDelegateBP OnEvaluateTickBP;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEvaluateTickDelegate, const FVector, const float);
	FOnEvaluateTickDelegate OnEvaluateTick;

	virtual bool IsValidCurve() const override { return Super::IsValidCurve() && Curve.HasAnyData(); }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		FInlineVectorCurve Curve;

	virtual void InitializeCurve() override;
	virtual void BroadcastEval() const override;
};
