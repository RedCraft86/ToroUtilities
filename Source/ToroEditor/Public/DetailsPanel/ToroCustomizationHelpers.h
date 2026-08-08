// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

namespace ToroCustomizationHelpers
{
	inline void GetRenameCategories(const UClass* Class, TMap<FString, FText>& Renames)
	{
		static const FName META_RenameCategories("RenameCategories");

		TArray<FString> MetaEntries;
		Class->GetMetaData(META_RenameCategories).ParseIntoArray(MetaEntries, TEXT(","));

		Renames.Empty(MetaEntries.Num());
		for (const FString& MetaEntry : MetaEntries)
		{
			FString From, To;
			MetaEntry.Split(TEXT("="), &From, &To, ESearchCase::IgnoreCase);
			Renames.Add(From.TrimStartAndEnd(), FText::FromString(To.TrimStartAndEnd()));
		}
	}
}