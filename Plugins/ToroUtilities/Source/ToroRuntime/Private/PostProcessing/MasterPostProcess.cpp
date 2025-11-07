// Copyright (C) RedCraft86. All Rights Reserved.

#include "PostProcessing/MasterPostProcess.h"
#include "Components/PostProcessComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToroRuntime.h"

AMasterPostProcess::AMasterPostProcess()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	PostProcess = CreateDefaultSubobject<UPostProcessComponent>("PostProcess");
	PostProcess->SetupAttachment(GetRootComponent());
	PostProcess->BlendWeight = 1.0f;
	PostProcess->Priority = 1.0f;
	PostProcess->bUnbound = true;
	PostProcess->bEnabled = true;

#if WITH_EDITOR
	UPDATE_VISUAL_MAX_COMP(2);
	UPDATE_VISUAL_ICON(TEXT("/ToroUtilities/Icons/MasterPostProcess.MasterPostProcess"))
#endif

	Settings.bOverride_AutoExposureMethod = true;
	Settings.AutoExposureMethod = AEM_Manual;
	
	Settings.bOverride_AutoExposureBias = true;
	Settings.AutoExposureBias = 11.0f;

	PostProcess->Settings = Settings;
}

AMasterPostProcess* AMasterPostProcess::Get(const UObject* ContextObject, const bool bCreateIfNull)
{
	AMasterPostProcess* Out = Cast<AMasterPostProcess>(UGameplayStatics::GetActorOfClass(ContextObject, StaticClass()));
	if (!Out && bCreateIfNull)
	{
		UE_LOG(LogToroRuntime, Warning, TEXT("Toro Post Process actor not found! Attempting to spawn one."));
		UWorld* World = GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::LogAndReturnNull);
		Out = World ? World->SpawnActor<AMasterPostProcess>() : nullptr;
	}
	return Out;
}

UMaterialInstanceDynamic* AMasterPostProcess::FindBlendable(UMaterialInterface* InParent)
{
	if (!InParent) return nullptr;
	for (auto It = Blendables.CreateIterator(); It; ++It)
	{
		UMaterialInstanceDynamic* MID = *It;
		if (!MID)
		{
			It.RemoveCurrent();
		}
		else if (MID->Parent == InParent)
		{
			return MID;
		}
	}
	return nullptr;
}

UMaterialInstanceDynamic* AMasterPostProcess::FindOrAddBlendable(UMaterialInterface* InParent)
{
	if (!InParent) return nullptr;
	UMaterialInstanceDynamic* MID = FindBlendable(InParent);
	if (!MID)
	{
		MID = UMaterialInstanceDynamic::Create(InParent, this);
		Blendables.Add(MID);
		ApplySettings();
	}
	return MID;
}

void AMasterPostProcess::RemoveBlendable(UMaterialInterface* InParent)
{
	if (!InParent) return;
	for (auto It = Blendables.CreateIterator(); It; ++It)
	{
		const UMaterialInstanceDynamic* MID = *It;
		if (!MID || MID->Parent == InParent)
		{
			It.RemoveCurrent();
		}
	}
}

void AMasterPostProcess::SetUDSSettings(const FUDSSettings& InSettings) const
{
	if (UDSSetterObj)
	{
		UDSSetterObj->SetSettings(this, InSettings);
	}
}

void AMasterPostProcess::ApplySettings()
{
	if (const UToroUserSettings* UserSettings = UToroUserSettings::Get())
	{
		BloomOverride.ApplyChoice(Settings, UserSettings->GetFancyBloom());
		MotionBlurOverride.ApplyChoice(Settings, UserSettings->GetMotionBlur());
		LumenOverride.ApplyChoice(Settings, UserSettings->GetLumenGI(),
			UserSettings->GetLumenReflection(), UserSettings->GetHitLighting());

		if (BrightnessMID.IsValid())
		{
			BrightnessMID->SetScalarParameterValue("Brightness", UserSettings->GetBrightness() * 0.01f);
		}
	}

	PostProcess->Settings = Settings;
	for (const TObjectPtr<UMaterialInstanceDynamic>& MID : Blendables)
	{
		PostProcess->AddOrUpdateBlendable(MID);
	}
}

bool AMasterPostProcess::HasLumenGI() const
{
	return PostProcess->Settings.bOverride_DynamicGlobalIlluminationMethod
		&& PostProcess->Settings.DynamicGlobalIlluminationMethod == EDynamicGlobalIlluminationMethod::Lumen;
}

void AMasterPostProcess::OnSettingsUpdated(const ESettingApplyType Type)
{
	if (Type == ESettingApplyType::Dynamic || Type == ESettingApplyType::Manual)
	{
		ApplySettings();
	}
}

void AMasterPostProcess::BeginPlay()
{
	Super::BeginPlay();
#if WITH_EDITOR
	if (MultiActorCheck())
#endif
	{
		const UToroSettings* ToroSettings = UToroSettings::Get();
		BrightnessMID = FindOrAddBlendable(ToroSettings->BrightnessPPM.LoadSynchronous());
		if (const TSubclassOf<UUDSSetterObject> Class = ToroSettings->UDS_Setter.LoadSynchronous())
		{
			UDSSetterObj = NewObject<UUDSSetterObject>(this, Class);
		}

		ApplySettings();
	}

	if (UToroUserSettings* UserSettings = UToroUserSettings::Get())
	{
		UserSettings->OnSettingsUpdated.AddUObject(this, &AMasterPostProcess::OnSettingsUpdated);
	}
}

void AMasterPostProcess::EnableStateChanged(const bool bState)
{
	PostProcess->bEnabled = bState;
}

#if WITH_EDITOR
void AMasterPostProcess::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (MultiActorCheck())
	{
		ApplySettings();
	}
}

bool AMasterPostProcess::MultiActorCheck() const
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, StaticClass(), Actors);
	if (Actors.Num() > 1 && Actors[0] != this)
	{
		UE_LOG(LogToroRuntime, Warning, TEXT(
			"Only one instance of Master Post Process can exist per world! Remove %s"),
			*GetActorLabel())
		return false;
	}
	return true;
}
#endif
