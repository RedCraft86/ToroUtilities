// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/GameInstance.h"
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

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerInvincible, bool);
	FOnPlayerInvincible OnPlayerInvincible;

protected:

	bool bUnlitMode;
	bool bInvinciblePlayer;

	void OnSettingUpdate(const ESettingApplyType Type);

	virtual void OnFirstLaunch();
	virtual void OnWorldBeginPlay(UWorld* InWorld);
	virtual void OnWorldTick(const float DeltaSeconds) {}

	virtual void Init() override;
};
