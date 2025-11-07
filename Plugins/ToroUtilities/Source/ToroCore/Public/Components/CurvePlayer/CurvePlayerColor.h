// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "BaseCurvePlayer.h"
#include "DataTypes/MathTypes.h"
#include "CurvePlayerColor.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Animation), meta = (BlueprintSpawnableComponent))
class TOROCORE_API UCurvePlayerColor final: public UBaseCurvePlayer
{
	GENERATED_BODY()

public:

	UCurvePlayerColor();

	UFUNCTION(BlueprintCallable, Category = CurvePlayer)
		void SetCurve(const FInlineColorCurve& InCurve);

	UFUNCTION(BlueprintPure, Category = CurvePlayer)
		const FInlineColorCurve& GetCurve() const { return Curve; }

	UFUNCTION(BlueprintPure, Category = CurvePlayer)
		FLinearColor GetValue() const { return Curve.GetValue(CurrentTime); }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEvaluateTickBP, const FLinearColor, Value, const float, Time);
	UPROPERTY(BlueprintAssignable, DisplayName = "On Evaluate Tick")
		FOnEvaluateTickBP OnEvaluateTickBP;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEvaluateTick, const FLinearColor, const float);
	FOnEvaluateTick OnEvaluateTick;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		FInlineColorCurve Curve;

	virtual void BroadcastEval() const override;
	virtual void BeginPlay() override;
};
