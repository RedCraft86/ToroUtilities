// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#define LOCTEXT_NAMESPACE "ToroGraphEd"

class FToroGraphCommands final : public TCommands<FToroGraphCommands>
{
public:

	FToroGraphCommands()
		: TCommands("ToroGraphEd",
			NSLOCTEXT("Contexts", "ToroGraphEd", "ToroGraph Editor"),
			NAME_None, FAppStyle::GetAppStyleSetName())
	{}

	virtual void RegisterCommands() override
	{
		UI_COMMAND(AutoArrange, "Auto Arrange", "Auto Arrange", EUserInterfaceActionType::Button, FInputChord());
	}

	TSharedPtr<FUICommandInfo> AutoArrange;
};

#undef LOCTEXT_NAMESPACE