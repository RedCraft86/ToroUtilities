// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroRuntime.h"
#include "Helpers/ClassGetterMacros.h"
#include "Helpers/GameplayTagMacros.h"
#include "Subsystems/WorldSubsystem.h"
#include "LevelZoneManager.generated.h"

namespace ZoneTags
{
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(Zone)
	TORORUNTIME_API CREATE_TAG_VERIFIER(Zone)
}

UCLASS(BlueprintType, DisplayName = "Zone Manager")
class TORORUNTIME_API ULevelZoneManager final : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	ULevelZoneManager() {}

	WORLD_SUBSYSTEM_GETTER(ULevelZoneManager);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FZoneActivity, const FGameplayTag&, Character, const FGameplayTag&, Zone);
	UPROPERTY(BlueprintAssignable) FZoneActivity OnEnterZone;
	UPROPERTY(BlueprintAssignable) FZoneActivity OnExitZone;

	UFUNCTION(BlueprintCallable, Category = LevelZone)
		void AssignCharacterToZone(
			UPARAM(meta = (Categories = "Character")) const FGameplayTag Character,
			UPARAM(meta = (Categories = "Zone")) const FGameplayTag Zone);
	
	UFUNCTION(BlueprintCallable, Category = LevelZone)
		bool IsCharacterInZone(UPARAM(meta = (Categories = "Character")) const FGameplayTag Character,
			UPARAM(meta = (Categories = "Zone")) const FGameplayTag Zone);

	UFUNCTION(BlueprintCallable, Category = LevelZone)
		FGameplayTag GetZoneByCharacter(UPARAM(meta = (Categories = "Character")) const FGameplayTag Character);

	UFUNCTION(BlueprintCallable, Category = LevelZone)
		FGameplayTagContainer GetCharactersInZone(UPARAM(meta = (Categories = "Zone")) const FGameplayTag Zone);

private:

	TMap<FGameplayTag, FGameplayTag> CharToZone;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
};
