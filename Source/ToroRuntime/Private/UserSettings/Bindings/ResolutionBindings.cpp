// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "UserSettings/Bindings/ResolutionBindings.h"
#include "UserSettings/Widgets/ResolutionWidget.h"
#include "UserInterface/ToroWidgetManager.h"

FResolutionBindings::FResolutionBindings(): bReverting(false)
{
	Name = INVTEXT("Resolution");
	Tooltip = INVTEXT("The target resolution of the game window.");
	Impact = EUserOptionImpact::Varies;
}

FString FResolutionBindings::GetValue() const
{
	FIntPoint Res = GetSettings()->GetScreenResolution();
	if (bReverting)
	{
		Res = OldOption;
		const_cast<FResolutionBindings*>(this)->bReverting = false;
	}
	return FString::Printf(TEXT("%dx%d"), Res.X, Res.Y);
}

void FResolutionBindings::SetValue(const FString InValue)
{
	const TArray<FIntPoint>& Resolutions = UToroUserSettings::GetSupportedResolutions();
	const FIntPoint& Selection = DecomposeResolution(InValue);
	if (Resolutions.Contains(Selection))
	{
		ApplyInternal(Selection, false);
		if (UResolutionWidget* Widget = AToroWidgetManager::GetWidget<UResolutionWidget>(GetGameInstance()))
		{
			Widget->ShowWidget([this]()
			{
				RevertValue();
			}, [this, Selection]()
			{
				OldOption = Selection;
			});
		}
	}
}

void FResolutionBindings::InitBinding()
{
	Super::InitBinding();
	const TArray<FIntPoint>& Resolutions = UToroUserSettings::GetSupportedResolutions();
	Options.Empty(Resolutions.Num());
	for (const FIntPoint& Res : Resolutions)
	{
		Options.Add(FString::Printf(TEXT("%dx%d"), Res.X, Res.Y));
	}
	OldOption = GetSettings()->GetScreenResolution();
}

void FResolutionBindings::RevertValue() const
{
	const_cast<FResolutionBindings*>(this)->bReverting = true;
	ApplyInternal(OldOption, true);
}

void FResolutionBindings::ApplyInternal(const FIntPoint& Resolution, const bool bRefreshUI) const
{
	GetSettings()->SetAdjustedFullscreenMode(Resolution);
	GetSettings()->SetScreenResolution(Resolution);
	GetSettings()->ApplyResolutionSettings(false);
	if (bRefreshUI) GetSettings()->OnSettingsUpdated.Broadcast(ESettingApplyType::UI);
}

FIntPoint FResolutionBindings::DecomposeResolution(const FString& Option)
{
	FString LHS = TEXT("1920"), RHS = TEXT("1080");
	Option.Split(TEXT("x"), &LHS, &RHS, ESearchCase::CaseSensitive);
	return {FCString::Atoi(*LHS), FCString::Atoi(*RHS)};
}

FBorderlessBinding::FBorderlessBinding()
{
	Name = INVTEXT("Borderless");
	Tooltip = INVTEXT("Use borderless window instead of fullscreen or windowed.");
}

bool FBorderlessBinding::GetValue() const
{
	return GetSettings()->GetBorderless();
}

void FBorderlessBinding::SetValue(const bool InValue)
{
	bOldValue = GetValue();
	ApplyInternal(InValue, false);
	if (UResolutionWidget* Widget = AToroWidgetManager::GetWidget<UResolutionWidget>(GetGameInstance()))
	{
		Widget->ShowWidget([this]()
		{
			RevertValue();
		});
	}
}

void FBorderlessBinding::RevertValue() const
{
	ApplyInternal(bOldValue, true);
}

void FBorderlessBinding::ApplyInternal(const bool bBorderless, const bool bRefreshUI) const
{
	GetSettings()->SetBorderless(bBorderless);
	GetSettings()->ApplyResolutionSettings(false);
	if (bRefreshUI) GetSettings()->OnSettingsUpdated.Broadcast(ESettingApplyType::UI);
}

FResolutionScaleBinding::FResolutionScaleBinding()
{
	Name = INVTEXT("Resolution Scale");
	Tooltip = INVTEXT("Percentage of target resolution to render. Low values improve performance but reduce quality.");
	Impact = EUserOptionImpact::Varies;
}
