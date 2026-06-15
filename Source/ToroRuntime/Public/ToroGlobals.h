// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "NativeGameplayTags.h"

#define ALLOW_PACKAGED_FEATURES !WITH_EDITOR

#define IMPLEMENT_TAG_CHECK() inline bool IsValidTag(const FGameplayTag& InTag) \
	{ \
		return InTag.MatchesTag(BaseTag.GetTag()) && InTag != BaseTag.GetTag(); \
	}

namespace ToroGameplayTags
{
	namespace Flag
	{
		TORORUNTIME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BaseTag)
		IMPLEMENT_TAG_CHECK()
	}

	namespace Event
	{
		TORORUNTIME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BaseTag)
		IMPLEMENT_TAG_CHECK()
	}
}

#undef IMPLEMENT_TAG_CHECK