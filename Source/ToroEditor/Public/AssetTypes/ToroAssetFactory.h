// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "Factories/Factory.h"
#include "ToroAssetClassFilter.h"
#include "MiscObjects/ToroDataAsset.h"
#include "Kismet2/SClassPickerDialog.h"
#include "ToroAssetFactory.generated.h"

/**
 * Base factory for creating Toro data assets with an optional class picker.
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
		SupportedClass = UToroDataAsset::StaticClass();
	}

	virtual bool ConfigureProperties() override
	{
		static constexpr EClassFlags BadClassFlags = CLASS_Abstract | CLASS_Deprecated | CLASS_NewerVersionExists | CLASS_HideDropDown;
		if (!SupportedClass)
		{
			return false;
		}

		AssetClass = nullptr;
		if (bUseClassPicker)
		{
			// Ensure the class viewer module is available before opening the picker.
			FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");

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
			const FString ValidAssetName = FPaths::MakeValidFileName(AssetName);
			if (ValidAssetName.Len() > 2)
			{
				return TEXT("New") + ValidAssetName;
			}
		}

		const UClass* NamingClass = AssetClass ? AssetClass.Get() : SupportedClass.Get();
		return TEXT("New") + (NamingClass ? NamingClass->GetName() : TEXT("DataAsset"));
	}

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name,
		EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override
	{
		const UClass* ClassToCreate = AssetClass ? AssetClass.Get() : Class;
		if (!ensure(ClassToCreate && ClassToCreate->IsChildOf(UDataAsset::StaticClass())))
		{
			return nullptr;
		}

		return NewObject<UDataAsset>(InParent, ClassToCreate, Name, Flags | RF_Transactional);
	}

protected:

	FString AssetName;
	bool bUseClassPicker;
	TSubclassOf<UDataAsset> AssetClass;
};