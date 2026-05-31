// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Helpers/WorldGetter.h"
#include "Engine/GameInstance.h"
#include "ToroGameInstance.generated.h"

UCLASS(Blueprintable, BlueprintType)
class TORORUNTIME_API UToroGameInstance : public UGameInstance
{
	GENERATED_BODY()

	friend class AToroGameMode;

public:

	UToroGameInstance() {}

	template<typename T = UToroGameInstance>
	[[nodiscard]] static T* Get(const UObject* ContextObject)
	{
		const UWorld* World = FWorldGetter::Get(ContextObject);
		return IsValid(World) ? World->GetGameInstance<T>() : nullptr;
	}

	UFUNCTION(BlueprintCallable, Category = Instance)
		void RestartGame();

	UFUNCTION(BlueprintImplementableEvent)
		void WorldBeginPlay();

	UFUNCTION(BlueprintImplementableEvent)
		void WorldTick(const float DeltaTime);

protected:

	FString LockPath;
	FArchive* InstanceLock;

	virtual void ClearInstanceLock();
	virtual void OnWorldBeginPlay(UWorld* InWorld);
	virtual void OnWorldTick(UWorld* InWorld, const float DeltaTime);

	virtual void Init() override;
	virtual void Shutdown() override;
};
