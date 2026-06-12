// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "UserWidgets/ToroUserDialog.h"
#include "Animation/WidgetAnimation.h"
#include "Framework/ToroPlayerHUD.h"
#include "ToroUtilitiesSettings.h"

UToroUserDialog::UToroUserDialog(const FObjectInitializer& ObjectInit)
	: Super(ObjectInit), EntryPadding(2.0f)
{
}

UToroUserDialog* UToroUserDialog::CreateUserDialog(const UObject* ContextObject, const FText& Title, 
	const FText& Message, const TArray<FToroUserDialogEntry>& Buttons, const TEnumAsByte<EOrientation> ButtonLayout)
{
	const UToroUtilitiesSettings* Settings = UToroUtilitiesSettings::Get();
	if (Settings && Settings->UserDialogClass.LoadSynchronous())
	{
		AToroPlayerHUD* HUD = AToroPlayerHUD::Get(ContextObject);
		UToroMasterWidget* MasterWidget = HUD ? HUD->GetMasterWidget() : nullptr;
		if (UToroUserDialog* Dialog = CreateWidget<UToroUserDialog>(MasterWidget, Settings->UserDialogClass.Get()))
		{
			Dialog->ConstructDialog(MasterWidget, Title, Message, Buttons, ButtonLayout);
			return Dialog;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create user dialog as class isn't provided."));
	}

	return nullptr;
}

void UToroUserDialog::PushUserDialog()
{
	MasterWidget->PushInstanceToOverlay(this);
}

void UToroUserDialog::OnButtonClicked(UCommonLabeledButton* Button)
{
	if (const FName* Tag = ButtonToIdentifier.Find(Button))
	{
		OnResultSelected.Broadcast(*Tag);
		OnResultSelectedBP.Broadcast(*Tag);
	}

	FadeOutAndRemoveFromParent();
}

void UToroUserDialog::ConstructDialog(UToroMasterWidget* Master, const FText& TitleText, 
	const FText& MessageText, const TArray<FToroUserDialogEntry>& Entries, const TEnumAsByte<EOrientation> Layout)
{
	MasterWidget = Master;
	TitleLabel->SetText(TitleText);
	MessageLabel->SetText(MessageText);
	ButtonContainer->SetOrientation(Layout);

	for (const FToroUserDialogEntry& Entry : Entries)
	{
		if (!Entry.IsValidEntry())
		{
			continue;
		}

		if (UCommonLabeledButton* Button = WidgetTree->ConstructWidget<UCommonLabeledButton>(
			UCommonLabeledButton::StaticClass(), FName(Entry.Identifier.ToString() + TEXT("_Button"))))
		{
			Button->SetPadding(FMargin(EntryPadding));
			Button->SetLabelContentText(Entry.GetDisplayText());
			Button->OnClicked().AddUObject(this, &UToroUserDialog::OnButtonClicked, Button);
			ButtonToIdentifier.Add(Button, Entry.Identifier);
			ButtonContainer->AddChild(Button);
		}
	}
}
