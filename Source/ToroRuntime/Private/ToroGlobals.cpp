// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "ToroGlobals.h"

#define DEFINE_BASE_TAG(Category) UE_DEFINE_GAMEPLAY_TAG(Category::BaseTag, #Category);
#define DEFINE_CHILD_TAG(Category, Name) UE_DEFINE_GAMEPLAY_TAG(Category::Name, UE_INLINE_STRINGIFY(Category.Name));

namespace ToroGameplayTags
{
	DEFINE_BASE_TAG(Flag)
	DEFINE_BASE_TAG(Event)
}

#undef DEFINE_CHILD_TAG
#undef DEFINE_BASE_TAG