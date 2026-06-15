// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "IDetailGroup.h"
#include "IDetailPropertyRow.h"
#include "DetailsPanel/ToroStructCustomization.h"

#include "DataTypes/SimpleCooldown.h"

#define STRUCT_NAME FSimpleCooldown
class FSimpleCooldownDetails final : public FToroStructCustomization
{
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		IDetailGroup& StructGroup = StructBuilder.AddGroup(TEXT("SimpleCooldown"), StructHandle->GetPropertyDisplayName());

		StructGroup.HeaderProperty(GET_STRUCT_PROPERTY_NS(Interval).ToSharedRef())
			.DisplayName(StructHandle->GetPropertyDisplayName());

		StructGroup.AddPropertyRow(GET_STRUCT_PROPERTY_NS(Cooldown).ToSharedRef());
	}
};
#undef STRUCT_NAME