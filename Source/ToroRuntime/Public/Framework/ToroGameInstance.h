// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"
#include "Helpers/ClassGetterMacros.h"
#include "UserSettings/ToroUserSettings.h"
#include "ToroGameInstance.generated.h"

UCLASS()
class TORORUNTIME_API UToroGameInstance : public UGameInstance
{
	GENERATED_BODY()

	friend class AToroGameMode;

public:

	UToroGameInstance(): bUnlitMode(false), bInvinciblePlayer(false) {}

	GLOBAL_CLASS_GETTER(UToroGameInstance, GetGameInstance)

	UFUNCTION(BlueprintCallable, Category = GameInstance)
		void SetUnlitViewmode(const bool bUnlit);

	UFUNCTION(BlueprintPure, Category = GameInstance)
		bool IsUnlitViewmode() const { return bUnlitMode; }

	UFUNCTION(BlueprintCallable, Category = GameInstance)
		void SetPlayerInvincible(const bool bInvincible);

	UFUNCTION(BlueprintPure, Category = GameInstance)
		bool IsPlayerInvincible() const { return bInvinciblePlayer; }

	UFUNCTION(BlueprintCallable, Category = GlobalFlags, meta = (WorldContext = "ContextObject"))
		static void AddTransientFlag(const UObject* ContextObject, const FGameplayTag InFlag);

	UFUNCTION(BlueprintCallable, Category = GlobalFlags, meta = (WorldContext = "ContextObject"))
		static void RemoveTransientFlag(const UObject* ContextObject, const FGameplayTag InFlag);

	UFUNCTION(BlueprintPure, Category = GlobalFlags, meta = (WorldContext = "ContextObject"))
		static bool HasTransientFlag(const UObject* ContextObject, const FGameplayTag InFlag);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerInvincible, bool);
	FOnPlayerInvincible OnPlayerInvincible;

protected:

	bool bUnlitMode;
	bool bInvinciblePlayer;
	FGameplayTagContainer TransientFlags;

	void OnSettingUpdate(const ESettingApplyType Type);

	virtual void OnFirstLaunch();
	virtual void OnWorldBeginPlay(UWorld* InWorld);
	virtual void OnWorldTick(const float DeltaSeconds) {}

	virtual void Init() override;
};
