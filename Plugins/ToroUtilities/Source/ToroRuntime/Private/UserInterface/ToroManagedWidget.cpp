// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserInterface/ToroManagedWidget.h"
#include "UserInterface/ToroContainerWidget.h"
#include "UserInterface/ToroWidgetManager.h"
#include "Helpers/LoggingMacros.h"
#include "ToroRuntime.h"

void UToroManagedWidget::PushWidget()
{
	if (UToroContainerWidget* Container = GetContainerWidget())
	{
		if (Container->PushEntry(this))
		{
			bIsPushed = true;
		}
	}
}

void UToroManagedWidget::PopWidget()
{
	if (UToroContainerWidget* Container = GetContainerWidget())
	{
		if (Container->PopEntry(this))
		{
			bIsPushed = false;
		}
	}
}

UToroContainerWidget* UToroManagedWidget::GetContainerWidget() const
{
	if (!ContainerClass)
	{
		UE_LOG_ERROR(LogToroRuntime, 1.0f,
			TEXT("Widget %s has no Container Class!"), *GetName())
		return nullptr;
	}

	const APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		UE_LOG_ERROR(LogToroRuntime, 1.0f,
			TEXT("Widget %s has no Owning Player!"), *GetName())
		return nullptr;
	}

	if (AToroWidgetManager* Manager = PC->GetHUD<AToroWidgetManager>())
	{
		return Manager->FindWidget<UToroContainerWidget>();
	}

	return nullptr;
}

void UToroManagedWidget::BeginDestroy()
{
	if (bIsPushed) PopWidget();
	Super::BeginDestroy();
}

void UToroManagedWidget::InitWidget(APlayerController* Controller)
{
	Super::InitWidget(Controller);
	if (!ContainerClass)
	{
		UE_LOG_ERROR(LogToroRuntime, 1.0f,
			TEXT("Widget %s has no expected Container Class!"), *GetName())
		return;
	}

	if (bAutoPush)
	{
		PushWidget();
	}
}
