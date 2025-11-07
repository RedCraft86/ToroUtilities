// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroRuntime.h"
#include "Helpers/ClassGetterMacros.h"
#include "Subsystems/WorldSubsystem.h"
#include "PostProcessing/MasterPostProcess.h"
#include "LightProbeManager.generated.h"

UCLASS()
class TORORUNTIME_API ULightProbeManager final : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:

	ULightProbeManager(): TickTime(0.0f), bHasLumenGI(false) {}

	WORLD_SUBSYSTEM_GETTER(ULightProbeManager);

	void ForceProbeRecollection() { TickTime = 1.0f; }

private:

	float TickTime;
	bool bHasLumenGI;
	TObjectPtr<AMasterPostProcess> MasterPP;
	TObjectPtr<APlayerCameraManager> CamManager;
	TWeakObjectPtr<UMaterialInstanceDynamic> LightProbeMID;
	TArray<TWeakObjectPtr<class ALightProbeActor>> LightProbes;

	void UpdateProbes();
	void CollectProbes();
	void ResetPPM(const uint8 Idx);

	FTransform GetCameraTransform() const;
	UMaterialInstanceDynamic* GetLightProbeMID();

	virtual bool IsTickable() const override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual TStatId GetStatId() const override { return GetStatID(); }
#if WITH_EDITOR
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual bool IsTickableInEditor() const override { return true; }
#endif

public:

	static uint8 GetMaxProbes()
	{
		static uint8 Limit = UToroSettings::Get()->LightProbeLimit;
		return Limit;
	}

	static FName ProbeParam(const uint8 Idx, const bool bPos)
	{
		return *FString::Printf(TEXT("%s_%d"),
			bPos ? TEXT("Position") : TEXT("Color"), Idx + 1);
	}
};
