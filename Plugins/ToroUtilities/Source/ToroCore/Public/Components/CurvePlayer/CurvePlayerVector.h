// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "BaseCurvePlayer.h"
#include "DataTypes/MathTypes.h"
#include "CurvePlayerVector.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Animation), meta = (BlueprintSpawnableComponent))
class TOROCORE_API UCurvePlayerVector final: public UBaseCurvePlayer
{
	GENERATED_BODY()

public:

	UCurvePlayerVector();

	UFUNCTION(BlueprintCallable, Category = CurvePlayer)
		void SetCurve(const FInlineVectorCurve& InCurve);

	UFUNCTION(BlueprintPure, Category = CurvePlayer)
		const FInlineVectorCurve& GetCurve() const { return Curve; }

	UFUNCTION(BlueprintPure, Category = CurvePlayer)
		FVector GetValue() const { return Curve.GetValue(CurrentTime); }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEvaluateTickBP, const FVector, Value, const float, Time);
	UPROPERTY(BlueprintAssignable, DisplayName = "On Evaluate Tick")
		FOnEvaluateTickBP OnEvaluateTickBP;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEvaluateTick, const FVector, const float);
	FOnEvaluateTick OnEvaluateTick;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		FInlineVectorCurve Curve;

	virtual void BroadcastEval() const override;
	virtual void BeginPlay() override;
};
