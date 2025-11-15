// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "SaveSystem/ToroSaveManager.h"
#include "SaveSystem/ToroGlobalSave.h"
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

void UToroSaveManager::AddPermanentFlag(const UObject* ContextObject, const FGameplayTag InFlag)
{
	if (UToroSaveManager* SM = UToroSaveManager::Get(ContextObject))
	{
		if (UToroGlobalSave* GS = SM->FindOrAddSave<UToroGlobalSave>())
		{
			GS->Content.Add(InFlag);
			GS->SaveObject(nullptr);
		}
	}
}

void UToroSaveManager::RemovePermanentFlag(const UObject* ContextObject, const FGameplayTag InFlag)
{
	if (UToroSaveManager* SM = UToroSaveManager::Get(ContextObject))
	{
		if (UToroGlobalSave* GS = SM->FindOrAddSave<UToroGlobalSave>())
		{
			GS->Content.Remove(InFlag);
			GS->SaveObject(nullptr);
		}
	}
}

bool UToroSaveManager::HasPermanentFlag(const UObject* ContextObject, const FGameplayTag InFlag)
{
	if (UToroSaveManager* SM = UToroSaveManager::Get(ContextObject))
	{
		if (const UToroGlobalSave* GS = SM->FindOrAddSave<UToroGlobalSave>())
		{
			return GS->Content.Contains(InFlag);
		}
	}
	return false;
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
