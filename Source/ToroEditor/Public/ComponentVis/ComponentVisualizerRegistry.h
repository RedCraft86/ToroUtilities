// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "ToroEditor.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"

/**
 * A centralized registry for registering and unregistering component visualizers
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
				)
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