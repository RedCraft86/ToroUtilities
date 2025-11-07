// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "BaseCurvePlayer.h"
#include "DataTypes/MathTypes.h"
#include "CurvePlayerFloat.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Animation), meta = (BlueprintSpawnableComponent))
class TOROCORE_API UCurvePlayerFloat final: public UBaseCurvePlayer
{
	GENERATED_BODY()

public:

	UCurvePlayerFloat();

	UFUNCTION(BlueprintCallable, Category = CurvePlayer)
		void SetCurve(const FInlineFloatCurve& InCurve);

	UFUNCTION(BlueprintPure, Category = CurvePlayer)
		const FInlineFloatCurve& GetCurve() const { return Curve; }

	UFUNCTION(BlueprintPure, Category = CurvePlayer)
		float GetValue() const { return Curve.GetValue(CurrentTime); }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEvaluateTickBP, const float, Value, const float, Time);
	UPROPERTY(BlueprintAssignable, DisplayName = "On Evaluate Tick")
		FOnEvaluateTickBP OnEvaluateTickBP;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEvaluateTick, const float, const float);
	FOnEvaluateTick OnEvaluateTick;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		FInlineFloatCurve Curve;

	virtual void BroadcastEval() const override;
	virtual void BeginPlay() override;
};
