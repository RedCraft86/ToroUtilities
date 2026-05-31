// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Helpers/WorldGetter.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"
#include "ToroPlayerCameraManager.generated.h"

UCLASS(NotPlaceable, Blueprintable, BlueprintType)
class TORORUNTIME_API AToroPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:

	AToroPlayerCameraManager();

	template<typename T = AToroPlayerCameraManager>
	[[nodiscard]] static T* Get(const UObject* ContextObject)
	{
		const UWorld* World = FWorldGetter::Get(ContextObject);
		const APlayerController* PC = IsValid(World) ? World->GetFirstPlayerController() : nullptr;
		return IsValid(PC) ? Cast<T>(PC->PlayerCameraManager) : nullptr;
	}

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
