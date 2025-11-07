// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Inspection/InspectionManager.h"
#include "Gameplay/Inventory/InventoryManager.h"
#include "UserInterface/ToroWidgetManager.h"

UInspectionManager::UInspectionManager()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UInspectionManager::BeginInspection(UInventoryAsset* InArchive)
{
	if (InspectArchive.IsValid()) EndInspection();
	GetWorld()->GetTimerManager().SetTimerForNextTick([this, Archive = InArchive]()
	{
		InspectArchive = Archive;
		Inventory->AddArchive(InspectArchive.Get());
		if (UInspectionWidget* Widget = GetInspectionWidget())
		{
			Widget->ShowWidget(InspectArchive.Get());
		}
	});
}

void UInspectionManager::EndInspection()
{
	if (InspectArchive.IsValid())
	{
		InspectArchive.Reset();
		if (UInspectionWidget* Widget = GetInspectionWidget())
		{
			Widget->PopWidget();
		}
	}
}

UInspectionWidget* UInspectionManager::GetInspectionWidget()
{
	if (!InspectionWidget)
	{
		InspectionWidget = AToroWidgetManager::GetWidget<UInspectionWidget>(this);
	}
	return InspectionWidget;
}

void UInspectionManager::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		Inventory = UInventoryManager::Get(this);
	});
}
