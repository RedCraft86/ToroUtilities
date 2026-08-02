// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "Engine/DataAsset.h"
#include "ToroDataAsset.generated.h"

// TODO: Factory and definition

/**
 * Base class for data-driven assets in the ToroUtilities framework.
 */
UCLASS(Abstract, PrioritizeCategories = (Asset))
class TOROCORE_API UToroDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UToroDataAsset() {}

protected:

#if WITH_EDITORONLY_DATA
	/**
	 * A concatenated string of validation errors or warnings found during ValidateData().
	 * Displayed in the editor to assist designers in identifying data setup issues.
	 */
	UPROPERTY(Transient, VisibleAnywhere, Category = Asset, meta = (MultiLine = true, DisplayPriority = 999))
		FString Issues;
#endif

#if WITH_EDITOR
	/**
	 * Utility function called by the editor to validate data.
	 * (This function is called by <c>PostLoad</c>, <c>PostInitProperties</c>, and <c>PostEditChangeProperty</c>)
	 * @note Base implementation clears the existing Issues string.
	 */
	UFUNCTION(CallInEditor, Category = Asset)
		virtual void ValidateData() { Issues.Empty(); }

	/**
	 * Appends a new validation message to the Issues list for display in the editor.
	 */
	void AppendIssue(const FString& IssueText);

	virtual void PostLoad() override;
	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
