// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Framework/ToroPlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "ToroRuntime.h"

AToroPlayerHUD::AToroPlayerHUD()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	bEnableAutoLODGeneration = false; // Include Actor in HLOD option

	SetCanBeDamaged(false);
}

void AToroPlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	if (MasterWidgetClass)
	{
		MasterWidget = CreateWidget<UToroMasterWidget>(GetWorld(), MasterWidgetClass);
		if (MasterWidget)
		{
			MasterWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogToroRuntime, Error, TEXT("Failed to add MasterWidget even though Class provided is valid."))
		}
	}
	else
	{
		UE_LOG(LogToroRuntime, Warning, TEXT("MasterWidgetClass is not provided."))
	}
}
