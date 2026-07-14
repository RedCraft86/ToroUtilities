// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "Framework/ToroPlayerHUD.h"
#include "UserSettings/Widgets/ToroSettingsWidget.h"
#include "Blueprint/UserWidget.h"
#include "ToroSettings.h"
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

void AToroPlayerHUD::PushSettings() const
{
	if (MasterWidget)
	{
		const UToroSettings* Settings = UToroSettings::Get();
		if (Settings->SettingsWidgetClass.LoadSynchronous())
		{
			MasterWidget->PushToStack(Settings->SettingsWidgetClass.Get());
		}
		else
		{
			UE_LOG(LogToroRuntime, Warning, TEXT("SettingsWidgetClass is not provided (Project Settings)."))
		}
	}
	else
	{
		UE_LOG(LogToroRuntime, Warning, TEXT("MasterWidget was not created, failed to push Settings widget."))
	}
}

void AToroPlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	const UToroSettings* Settings = UToroSettings::Get();
	if (Settings->MasterWidgetClass.LoadSynchronous())
	{
		MasterWidget = CreateWidget<UToroMasterWidget>(GetWorld(), Settings->MasterWidgetClass.Get());
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
		UE_LOG(LogToroRuntime, Warning, TEXT("MasterWidgetClass is not provided (Project Settings)."))
	}
}
