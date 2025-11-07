// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserInterface/ToroWidgetManager.h"
#include "UserInterface/ToroContainerWidget.h"
#include "ToroRuntime.h"

AToroWidgetManager::AToroWidgetManager()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
}

UToroWidgetBase* AToroWidgetManager::FindOrAddWidget(const TSubclassOf<UToroWidgetBase> WidgetClass)
{
	if (!WidgetClass)
	{
		return nullptr;
	}

	UToroWidgetBase* Widget = FindWidget(WidgetClass);
	if (!Widget)
	{
		Widget = UToroWidgetBase::CreateToroWidget(GetOwningPlayerController(), WidgetClass);
		WidgetObjects.Add(Widget);
	}

	return Widget;
}

UToroWidgetBase* AToroWidgetManager::FindWidget(const TSubclassOf<UToroWidgetBase> WidgetClass)
{
	if (!WidgetClass)
	{
		return nullptr;
	}

	for (UToroWidgetBase* Widget : WidgetObjects)
	{
		if (Widget->IsA(WidgetClass))
		{
			return Widget;
		}
	}

	return nullptr;
}

void AToroWidgetManager::BeginPlay()
{
	Super::BeginPlay();
	TArray<TSoftClassPtr<UToroWidgetBase>> Widgets = UToroSettings::Get()->DefaultWidgets;
	Widgets.Sort([](const TSoftClassPtr<UToroWidgetBase>& A, const TSoftClassPtr<UToroWidgetBase>& B)
	{
		// Ensure Containers are always loaded first since some Managed widgets might auto push to them
		const UClass* ClassA = A.LoadSynchronous();
		const UClass* ClassB = B.LoadSynchronous();
		const bool AIsContainer = ClassA && ClassA->IsChildOf<UToroContainerWidget>();
		const bool BIsContainer = ClassB && ClassB->IsChildOf<UToroContainerWidget>();
		return AIsContainer > BIsContainer;
	});

	for (const TSoftClassPtr<UToroWidgetBase>& Widget : Widgets)
	{
		FindOrAddWidget(Widget.LoadSynchronous());
	}
}
