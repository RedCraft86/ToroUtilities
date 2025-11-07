// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

template<typename T>
struct TToroModifierBase
{
	T Base;
	TMap<FName, T> Modifiers;

	FORCEINLINE TToroModifierBase() {}
	FORCEINLINE TToroModifierBase(const T InBase): Base(InBase) {}
	FORCEINLINE virtual ~TToroModifierBase() = default;

	FORCEINLINE bool HasMod(const FName Key) const { return Modifiers.Contains(Key); }
	FORCEINLINE void AddMod(const FName Key, const T Value) { Modifiers.Add(Key, Value); }
	FORCEINLINE void RemoveMod(const FName Key) { Modifiers.Remove(Key); }
	FORCEINLINE void ClearMods() { Modifiers.Empty(); }
	FORCEINLINE T Evaluate() const
	{
		if (Modifiers.IsEmpty())
		{
			return Base;
		}

		T Value = Base;
		for (const TPair<FName, T>& Pair : Modifiers)
		{
			Operation(Value, Pair.Value);
		}
		return Value;
	}
protected:
	virtual void Operation(T& Main, const T Other) const {}
};

struct FToroSumFloat final : TToroModifierBase<float>
{
	FORCEINLINE FToroSumFloat(): TToroModifierBase(0.0f) {}
	FORCEINLINE FToroSumFloat(const float InBase): TToroModifierBase(InBase) {}
protected:
	virtual void Operation(float& Main, const float Other) const override { Main += Other; }
};

struct FToroMultiFloat final : TToroModifierBase<float>
{
	FORCEINLINE FToroMultiFloat(): TToroModifierBase(0.0f) {}
	FORCEINLINE FToroMultiFloat(const float InBase): TToroModifierBase(InBase) {}
protected:
	virtual void Operation(float& Main, const float Other) const override { Main *= Other; }
};

struct FToroSumVector2D final : TToroModifierBase<FVector2D>
{
	FORCEINLINE FToroSumVector2D(): TToroModifierBase(FVector2D::ZeroVector) {}
	FORCEINLINE FToroSumVector2D(const FVector2D& InBase): TToroModifierBase(InBase) {}
protected:
	virtual void Operation(FVector2D& Main, const FVector2D Other) const override { Main += Other; }
};

struct FToroMultiVector2D final : TToroModifierBase<FVector2D>
{
	FORCEINLINE FToroMultiVector2D(): TToroModifierBase(FVector2D::ZeroVector) {}
	FORCEINLINE FToroMultiVector2D(const FVector2D& InBase): TToroModifierBase(InBase) {}
protected:
	virtual void Operation(FVector2D& Main, const FVector2D Other) const override { Main *= Other; }
};

struct FToroSumVector final : TToroModifierBase<FVector>
{
	FORCEINLINE FToroSumVector(): TToroModifierBase(FVector::ZeroVector) {}
	FORCEINLINE FToroSumVector(const FVector& InBase): TToroModifierBase(InBase) {}
protected:
	virtual void Operation(FVector& Main, const FVector Other) const override { Main += Other; }
};

struct FToroMultiVector final : TToroModifierBase<FVector>
{
	FORCEINLINE FToroMultiVector(): TToroModifierBase(FVector::ZeroVector) {}
	FORCEINLINE FToroMultiVector(const FVector& InBase): TToroModifierBase(InBase) {}
protected:
	virtual void Operation(FVector& Main, const FVector Other) const override { Main *= Other; }
};