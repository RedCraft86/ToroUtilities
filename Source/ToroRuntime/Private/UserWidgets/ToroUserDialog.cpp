// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "UserWidgets/ToroUserDialog.h"
#include "Framework/ToroPlayerController.h"
#include "Animation/WidgetAnimation.h"
#include "Framework/ToroPlayerHUD.h"
#include "Components/OverlaySlot.h"
#include "ToroSettings.h"
#include "ToroRuntime.h"

FText FToroUserDialogEntry::GetDisplayText() const
{
	const FText BaseText = DisplayText.IsEmptyOrWhitespace() ? FText::FromName(Identifier) : DisplayText;
	return (SelectTime > 0) ? FText::Format(INVTEXT("{0} ({1})"), BaseText, SelectTime) : BaseText;
}

UToroUserDialog::UToroUserDialog(const FObjectInitializer& ObjectInit)
	: Super(ObjectInit), EntryPadding(2.0f), AutoSelectTime(0.0f)
{
	bIsModal = true;
	bAutoActivate = false;

	bSetVisibilityOnActivated = true;
	ActivatedVisibility = ESlateVisibility::Visible;

	bSetVisibilityOnDeactivated = true;
	DeactivatedVisibility = ESlateVisibility::HitTestInvisible;
}

UToroUserDialog* UToroUserDialog::CreateUserDialog(const UObject* ContextObject, const FText& Title, const FText& Message, 
	const TArray<FToroUserDialogEntry>& Buttons, const TEnumAsByte<EOrientation> ButtonLayout, const bool bPauseGame)
{
	const UToroSettings* Settings = UToroSettings::Get();
	if (Settings && Settings->UserDialogClass.LoadSynchronous())
	{
		AToroPlayerHUD* HUD = AToroPlayerHUD::Get(ContextObject);
		UToroMasterWidget* MasterWidget = HUD ? HUD->GetMasterWidget() : nullptr;
		if (UToroUserDialog* Dialog = CreateWidget<UToroUserDialog>(MasterWidget, Settings->UserDialogClass.Get()))
		{
			Dialog->ConstructDialog(MasterWidget, Title, Message, Buttons, ButtonLayout, bPauseGame);
			return Dialog;
		}

		UE_LOG(LogToroRuntime, Error, TEXT("Failed to create user dialog as Master Widget could not be obtained."));
	}
	else
	{
		UE_LOG(LogToroRuntime, Error, TEXT("Failed to create user dialog as class isn't provided."));
	}

	return nullptr;
}

void UToroUserDialog::PushUserDialog()
{
	if (UOverlaySlot* OverlaySlot = MasterWidget->PushInstanceToOverlay(this))
	{
		OverlaySlot->SetPadding(FMargin(0.0f));
		OverlaySlot->SetHorizontalAlignment(HAlign_Fill);
		OverlaySlot->SetVerticalAlignment(VAlign_Fill);
		ActivateWidget();

		if (PauserController.IsValid() && !PauserController->IsPaused())
		{
			PauserController->SetPause(true);
		}
		else
		{
			// If game is already pause, we do not want to mess it up by unpausing after this dialog
			PauserController.Reset();
		}
	}
	else
	{
		UE_LOG(LogToroRuntime, Error, TEXT("Failed to push user dialoge to an overlay slot."));
	}
}

void UToroUserDialog::OnButtonClicked(UCommonLabeledButton* Button)
{
	if (!IsActivated())
	{
		return;
	}

	if (PauserController.IsValid())
	{
		PauserController->SetPause(false);
	}

	const FToroUserDialogEntry& Entry = ButtonToEntry[Button];
	OnResultSelected.Broadcast(Entry.Identifier);
	OnResultSelectedBP.Broadcast(Entry.Identifier);

	DeactivateWidget();
	FadeOutAndRemoveFromParent();
}

void UToroUserDialog::ConstructDialog(UToroMasterWidget* Master, const FText& TitleText, const FText& MessageText, 
	const TArray<FToroUserDialogEntry>& Entries, const TEnumAsByte<EOrientation> Layout, const bool bPauseGame)
{
	MasterWidget = Master;
	TitleLabel->SetText(TitleText);
	MessageLabel->SetText(MessageText);
	ButtonContainer->SetOrientation(Layout);

	if (bPauseGame)
	{
		PauserController = AToroPlayerController::Get(Master);
	}
	else
	{
		PauserController.Reset();
	}

	for (const FToroUserDialogEntry& Entry : Entries)
	{
		if (!Entry.IsValidEntry())
		{
			continue;
		}

		if (UCommonLabeledButton* Button = WidgetTree->ConstructWidget<UCommonLabeledButton>(UCommonLabeledButton::StaticClass()))
		{
			Button->SetPadding(FMargin(EntryPadding));
			Button->SetLabelContentText(Entry.GetDisplayText());
			Button->OnClicked().AddUObject(this, &UToroUserDialog::OnButtonClicked, Button);
			ButtonToEntry.Add(Button, Entry);
			ButtonContainer->AddChild(Button);

			if (!AutoSelectButton.IsValid() && Entry.SelectTime > 0)
			{
				AutoSelectButton = Button;
				AutoSelectTime = Entry.SelectTime;
			}
		}
	}
}

void UToroUserDialog::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (Background)
	{
		Background->SetVisibility(ESlateVisibility::Visible);
	}
}

UWidget* UToroUserDialog::NativeGetDesiredFocusTarget() const
{
	// If we have an auto-select button, focus that first.
	if (AutoSelectButton.IsValid())
	{
		return AutoSelectButton.Get();
	}

	// Fallback: Focus the very first button in the container
	if (ButtonContainer && ButtonContainer->GetChildrenCount() > 0)
	{
		return ButtonContainer->GetChildAt(0);
	}

	return Super::NativeGetDesiredFocusTarget();
}

TOptional<FUIInputConfig> UToroUserDialog::GetDesiredInputConfig() const
{
	return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::CapturePermanently);
}

void UToroUserDialog::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (IsActivated() && AutoSelectButton.IsValid() && AutoSelectTime > 0.0f)
	{
		if (FToroUserDialogEntry* Entry = ButtonToEntry.Find(AutoSelectButton.Get()))
		{
			AutoSelectTime -= InDeltaTime;
			const uint8 TimeAsInt = FMath::Max(0, FMath::CeilToInt32(AutoSelectTime));
			if (Entry->SelectTime != TimeAsInt)
			{
				Entry->SelectTime = TimeAsInt;
				AutoSelectButton->SetLabelContentText(Entry->GetDisplayText());

				if (TimeAsInt == 0)
				{
					OnButtonClicked(AutoSelectButton.Get());
				}
			}
		}
	}
}
