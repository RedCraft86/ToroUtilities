// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroVolume.h"
#include "InteractionMarker.h"
#include "InteractionInterface.h"
#include "InteractableVolume.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API AInteractableVolume : public AToroVolume, public IInteractionInterface
{
	GENERATED_BODY()

public:

	AInteractableVolume();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UInteractionMarker> Marker;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractedBP, AInteractableVolume*, Target, AToroPlayerCharacter*, Player);
	UPROPERTY(BlueprintAssignable, DisplayName = "On Interacted")
		FOnInteractedBP OnInteractedBP;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInteracted, AInteractableVolume*, AToroPlayerCharacter*)
	FOnInteracted OnInteracted;

	virtual void SetMarkerState_Implementation(const bool bVisible) override;
	virtual bool GetInteractInfo_Implementation(const FHitResult& Hit, FInteractionInfo& Info) override;
	virtual void OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& Hit) override;

protected:

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		FInteractionInfo Interaction;
};
