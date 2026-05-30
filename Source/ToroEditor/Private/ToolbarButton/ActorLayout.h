// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "ToroEditorStyle.h"
#include "ToolbarButton/ToroToolbarButton.h"

class FActorLayout final : public FToroToolbarButton
{
public:

	FActorLayout()
		: FToroToolbarButton(TEXT("ActorLayout"), INVTEXT("Actor Layout"), 
			INVTEXT("Lays out actors in a grid form"))
	{
		MenuHook = TEXT("LevelEditor.LevelEditorToolBar.PlayToolBar");
		SectionName = TEXT("Actor");
	}

	static inline int32 OffsetX = 500;
	static float GetOffsetX() { return OffsetX; }
	static void SetOffsetX(const float InValue) { OffsetX = FMath::Max(FMath::RoundToInt(InValue), 1); }

	static inline int32 OffsetY = 500;
	static float GetOffsetY() { return OffsetY; }
	static void SetOffsetY(const float InValue) { OffsetY = FMath::Max(FMath::RoundToInt(InValue), 1); }

	static inline int32 Yaw = 0;
	static float GetYaw() { return Yaw; }
	static void SetYaw(const float InValue) { Yaw = FMath::Clamp(FMath::RoundToInt(InValue), -90, 90); }

	static inline uint8 PerRow = 10;
	static float GetPerRow() { return PerRow; }
	static void SetPerRow(const float InValue) { PerRow = FMath::Max(FMath::RoundToInt(InValue), 1); }

	static void LayoutActors();

private:

	static inline TSharedPtr<SWindow> Window = nullptr;

	virtual FSlateIcon GetSlateIcon() override
	{
		return FSlateIcon(FToroEditorStyle::GetName(), TEXT("Toolbar.ActorLayout"));
	}

	virtual void Execute() override;
};