// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "ToroEditor.h"
#include "ToroToolbarButton.h"

/**
 * Central registry for registering and unregistering simple toolbar buttons.
 * Registered buttons are collectively removed during module shutdown.
 *
 * <b>Usage:</b>
 * <pre>
 *   FToolbarButtonRegistry::AddButton<FMyButton>();
 * </pre>
 */
class FToolbarButtonRegistry final
{
	friend class FToroEditorModule;

public:

	/**
	 * Creates and registers one toolbar button of the requested type.
	 * Duplicate names are rejected so each menu entry remains uniquely addressable.
	 * @tparam T Toolbar button type derived from FToroToolbarButton.
	 */
	template<typename T>
	static void AddButton()
	{
		static_assert(TIsDerivedFrom<T, FToroToolbarButton>::Value, "T must derive from FToroToolbarButton");

		const TSharedPtr<T> Button = MakeShared<T>();
		if (Buttons.Contains(Button->GetName()))
		{
			UE_LOG(LogToroEditor, Warning,
				TEXT("Attempting to register multiple Toolbar Buttons with name '%s'"),
				*Button->GetName().ToString()
			);
			return;
		}

		Buttons.Add(Button->GetName(), Button);
		if (bToolMenusLoaded)
		{
			Button->RegisterButton();
		}

		UE_LOG(LogToroEditor, Display, TEXT("Registering toolbar button '%s'"), *Button->GetName().ToString());
	}

private:

	TOROEDITOR_API static inline bool bToolMenusLoaded = false;
	TOROEDITOR_API static inline FDelegateHandle StartupCallback = FDelegateHandle();
	TOROEDITOR_API static inline TMap<FName, TSharedPtr<FToroToolbarButton>> Buttons = {};

	/**
	 * Registers all queued buttons after ToolMenus becomes available.
	 */
	static void RegisterButtons()
	{
		for (const TPair<FName, TSharedPtr<FToroToolbarButton>>& Button : Buttons)
		{
			if (Button.Value.IsValid())
			{
				Button.Value->RegisterButton();
			}
		}

		bToolMenusLoaded = true;
	}

	/**
	 * Queues toolbar registration for the ToolMenus startup phase.
	 */
	static void Register()
	{
		if (!bToolMenusLoaded && !StartupCallback.IsValid())
		{
			StartupCallback = UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::
				FDelegate::CreateStatic(&FToolbarButtonRegistry::RegisterButtons));
		}
	}

	/**
	 * Removes the startup callback, unregisters every button, and releases all instances.
	 */
	static void UnregisterAll()
	{
		UToolMenus::UnRegisterStartupCallback(StartupCallback);
		StartupCallback.Reset();
		bToolMenusLoaded = false;

		for (TPair<FName, TSharedPtr<FToroToolbarButton>>& Button : Buttons)
		{
			if (Button.Value.IsValid())
			{
				Button.Value->UnregisterButton();
				Button.Value.Reset();
			}
		}

		Buttons.Empty();
	}
};