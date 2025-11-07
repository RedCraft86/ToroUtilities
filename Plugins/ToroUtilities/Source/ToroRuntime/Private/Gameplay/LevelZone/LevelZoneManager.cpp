// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/LevelZone/LevelZoneManager.h"
#include "Actors/ToroCharacter.h"

namespace ZoneTags
{
	DEFINE_GAMEPLAY_TAG(Zone)
}

void ULevelZoneManager::AssignCharacterToZone(const FGameplayTag Character, const FGameplayTag Zone)
{
	if (!CharacterTags::IsValidTag(Character) || !ZoneTags::IsValidTag(Zone)) return;
	const FGameplayTag LastZone = CharToZone.FindRef(Character);
	if (LastZone != Zone)
	{
		CharToZone.Add(Character, Zone);
		OnExitZone.Broadcast(Character, LastZone);
		OnEnterZone.Broadcast(Character, Zone);
	}
}

bool ULevelZoneManager::IsCharacterInZone(const FGameplayTag Character, const FGameplayTag Zone)
{
	if (!CharacterTags::IsValidTag(Character) || !ZoneTags::IsValidTag(Zone)) return false;
	return CharToZone.FindRef(Character) == Zone;
}

FGameplayTag ULevelZoneManager::GetZoneByCharacter(const FGameplayTag Character)
{
	return CharacterTags::IsValidTag(Character) ? CharToZone.FindRef(Character) : FGameplayTag::EmptyTag;
}

FGameplayTagContainer ULevelZoneManager::GetCharactersInZone(const FGameplayTag Zone)
{
	FGameplayTagContainer Characters;
	if (!ZoneTags::IsValidTag(Zone)) return Characters;
	for (const TPair<FGameplayTag, FGameplayTag>& Pair : CharToZone)
	{
		if (Pair.Value == Zone) Characters.AddTag(Pair.Key);
	}
	return Characters;
}

bool ULevelZoneManager::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer) && UToroSettings::Get()->IsOnMap(Outer, EToroMapType::Gameplay);
}

bool ULevelZoneManager::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE ||
		WorldType == EWorldType::GamePreview || WorldType == EWorldType::GameRPC;
}
