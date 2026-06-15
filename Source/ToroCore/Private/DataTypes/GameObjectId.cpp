// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "DataTypes/GameObjectId.h"

const FGameObjectId& FGameObjectId::GetEmpty()
{
	static const FGameObjectId EmptyId;
	return EmptyId;
}

void FGameObjectId::Invalidate()
{
	Group = FGameplayTag::EmptyTag;
	Guid.Invalidate();
}

void FGameObjectId::RegenerateGuid(const bool bOnlyIfInvalid)
{
	if (!Guid.IsValid() || !bOnlyIfInvalid)
	{
		Guid = FGuid::NewGuid();
	}
}
