// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "ToolMenus.h"
#include "Templates/SharedPointer.h"

class TOROEDITOR_API FToroToolbarButton : public TSharedFromThis<FToroToolbarButton>
{
protected:

	FText Label, Tooltip;
	FName Name, MenuHook, SectionName;
	bool bRegistered;

	virtual ~FToroToolbarButton() = default;

	FToroToolbarButton(const FName InName, const FText& InLabel, const FText& InTooltip)
		: Label(InLabel), Tooltip(InTooltip), Name(InName)
		, MenuHook(TEXT("LevelEditor.LevelEditorToolBar.PlayToolBar"))
		, SectionName(TEXT("ToroUtilities")), bRegistered(false)
	{}

	virtual FSlateIcon GetSlateIcon()
	{
		return FSlateIcon(FCoreStyle::Get().GetStyleSetName(), TEXT("Launcher.Instance_Unknown"));
	}

	virtual void Execute() = 0;

public:

	FName GetName() const
	{
		return Name;
	}

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
};