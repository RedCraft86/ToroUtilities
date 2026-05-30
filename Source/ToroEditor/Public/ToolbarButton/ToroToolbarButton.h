// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "ToolMenus.h"
#include "Templates/SharedPointer.h"

class TOROEDITOR_API FToroToolbarButton : public TSharedFromThis<FToroToolbarButton>
{
protected:

	virtual ~FToroToolbarButton() = default;

	FToroToolbarButton(const FName InName, const FText& InLabel, const FText& InTooltip)
		: Label(InLabel), Tooltip(InTooltip), Name(InName)
		, MenuHook(TEXT("LevelEditor.LevelEditorToolBar.PlayToolBar"))
		, SectionName(TEXT("ToroUtilities")), bRegistered(false)
	{}

public:

	virtual void RegisterButton()
	{
		if (!bRegistered)
		{
			bRegistered = true;

			FToolMenuOwnerScoped OwnerScoped(this);
			if (UToolMenu* Menu = UToolMenus::Get()->ExtendMenu(MenuHook))
			{
				FToolMenuSection& Section = Menu->FindOrAddSection(SectionName);
				Section.AddEntry(FToolMenuEntry::InitToolBarButton(Name, 
					FExecuteAction::CreateRaw(this, &FToroToolbarButton::Execute),
					Label, Tooltip, GetSlateIcon()));
			}
		}
	}

	virtual void UnregisterButton()
	{
		if (bRegistered)
		{
			bRegistered = false;
			UToolMenus::UnregisterOwner(this);
		}
	}

	FName GetName() const
	{
		return Name;
	}

protected:

	FText Label, Tooltip;
	FName Name, MenuHook, SectionName;
	bool bRegistered;

	virtual void Execute() = 0;

	virtual FSlateIcon GetSlateIcon()
	{
		return FSlateIcon(FCoreStyle::Get().GetStyleSetName(), TEXT("Launcher.Instance_Unknown"));
	}
};
