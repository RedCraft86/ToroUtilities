// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataTypes/GameInputConfig.h"
#include "UserInterface/ToroManagedWidget.h"
#include "UserDialogue.generated.h"

class UButton;
class UTextBlock;
class UExprTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUserDialoguePicked, const uint8, Option);

UCLASS(Abstract)
class TORORUNTIME_API UUserDialogueButton final : public UUserWidget
{
	GENERATED_BODY()

public:

	UUserDialogueButton(const FObjectInitializer& ObjectInitializer)
		: UUserWidget(ObjectInitializer)
	{}

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnButtonClicked, const uint8);

	FOnButtonClicked& InitializeWidget(const FText& Text, const uint8 Idx);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> Button;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> Label;
	
	uint8 Index;
	FOnButtonClicked OnClicked;

	// ReSharper disable once CppMemberFunctionMayBeConst
	UFUNCTION() void OnButtonClicked() { OnClicked.Broadcast(Index); }
};

UCLASS(Abstract)
class TORORUNTIME_API UUserDialogue final : public UToroManagedWidget
{
	GENERATED_BODY()

public:

	UUserDialogue(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = Game, meta = (WorldContext = "ContextObject"))
	static UUserDialogue* ShowUserDialogue(UObject* ContextObject, const FText& Title, 
		const FText& Content, const TArray<FText>& Options)
	{
		return ShowDialogue(ContextObject, Title, Content, Options, nullptr);
	}
	
	static UUserDialogue* ShowDialogue(UObject* ContextObject, const FText& Title, const FText& Content, 
		const TArray<FText>& Options, const TFunction<void(uint8)>& PickedCallback);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> Label;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UExprTextBlock> Desc;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> ButtonBox;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> ShowAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements)
		TSet<TObjectPtr<UUserDialogueButton>> Buttons;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
		TSubclassOf<UUserDialogueButton> EntryClass;
	
	UPROPERTY(BlueprintAssignable, DisplayName = "On Picked")
		FOnUserDialoguePicked OnPickedBP;
	
	TFunction<void(uint8)> OnPicked;
	FGameInputConfig InputConfig;
	
	void OnSelected(const uint8 Idx);
	void AddButtons(const TArray<FText>& Options);

	virtual void PushWidget() override;
	virtual void PopWidget() override;
};
