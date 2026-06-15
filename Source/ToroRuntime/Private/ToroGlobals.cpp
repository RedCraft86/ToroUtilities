// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

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