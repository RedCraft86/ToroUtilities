// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroRuntime.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/RichTextBlockImageDecorator.h"
#include "UserInterface/ToroManagedWidget.h"
#include "UserInterface/ExprTextBlock.h"
#include "NoticeWidget.generated.h"

USTRUCT(BlueprintType)
struct FToroSimpleMsg
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SimpleMessage)
		FText Message;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SimpleMessage, meta = (ClampMin = 0.0f, UIMin = 0.0f))
		float ExtraTime;

	FToroSimpleMsg(): ExtraTime(0.0f) {}
	FToroSimpleMsg(const FText& InMessage, const float InExtraTime = 0.0f)
		: Message(InMessage), ExtraTime(InExtraTime)
	{}

	bool Matches(const FToroSimpleMsg& Other) const { return Message.EqualTo(Other.Message); }
	bool IsValidData() const { return !Message.IsEmptyOrWhitespace() && ExtraTime >= 0.0f; }
	float CalcTime() const { return ExtraTime + UToroSettings::CalcReadingTime(Message); }
};

USTRUCT(BlueprintType)
struct FToroInputPrompt
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Prompt)
		FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Prompt)
		TSet<FKey> Keys;

	FToroInputPrompt() {}
	FToroInputPrompt(const FText& InName, const TSet<FKey>& InKeys): Name(InName), Keys(InKeys) {}
	bool IsValidData() const { return !Name.IsEmptyOrWhitespace() && !Keys.IsEmpty(); }
};

UCLASS(Abstract)
class TORORUNTIME_API UInputPreviewEntry final : public UUserWidget
{
	GENERATED_BODY()

public:

	UInputPreviewEntry(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{}

	void InitializeWidget(const FToroInputPrompt& InPreview);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> NameText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UHorizontalBox> KeysBox;

	UFUNCTION(BlueprintImplementableEvent)
		FRichImageRow GetKeyIcon(const FName& InKey);
};

UCLASS(Abstract)
class TORORUNTIME_API UNoticeWidget final : public UToroManagedWidget
{
	GENERATED_BODY()

public:

	UNoticeWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = Message, meta = (WorldContext = "ContextObject"))
		static void QueueNotice(const UObject* ContextObject, const FToroSimpleMsg& InNotice, bool bImmediate = false);

	UFUNCTION(BlueprintCallable, Category = Message, meta = (WorldContext = "ContextObject"))
		static void QueueTitle(const UObject* ContextObject, const FToroSimpleMsg& InTitle, bool bImmediate);

	UFUNCTION(BlueprintCallable, Category = Message, meta = (WorldContext = "ContextObject"))
		static void ShowInputPrompt(const UObject* ContextObject, const FName InKey, const FToroInputPrompt& InData);

	UFUNCTION(BlueprintCallable, Category = Message, meta = (WorldContext = "ContextObject"))
		static void HideInputPrompt(const UObject* ContextObject, const FName InKey);

	UFUNCTION(BlueprintCallable, Category = Message, meta = (WorldContext = "ContextObject"))
		static void ClearInputPrompts(const UObject* ContextObject);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UExprTextBlock> NoticeText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UExprTextBlock> TitleText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> InputContainer;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> NoticeAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> TitleAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> InputRowAnim;

	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<UInputPreviewEntry> InputEntryClass;

	UPROPERTY(Transient)
		TMap<FName, TObjectPtr<UInputPreviewEntry>> InputPreviews;

	FTimerHandle NoticeTimer;
	TArray<FToroSimpleMsg> Notices;
	void ShowNotice(const FToroSimpleMsg& InData);
	void AddNotice(const FToroSimpleMsg& InNotice, const bool bImmediate = false);
	void NextNotice();

	FTimerHandle TitleTimer;
	TArray<FToroSimpleMsg> Titles;
	void ShowTitle(const FToroSimpleMsg& InData);
	void AddTitle(const FToroSimpleMsg& InTitle, const bool bImmediate = false);
	void NextTitle();

	void ClearInputRows();
	void RemoveInputRow(const FName& InKey);
	void AddInputRow(const FName& InKey, const FToroInputPrompt& InData);

	virtual void InitWidget(APlayerController* Controller) override;
};
