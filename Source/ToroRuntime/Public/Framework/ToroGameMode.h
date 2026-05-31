// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Helpers/WorldGetter.h"
#include "GameFramework/GameModeBase.h"
#include "ToroGameMode.generated.h"

UCLASS(NotPlaceable, Blueprintable, BlueprintType)
class TORORUNTIME_API AToroGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AToroGameMode();

	template<typename T = AToroGameMode>
	[[nodiscard]] static T* Get(const UObject* ContextObject)
	{
		const UWorld* World = FWorldGetter::Get(ContextObject);
		return IsValid(World) ? World->GetAuthGameMode<T>() : nullptr;
	}

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(Transient)
		TWeakObjectPtr<class UToroGameInstance> GameInstance;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
