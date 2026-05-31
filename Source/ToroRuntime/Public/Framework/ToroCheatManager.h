// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Helpers/WorldGetter.h"
#include "GameFramework/CheatManager.h"
#include "GameFramework/PlayerController.h"
#include "ToroCheatManager.generated.h"

UCLASS(Blueprintable, BlueprintType)
class TORORUNTIME_API UToroCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:

	UToroCheatManager();

	template<typename T = UToroCheatManager>
	[[nodiscard]] static T* Get(const UObject* ContextObject)
	{
		const UWorld* World = FWorldGetter::Get(ContextObject);
		const APlayerController* PC = IsValid(World) ? World->GetFirstPlayerController() : nullptr;
		return IsValid(PC) ? Cast<T>(PC->CheatManager) : nullptr;
	}
};
