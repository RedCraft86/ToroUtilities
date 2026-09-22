// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "ToroEditor.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"

/**
 * Central registry for component visualizers.
 * Registered visualizers are collectively removed during module shutdown.
 *
 * <b>Usage:</b>
 * <pre>
 *   FComponentVisualizerRegistry::Register<UMyComponent, FMyComponentVisualizer>();
 * </pre>
 */
class FComponentVisualizerRegistry final
{
	friend class FToroEditorModule;

public:

	/**
	 * Creates and registers a visualizer for a component class.
	 * @tparam Component Actor component type handled by the visualizer.
	 * @tparam Visualizer Component visualizer type to instantiate.
	 */
	template<typename Component, typename Visualizer>
	static void Register()
	{
		static_assert(TIsDerivedFrom<Component, UActorComponent>::Value,
			"Component must be a UClass derived from UActorComponent");

		static_assert(TIsDerivedFrom<Visualizer, FComponentVisualizer>::Value,
			"Visualizer must derive from FComponentVisualizer");

		if (GUnrealEd)
		{
			const FName CompName = Component::StaticClass()->GetFName();
			if (ComponentNames.Contains(CompName))
			{
				UE_LOG(LogToroEditor, Warning,
					TEXT("Attempting to register multiple Component Visualizers for %s"), *CompName.ToString()
				);
				return;
			}

			ComponentNames.Add(CompName);
			const TSharedPtr<FComponentVisualizer> VisInstance = MakeShared<Visualizer>();
			GUnrealEd->RegisterComponentVisualizer(CompName, VisInstance);
			VisInstance->OnRegister();

			UE_LOG(LogToroEditor, Display, TEXT("Registering Component Visualizer for %s"), *CompName.ToString());
		}
	}

private:

	TOROEDITOR_API static inline TSet<FName> ComponentNames = {};

	/**
	 * Unregisters every component visualizer registered through this registry.
	 */
	static void UnregisterAll()
	{
		if (GUnrealEd)
		{
			for (const FName& Name : ComponentNames)
			{
				GUnrealEd->UnregisterComponentVisualizer(Name);
			}
		}

		ComponentNames.Empty();
		UE_LOG(LogToroEditor, Display, TEXT("Unregistered Component Visualizers"));
	}
};