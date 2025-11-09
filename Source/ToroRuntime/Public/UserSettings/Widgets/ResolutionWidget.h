// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Components/Button.h"
#include "UserInterface/ToroManagedWidget.h"
#include "ResolutionWidget.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UResolutionWidget final : public UToroManagedWidget
{
	GENERATED_BODY()

protected:

	UResolutionWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> RevertButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> AcceptButton;

	UFUNCTION(BlueprintImplementableEvent)
		void OnTimeUpdate(const uint8 Seconds);

public:

	void ShowWidget(const TFunction<void()>& OnRevert);

private:

	float Time;
	TFunction<void()> RevertFunc;

	UFUNCTION() void OnRevert();
	UFUNCTION() void OnAccept() { PopWidget(); }

	virtual void PopWidget() override;
	virtual void PushWidget() override;
	virtual void InitWidget(APlayerController* Controller) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
