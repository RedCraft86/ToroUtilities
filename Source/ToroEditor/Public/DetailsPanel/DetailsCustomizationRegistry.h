// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "ToroEditor.h"
#include "PropertyEditorModule.h"
#include "IDetailCustomization.h"
#include "IPropertyTypeCustomization.h"

/**
 * A centralized registry for registering and unregistering details panel customizations for both UClass and UStruct.
 *
 * <b>Usage:</b>
 * <pre>
 *   FDetailsCustomizationRegistry::Register<UMyObject, FMyObjectCustomization>();
 *   FDetailsCustomizationRegistry::Register<FMyStruct, FMyStructCustomization>();
 * </pre>
 */
class FDetailsCustomizationRegistry final
{
	friend class FToroEditorModule;

public:

	template<typename Type, typename Customization>
	static void Register()
	{
		static_assert(
		   (TModels<CStaticClassProvider, Type>::Value && TIsDerivedFrom<Customization, IDetailCustomization>::Value) ||
		   (TModels<CStaticStructProvider, Type>::Value && TIsDerivedFrom<Customization, IPropertyTypeCustomization>::Value),
		   "Must be a UClass with IDetailCustomization or UStruct with IPropertyTypeCustomization");

		if (!ModulePtr)
		{
			ModulePtr = FModuleManager::LoadModulePtr<FPropertyEditorModule>(TEXT("PropertyEditor"));
			if (!ModulePtr)
			{
				UE_LOG(LogToroEditor, Error, TEXT("Failed to load property editor while registering customization!"));
				return;
			}
		}

		if constexpr (TModels<CStaticClassProvider, Type>::Value)
		{
			RegisterClass<Type, Customization>(ModulePtr);
		}
		else if constexpr (TModels<CStaticStructProvider, Type>::Value)
		{
			RegisterStruct<Type, Customization>(ModulePtr);
		}
	}
	
private:

	TOROEDITOR_API static inline TSet<FName> ClassNames = {};
	TOROEDITOR_API static inline TSet<FName> StructNames = {};
	TOROEDITOR_API static inline FPropertyEditorModule* ModulePtr = nullptr;

	template<typename In, typename Out>
	static TSharedRef<Out> MakeCustomization()
	{
		return MakeShared<In>();
	}

	template<typename Class, typename Customization>
	static void RegisterClass(FPropertyEditorModule* Module)
	{
		const FName ClassName = Class::StaticClass()->GetFName();
		if (ClassNames.Contains(ClassName))
		{
			UE_LOG(LogToroEditor, Warning, 
				TEXT("Class Details Customizations for %s already exist!"), *ClassName.ToString()
			)
			return;
		}

		ClassNames.Add(ClassName);
		Module->RegisterCustomClassLayout(ClassName, FOnGetDetailCustomizationInstance::CreateStatic(
				&FDetailsCustomizationRegistry::MakeCustomization<Customization, IDetailCustomization>));

		UE_LOG(LogToroEditor, Display, TEXT("Registering Class customization for %s"), *ClassName.ToString());
	}

	template<typename Struct, typename Customization>
	static void RegisterStruct(FPropertyEditorModule* Module)
	{
		const FName StructName = Struct::StaticStruct()->GetFName();
		if (StructNames.Contains(StructName))
		{
			UE_LOG(LogToroEditor, Warning, 
				TEXT("Struct Property Customizations for %s already exist!"), *StructName.ToString()
			)
			return;
		}

		StructNames.Add(StructName);
		Module->RegisterCustomPropertyTypeLayout(StructName, FOnGetPropertyTypeCustomizationInstance::CreateStatic(
				&FDetailsCustomizationRegistry::MakeCustomization<Customization, IPropertyTypeCustomization>));

		UE_LOG(LogToroEditor, Display, TEXT("Registering Struct customization for %s"), *StructName.ToString());
	}

	static void UnregisterAll()
	{
		ModulePtr = FModuleManager::GetModulePtr<FPropertyEditorModule>(TEXT("PropertyEditor"));
		if (ModulePtr)
		{
			for (const FName& ClassName : ClassNames)
			{
				ModulePtr->UnregisterCustomClassLayout(ClassName);
			}
			for (const FName& StructName : StructNames)
			{
				ModulePtr->UnregisterCustomPropertyTypeLayout(StructName);
			}
		}

		ClassNames.Empty();
		StructNames.Empty();
		ModulePtr = nullptr;

		UE_LOG(LogToroEditor, Display, TEXT("Unregistered Struct and Class customizations"));
	}
};