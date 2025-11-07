// Copyright (C) RedCraft86. All Rights Reserved.

#include "LightProbes/LightProbeManager.h"
#include "LightProbes/LightProbeActor.h"
#include "EngineUtils.h"
#if WITH_EDITOR
#include "Subsystems/UnrealEditorSubsystem.h"
#endif

void ULightProbeManager::UpdateProbes()
{
	if (UMaterialInstanceDynamic* MID = GetLightProbeMID())
	{
		const FVector CamPos = GetCameraTransform().GetTranslation();
		LightProbes.RemoveAll([](const TWeakObjectPtr<ALightProbeActor>& Elem) { return !Elem.IsValid(); });
		LightProbes.Sort([CamPos](const TWeakObjectPtr<ALightProbeActor>& A, const TWeakObjectPtr<ALightProbeActor>& B)
		{
			return FVector::Dist(A->GetActorLocation(), CamPos) < FVector::Dist(B->GetActorLocation(), CamPos);
		});

		for (uint8 i = 0; i < LightProbes.Num(); i++)
		{
			if (i >= GetMaxProbes())
			{
				break;
			}

			if (LightProbes[i].IsValid())
			{
				LightProbes[i]->ApplyData(MID, i, CamPos);
			}
			else
			{
				ResetPPM(i);
			}
		}
	}
}

void ULightProbeManager::CollectProbes()
{
	if (!LightProbes.IsEmpty())
	{
		for (uint8 i = 0; i < GetMaxProbes(); i++)
		{
			ResetPPM(i);
		}
		LightProbes.Empty();
	}

	if (!MasterPP)
	{
		return;
	}
	
	const FTransform Camera = GetCameraTransform();
	for (ALightProbeActor* Probe : TActorRange<ALightProbeActor>(GetWorld()))
	{
		if (Probe && Probe->IsRelevantProbe(Camera, bHasLumenGI))
		{
			LightProbes.AddUnique(Probe);
		}
	}

	UpdateProbes();
}

void ULightProbeManager::ResetPPM(const uint8 Idx)
{
	if (UMaterialInstanceDynamic* MID = GetLightProbeMID())
	{
		MID->SetVectorParameterValue(ProbeParam(Idx, false), FLinearColor::Transparent);
		MID->SetVectorParameterValue(ProbeParam(Idx, true), FLinearColor::Transparent);
	}
}

FTransform ULightProbeManager::GetCameraTransform() const
{
	FVector Position = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
#if WITH_EDITOR
	if (!FApp::IsGame())
	{
		if (!GEditor) return FTransform::Identity;
		if (UUnrealEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>())
		{
			Subsystem->GetLevelViewportCameraInfo(Position, Rotation);
		}
	}
	else
#endif
	if (CamManager)
	{
		Position = CamManager->GetCameraLocation();
		Rotation = CamManager->GetCameraRotation();
	}

	return FTransform(Rotation, Position);
}

UMaterialInstanceDynamic* ULightProbeManager::GetLightProbeMID()
{
	if (!LightProbeMID.IsValid() && MasterPP)
	{
		LightProbeMID = MasterPP->FindOrAddBlendable(UToroSettings::Get()->LightProbePPM.LoadSynchronous());
	}
	return LightProbeMID.Get();
}

bool ULightProbeManager::IsTickable() const
{
	return Super::IsTickable() && ((MasterPP && CamManager) || !FApp::IsGame());
}

void ULightProbeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
#if WITH_EDITOR
	if (TickTime > (FApp::IsGame() ? 0.25f : 0.01f))
	{
		if (!FApp::IsGame() && !MasterPP)
		{
			MasterPP = AMasterPostProcess::Get(this, false);
		}
#else
	if (TickTime > 0.25f)
	{
#endif
		bHasLumenGI = MasterPP && MasterPP->HasLumenGI();
		TickTime = 0.0f;
		CollectProbes();
	}
	else
	{
		TickTime += DeltaTime;
		UpdateProbes();
	}
}

void ULightProbeManager::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	InWorld.GetTimerManager().SetTimerForNextTick([this]()
	{
		MasterPP = AMasterPostProcess::Get(this);
		CamManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	});
}

#if WITH_EDITOR
bool ULightProbeManager::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer) && !UToroSettings::Get()->LightProbePPM.IsNull();
}

void ULightProbeManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogToroRuntime, Display, TEXT("Light Probe Manager initialized by %s."), *GetNameSafe(GetWorld()));
	if (!FApp::IsGame()) MasterPP = AMasterPostProcess::Get(this, false);
}
#endif
