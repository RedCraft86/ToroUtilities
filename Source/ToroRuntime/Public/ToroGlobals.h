// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "NativeGameplayTags.h"

#define ALLOW_PACKAGED_FEATURES !WITH_EDITOR

#define DECLARE_TAG_CHECK() bool IsValidTag(const FGameplayTag& InTag);

namespace ToroGameplayTags
{
	namespace Flag
	{
		TORORUNTIME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BaseTag)
        TORORUNTIME_API DECLARE_TAG_CHECK()
	}

	namespace Event
	{
		TORORUNTIME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BaseTag)
        TORORUNTIME_API DECLARE_TAG_CHECK()
	}
}

#undef DECLARE_TAG_CHECK