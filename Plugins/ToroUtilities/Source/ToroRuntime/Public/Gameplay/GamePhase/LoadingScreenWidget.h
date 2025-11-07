// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UserInterface/ToroManagedWidget.h"
#include "LoadingScreenWidget.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API ULoadingScreenWidget final : public UToroManagedWidget
{
	GENERATED_BODY()

public:

	ULoadingScreenWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintNativeEvent)
		FText FormatProgress(const FIntPoint& Progress);
	FText FormatProgress_Implementation(const FIntPoint& Progress);

	void ShowWidget(const bool bMinimalUI);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UWidgetSwitcher> ScreenSwitch;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UProgressBar> LoadProgress;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> LoadMessage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> BackgroundImage;

	bool bMinimal;
	int32 MaxPackages;

	virtual void PushWidget() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
