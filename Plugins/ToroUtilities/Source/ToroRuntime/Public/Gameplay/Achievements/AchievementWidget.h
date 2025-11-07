// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroRuntime.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "AchievementDatabase.h"
#include "UserInterface/ToroManagedWidget.h"
#include "AchievementWidget.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UAchievementEntryWidget final : public UUserWidget
{
	GENERATED_BODY()

public:

	UAchievementEntryWidget(const FObjectInitializer& ObjectInitializer)
		: UUserWidget(ObjectInitializer)
	{}

	void InitializeWidget(const FAchievementEntry& Entry);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> IconImage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> EntryAnim;

	FDelegateHandle OnFinishHandle;

	void OnAnimationFinished(FWidgetAnimationState& State)
	{
		RemoveFromParent();
	}
};

UCLASS(Abstract)
class TORORUNTIME_API UAchievementWidget final : public UToroManagedWidget
{
	GENERATED_BODY()

public:

	UAchievementWidget(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> AchievementList;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
		TSubclassOf<UAchievementEntryWidget> EntryClass;

	void OnAchievement(const FAchievementEntry& Data);

	virtual void InitWidget(APlayerController* Controller) override;
	virtual bool CanCreateWidget(const UObject* ContextObject) const override
	{
		return !UToroSettings::Get()->IsOnMap(ContextObject, EToroMapType::MainMenu);
	}
};
