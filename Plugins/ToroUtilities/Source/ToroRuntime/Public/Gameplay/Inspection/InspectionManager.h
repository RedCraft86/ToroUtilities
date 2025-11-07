// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InspectionWidget.h"
#include "Inventory/InventoryAsset.h"
#include "Components/ActorComponent.h"
#include "Framework/ToroPlayerState.h"
#include "Helpers/ClassGetterMacros.h"
#include "Framework/ToroPlayerController.h"
#include "InspectionManager.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UInspectionManager final : public UActorComponent
{
	GENERATED_BODY()

public:

	UInspectionManager();

	PLAYER_COMPONENT_GETTER(UInspectionManager, AToroPlayerState, Inspection)

	UFUNCTION(BlueprintCallable, Category = Statics, meta = (WorldContext = "ContextObject"))
	static UInspectionManager* GetInspectionManager(const UObject* ContextObject, const int32 PlayerIdx = 0)
	{
		return Get(ContextObject, PlayerIdx);
	}

	UFUNCTION(BlueprintCallable, Category = Inspection)
		void BeginInspection(UInventoryAsset* InArchive);

	UFUNCTION(BlueprintCallable, Category = Inspection)
		void EndInspection();

	UFUNCTION(BlueprintCallable, Category = Inspection)
		bool IsInspecting() const { return InspectArchive.IsValid(); }

	UFUNCTION(BlueprintCallable, Category = Inspection)
		UInventoryAsset* GetInspectable() const { return InspectArchive.Get(); }

private:

	UPROPERTY(Transient)
		TWeakObjectPtr<UInventoryAsset> InspectArchive;

	TObjectPtr<UInventoryManager> Inventory;

	TObjectPtr<UInspectionWidget> InspectionWidget;
	UInspectionWidget* GetInspectionWidget();

	virtual void BeginPlay() override;
};
