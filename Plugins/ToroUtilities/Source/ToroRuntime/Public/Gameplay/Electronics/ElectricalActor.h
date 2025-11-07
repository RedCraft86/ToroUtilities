// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroActor.h"
#include "DataTypes/ToroDelegates.h"
#include "ElectricalActor.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API AElectricalActor : public AToroActor
{
	GENERATED_BODY()

public:

	AElectricalActor();

	/* Whether the powered state should be reevaluated on begin play.
	 * Better to keep disabled on switched lights
	 */
	UPROPERTY(EditAnywhere, Category = Settings)
		bool bRefreshOnInit;

	UPROPERTY(EditAnywhere, Category = Settings)
		uint8 MinProviders;

	UPROPERTY(EditAnywhere, Category = Settings)
		TSet<TWeakObjectPtr<AElectricalActor>> Targets;

	UPROPERTY(VisibleAnywhere, Category = Settings)
		TSet<TWeakObjectPtr<AElectricalActor>> Providers;

	UPROPERTY(BlueprintAssignable, DisplayName = "On Powered State Changed")
		FToroBoolDelegateBP OnPoweredStateChangedBP;

	UFUNCTION(BlueprintPure, Category = Electricals)
		bool IsPowered() const { return bPowered; }

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Powered State Changed")
		void PoweredStateChangedBP(bool bState);

	FToroBoolDelegate OnPoweredStateChanged;
	void AddProvider(AElectricalActor* InProvider);
	void RemoveProvider(AElectricalActor* InProvider);

protected:

	bool bPowered;

	void BroadcastPoweredState();
	void UpdatePoweredState(bool bForceUpdate);

	virtual bool CalcPoweredState();
	virtual void PoweredStateChanged(const bool bState);
	virtual void EnableStateChanged(const bool bState) override;
	virtual void BeginPlay() override;
};
