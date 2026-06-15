// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "ToroEditor.h"
#include "Misc/Paths.h"
#include "Factories/Factory.h"
#include "DataAssets/ToroDataAsset.h"
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
		AssetName = FString();
	}

	virtual bool ConfigureProperties() override
	{
		return false;
	}

	virtual FString GetDefaultNewAssetName() const override
	{
		if (!AssetName.IsEmpty())
		{
			AssetName = FPaths::MakeValidFileName(AssetName);
			if (AssetName.Len() > 2)
			{
				return TEXT("New") + AssetName;
			}
			UE_LOG(LogToroEditor, Warning, TEXT("Name '%s' is too short to be used as an asset name."), *AssetName);
		}
		return TEXT("New") + SupportedClass->GetName();
	}

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* Parent, FName Name,
		EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override
	{
		check(Class && Class->IsChildOf(SupportedClass));
		return NewObject<UObject>(Parent, Class, Name, Flags);
	}

protected:

	mutable FString AssetName;
};