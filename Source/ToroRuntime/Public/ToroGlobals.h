// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "NativeGameplayTags.h"

#define ALLOW_PACKAGED_FEATURES !WITH_EDITOR

#define TAG_VALID_CHECK() inline bool IsValidTag(const FGameplayTag& InTag) \
	{ \
		return InTag.MatchesTag(BaseTag.GetTag()) && InTag != BaseTag.GetTag(); \
	}

namespace ToroGameplayTags
{
	namespace Flag
	{
		TORORUNTIME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BaseTag)
		TAG_VALID_CHECK()
	}

	namespace Event
	{
		TORORUNTIME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BaseTag)
		TAG_VALID_CHECK()
	}
}

#undef TAG_VALID_CHECK