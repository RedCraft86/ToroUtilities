// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "LightProbes/LightProbeManager.h"
#include "LightProbes/LightProbeActor.h"
#include "Libraries/ToroWorldLibrary.h"
#include "ToroRuntime.h"
#include "EngineUtils.h"
#if WITH_EDITOR
#include "UObject/ConstructorHelpers.h"
#endif

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

	const ConstructorHelpers::FObjectFinder<UMaterialInterface> PPMFinder(
		TEXT("/ToroUtilities/LightProbes/PPM_LightProbe_16x.PPM_LightProbe_16x"));
	if (PPMFinder.Succeeded()) PostProcessMaterial = PPMFinder.Object;
#endif
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
	const bool bHasLumenGI = GetOwner<AToroWorldSettings>()->IsUsingLumenGI();
	for (ALightProbeActor* Probe : TActorRange<ALightProbeActor>(GetWorld()))
	{
		if (Probe && Probe->IsRelevantProbe(Camera, bHasLumenGI))
		{
			LightProbes.AddUnique(Probe);
		}
	}
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
