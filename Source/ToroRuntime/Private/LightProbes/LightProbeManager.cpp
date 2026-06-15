// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "LightProbes/LightProbeManager.h"
#include "LightProbes/LightProbeActor.h"
#include "PostProcess/GlobalPostProcess.h"
#include "Libraries/ToroWorldLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineUtils.h"
#include "ToroRuntime.h"

#define COLOR_PARAM_NAME(Idx) *FString::Printf(TEXT("Color_%d"), Idx + 1)
#define POSITION_PARAM_NAME(Idx) *FString::Printf(TEXT("Position_%d"), Idx + 1)

ULightProbeManager::ULightProbeManager(): MaxProbeCount(16)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bTickEvenWhenPaused = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
#if WITH_EDITOR
	bTickInEditor = true;
#endif

	if (!IsRunningCommandlet() && !IsRunningDedicatedServer())
	{
		static ConstructorHelpers::FObjectFinder<UMaterialInterface> PPMFinder(
		   TEXT("/ToroUtilities/LightProbes/PPM_LightProbe_16x.PPM_LightProbe_16x"));
		if (PPMFinder.Succeeded()) PostProcessMaterial = PPMFinder.Object;
	}
}

void ULightProbeManager::UpdateProbes()
{
	const FVector CamPos = UToroWorldLibrary::GetMainCameraTransform(this).GetLocation();
	LightProbes.Sort([CamPos](const TWeakObjectPtr<ALightProbeActor>& A, const TWeakObjectPtr<ALightProbeActor>& B)
	{
		if (!A.IsValid()) return true;
		if (!B.IsValid()) return false;
		return FVector::Dist(A->GetActorLocation(), CamPos) < FVector::Dist(B->GetActorLocation(), CamPos);
	});

	for (uint8 i = 0; i < MaxProbeCount; i++)
	{
		FLinearColor PosParam = FLinearColor::Transparent;
		FLinearColor ColorParam = FLinearColor::Transparent;
		if (i < LightProbes.Num() && LightProbes[i].IsValid())
		{
			LightProbes[i]->GetProbeData(ColorParam, PosParam, CamPos);
		}

		LightProbeMID->SetVectorParameterValue(COLOR_PARAM_NAME(i), ColorParam);
		LightProbeMID->SetVectorParameterValue(POSITION_PARAM_NAME(i), PosParam);
	}
}

void ULightProbeManager::CollectProbes()
{
	LightProbes.Empty();
	const FTransform Camera = UToroWorldLibrary::GetMainCameraTransform(this);

	const UGlobalPostProcess* PP = UGlobalPostProcess::Get(this);
	const bool bHasLumenGI = PP && PP->IsUsingLumenGI();

	for (ALightProbeActor* Probe : TActorRange<ALightProbeActor>(GetWorld()))
	{
		if (Probe && Probe->IsRelevantProbe(Camera, bHasLumenGI))
		{
			LightProbes.AddUnique(Probe);
		}
	}
}

UMaterialInstanceDynamic* ULightProbeManager::CreateLightProbeMID() const
{
	UGlobalPostProcess* PP = UGlobalPostProcess::Get(this);
	if (PP && PostProcessMaterial)
	{
		return Cast<UMaterialInstanceDynamic>(PP->FindOrAddBlendable(PostProcessMaterial, true));
	}

	return nullptr;
}

void ULightProbeManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	if (LightProbeMID.Get())
	{
		if (UpdateInterval.TickCooldown(DeltaTime))
		{
			CollectProbes();
#if WITH_EDITOR
			if (!FApp::IsGame()) // Make sure everything is fast and snappy in the editor
			{
				UpdateInterval.Cooldown = 0.01f;
				UpdateProbes();
			}
#endif
		}
		else
		{
			UpdateProbes();
		}
	}
#if WITH_EDITOR
	else if (FApp::IsGame() && UpdateInterval.TickCooldown(DeltaTime))
	{
		UpdateInterval.Cooldown = 1.0f;
		UE_LOG(LogToroRuntime, Error, TEXT("Failed to process LightProbes as the MID is NULL."));
	}
#endif
}

#undef POSITION_PARAM_NAME
#undef COLOR_PARAM_NAME
