// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "TeleportPoint.generated.h"

UCLASS(MinimalAPI, HideCategories = (Rendering, HLOD, Replication, Collision, Physics, Networking, Input, Cooking), meta = (ChildCanTick))
class ATeleportPoint : public AActor
{
	GENERATED_BODY()

public:

	ATeleportPoint();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Actor)
		TOROCORE_API void TeleportActor(AActor* TargetActor) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Actor)
		TOROCORE_API void TeleportPlayer(const int32 PlayerIndex = 0) const;

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

#if WITH_EDITORONLY_DATA
private:
	UPROPERTY(Transient)
		TObjectPtr<UBillboardComponent> Icon;

	UPROPERTY(Transient)
		TObjectPtr<class UArrowComponent> Arrow;

	UPROPERTY(EditAnywhere, Category = Tools)
		float DesiredOffset = 88.0f;
#endif
#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = Tools)
		void OffsetFromFloor();
	
	bool bDidOffset = false;
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
