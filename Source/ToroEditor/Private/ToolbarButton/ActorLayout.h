// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "SlateStyle/ToroEditorStyle.h"
#include "ToolbarButton/ToroToolbarButton.h"

class FActorLayoutParameter
{
	int32 Value;
	TFunction<int32(const int32)> Processor;

public:

	FActorLayoutParameter(const int32 InValue, const TFunction<int32(const int32)>& InProcessor)
		: Value(InValue), Processor(InProcessor)
	{}

	int32 GetValue() const
	{
		return Value;
	}

	void SetValue(const float InValue)
	{
		Value = Processor(FMath::TruncToInt32(InValue));
	}
};

class FActorLayoutButton final : public FToroToolbarButton
{
public:

	FActorLayoutButton()
		: FToroToolbarButton(MAKE_TOOLBAR_BUTTON_NAME(ActorLayout), INVTEXT("Actor Layout"),
			INVTEXT("Lays out actors in a grid form"))
	{
		MenuHook = TEXT("LevelEditor.LevelEditorToolBar.PlayToolBar");
		SectionName = TEXT("Actor");
	}

	static inline FActorLayoutParameter MaxColumns {
		10, [](const int32 InValue){
			return FMath::Max(InValue, 1);
		}
	};

	static inline FActorLayoutParameter XOffset {
		100, [](const int32 InValue){
			return FMath::Max(InValue, 1);
		}
	};

	static inline FActorLayoutParameter YOffset {
		100, [](const int32 InValue){
			return FMath::Max(InValue, 1);
		}
	};

	static inline FActorLayoutParameter YawValue {
		0, [](const int32 InValue){
			return FMath::Clamp(InValue, 0, 360);
		}
	};

private:

	static inline TSharedPtr<SWindow> Window = nullptr;

	virtual FSlateIcon GetSlateIcon() override
	{
		return FSlateIcon(FToroEditorStyle::GetName(), TEXT("Toolbar.ActorLayout"));
	}

	virtual void Execute() override;
};