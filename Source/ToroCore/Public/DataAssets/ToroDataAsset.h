// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Engine/DataAsset.h"
#include "ToroDataAsset.generated.h"

// TODO: Factory and definition

/**
 * Base class for all data-driven assets in the ToroUtilities framework.
 * Provides a persistent GUID for asset tracking and built-in editor validation support.
 */
UCLASS(Abstract, PrioritizeCategories = (Asset))
class TOROCORE_API UToroDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UToroDataAsset()
		: AssetGuid(FGuid::NewGuid())
	{}

	UToroDataAsset(const FObjectInitializer& ObjectInit)
		: Super(ObjectInit), AssetGuid(FGuid::NewGuid())
	{}

	const FGuid& GetAssetGuid() const { return AssetGuid; }

protected:

	/** 
	 * Unique identifier used for stable referencing without path references. 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Asset, meta = (DisplayPriority = -1))
		FGuid AssetGuid;

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
	 * Utility function called by the editor to re-cache or refresh internal data.
	 * Overridable by child classes to handle specialized synchronization or validation logic.
	 * (This function is automatically called by PostLoad, PostInitProperties, and PostEditChangeProperty)
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
