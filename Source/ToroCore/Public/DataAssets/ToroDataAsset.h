// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Misc/App.h"
#include "Engine/DataAsset.h"
#include "ToroDataAsset.generated.h"

/**
 * Base class for all data-driven assets in the ToroUtilities framework.
 * Provides a standardized identity (GUID) and metadata (Display Name, Description).
 * 
 * Features automatic data refreshing in the editor during property changes, 
 * loading, and initialization to ensure internal state remains consistent.
 */
UCLASS(Abstract, PrioritizeCategories = (Asset))
class TOROCORE_API UToroDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UToroDataAsset()
		: DisplayName(FText::GetEmpty())
		, Description(FText::GetEmpty())
		, Identifier(FGuid::NewGuid())
	{}

	const FText& GetDisplayName() const { return DisplayName; }
	const FText& GetDescription() const { return Description; }
	const FGuid& GetIdentifier() const { return Identifier; }

#if WITH_EDITOR
	/** 
	 * Utility function called by the editor to re-cache or refresh internal data.
	 * Overridable by child classes to handle specialized synchronization logic.
	 */
	UFUNCTION(CallInEditor, Category = Editor)
		virtual void RefreshData() {}
#endif

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Asset, meta = (SortPriority = -10))
		FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Asset, meta = (MultiLine = true, SortPriority = -10))
		FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Asset, meta = (SortPriority = -10))
		FGuid Identifier;

#if WITH_EDITOR
	virtual void PostLoad() override
	{
		Super::PostLoad();
		if (!FApp::IsGame()) RefreshData();
	}

	virtual void PostInitProperties() override
	{
		Super::PostInitProperties();
		if (!FApp::IsGame()) RefreshData();
	}

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);
		if (!FApp::IsGame()) RefreshData();
	}
#endif
};
