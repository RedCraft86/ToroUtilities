// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroWidgetBase.h"
#include "ToroManagedWidget.generated.h"

class UToroContainerWidget;

/* A Managed Widget is a widget Managed/Hosted by a Container Widget. */
UCLASS(Abstract)
class TORORUNTIME_API UToroManagedWidget : public UToroWidgetBase
{
	GENERATED_BODY()

	friend class AToroWidgetManager;

public:
	
	UToroManagedWidget(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer), bAutoPush(true), bIsPushed(false)
	{}

	UFUNCTION(BlueprintPure, Category = ManagedWidget)
		bool IsPushed() const { return bIsPushed; }

	virtual void PushWidget();
	virtual void PopWidget();

protected:

	UPROPERTY(EditDefaultsOnly, Category = Settings)
		TSubclassOf<UToroContainerWidget> ContainerClass;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
		bool bAutoPush;

	bool bIsPushed;

	virtual UToroContainerWidget* GetContainerWidget() const;

	virtual void BeginDestroy() override;
	virtual void InitWidget(APlayerController* Controller) override;
};
