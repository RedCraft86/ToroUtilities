// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Helpers/WorldGetter.h"
#include "CommonGameViewportClient.h"
#include "ToroGameViewportClient.generated.h"

/**
 * Custom viewport logic for handling window focus and global UI rendering.
 */
UCLASS(Blueprintable, BlueprintType)
class TORORUNTIME_API UToroGameViewportClient : public UCommonGameViewportClient
{
	GENERATED_BODY()

public:

	UToroGameViewportClient();

	template<typename T = UToroGameViewportClient>
	[[nodiscard]] static T* Get(const UObject* ContextObject)
	{
		const UWorld* World = FWorldGetter::Get(ContextObject);
		return IsValid(World) ? Cast<T>(World->GetGameViewport()) : nullptr;
	}
};
