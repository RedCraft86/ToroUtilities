// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Engine/TargetPoint.h"
#include "TeleportPoint.generated.h"

UCLASS(MinimalAPI, NotBlueprintable, meta = (HideCategories = "*", ShowCategories = "Tools"))
class ATeleportPoint final : public ATargetPoint
{
	GENERATED_BODY()

public:

	ATeleportPoint();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Actor)
		TORORUNTIME_API void TeleportActor(AActor* TargetActor) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Actor)
		TORORUNTIME_API void TeleportPlayer(const int32 PlayerIndex = 0) const;

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Tools)
		float FloorOffset = 88.0f;

	UPROPERTY(EditAnywhere, Category = Tools, AdvancedDisplay)
		bool bDidOffset = false;
#endif

#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = Tools)
		void OffsetFromFloor();

	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
