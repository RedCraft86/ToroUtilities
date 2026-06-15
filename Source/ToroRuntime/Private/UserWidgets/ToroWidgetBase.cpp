// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "UserWidgets/ToroWidgetBase.h"
#include "Animation/WidgetAnimation.h"

UE5Coro::TCoroutine<> FadeOutRemoveWidget(TWeakObjectPtr<UUserWidget> Widget, UWidgetAnimation* FadeAnim)
{
	if (!Widget.IsValid())
	{
		co_return;
	}

	if (!FadeAnim)
	{
		Widget->RemoveFromParent();
		co_return;
	}

	Widget->PlayAnimationReverse(FadeAnim);

	// Wait the difference in seconds from start to current time as PlayAnimationReverse works based on current state
	const float WaitTime = FMath::Max(Widget->GetAnimationCurrentTime(FadeAnim) - FadeAnim->GetStartTime(), 0.0f);
	co_await UE5Coro::Latent::RealSeconds(WaitTime + 0.1f); // Minor offset

	if (Widget.IsValid())
	{
		Widget->RemoveFromParent();
	}
}

UToroUserWidget::UToroUserWidget(const FObjectInitializer& ObjectInit)
	: Super(ObjectInit), bFadeInOnConstruct(true)
{
}

void UToroUserWidget::FadeOutAndRemoveFromParent()
{
	FadeOutRemoveWidget(this, FadeAnim);
}

void UToroUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (FadeAnim)
	{
		if (bFadeInOnConstruct)
		{
			PlayAnimation(FadeAnim);
		}
		else
		{
			// Somewhat hacky way to skip the animation as SetAnimationCurrentTime does not evaluate
			PlayAnimation(FadeAnim, FadeAnim->GetEndTime() - UE_KINDA_SMALL_NUMBER);
		}
	}
}

#if WITH_EDITOR
const FText UToroUserWidget::GetPaletteCategory()
{
	return PaletteCategory;
}
#endif

UToroActivatableWidget::UToroActivatableWidget(const FObjectInitializer& ObjectInit)
	: Super(ObjectInit), bFadeInOnConstruct(false)
{
	bAutoActivate = true;
}

void UToroActivatableWidget::FadeOutAndRemoveFromParent()
{
	FadeOutRemoveWidget(this, FadeAnim);
}

void UToroActivatableWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (FadeAnim)
	{
		if (bFadeInOnConstruct)
		{
			PlayAnimation(FadeAnim);
		}
		else
		{
			// Somewhat hacky way to skip the animation as SetAnimationCurrentTime does not evaluate
			PlayAnimation(FadeAnim, FadeAnim->GetEndTime() - UE_KINDA_SMALL_NUMBER);
		}
	}
}

#if WITH_EDITOR
const FText UToroActivatableWidget::GetPaletteCategory()
{
	return UToroUserWidget::PaletteCategory;
}
#endif
