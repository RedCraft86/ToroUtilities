// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

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
