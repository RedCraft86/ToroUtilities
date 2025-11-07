// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSaveTypes.h"
#include "Helpers/ClassGetterMacros.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ToroSaveManager.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FSaveSlots
{
	GENERATED_BODY()

	// Can be an Array but using a Map instead for sparse access. i.e, accessing Slot 5 before 1-4
	UPROPERTY(Transient)
		TMap<uint8, TObjectPtr<UToroSaveObject>> Slots;

	void AddSlotObject(UToroSaveObject* Object);
	UToroSaveObject* GetSlotObject(const uint8 Slot);
};

UCLASS()
class TORORUNTIME_API UToroSaveManager final : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	friend UToroSaveObject;

public:

	UToroSaveManager(): ActiveSlot(0) {}

	GAME_INSTANCE_SUBSYSTEM_GETTER(UToroSaveManager)

	UFUNCTION(BlueprintCallable, Category = SaveSystem)
		void SetActiveSaveSlot(const uint8 InSlot);

	UFUNCTION(BlueprintPure, Category = SaveSystem)
		uint8 GetActiveSaveSlot() const { return ActiveSlot; }

	UFUNCTION(BlueprintCallable, Category = SaveSystem, meta = (DeterminesOutputType = "SaveClass"))
		UToroSaveObject* FindOrAddSave(TSubclassOf<UToroSaveObject> SaveClass, const uint8 Slot = 0);

	template<typename T = UToroSaveObject>
	T* FindOrAddSave(const uint8 Slot)
	{
		return Cast<T>(FindOrAddSave(T::StaticClass(), Slot));
	}

	template<typename T = UToroSaveObject>
	T* FindOrAddSave()
	{
		return Cast<T>(FindOrAddSave(T::StaticClass(), ActiveSlot));
	}

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSaveActivity, const UToroSaveObject*, const ESaveGameActivity);
	FOnSaveActivity OnSaveActivity;

private:

	UPROPERTY(Transient)
		TMap<TSubclassOf<UToroSaveObject>, FSaveSlots> SaveObjects;

	uint8 ActiveSlot;
	
	void OnActivity(const UToroSaveObject* Save, const ESaveGameActivity Activity) const;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};
