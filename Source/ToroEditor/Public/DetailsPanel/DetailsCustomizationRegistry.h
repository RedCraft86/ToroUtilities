// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "ToroEditor.h"
#include "PropertyEditorModule.h"
#include "IDetailCustomization.h"
#include "IPropertyTypeCustomization.h"

/**
 * Central registry for class and property-type Details panel customizations.
 * Registered customizations are collectively removed during module shutdown.
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

	/**
	 * Registers a customization for a UObject class or reflected struct.
	 * @tparam Type UObject class or reflected struct being customized.
	 * @tparam Customization Customization type created for each Details panel.
	 */
	template<typename Type, typename Customization>
	static void Register()
	{
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
			static_assert(TIsDerivedFrom<Customization, IDetailCustomization>::Value,
				"Customization must derive from IDetailCustomization");

			RegisterClass<Type, Customization>(ModulePtr);
		}
		else if constexpr (TModels<CStaticStructProvider, Type>::Value)
		{
			static_assert(TIsDerivedFrom<Customization, IPropertyTypeCustomization>::Value,
				"Customization must derive from IPropertyTypeCustomization");

			RegisterStruct<Type, Customization>(ModulePtr);
		}
	}

private:

	TOROEDITOR_API static inline TSet<FName> ClassNames = {};
	TOROEDITOR_API static inline TSet<FName> StructNames = {};
	TOROEDITOR_API static inline FPropertyEditorModule* ModulePtr = nullptr;

	/**
	 * Creates a customization instance for a Property Editor delegate.
	 * @tparam In Concrete customization type to instantiate.
	 * @tparam Out Customization interface returned to the Property Editor.
	 */
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
			);
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
				TEXT("Struct Details Customizations for %s already exist!"), *StructName.ToString()
			);
			return;
		}

		StructNames.Add(StructName);
		Module->RegisterCustomPropertyTypeLayout(StructName, FOnGetPropertyTypeCustomizationInstance::CreateStatic(
				&FDetailsCustomizationRegistry::MakeCustomization<Customization, IPropertyTypeCustomization>));

		UE_LOG(LogToroEditor, Display, TEXT("Registering Struct customization for %s"), *StructName.ToString());
	}

	/**
	 * Unregisters every customization registered through this registry.
	 */
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