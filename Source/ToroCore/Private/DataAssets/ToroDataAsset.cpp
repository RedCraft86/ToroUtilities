// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "DataAssets/ToroDataAsset.h"
#include "Misc/App.h"

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
	if (!FApp::IsGame()) ValidateData();
}

void UToroDataAsset::PostInitProperties()
{
	Super::PostInitProperties();
	if (!FApp::IsGame()) ValidateData();
}

void UToroDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!FApp::IsGame()) ValidateData();
}
#endif