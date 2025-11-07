// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Gameplay/WorldActions/WorldActionBase.h"
#include "ActionFlow.generated.h"

USTRUCT(BlueprintType, DisplayName = "Delay")
struct TORORUNTIME_API FWAFlow_Delay final : public FWorldActionBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Flow)
		bool bRetriggerable;
	
	UPROPERTY(EditAnywhere, Category = Flow, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Delay;

	UPROPERTY(EditAnywhere, Category = Flow)
		FWorldActionArray Actions;

	FWAFlow_Delay(): bRetriggerable(false), Delay(0.2f) {}

protected:

	FTimerHandle DelayHandle;

	FTimerManager* GetTimerManager() const;

	virtual void RunAction() override;
	virtual void OnBeginPlay() override;
	virtual void OnConstruction() override;
	virtual void OnTick(const float DeltaTime) override;
};

USTRUCT(BlueprintType, DisplayName = "Do Once")
struct TORORUNTIME_API FWAFlow_DoOnce final : public FWorldActionBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Flow)
		FWorldActionArray Actions;

	FWAFlow_DoOnce(): bTriggered(false) {}

protected:

	bool bTriggered;

	virtual void RunAction() override;
	virtual void OnBeginPlay() override;
	virtual void OnConstruction() override;
	virtual void OnTick(const float DeltaTime) override;
};

USTRUCT(BlueprintType, DisplayName = "Flip Flop")
struct TORORUNTIME_API FWAFlow_FlipFlop final : public FWorldActionBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Flow)
		FWorldActionArray ActionsA;

	UPROPERTY(EditAnywhere, Category = Flow)
		FWorldActionArray ActionsB;

	FWAFlow_FlipFlop(): bIsA(false) {}

protected:

	bool bIsA;

	virtual void RunAction() override;
	virtual void OnBeginPlay() override;
	virtual void OnConstruction() override;
	virtual void OnTick(const float DeltaTime) override;
};