// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Framework/ToroPlayerController.h"
#include "Narrative/NarrativeManager.h"
#include "UserInterface/ToroContainerWidget.h"
#include "NativeContainers.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UGameplayWidgetContainer final : public UToroOverlayContainer
{
	GENERATED_BODY()

public:

	UGameplayWidgetContainer(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{
		ZOrder = 100;
	}

protected:

	TObjectPtr<UNarrativeManager> Narrative;

	virtual void InitWidget(APlayerController* Controller) override
	{
		Super::InitWidget(Controller);
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			Narrative = UNarrativeManager::Get(this);
		});
	}

	virtual bool ShouldHideWidget() const override
	{
		return (Narrative && Narrative->IsInDialogue()) || GetOwningPlayer<AToroPlayerController>()->bCinematicMode;
	}
};

UCLASS(Abstract)
class TORORUNTIME_API UMenuWidgetContainer final : public UToroStackContainer
{
	GENERATED_BODY()

public:

	UMenuWidgetContainer(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{
		ZOrder = 101;
	}
};

UCLASS(Abstract)
class TORORUNTIME_API UOverlayWidgetContainer final : public UToroOverlayContainer
{
	GENERATED_BODY()

public:

	UOverlayWidgetContainer(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{
		ZOrder = 102;
	}
};
