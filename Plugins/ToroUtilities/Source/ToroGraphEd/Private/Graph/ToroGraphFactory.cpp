// Copyright (C) RedCraft86. All Rights Reserved.

#include "Graph/ToroGraphFactory.h"
#include "Kismet2/SClassPickerDialog.h"
#include "ClassViewerModule.h"
#include "ClassViewerFilter.h"
#include "ToroDataGraph.h"

class FToroGraphAssetFilter final : public IClassViewerFilter
{
public:

	FToroGraphAssetFilter() {}

	static inline TSet<const UClass*> AllowedChildrenOfClasses{UToroDataGraph::StaticClass()};

	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions,
		const UClass* InClass, TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
	{
		return !InClass->HasAnyClassFlags(CLASS_Abstract | CLASS_Deprecated | CLASS_NewerVersionExists | CLASS_HideDropDown | CLASS_CompiledFromBlueprint)
			&& !InClass->GetBoolMetaDataHierarchical(FBlueprintMetadata::MD_IsBlueprintBase)
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InClass) != EFilterReturn::Failed;
	}

	virtual bool IsUnloadedClassAllowed(
		const FClassViewerInitializationOptions& InInitOptions,
		const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData,
		TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
	{
		return false;
	}
};

UToroGraphFactory::UToroGraphFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UToroDataGraph::StaticClass();
}

bool UToroGraphFactory::ConfigureProperties()
{
	GraphClass = nullptr;

	FClassViewerInitializationOptions Options;
	Options.Mode = EClassViewerMode::ClassPicker;
	Options.ClassFilters.Add(MakeShareable(new FToroGraphAssetFilter()));

	UClass* ChosenClass = nullptr;
	const bool bPicked = SClassPickerDialog::PickClass(
		NSLOCTEXT("ToroGraphEd", "GraphFactory", "Pick Data Graph Class"),
		Options, ChosenClass, UToroDataGraph::StaticClass());

	if (bPicked) GraphClass = ChosenClass;
	return bPicked;
}

FString UToroGraphFactory::GetDefaultNewAssetName() const
{
	return TEXT("New") + (GraphClass ? GraphClass->GetName() : GetSupportedClass()->GetName());
}

UObject* UToroGraphFactory::FactoryCreateNew(UClass* Class, UObject* InParent,
	FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	const UClass* ActualClass = GraphClass ? *GraphClass : Class;
	check(ActualClass->IsChildOf<UToroDataGraph>());
	return NewObject<UDataAsset>(InParent, ActualClass, Name, Flags | RF_Transactional);
}
