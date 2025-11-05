// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Misc/ToroDataAsset.h"

#if WITH_EDITOR
void UToroDataAsset::PostLoad()
{
	Super::PostLoad();
	if (!FApp::IsGame())
	{
		RefreshData();
	}
}

void UToroDataAsset::PostInitProperties()
{
	Super::PostInitProperties();
	if (!FApp::IsGame())
	{
		RefreshData();
	}
}

void UToroDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!FApp::IsGame())
	{
		RefreshData();
	}
}

FText UToroDatabase::GetDescription() const
{
	const FIntPoint Data = GetStatistics();
	if (Data.X == Data.Y)
	{
		return FText::Format(INVTEXT("{0} Valid Entries"), Data.X);
	}

	const int Total = Data.GetMax(), Valid = Data.GetMin();
	return FText::Format(INVTEXT("{0} Entries | {1} Valid | {2} Invalid"), Total, Valid, Total - Valid);
}
#endif
