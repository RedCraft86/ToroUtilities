// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "IDetailCustomization.h"
#include "BlueprintEditorModule.h"

class FPropertyMetadataDetails final : public IDetailCustomization
{
public:

	FPropertyMetadataDetails(const TSharedPtr<IBlueprintEditor>& InBlueprintEd, UBlueprint* Blueprint)
		: PropIdx(INDEX_NONE), PropName(NAME_None), BlueprintEd(InBlueprintEd), BlueprintPtr(Blueprint)
	{}

	static TSharedPtr<IDetailCustomization> MakeInstance(TSharedPtr<IBlueprintEditor> InBlueprintEd);

private:

	int32 PropIdx;
	FName PropName;
	TWeakPtr<IBlueprintEditor> BlueprintEd;
	TWeakObjectPtr<UBlueprint> BlueprintPtr;

	int32 GetNumMetadata() const;
	bool HasMetadata(const FName Key) const;
	void VerifyMetadata(const FName Key) const; // Metadata must have a value otherwise removed

	void RemoveMedata(const FName Key) const;
	FString GetMetadata(const FName Key) const;
	void SetMetadata(const FName Key, const FString& Value) const;
	void ChangeMetadataKey(const FName OldKey, const FName NewKey) const;
	
	void AddNewMetadata() const;
	void OnChangeMetaKey(const FText& NewKey, ETextCommit::Type Type, const FName OldKey) const;
	void OnChangeMetaValue(const FText& NewValue, ETextCommit::Type Type, const FName Key) const;

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
};