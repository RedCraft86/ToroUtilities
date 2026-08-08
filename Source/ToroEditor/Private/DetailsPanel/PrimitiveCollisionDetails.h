// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "SNameComboBox.h"
#include "DataTypes/PrimitiveCollision.h"
#include "DetailsPanel/ToroStructCustomization.h"

class FPrimitiveCollisionDetails final : public FToroStructCustomization
{
public:

	FPrimitiveCollisionDetails()
		: StructPtr(nullptr)
	{}

private:

	struct FCollisionChannelInfo
	{
		FString RawName;
		FString DisplayName;
		ECollisionChannel Channel;

		FCollisionChannelInfo(const FString& InRaw, const FString& InDisplay, const ECollisionChannel InChannel)
			: RawName(InRaw), DisplayName(InDisplay), Channel(InChannel)
		{}
	};

	FPrimitiveCollision* StructPtr;
	TSharedPtr<FName> DefaultPreset;
	TArray<TSharedPtr<FName>> Presets;
	TArray<FCollisionChannelInfo> TraceChannels;
	TArray<FCollisionChannelInfo> ObjectChannels;
	
	TSharedPtr<SNameComboBox> ComboBoxPtr;

	FPrimitiveCollision* GetStructPtr(const TSharedRef<IPropertyHandle>& PropertyHandle);

	bool CanCustomize() const { return StructPtr && StructPtr->GetProfileName() == UCollisionProfile::CustomCollisionProfileName; }
	void OnPresetSelected(TSharedPtr<FName> NewPreset, ESelectInfo::Type SelectInfo, const TSharedRef<IPropertyHandle> StructHandle) const;

	ECheckBoxState HasCommonResponse(const ECollisionResponse ResponseToCheck) const;
	void SetAllResponses(ECheckBoxState CheckState, const ECollisionResponse Response, const TSharedRef<IPropertyHandle> StructHandle) const;

	ECheckBoxState HasResponse(const ECollisionChannel Channel, const ECollisionResponse ResponseToCheck) const;
	void SetResponse(ECheckBoxState CheckState, const ECollisionChannel Channel, const ECollisionResponse Response, const TSharedRef<IPropertyHandle> StructHandle) const;

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructHandle, FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& CustomizationUtils) override;

	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructHandle, IDetailChildrenBuilder& ChildBuilder,
		IPropertyTypeCustomizationUtils& CustomizationUtils) override;
};