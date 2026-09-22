// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

/**
 * Helpers shared by Toro Details panel customizations.
 */
namespace ToroCustomizationHelpers
{
	/**
	 * Parses RenameCategories metadata into internal names and display labels.
	 * @param Class Class containing the metadata.
	 * @param Renames Receives the parsed category mappings.
	 */
	inline void GetRenameCategories(const UClass* Class, TMap<FString, FText>& Renames)
	{
		static const FName META_RenameCategories("RenameCategories");

		TArray<FString> MetaEntries;
		Class->GetMetaData(META_RenameCategories).ParseIntoArray(MetaEntries, TEXT(","));

		Renames.Empty(MetaEntries.Num());
		for (const FString& MetaEntry : MetaEntries)
		{
			FString From, To;
			if (!MetaEntry.Split(TEXT("="), &From, &To, ESearchCase::IgnoreCase))
			{
				continue;
			}

			From.TrimStartAndEndInline();
			To.TrimStartAndEndInline();
			if (!From.IsEmpty() && !To.IsEmpty())
			{
				Renames.Add(From, FText::FromString(To));
			}
		}
	}
}