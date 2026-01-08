// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "UserInterface/ToroManagedWidget.h"
#include "UserInterface/ToroContainerWidget.h"
#include "UserInterface/ToroWidgetManager.h"
#include "Framework/ToroPlayerController.h"
#include "Helpers/LoggingMacros.h"
#include "ToroRuntime.h"

void UToroManagedWidget::PushWidget()
{
	if (UToroContainerWidget* Container = GetContainerWidget())
	{
		if (Container->PushEntry(this))
		{
			bIsPushed = true;
			if (!bAutoPush)
			{
				UGameplayStatics::PlaySound2D(this, PushSound);
			}
			bAutoPush = false;
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
			UGameplayStatics::PlaySound2D(this, PopSound);
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

	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(this))
	{
		return Cast<UToroContainerWidget>(Manager->FindWidget(ContainerClass));
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
