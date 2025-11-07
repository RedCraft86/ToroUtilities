// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

template<typename T>
struct TToroInterpBase
{
	T Current;
	T Target;
	float Speed;
	bool bConstant;
	
	FORCEINLINE virtual ~TToroInterpBase() = default;
	FORCEINLINE TToroInterpBase(): Speed(5.0f), bConstant(false) {}
	FORCEINLINE TToroInterpBase(const T InTarget, const float InSpeed = 5.0f)
		: Current(InTarget), Target(InTarget), Speed(InSpeed), bConstant(false)
	{}

	FORCEINLINE void SnapToTarget() { Current = Target; }
	FORCEINLINE virtual T Tick(const float InDeltaTime) { return Current; }
};

struct FToroInterpFloat final : TToroInterpBase<float>
{
	FORCEINLINE FToroInterpFloat() {}
	FORCEINLINE FToroInterpFloat(const float InTarget, const float InSpeed): TToroInterpBase(InTarget, InSpeed) {}
	FORCEINLINE bool IsComplete(const float Tolerance = 0.01f) const
	{
		return FMath::IsNearlyEqual(Current, Target, Tolerance);
	}
	FORCEINLINE virtual float Tick(const float InDeltaTime) override
	{
		if (!IsComplete())
		{
			Current = bConstant
				? FMath::FInterpConstantTo(Current, Target, InDeltaTime, Speed)
				: FMath::FInterpTo(Current, Target, InDeltaTime, Speed);
		}
		return Current;
	}
};

struct FToroInterpVector2D final : TToroInterpBase<FVector2D>
{
	FORCEINLINE FToroInterpVector2D() {}
	FORCEINLINE FToroInterpVector2D(const FVector2D& InTarget, const float InSpeed): TToroInterpBase(InTarget, InSpeed) {}
	FORCEINLINE bool IsComplete(const float Tolerance = 0.01f) const
	{
		return Current.Equals(Target, Tolerance);
	}
	FORCEINLINE virtual FVector2D Tick(const float InDeltaTime) override
	{
		if (!IsComplete())
		{
			Current = bConstant
				? FMath::Vector2DInterpConstantTo(Current, Target, InDeltaTime, Speed)
				: FMath::Vector2DInterpTo(Current, Target, InDeltaTime, Speed);
		}
		return Current;
	}
};

struct FToroInterpVector final : TToroInterpBase<FVector>
{
	FORCEINLINE FToroInterpVector() {}
	FORCEINLINE FToroInterpVector(const FVector& InTarget, const float InSpeed): TToroInterpBase(InTarget, InSpeed) {}
	FORCEINLINE bool IsComplete(const float Tolerance = 0.01f) const
	{
		return Current.Equals(Target, Tolerance);
	}
	FORCEINLINE virtual FVector Tick(const float InDeltaTime) override
	{
		if (!IsComplete())
		{
			Current = bConstant
				? FMath::VInterpConstantTo(Current, Target, InDeltaTime, Speed)
				: FMath::VInterpTo(Current, Target, InDeltaTime, Speed);
		}
		return Current;
	}
};