// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "CustomizationHelpers.h"
#include "DetailLayoutBuilder.h"
#include "IDetailCustomization.h"
#include "Electronics/ElectricalActor.h"
#include "Electronics/ElectricalSwitch.h"
#include "Electronics/ElectricalProvider.h"
#include "Electronics/ElectricalReceiver.h"

#define CLASSNAME AElectricalActor
class TOROEDITOR_API FElectricActorDetails final : public IDetailCustomization
{
public:

	enum class ERelation : uint8
	{
		Both = 0,
		Provide = 1,
		Receive = 2
	};

	static inline TMap<UClass*, ERelation> Entries = {
		{AElectricalSwitch::StaticClass(),		ERelation::Both},
		{AElectricalProvider::StaticClass(),	ERelation::Provide},
		{AElectricalReceiver::StaticClass(),	ERelation::Receive}
	};

	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShared<FElectricActorDetails>();
	}

protected:

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		TArray<TWeakObjectPtr<UObject>> Objects;
		DetailBuilder.GetObjectsBeingCustomized(Objects);
		switch (GetRelation(Objects))
		{
			case ERelation::Provide:
				CLASS_PROPERTY_NS(MinProviders)->MarkHiddenByCustomization();
				CLASS_PROPERTY_NS(Providers)->MarkHiddenByCustomization();
				break;
			case ERelation::Receive:
				CLASS_PROPERTY_NS(Targets)->MarkHiddenByCustomization();
				break;
			default: break;
		}
	}

	static ERelation GetRelation(const TArray<TWeakObjectPtr<UObject>>& Objects)
	{
		for (const TWeakObjectPtr<UObject>& Object : Objects)
		{
			if (!Object.IsValid()) continue;
			for (const TPair<UClass*, ERelation>& Entry : Entries)
			{
				if (Object->IsA(Entry.Key))
				{
					return Entry.Value;
				}
			}
		}
		return ERelation::Both;
	}
};
#undef CLASSNAME