// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "ToroGlobals.h"

#define DEFINE_BASE_TAG(Category) UE_DEFINE_GAMEPLAY_TAG(Category::BaseTag, #Category);
#define DEFINE_CHILD_TAG(Category, Name) UE_DEFINE_GAMEPLAY_TAG(Category::Name, UE_INLINE_STRINGIFY(Category.Name));

#define DEFINE_TAG_CHECK(Category) bool Category::IsValidTag(const FGameplayTag& InTag) \
	{ \
		InTag.MatchesTag(Category::BaseTag.GetTag()) && InTag != Category::BaseTag.GetTag(); \
	}

namespace ToroGameplayTags
{
	DEFINE_BASE_TAG(Flag)
	DEFINE_TAG_CHECK(Flag)

	DEFINE_BASE_TAG(Event)
	DEFINE_TAG_CHECK(Event)
}

#undef DEFINE_CHILD_TAG
#undef DEFINE_BASE_TAG