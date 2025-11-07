// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DialogueSM.h"
#include "ToroRuntime.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "DataTypes/GameInputConfig.h"
#include "UserInterface/ExprTextBlock.h"
#include "UserInterface/ToroManagedWidget.h"
#include "DialogueWidget.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UDialogueReplyWidget final : public UUserWidget
{
	GENERATED_BODY()

	friend class UDialogueWidget;
	
public:

	UDialogueReplyWidget(const FObjectInitializer& ObjectInitializer);

	void SetSelectState(const bool bSelected);
	void InitializeWidget(UDialogueWidget* InParent, UDialogue* InDialogue, UDialogueNode_Player* InReply);
	FName GetID() const { return ReplyObj ? ReplyObj->GetID() : NAME_None; }

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> DisplayText;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> SelectButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		FLinearColor SelectedColor;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		FLinearColor UnselectedColor;

	UPROPERTY(Transient)
		TObjectPtr<UDialogueNode_Player> ReplyObj;
	
	TObjectPtr<UDialogueWidget> ParentUI;

	UFUNCTION() void OnOptionClicked();
};

UCLASS(Abstract)
class TORORUNTIME_API UDialogueWidget final : public UToroManagedWidget
{
	GENERATED_BODY()

	friend class UDialogueReplyWidget;

public:

	UDialogueWidget(const FObjectInitializer& ObjectInitializer);

	virtual void PushWidget() override;
	virtual void PopWidget() override;

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> SpeakerText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UExprTextBlock> LineText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> SkipButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> ReplyBox;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> FadeAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> RepliesAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		FLinearColor NameColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		FLinearColor PlayerColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		TObjectPtr<const UNarrativeDataTask> ReplyTaskType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		TSubclassOf<UDialogueReplyWidget> ReplyClass;

	UPROPERTY(Transient)
		TSet<TObjectPtr<UDialogueReplyWidget>> Replies;

	FGameInputConfig InputConfig;
	FDelegateHandle ReplyFinishHandle;
	TObjectPtr<class UNarrativeManager> Manager;

	UFUNCTION() void OnSkipClicked();
	UFUNCTION() void OnFinished(UDialogue* Dialogue, const bool bStartingNewDialogue);
	UFUNCTION() void OnRepliesAvailable(UDialogue* Dialogue, const TArray<UDialogueNode_Player*>& PlayerReplies);
	UFUNCTION() void OnPlayerLineStarted(UDialogue* Dialogue, UDialogueNode_Player* Node, const FDialogueLine& Line);
	UFUNCTION() void OnPlayerLineFinished(UDialogue* Dialogue, UDialogueNode_Player* Node, const FDialogueLine& Line);
	UFUNCTION() void OnNPCLineStarted(UDialogue* Dialogue, UDialogueNode_NPC* Node, const FDialogueLine& Line, const FSpeakerInfo& Speaker);

	void OnReplyAnimFinished(FWidgetAnimationState& State);
	bool HasChosenOption(const UDialogueNode_Player* InChoice) const;
	void SelectReplyOption(UDialogueNode_Player* InChoice);
	FText InjectTextVariables(const FText& InText) const;

	virtual void InitWidget(APlayerController* Controller) override;
	virtual bool CanCreateWidget(const UObject* ContextObject) const override
	{
		return !UToroSettings::Get()->IsOnMap(ContextObject, EToroMapType::MainMenu);
	}
};
