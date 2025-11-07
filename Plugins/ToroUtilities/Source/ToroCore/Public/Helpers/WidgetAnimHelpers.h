// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Animation/WidgetAnimation.h"
#include "Blueprint/UserWidget.h"

namespace WidgetAnimHelpers
{
	inline void InitAnim(UUserWidget* InWidget, UWidgetAnimation* InAnimation)
	{
		InWidget->PlayAnimation(InAnimation);
		InWidget->StopAnimation(InAnimation);
		InWidget->SetAnimationCurrentTime(InAnimation, InAnimation->GetStartTime());
	}

	inline void PlayOrSnapAnim(UUserWidget* InWidget, UWidgetAnimation* InAnimation, const bool bForward, const bool bImmediate)
	{
		const float FinalFrame = bForward
			? InAnimation->GetEndTime() - UE_SMALL_NUMBER
			: InAnimation->GetStartTime() + UE_SMALL_NUMBER;
		const float StartTime = bImmediate ? FinalFrame : InWidget->GetAnimationCurrentTime(InAnimation);
		InWidget->PlayAnimation(InAnimation, StartTime, 1, bForward
			? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
	}

	inline float GetAnimDuration(const UWidgetAnimation* InAnimation)
	{
		return InAnimation->GetEndTime() - InAnimation->GetStartTime();
	}
}
