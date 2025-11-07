// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroActor.h"
#include "InteractionMarker.h"
#include "InteractionInterface.h"
#include "InteractableActor.generated.h"

UCLASS(Abstract, meta = (ChildCanTick = true))
class TORORUNTIME_API AInteractableActor : public AToroActor, public IInteractionInterface
{
	GENERATED_BODY()

public:

	AInteractableActor();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UInteractionMarker> Marker;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractedBP, AInteractableActor*, Target, AToroPlayerCharacter*, Player);
	UPROPERTY(BlueprintAssignable, DisplayName = "On Interacted")
		FOnInteractedBP OnInteractedBP;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInteracted, AInteractableActor*, AToroPlayerCharacter*)
	FOnInteracted OnInteracted;

	virtual void SetMarkerState_Implementation(const bool bVisible) override;
	virtual bool GetInteractInfo_Implementation(const FHitResult& Hit, FInteractionInfo& Info) override;
	virtual void OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& Hit) override;

protected:

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		FInteractionInfo Interaction;
};
