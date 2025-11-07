// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InventoryManager.h"
#include "Interaction/InteractableActor.h"
#include "InventoryItem.generated.h"

UCLASS(meta = (ChildCanTick = true))
class TORORUNTIME_API AInventoryItem final : public AInteractableActor
{
	GENERATED_BODY()

public:

	AInventoryItem();

	UFUNCTION(BlueprintCallable, Category = InventoryItem)
		void RespawnItem();

	virtual bool GetInteractInfo_Implementation(const FHitResult& Hit, FInteractionInfo& Info) override;
	virtual void OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& Hit) override;

protected:

	UPROPERTY(EditAnywhere, Category = Settings)
		UInventoryAsset* Item;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 1, UIMin = 1))
		uint8 Amount;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0, UIMin = 0, Units = "min"))
		int32 RespawnTime;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		FText LabelFormat;

	FTimerHandle RespawnTimer;
	TObjectPtr<UInventoryManager> InventoryManager;

	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
