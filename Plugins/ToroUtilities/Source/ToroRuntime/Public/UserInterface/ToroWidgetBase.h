// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "CommonActivatableWidget.h"
#include "ToroWidgetBase.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UToroWidgetBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:

	UToroWidgetBase(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = UserInterface, DisplayName = "Create ToroWidget", meta = (DeterminesOutputType = "WidgetClass"))
		static UToroWidgetBase* CreateToroWidget(APlayerController* Owner, const TSubclassOf<UToroWidgetBase> WidgetClass);

	UFUNCTION(BlueprintCallable, Category = UserInterface)
		void SetHidden(const bool bInHidden);

	UFUNCTION(BlueprintPure, Category = UserInterface)
		bool WantsHidden() const { return bWantsToHide; }

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> HideAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Widget)
		TObjectPtr<AGameModeBase> GameMode;

	UPROPERTY(BlueprintReadOnly, Category = Widget)
		bool bWantsToHide;

	bool bHidden;
	float HideCheckTime;

	void UpdateFadeState();

	virtual void InitWidget(APlayerController* Controller);
	virtual bool ShouldHideWidget() const { return false; }
	virtual bool CanCreateWidget(const UObject* ContextObject) const { return IsValid(ContextObject); }

	virtual void InternalProcessActivation() override;
	virtual void InternalProcessDeactivation() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
