// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "ToroEditor.h"
#include "ToroToolbarButton.h"

/**
 * A centralized registry for registering and unregistering simple toolbar buttons.
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
	
	template<typename T>
	static void AddButton()
	{
		static_assert(TIsDerivedFrom<T, FToroToolbarButton>::Value, 
			"T must derive from FToroToolbarButton");

		const TSharedPtr<T> Button = MakeShared<T>();
		if (Buttons.Contains(Button->GetName()))
		{
			UE_LOG(LogToroEditor, Warning, 
				TEXT("Attempting to register multiple Toolbar Buttons with name '%s'"), 
				*Button->GetName().ToString())
			return;
		}

		Buttons.Add(Button->GetName(), Button);
		if (bToolMenusLoaded)
		{
			Button->RegisterButton();
		}

		UE_LOG(LogToroEditor, Display, TEXT("Registering toolbar button '%s'"), *Button->GetName().ToString())
	}

private:

	TOROEDITOR_API static inline bool bToolMenusLoaded = false;
	TOROEDITOR_API static inline FDelegateHandle StartupCallback = FDelegateHandle();
	TOROEDITOR_API static inline TMap<FName, TSharedPtr<FToroToolbarButton>> Buttons;

	static void Register()
	{
		if (!bToolMenusLoaded && !StartupCallback.IsValid())
		{
			StartupCallback = UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::
				FDelegate::CreateStatic(&FToolbarButtonRegistry::RegisterToolMenus));
		}
	}

	static void UnregisterAll()
	{
		bToolMenusLoaded = false;
		UToolMenus::UnRegisterStartupCallback(StartupCallback);
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
	
	static void RegisterToolMenus()
	{
		for (const TPair<FName, TSharedPtr<FToroToolbarButton>>& Button : Buttons)
		{
			if (Button.Value.IsValid()) Button.Value->RegisterButton();
		}
		bToolMenusLoaded = true;
	}
};