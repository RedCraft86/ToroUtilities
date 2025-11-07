// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserSettings/Bindings/ResolutionBindings.h"
#include "UserSettings/Widgets/ResolutionWidget.h"
#include "UserInterface/ToroWidgetManager.h"

FResolutionBindings::FResolutionBindings()
{
	Name = INVTEXT("Resolution");
	Tooltip = INVTEXT("The target resolution of the game window.");
	Impact = EUserOptionImpact::Varies;
}

FString FResolutionBindings::GetValue() const
{
	const FIntPoint Res = GetSettings()->GetScreenResolution();
	return FString::Printf(TEXT("%dx%d"), Res.X, Res.Y);
}

void FResolutionBindings::SetValue(const FString InValue)
{
	OldOption = GetValue();
	const TArray<FIntPoint>& Resolutions = UToroUserSettings::GetSupportedResolutions();
	const FIntPoint& Selection = DecomposeResolution(InValue);
	if (Resolutions.Contains(Selection))
	{
		ApplyInternal(Selection);
		if (UResolutionWidget* Widget = AToroWidgetManager::GetWidget<UResolutionWidget>(GetGameInstance()))
		{
			Widget->ShowWidget([this]()
			{
				RevertValue();
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
}

void FResolutionBindings::RevertValue() const
{
	const TArray<FIntPoint>& Resolutions = UToroUserSettings::GetSupportedResolutions();
	const FIntPoint& Selection = DecomposeResolution(OldOption);
	if (Resolutions.Contains(Selection))
	{
		ApplyInternal(Selection);
	}
}

void FResolutionBindings::ApplyInternal(const FIntPoint& Resolution) const
{
	GetSettings()->SetScreenResolution(Resolution);
	GetSettings()->ApplyResolutionSettings(true);
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
	ApplyInternal(InValue);
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
	ApplyInternal(bOldValue);
}

void FBorderlessBinding::ApplyInternal(const bool bBorderless) const
{
	GetSettings()->SetBorderless(bBorderless);
	GetSettings()->ApplyResolutionSettings(true);
}

FResolutionScaleBinding::FResolutionScaleBinding()
{
	Name = INVTEXT("Resolution Scale");
	Tooltip = INVTEXT("Percentage of target resolution to render. Low values improve performance but reduce quality.");
	Impact = EUserOptionImpact::Varies;
}
