// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Framework/ToroPlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "EquipmentActor.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API AEquipmentActor : public AActor
{
	GENERATED_BODY()

	friend class UInventoryManager;

public:

	AEquipmentActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Subobject)
		TObjectPtr<USceneComponent> SceneRoot;

	UFUNCTION(BlueprintNativeEvent, Category = Equipment)
		void OnBeginUse();
	virtual void OnBeginUse_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, Category = Equipment)
		void OnEndUse();
	virtual void OnEndUse_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, Category = Equipment)
		void OnEquip(USceneComponent* EquipmentHandle);
	virtual void OnEquip_Implementation(USceneComponent* EquipmentHandle);

	UFUNCTION(BlueprintNativeEvent, Category = Equipment)
		void OnUnequip();
	virtual void OnUnequip_Implementation();

	UFUNCTION(BlueprintPure, Category = Equipment)
		bool IsBeingUsed() const { return bUsing; }

protected:

	UPROPERTY(BlueprintReadOnly, Category = Equipment)
		TObjectPtr<AToroPlayerCharacter> PlayerChar;

	bool bUsing;

	void EquipItem();
	void UnequipItem();
	void SetUseState(const bool bInUse);
};