// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "GameFramework/Pawn.h"
#include "Interfaces/IIdentifiable.h"
#include "Interfaces/IPawnEntity.h"
#include "ToroPawn.generated.h"

/**
 * A base Pawn class for the ToroUtilities framework with an identity and general pawn functions.
 */
UCLASS(Abstract, Blueprintable, BlueprintType, PrioritizeCategories = (Settings, Tools), meta = (ChildCanTick = true))
class TOROCORE_API AToroPawn : public APawn, public IIdentifiable, public IPawnEntity
{
	GENERATED_BODY()

public:

	AToroPawn();

	virtual FGameObjectId GetIdentity_Implementation() const override { return UniqueId; }
	virtual void Teleport_Implementation(const FVector& Location, const FRotator& Rotation) override;
	virtual bool SetControlRotation_Implementation(const FRotator& Rotation, const bool bApplyRoll = false) override;
	virtual bool GetLookTarget_Implementation(FVector& Location) const override;
	virtual FVector GetFocusPoint_Implementation() const override;

protected:

	/**
	 * Identifier for saves system and other systems that involves identification.
	 */
	UPROPERTY(EditAnywhere, Category = Settings, NonPIEDuplicateTransient, TextExportTransient, NonTransactional, meta = (DisplayPriority = -100))
		FGameObjectId UniqueId;

#if WITH_EDITORONLY_DATA
	/**
	 * If true, the Tick function will run inside the Level Editor viewport.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Tick)
		bool bTickInEditor = false;
#endif

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual bool ShouldTickIfViewportsOnly() const override { return bTickInEditor; }
#endif
};
