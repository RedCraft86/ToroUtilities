// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "PostProcess/GlobalPostProcess.h"
#include "UserSettings/ToroGameUserSettings.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Libraries/ToroWorldLibrary.h"
#include "SceneInterface.h"
#include "RenderUtils.h"

UGlobalPostProcess::UGlobalPostProcess()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bTickEvenWhenPaused = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PrimaryComponentTick.TickInterval = 0.2f;

	BlendRadius = 100.0f;
	BlendWeight = 1.0f;
	Priority = -1.0f;
	bUnbound = true;
	bEnabled = true;

	Settings.bOverride_AutoExposureMethod = true;
	Settings.AutoExposureMethod = AEM_Manual;
	Settings.bOverride_AutoExposureBias = true;
	Settings.AutoExposureBias = 11.0f;
	Settings.bOverride_BloomMethod = true;
	Settings.BloomMethod = EBloomMethod::BM_FFT;

	bAdvancedLumenCheck = true;
	bUsesLumenGI = false;
}

UMaterialInterface* UGlobalPostProcess::FindBlendable(const UMaterialInterface* InMaterial) const
{
	if (!InMaterial)
	{
		return nullptr;
	}

	for (const FWeightedBlendable& Blendable : Settings.WeightedBlendables.Array)
	{
		if (UMaterialInterface* Material = Cast<UMaterialInterface>(Blendable.Object))
		{
			if (Material == InMaterial)
			{
				return Material;
			}

			const UMaterialInstanceDynamic* MID = Cast<UMaterialInstanceDynamic>(Material);
			if (MID && MID->Parent == InMaterial)
			{
				return Material;
			}
		}
	}

	return nullptr;
}

UMaterialInterface* UGlobalPostProcess::FindOrAddBlendable(UMaterialInterface* InMaterial, const bool bDynamic)
{
	if (!InMaterial)
	{
		return nullptr;
	}

	UMaterialInterface* Blendable = FindBlendable(InMaterial);
	if (Blendable)
	{
		// Reset weight to 1 just in case
		Settings.AddBlendable(Blendable, 1.0f);
		return Blendable;
	}

	Blendable = bDynamic ? UMaterialInstanceDynamic::Create(InMaterial, this) : InMaterial;
	Settings.AddBlendable(Blendable, 1.0f);

	return Blendable;
}

void UGlobalPostProcess::RemoveBlendable(UMaterialInterface* InMaterial)
{
	if (UMaterialInterface* Blendable = FindBlendable(InMaterial))
	{
		Settings.RemoveBlendable(Blendable);
	}
}

void UGlobalPostProcess::SyncUserSettings(const UToroGameUserSettings* UserSettings, const EUserSettingApplyType ApplyType)
{
	if (ApplyType == EUserSettingApplyType::Manual)
	{
		const ELumenUsageMode LumenMode = UserSettings->GetLumenMode();

		Settings.bOverride_ReflectionMethod = true;
		Settings.ReflectionMethod = LumenReflectionAllowed(LumenMode) 
			? EReflectionMethod::Lumen : EReflectionMethod::ScreenSpace;

		Settings.bOverride_DynamicGlobalIlluminationMethod = true;
		Settings.DynamicGlobalIlluminationMethod = LumenLightingAllowed(LumenMode) 
			? EDynamicGlobalIlluminationMethod::Lumen : EDynamicGlobalIlluminationMethod::None;

		if (UserSettings->GetGlobalIlluminationQuality() >= 4)
		{
			Settings.LumenRayLightingMode = ELumenRayLightingModeOverride::HitLighting;
		}
		else
		{
			Settings.LumenRayLightingMode = UserSettings->GetReflectionQuality() >= 3
				? ELumenRayLightingModeOverride::HitLightingForReflections
				: ELumenRayLightingModeOverride::SurfaceCache;
		}
	}
}

void UGlobalPostProcess::BeginPlay()
{
	Super::BeginPlay();
	
	// Fixes MotionBlur and AA issues when camera is moved while paused
	GetWorld()->bIsCameraMoveableWhenPaused = true;

	if (UToroGameUserSettings* UserSettings = UToroGameUserSettings::Get())
	{
		UserSettings->OnSettingsApplied.AddUObject(this, &UGlobalPostProcess::SyncUserSettings);
		SyncUserSettings(UserSettings, EUserSettingApplyType::Manual);
	}
}

