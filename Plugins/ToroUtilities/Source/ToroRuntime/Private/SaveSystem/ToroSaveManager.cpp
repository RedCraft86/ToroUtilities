// Copyright (C) RedCraft86. All Rights Reserved.

#include "SaveSystem/ToroSaveManager.h"
#include "ToroRuntime.h"

void FSaveSlots::AddSlotObject(UToroSaveObject* Object)
{
	if (Object)
	{
		Slots.Add(Object->GetSaveSlot(), Object);
	}
}

UToroSaveObject* FSaveSlots::GetSlotObject(const uint8 Slot)
{
	TObjectPtr<UToroSaveObject>* Obj = Slots.Find(Slot);
	return Obj ? *Obj : nullptr;
}

void UToroSaveManager::SetActiveSaveSlot(const uint8 InSlot)
{
	if (UToroSettings::Get()->IsOnMap(this, EToroMapType::MainMenu))
	{
		ActiveSlot = InSlot;
	}
}

UToroSaveObject* UToroSaveManager::FindOrAddSave(const TSubclassOf<UToroSaveObject> SaveClass, const uint8 Slot)
{
	if (!SaveClass)
	{
		return nullptr;
	}

	const UToroSaveObject* DefaultObj = SaveClass->GetDefaultObject<UToroSaveObject>();
	const uint8 ActualSlot = DefaultObj && DefaultObj->bSingleSlot ? 0 : Slot;

	FSaveSlots* Slots = SaveObjects.Find(SaveClass);
	if (UToroSaveObject* ExistingObj = Slots ? Slots->GetSlotObject(ActualSlot) : nullptr)
	{
		return ExistingObj;
	}

	if (UToroSaveObject* NewObj = UToroSaveObject::Create(this, SaveClass, ActualSlot))
	{
		SaveObjects.FindOrAdd(SaveClass).AddSlotObject(NewObj);
		return NewObj;
	}
    
	return nullptr;
}

void UToroSaveManager::OnActivity(const UToroSaveObject* Save, const ESaveGameActivity Activity) const
{
	OnSaveActivity.Broadcast(Save, Activity);
}

void UToroSaveManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UToroSettings* Settings = UToroSettings::Get();
	for (TPair<TSoftClassPtr<UToroSaveObject>, uint8>& DefaultSave : Settings->InitSaves)
	{
		FindOrAddSave(DefaultSave.Key.LoadSynchronous(), DefaultSave.Value);
	}
}

void UToroSaveManager::Deinitialize()
{
	Super::Deinitialize();
	for (const TPair<TSubclassOf<UToroSaveObject>, FSaveSlots>& Types : SaveObjects)
	{
		for (const TPair<uint8, TObjectPtr<UToroSaveObject>>& Slot : Types.Value.Slots)
		{
			Slot.Value->SaveObject(nullptr);
		}
	}
}
