// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "UserInterface/ToroWidgetBase.h"
#include "Helpers/WorldGetter.h"

#define HIDE_CHECK_INTERVAL 0.5f

UToroWidgetBase::UToroWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bCollapseOnDeactivate(false), bWantsToHide(false)
	, bHidden(false), HideCheckTime(0.0f)
{
	bAutoActivate = true;
}

UToroWidgetBase* UToroWidgetBase::CreateToroWidget(APlayerController* Owner, const TSubclassOf<UToroWidgetBase> WidgetClass)
{
	if (!Owner || !WidgetClass)
	{
		return nullptr;
	}

	const UToroWidgetBase* CDO = WidgetClass->GetDefaultObject<UToroWidgetBase>();
	if (!CDO || !CDO->CanCreateWidget(Owner))
	{
		return nullptr;
	}

	if (UToroWidgetBase* Widget = CreateWidget<UToroWidgetBase>(FWorldGetter::Get(Owner), WidgetClass))
	{
		Widget->InitWidget(Owner);
		return Widget;
	}

	return nullptr;
}

void UToroWidgetBase::SetHidden(const bool bInHidden)
{
	if (bWantsToHide != bInHidden)
	{
		bWantsToHide = bInHidden;
		HideCheckTime = HIDE_CHECK_INTERVAL;
	}
}

void UToroWidgetBase::UpdateFadeState()
{
	HideCheckTime = 0.0f;
	const bool bTargetHidden = bWantsToHide || ShouldHideWidget();
	if (bHidden != bTargetHidden)
	{
		bHidden = bTargetHidden;
		bHidden ? PlayAnimationForward(HideAnim) : PlayAnimationReverse(HideAnim);
	}
}

void UToroWidgetBase::InitWidget(APlayerController* Controller)
{
	GameMode = Controller->GetWorld()->GetAuthGameMode();
}

void UToroWidgetBase::InternalProcessActivation()
{
	Super::InternalProcessActivation();
	SetHidden(false);

	if (!DefaultVisibility.IsSet())
	{
		DefaultVisibility = GetVisibility();
	}
	else if (bCollapseOnDeactivate)
	{
		SetVisibility(DefaultVisibility.GetValue());
	}
}

void UToroWidgetBase::InternalProcessDeactivation()
{
	if (!DefaultVisibility.IsSet())
	{
		DefaultVisibility = GetVisibility();
	}
	if (bCollapseOnDeactivate)
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}

	SetHidden(true);
	Super::InternalProcessDeactivation();
}

void UToroWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (HideCheckTime < HIDE_CHECK_INTERVAL)
	{
		HideCheckTime += InDeltaTime;
		return;
	}
	HideCheckTime = 0.0f;
	UpdateFadeState();
}