void UGlobalPostProcess::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);

	const IConsoleManager& CM = IConsoleManager::Get();
	static const auto GIAllowed = CM.FindTConsoleVariableDataInt(TEXT("r.Lumen.DiffuseIndirect.Allow"));
	if (!GIAllowed->GetValueOnGameThread() || !DoesPlatformSupportLumenGI(GetScene()->GetShaderPlatform()))
	{
		bUsesLumenGI = false;
		return;
	}

	if (!Settings.bOverride_DynamicGlobalIlluminationMethod)
	{
		static const auto GIMethod = CM.FindTConsoleVariableDataInt(TEXT("r.DynamicGlobalIlluminationMethod"));
		bUsesLumenGI = GIMethod->GetValueOnGameThread() == static_cast<int32>(EDynamicGlobalIlluminationMethod::Lumen);
	}
	else
	{
		bUsesLumenGI = Settings.DynamicGlobalIlluminationMethod == EDynamicGlobalIlluminationMethod::Lumen;
	}

	if (bAdvancedLumenCheck)
	{
		float HighestPriority = Priority;
		const FVector ViewLocation = UToroWorldLibrary::GetMainCameraTransform(this).GetLocation();
		for (IInterface_PostProcessVolume& PPVolume : GetWorld()->GetPostProcessVolumeIterator())
		{
			const FPostProcessVolumeProperties Prop = PPVolume.GetProperties();
			if (Prop.Settings && Prop.Priority > HighestPriority 
				&& Prop.Settings->bOverride_DynamicGlobalIlluminationMethod 
				&& (Prop.bIsUnbound || PPVolume.EncompassesPoint(ViewLocation, 0.0f, nullptr)))
			{
				HighestPriority = Prop.Priority;
				bUsesLumenGI = Prop.Settings->DynamicGlobalIlluminationMethod 
								== EDynamicGlobalIlluminationMethod::Lumen;
			}
		}
	}
}

FPostProcessVolumeProperties UGlobalPostProcess::GetProperties() const
{
	FPostProcessVolumeProperties Ret;
	Ret.BlendRadius = 100.0f;
	Ret.BlendWeight = 1.0f;
	Ret.Priority = -1.0f;
	Ret.bIsUnbound = true;
	Ret.bIsEnabled = true;
	Ret.Size = DBL_MAX;
	Ret.Settings = &Settings;
	Ret.VolumeGuid = VolumeGuid;
	return Ret;
}

void UGlobalPostProcess::OnRegister()
{
	USceneComponent::OnRegister();
	GetWorld()->AddPostProcessVolume(this);
}

void UGlobalPostProcess::OnUnregister()
{
	USceneComponent::OnUnregister();
	GetWorld()->RemovePostProcessVolume(this);
}

void UGlobalPostProcess::Serialize(FArchive& Ar)
{
	USceneComponent::Serialize(Ar);
	if (Ar.IsLoading())
	{
#if WITH_EDITORONLY_DATA
		Settings.OnAfterLoad();
#endif
	}
}

void UGlobalPostProcess::PostInitProperties()
{
	USceneComponent::PostInitProperties();
#if WITH_EDITOR
	if (!IsTemplate() && !VolumeGuid.IsValid())
	{
		VolumeGuid = FGuid::NewDeterministicGuid(GetPathName());
	}
#endif
}

#if WITH_EDITOR
bool UGlobalPostProcess::CanEditChange(const FProperty* InProperty) const
{
	const bool bSuper = Super::CanEditChange(InProperty);
	if (bSuper && InProperty)
	{
		return InProperty->GetFName() != GET_MEMBER_NAME_CHECKED(UGlobalPostProcess, Priority)
			&& InProperty->GetFName() != GET_MEMBER_NAME_CHECKED(UGlobalPostProcess, BlendRadius)
			&& InProperty->GetFName() != GET_MEMBER_NAME_CHECKED(UGlobalPostProcess, BlendWeight)
			&& InProperty->GetFName() != GET_MEMBER_NAME_CHECKED(UGlobalPostProcess, bUnbound)
			&& InProperty->GetFName() != GET_MEMBER_NAME_CHECKED(UGlobalPostProcess, bEnabled);
	}
	return bSuper;
}
#endif