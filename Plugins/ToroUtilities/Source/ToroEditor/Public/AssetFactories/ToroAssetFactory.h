// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Factories/Factory.h"
#include "MiscObjects/ToroDataAsset.h"
#include "ToroAssetFactory.generated.h"

UCLASS(Abstract)
class TOROEDITOR_API UToroAssetFactory : public UFactory
{
	GENERATED_BODY()

public:

	UToroAssetFactory()
	{
		bCreateNew = true;
		bEditAfterNew = true;
		SupportedClass = UToroDataAsset::StaticClass();
	}

	virtual bool ConfigureProperties() override
	{
		DataClass = SupportedClass;
		return DataClass ? true : false;
	}

	virtual FString GetDefaultNewAssetName() const override
	{
		if (!AssetName.IsEmpty())
		{
			const_cast<UToroAssetFactory*>(this)->AssetName = FPaths::MakeValidFileName(AssetName);
			if (AssetName.Len() > 2) return TEXT("New") + AssetName;
		}
		return TEXT("New") + (DataClass ? DataClass->GetName() : SupportedClass->GetName());
	}

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name,
		EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override
	{
		const UClass* ObjClass = DataClass ? *DataClass : Class;
		check(ObjClass->IsChildOf(SupportedClass));
		return NewObject<UDataAsset>(InParent, ObjClass, Name, Flags | RF_Transactional);
	}
	
protected:

	FString AssetName;
	TSubclassOf<UDataAsset> DataClass;
};