// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Narrative/Widgets/DialogueWidget.h"
#include "Gameplay/Narrative/NarrativeManager.h"
#include "UserInterface/NativeContainers.h"
#include "Framework/ToroPlayerController.h"
#include "Helpers/WidgetAnimHelpers.h"
#include "Blueprint/WidgetTree.h"
#include "NarrativeDataTask.h"

UDialogueReplyWidget::UDialogueReplyWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), SelectedColor(FLinearColor::Yellow), UnselectedColor(FLinearColor::Gray)
{}

void UDialogueReplyWidget::SetSelectState(const bool bSelected)
{
	SetRenderOpacity(bSelected ? 1.0f : 0.5f);
	DisplayText->SetColorAndOpacity(bSelected ? SelectedColor : UnselectedColor);
}

void UDialogueReplyWidget::InitializeWidget(UDialogueWidget* InParent, UDialogue* InDialogue, UDialogueNode_Player* InReply)
{
	ReplyObj = InReply;
	ParentUI = InParent;
	SelectButton->OnClicked.AddUniqueDynamic(this, &UDialogueReplyWidget::OnOptionClicked);
	DisplayText->SetText(ParentUI->InjectTextVariables(ReplyObj->GetOptionText(InDialogue)));
	if (ParentUI->HasChosenOption(ReplyObj))
	{
		DisplayText->SetColorAndOpacity(UnselectedColor);
	}
}

void UDialogueReplyWidget::OnOptionClicked()
{
	SetIsEnabled(false);
	ParentUI->SelectReplyOption(ReplyObj);
}

UDialogueWidget::UDialogueWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), NameColor(FLinearColor::White), PlayerColor(FLinearColor::Yellow)
{
	bAutoPush = false;
	ContainerClass = UGameplayWidgetContainer::StaticClass();
#if WITH_EDITOR
	const ConstructorHelpers::FObjectFinder<UNarrativeDataTask> ReplyTaskFinder(
		TEXT("/Narrative/DefaultTasks/PlayDialogueNode.PlayDialogueNode"));
	if (ReplyTaskFinder.Succeeded())
	{
		ReplyTaskType = ReplyTaskFinder.Object;
	}
#endif
}

void UDialogueWidget::PushWidget()
{
	Super::PushWidget();
	if (AToroPlayerController* PC = GetOwningPlayer<AToroPlayerController>())
	{
		InputConfig = PC->GetInputConfig();
		PC->SetInputConfig({EGameInputMode::GameAndUI, true,
			EMouseLockMode::LockAlways, false, this});
	}
}

void UDialogueWidget::PopWidget()
{
	if (AToroPlayerController* PC = GetOwningPlayer<AToroPlayerController>())
	{
		PC->SetInputConfig(InputConfig);
	}
	Super::PopWidget();
}

void UDialogueWidget::OnSkipClicked()
{
	Manager->SkipCurrentDialogueLine();
}

void UDialogueWidget::OnFinished(UDialogue* Dialogue, const bool bStartingNewDialogue)
{
	ReplyBox->ClearChildren();
	WidgetAnimHelpers::PlayOrSnapAnim(this, RepliesAnim, false, true);
	PopWidget();
}

void UDialogueWidget::OnRepliesAvailable(UDialogue* Dialogue, const TArray<UDialogueNode_Player*>& PlayerReplies)
{
	Replies.Empty();
	ReplyBox->ClearChildren();
	if (PlayerReplies.Num() == 1)
	{
		Manager->SelectDialogueOption(PlayerReplies[0]);
		return;
	}

	SkipButton->SetIsEnabled(false);
	SkipButton->SetRenderOpacity(0.25f);
	for (UDialogueNode_Player* Reply : PlayerReplies)
	{
		if (!Reply) continue;
		if (UDialogueReplyWidget* Widget = WidgetTree->ConstructWidget<UDialogueReplyWidget>(ReplyClass))
		{
			Replies.Add(Widget);
			ReplyBox->AddChild(Widget);
			Widget->InitializeWidget(this, Dialogue, Reply);
		}
	}

	PlayAnimation(RepliesAnim, 0.0f, 1,
		Replies.IsEmpty() ? EUMGSequencePlayMode::Reverse : EUMGSequencePlayMode::Forward);
}

void UDialogueWidget::OnPlayerLineStarted(UDialogue* Dialogue, UDialogueNode_Player* Node, const FDialogueLine& Line)
{
	SpeakerText->SetText(INVTEXT("You"));
	SpeakerText->SetColorAndOpacity(PlayerColor);
	LineText->SetText(InjectTextVariables(Line.Text));
	PlayAnimation(FadeAnim);
}

void UDialogueWidget::OnPlayerLineFinished(UDialogue* Dialogue, UDialogueNode_Player* Node, const FDialogueLine& Line)
{
	FWidgetAnimationState* State = PlayAnimationReverse(RepliesAnim).GetAnimationState();
	if (State && !ReplyFinishHandle.IsValid())
	{
		ReplyFinishHandle = State->GetOnWidgetAnimationFinished().AddUObject(
			this, &UDialogueWidget::OnReplyAnimFinished);
	}
}

void UDialogueWidget::OnNPCLineStarted(UDialogue* Dialogue, UDialogueNode_NPC* Node, const FDialogueLine& Line, const FSpeakerInfo& Speaker)
{
	SpeakerText->SetText(Speaker.SpeakerName);
	SpeakerText->SetColorAndOpacity(NameColor);
	LineText->SetText(InjectTextVariables(Line.Text));
	PlayAnimation(FadeAnim);
}

void UDialogueWidget::OnReplyAnimFinished(FWidgetAnimationState& State)
{
	Replies.Empty();
	ReplyBox->ClearChildren();
}

bool UDialogueWidget::HasChosenOption(const UDialogueNode_Player* InChoice) const
{
	return Manager->HasCompletedTask(ReplyTaskType, InChoice->GetID().ToString(), 1);
}

void UDialogueWidget::SelectReplyOption(UDialogueNode_Player* InChoice)
{
	Manager->SelectDialogueOption(InChoice);
	for (UDialogueReplyWidget* Reply : Replies)
	{
		Reply->SetSelectState(Reply->GetID() == InChoice->GetID());
	}

	SkipButton->SetIsEnabled(true);
	SkipButton->SetRenderOpacity(1.0f);
	PlayAnimation(RepliesAnim);
}

FText UDialogueWidget::InjectTextVariables(const FText& InText) const
{
	return Manager->InjectTextVariables(InText);
}

void UDialogueWidget::InitWidget(APlayerController* Controller)
{
	Super::InitWidget(Controller);
	SkipButton->OnClicked.AddDynamic(this, &ThisClass::OnSkipClicked);
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		Manager = UNarrativeManager::Get(this);

		Manager->OnDialogueFinished.AddDynamic(this, &ThisClass::OnFinished);
		Manager->OnDialogueRepliesAvailable.AddDynamic(this, &ThisClass::OnRepliesAvailable);
		Manager->OnPlayerDialogueLineStarted.AddDynamic(this, &ThisClass::OnPlayerLineStarted);
		Manager->OnPlayerDialogueLineFinished.AddDynamic(this, &ThisClass::OnPlayerLineFinished);
		Manager->OnNPCDialogueLineStarted.AddDynamic(this, &ThisClass::OnNPCLineStarted);
	});
}
