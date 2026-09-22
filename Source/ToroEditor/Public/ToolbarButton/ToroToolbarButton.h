// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "ToolMenus.h"
#include "Templates/SharedPointer.h"

#define MAKE_TOOLBAR_BUTTON_NAME(Name) \
	*FString::Printf(TEXT("%s.%s"), TEXT(UE_MODULE_NAME), TEXT(#Name))

/**
 * Base class for a toolbar button registered through Unreal's ToolMenus system.
 * Derived classes provide the action to execute and may customize the displayed icon.
 */
class TOROEDITOR_API FToroToolbarButton : public TSharedFromThis<FToroToolbarButton>
{
protected:

	/**
	 * Text displayed on the toolbar button.
	 */
	FText Label;

	/**
	 * Hover text describing the button action.
	 */
	FText Tooltip;

	/**
	 * Unique name used to identify the ToolMenus entry.
	 */
	FName Name;

	/**
	 * Name of the menu extended by this button.
	 */
	FName MenuHook;

	/**
	 * Name of the section that receives the button entry.
	 */
	FName SectionName;

	/**
	 * Whether this instance currently owns a registered menu entry.
	 */
	bool bRegistered;

	FToroToolbarButton(const FName InName, const FText& InLabel, const FText& InTooltip)
		: Label(InLabel), Tooltip(InTooltip), Name(InName)
		, MenuHook(TEXT("LevelEditor.LevelEditorToolBar.PlayToolBar"))
		, SectionName(TEXT("ToroUtilities")), bRegistered(false)
	{}

	virtual ~FToroToolbarButton() = default;

	/**
	 * Supplies the icon displayed by the toolbar entry.
	 */
	virtual FSlateIcon GetSlateIcon()
	{
		return FSlateIcon(FCoreStyle::Get().GetStyleSetName(), TEXT("Launcher.Instance_Unknown"));
	}

	/**
	 * Executes the action associated with the toolbar button.
	 */
	virtual void Execute() = 0;

public:

	/**
	 * Gets the unique name used for the ToolMenus entry.
	 */
	FName GetName() const
	{
		return Name;
	}

	/**
	 * Adds the button to its configured menu and section if it is not already registered.
	 */
	virtual void RegisterButton()
	{
		if (!bRegistered)
		{
			FToolMenuOwnerScoped OwnerScoped(this);
			if (UToolMenu* Menu = UToolMenus::Get()->ExtendMenu(MenuHook))
			{
				FToolMenuSection& Section = Menu->FindOrAddSection(SectionName);
				Section.AddEntry(FToolMenuEntry::InitToolBarButton(Name,
					FExecuteAction::CreateRaw(this, &FToroToolbarButton::Execute),
					Label, Tooltip, GetSlateIcon()));

				bRegistered = true;
			}
		}
	}

	/**
	 * Removes every ToolMenus entry owned by this button instance.
	 */
	virtual void UnregisterButton()
	{
		if (bRegistered)
		{
			bRegistered = false;
			UToolMenus::UnregisterOwner(this);
		}
	}
};