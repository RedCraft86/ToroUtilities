// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "SelectableString.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct FTSetupSelectableString final
{
	GENERATED_BODY()

	UPROPERTY(Config, EditAnywhere, Category = String)
		FString Value;

	UPROPERTY(Transient, VisibleDefaultsOnly)
		FString OptionSource;

	FTSetupSelectableString()
		: Value(FString())
	{}

	FTSetupSelectableString(const FString& InValue)
		: Value(InValue)
	{}

	FORCEINLINE bool operator==(const FTSetupSelectableString& Other) const
	{
		return Value == Other.Value;
	}

	FORCEINLINE bool operator!=(const FTSetupSelectableString& Other) const
	{
		return Value != Other.Value;
	}

	FORCEINLINE friend uint32 GetTypeHash(const FTSetupSelectableString& InPath)
	{
		return GetTypeHash(InPath.Value);
	}
};