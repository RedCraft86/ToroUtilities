// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "Factories/Factory.h"
#include "ToroAssetClassFilter.h"
#include "DataAssets/ToroDatabase.h"
#include "Kismet2/SClassPickerDialog.h"
#include "ToroAssetFactory.generated.h"

/**
 * Base class for custom asset factories in the ToroUtilities framework.
 */
UCLASS(Abstract)
class TOROEDITOR_API UToroAssetFactory : public UFactory
{
	GENERATED_BODY()

public:

	UToroAssetFactory()
	{
		bCreateNew = true;
		bEditAfterNew = true;
		bUseClassPicker = false;
		SupportedClass = UToroDatabase::StaticClass();
	}

	virtual bool ConfigureProperties() override
	{
		static constexpr EClassFlags BadClassFlags = CLASS_Abstract | CLASS_Deprecated | CLASS_NewerVersionExists | CLASS_HideDropDown;

		AssetClass = nullptr;
		if (bUseClassPicker)
		{
			// This isn't used in DataAssetFactory either, but it is stated to load for viewing the class picker.
			[[maybe_unused]] FClassViewerModule& ClassViewer = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");

			const TSharedPtr<FToroAssetClassFilter> Filter = MakeShared<FToroAssetClassFilter>(SupportedClass);
			Filter->DisallowedClassFlags = BadClassFlags;
			Filter->bDisallowBlueprint = false;

			FClassViewerInitializationOptions Options;
			Options.Mode = EClassViewerMode::ClassPicker;
			Options.NameTypeToDisplay = EClassViewerNameTypeToDisplay::DisplayName;
			Options.ClassFilters.Add(Filter.ToSharedRef());

			UClass* ChosenClass = nullptr;
			if (!SClassPickerDialog::PickClass(
				NSLOCTEXT("EditorFactories", "CreateDataAssetOptions", "Pick Class For Data Asset Instance"),
				Options, ChosenClass, SupportedClass))
			{
				return false;
			}

			AssetClass = ChosenClass;
			return true;
		}

		if (SupportedClass && !SupportedClass->HasAnyClassFlags(BadClassFlags))
		{
			AssetClass = SupportedClass;
			return IsValid(AssetClass);
		}

		return false;
	}

	virtual FString GetDefaultNewAssetName() const override
	{
		if (!AssetName.IsEmpty())
		{
			const_cast<UToroAssetFactory*>(this)->AssetName = FPaths::MakeValidFileName(AssetName);
			if (AssetName.Len() > 2)
			{
				return TEXT("New") + AssetName;
			}
		}

		return TEXT("New") + (AssetClass ? AssetClass->GetName() : SupportedClass->GetName());
	}

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name,
		EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override
	{
		if (AssetClass != nullptr)
		{
			return NewObject<UDataAsset>(InParent, AssetClass, Name, Flags | RF_Transactional);
		}

		check(Class->IsChildOf(UDataAsset::StaticClass()));
		return NewObject<UDataAsset>(InParent, Class, Name, Flags);
	}

protected:

	FString AssetName;
	bool bUseClassPicker;
	TSubclassOf<UDataAsset> AssetClass;
};
