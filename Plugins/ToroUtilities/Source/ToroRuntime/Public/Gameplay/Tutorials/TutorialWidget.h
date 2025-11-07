// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroRuntime.h"
#include "TutorialDatabase.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "DataTypes/GameInputConfig.h"
#include "UserInterface/ExprTextBlock.h"
#include "UserInterface/ToroManagedWidget.h"
#include "TutorialWidget.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UTutorialEntryWidget final : public UUserWidget
{
	GENERATED_BODY()

public:

	UTutorialEntryWidget(const FObjectInitializer& ObjectInitializer)
		: UUserWidget(ObjectInitializer)
	{}

	void InitializeWidget(const FTutorialEntry& Entry) const;

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> PreviewImage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UExprTextBlock> ContentText;
};

UCLASS(Abstract)
class TORORUNTIME_API UTutorialWidget final : public UToroManagedWidget
{
	GENERATED_BODY()

public:

	UTutorialWidget(const FObjectInitializer& ObjectInitializer);

	void ShowWidget(const FTutorialEntry& Entry);
	void SetCloseFunc(const TFunction<void()>& OnClose)
	{
		CloseFunc = OnClose;
	}

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> CloseButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> LeftButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> RightButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> PageCount;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UWidgetSwitcher> EntryContainer;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SwapAnimation;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
		TSubclassOf<UTutorialEntryWidget> EntryClass;

	TFunction<void()> CloseFunc;
	FGameInputConfig InputConfig;

	UFUNCTION() void MoveLeft();
	UFUNCTION() void MoveRight();
	UFUNCTION() void CloseMenu() { PopWidget(); }

	void UpdatePageCount() const;
	void CreateEntry(const FTutorialEntry& Entry);

	virtual void PushWidget() override;
	virtual void PopWidget() override;
	virtual void InitWidget(APlayerController* Controller) override;
	virtual bool CanCreateWidget(const UObject* ContextObject) const override
	{
		return !UToroSettings::Get()->IsOnMap(ContextObject, EToroMapType::MainMenu);
	}
};
