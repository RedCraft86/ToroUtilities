// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "WrappedTypes.generated.h"

USTRUCT(BlueprintType)
struct TOROCORE_API FWrappedBool final
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Wrapped)
		bool Value;

	FWrappedBool(): Value(false) {}

	FORCEINLINE bool& operator*() { return Value; }
	FORCEINLINE const bool& operator*() const { return Value; }
	FORCEINLINE FString ToString() const { return LexToString(Value); }

	FORCEINLINE bool operator==(const FWrappedBool& Other) const { return Value == Other.Value; }
	FORCEINLINE bool operator!=(const FWrappedBool& Other) const { return Value != Other.Value; }

	FORCEINLINE friend uint32 GetTypeHash(const FWrappedBool& InData)
	{
		return GetTypeHash(InData.Value);
	}
};

/**
 * Internally uses a Double actually
 */
USTRUCT(BlueprintType)
struct TOROCORE_API FWrappedFloat final
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Wrapped)
		double Value;

	FWrappedFloat(): Value(0.0f) {}

	FORCEINLINE double& operator*() { return Value; }
	FORCEINLINE const double& operator*() const { return Value; }
	FORCEINLINE FString ToString() const { return FString::SanitizeFloat(Value); }

	FORCEINLINE bool operator>(const FWrappedFloat& Other) const { return Value > Other.Value; }
	FORCEINLINE bool operator<(const FWrappedFloat& Other) const { return Value < Other.Value; }
	FORCEINLINE bool operator==(const FWrappedFloat& Other) const { return FMath::IsNearlyEqual(Value, Other.Value); }
	FORCEINLINE bool operator!=(const FWrappedFloat& Other) const { return !FMath::IsNearlyEqual(Value, Other.Value); }
	FORCEINLINE bool operator>=(const FWrappedFloat& Other) const { return *this > Other || *this == Other; }
	FORCEINLINE bool operator<=(const FWrappedFloat& Other) const { return *this < Other || *this == Other; }

	FORCEINLINE friend uint32 GetTypeHash(const FWrappedFloat& InData)
	{
		return GetTypeHash(InData.Value);
	}
};

USTRUCT(BlueprintType)
struct TOROCORE_API FWrappedByte final
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Wrapped)
		uint8 Value;

	FWrappedByte(): Value(0) {}

	FORCEINLINE uint8& operator*() { return Value; }
	FORCEINLINE const uint8& operator*() const { return Value; }
	FORCEINLINE FString ToString() const { return LexToString(Value); }

	FORCEINLINE bool operator>(const FWrappedByte& Other) const { return Value > Other.Value; }
	FORCEINLINE bool operator<(const FWrappedByte& Other) const { return Value < Other.Value; }
	FORCEINLINE bool operator==(const FWrappedByte& Other) const { return Value == Other.Value; }
	FORCEINLINE bool operator!=(const FWrappedByte& Other) const { return Value != Other.Value; }
	FORCEINLINE bool operator>=(const FWrappedByte& Other) const { return *this > Other || *this == Other; }
	FORCEINLINE bool operator<=(const FWrappedByte& Other) const { return *this < Other || *this == Other; }

	FORCEINLINE friend uint32 GetTypeHash(const FWrappedByte& InData)
	{
		return GetTypeHash(InData.Value);
	}
};

USTRUCT(BlueprintType)
struct TOROCORE_API FWrappedInt32 final
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Wrapped)
		int32 Value;

	FWrappedInt32(): Value(0) {}

	FORCEINLINE int32& operator*() { return Value; }
	FORCEINLINE const int32& operator*() const { return Value; }
	FORCEINLINE FString ToString() const { return LexToString(Value); }

	FORCEINLINE bool operator>(const FWrappedInt32& Other) const { return Value > Other.Value; }
	FORCEINLINE bool operator<(const FWrappedInt32& Other) const { return Value < Other.Value; }
	FORCEINLINE bool operator==(const FWrappedInt32& Other) const { return Value == Other.Value; }
	FORCEINLINE bool operator!=(const FWrappedInt32& Other) const { return Value != Other.Value; }
	FORCEINLINE bool operator>=(const FWrappedInt32& Other) const { return *this > Other || *this == Other; }
	FORCEINLINE bool operator<=(const FWrappedInt32& Other) const { return *this < Other || *this == Other; }

	FORCEINLINE friend uint32 GetTypeHash(const FWrappedInt32& InData)
	{
		return GetTypeHash(InData.Value);
	}
};

USTRUCT(BlueprintType)
struct TOROCORE_API FWrappedInt64 final
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Wrapped)
		int64 Value;

	FWrappedInt64(): Value(0) {}

	FORCEINLINE int64& operator*() { return Value; }
	FORCEINLINE const int64& operator*() const { return Value; }
	FORCEINLINE FString ToString() const { return LexToString(Value); }

	FORCEINLINE bool operator>(const FWrappedInt64& Other) const { return Value > Other.Value; }
	FORCEINLINE bool operator<(const FWrappedInt64& Other) const { return Value < Other.Value; }
	FORCEINLINE bool operator==(const FWrappedInt64& Other) const { return Value == Other.Value; }
	FORCEINLINE bool operator!=(const FWrappedInt64& Other) const { return Value != Other.Value; }
	FORCEINLINE bool operator>=(const FWrappedInt64& Other) const { return *this > Other || *this == Other; }
	FORCEINLINE bool operator<=(const FWrappedInt64& Other) const { return *this < Other || *this == Other; }

	FORCEINLINE friend uint32 GetTypeHash(const FWrappedInt64& InData)
	{
		return GetTypeHash(InData.Value);
	}
};

USTRUCT(BlueprintType)
struct TOROCORE_API FWrappedString final
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Wrapped)
		FString Value;

	FWrappedString(): Value(FString()) {}

	FORCEINLINE FString& operator*() { return Value; }
	FORCEINLINE const FString& operator*() const { return Value; }

	FORCEINLINE bool operator==(const FWrappedString& Other) const { return Value.Equals(Other.Value); }
	FORCEINLINE bool operator!=(const FWrappedString& Other) const { return !Value.Equals(Other.Value); }

	FORCEINLINE friend uint32 GetTypeHash(const FWrappedString& InData)
	{
		return GetTypeHash(InData.Value);
	}
};