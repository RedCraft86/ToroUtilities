// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InspectionManager.h"
#include "Inventory/InventoryAsset.h"
#include "Interaction/InteractableActor.h"
#include "InspectableActor.generated.h"

UCLASS()
class TORORUNTIME_API AInspectableActor final : public AInteractableActor
{
	GENERATED_BODY()

public:

	AInspectableActor();

	UPROPERTY(EditAnywhere, Category = Settings)
		TObjectPtr<UInventoryAsset> Archive;

	virtual void OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& Hit) override;
	virtual bool GetInteractInfo_Implementation(const FHitResult& Hit, FInteractionInfo& Info) override;

private:
	
	TObjectPtr<UInspectionManager> Manager;

	bool HasValidArchive() const { return Archive && Archive->IsValidArchive(); }

	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif	
};
