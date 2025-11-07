// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "MiscObjects/ToroDataAsset.h"

#if WITH_EDITOR
void UToroDataAsset::PostLoad()
{
	Super::PostLoad();
	if (!FApp::IsGame())
	{
		UpdateData();
	}
}

void UToroDataAsset::PostInitProperties()
{
	Super::PostInitProperties();
	if (!FApp::IsGame())
	{
		UpdateData();
	}
}

void UToroDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!FApp::IsGame())
	{
		UpdateData();
	}
}

FText UToroDatabase::GetDescription() const
{
	const FIntPoint Data = GetDataResults();
	return FText::Format(INVTEXT("{0} Entries | {1} Valid | {2} Invalid"),
		Data.GetMax(), Data.GetMin(), Data.GetMax() - Data.GetMin());
}
#endif