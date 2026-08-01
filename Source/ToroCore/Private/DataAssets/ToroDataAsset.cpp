// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "DataAssets/ToroDataAsset.h"

#if WITH_EDITOR
void UToroDataAsset::AppendIssue(const FString& IssueText)
{
	if (!IssueText.IsEmpty())
	{
		if (!Issues.IsEmpty())
		{
			Issues.Append("\n\n");
		}

		Issues.Append(IssueText);
	}
}

void UToroDataAsset::PostLoad()
{
	Super::PostLoad();
	if (!FApp::IsGame())
	{
		ValidateData();
	}
}

void UToroDataAsset::PostInitProperties()
{
	Super::PostInitProperties();
	if (!FApp::IsGame())
	{
		ValidateData();
	}
}

void UToroDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!FApp::IsGame())
	{
		ValidateData();
	}
}
#endif