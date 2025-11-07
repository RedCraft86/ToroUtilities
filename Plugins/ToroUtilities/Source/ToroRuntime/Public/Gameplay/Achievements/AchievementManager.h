// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "AchievementDatabase.h"
#include "Framework/ToroPlayerState.h"
#include "Components/ActorComponent.h"
#include "Helpers/ClassGetterMacros.h"
#include "SaveSystem/ToroSaveManager.h"
#include "AchievementManager.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UAchievementManager final : public UActorComponent
{
	GENERATED_BODY()

public:

	UAchievementManager() {}
	
	PLAYER_COMPONENT_GETTER(UAchievementManager, AToroPlayerState, Achievements)

	UFUNCTION(BlueprintCallable, Category = Statics, meta = (WorldContext = "ContextObject"))
	static UAchievementManager* GetAchievementManager(const UObject* ContextObject, const int32 PlayerIdx = 0)
	{
		return Get(ContextObject, PlayerIdx);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Achievements)
		void Grant(UPARAM(meta = (Categories = "Achievement")) const FGameplayTag Key, const uint8 Value = 1) const;

	UFUNCTION(BlueprintCallable, Category = Achievements)
		uint8 GetProgress(UPARAM(meta = (Categories = "Achievement")) const FGameplayTag Key) const;
	
	UFUNCTION(BlueprintCallable, Category = Achievements)
		bool HasAchievement(UPARAM(meta = (Categories = "Achievement")) const FGameplayTag Key) const;

	DECLARE_MULTICAST_DELEGATE_OneParam(FAchievementEvent, const FAchievementEntry&)
	FAchievementEvent OnAchievement;

private:

	UPROPERTY(Transient)
		TObjectPtr<UAchievementDatabase> Database;

	TObjectPtr<UToroSaveManager> SaveManager;

	virtual void BeginPlay() override;
};
