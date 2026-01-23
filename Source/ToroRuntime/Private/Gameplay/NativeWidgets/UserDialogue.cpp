// Copyright (C) RedCraft86. All Rights Reserved.

#include "NativeWidgets/UserDialogue.h"
#include "UserInterface/NativeContainers.h"
#include "UserInterface/ToroWidgetManager.h"
#include "UserInterface/ExprTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

UUserDialogueButton::FOnButtonClicked& UUserDialogueButton::InitializeWidget(const FText& Text, const uint8 Idx)
{
	Label->SetText(Text);
	Button->OnClicked.AddDynamic(this, &UUserDialogueButton::OnButtonClicked);

	Index = Idx;
	return OnClicked;
}

UUserDialogue::UUserDialogue(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoPush = false;
	ContainerClass = UOverlayWidgetContainer::StaticClass();
	UUserWidget::SetVisibility(ESlateVisibility::Visible);
}

UUserDialogue* UUserDialogue::ShowDialogue(UObject* ContextObject, const FText& Title, const FText& Content,
	const TArray<FText>& Options, const TFunction<void(uint8)>& PickedCallback)
{
	if (UUserDialogue* Widget = AToroWidgetManager::GetWidget<UUserDialogue>(ContextObject))
	{
		Widget->OnPicked = PickedCallback;
		Widget->Label->SetText(Title);
		Widget->Desc->SetText(Content);
		Widget->AddButtons(Options);
		Widget->PushWidget();
		return Widget;
	}
	return nullptr;
}

void UUserDialogue::OnSelected(const uint8 Idx)
{
	OnPickedBP.Broadcast(Idx);
	if (OnPicked) OnPicked(Idx);
	PopWidget();
}

void UUserDialogue::AddButtons(const TArray<FText>& Options)
{
	Buttons.Reserve(Options.Num());
	for (int32 i = 0; i < Options.Num(); ++i)
	{
		if (Options[i].IsEmptyOrWhitespace())
		{
			continue;
		}
		
		if (UUserDialogueButton* NewButton = CreateWidget<UUserDialogueButton>(this, EntryClass))
		{
			NewButton->InitializeWidget(Options[i], i).AddUObject(this, &UUserDialogue::OnSelected);
			ButtonBox->AddChild(NewButton);
			Buttons.Add(NewButton);
		}
	}
}

void UUserDialogue::PushWidget()
{
	Super::PushWidget();
	PlayAnimation(ShowAnim);

	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		InputConfig = PC->GetInputConfig();
		PC->SetInputConfig({EGameInputMode::UI_Only, true,
			EMouseLockMode::LockAlways, false});
		PC->AddPauseRequest(this);
	}
}

void UUserDialogue::PopWidget()
{
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->SetInputConfig(InputConfig);
		PC->RemovePauseRequest(this);
	}

	for (UUserDialogueButton* Button : Buttons)
	{
		Button->RemoveFromParent();
	}

	Buttons.Empty();

	OnPicked = nullptr;
	OnPickedBP.Clear();
	Super::PopWidget();
}
