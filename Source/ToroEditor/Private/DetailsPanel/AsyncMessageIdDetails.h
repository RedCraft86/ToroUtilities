// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "IDetailPropertyRow.h"
#include "DetailsPanel/ToroStructCustomization.h"

#include "AsyncMessageId.h"

/**
 * @warning This customizes a struct in the <c>AsyncMessageSystem</c> plugin. It may break with plugin changes.
 */
class FAsyncMessageIdDetails final : public FToroStructCustomization
{
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		FToroStructCustomization::CustomizeChildren(StructHandle, StructBuilder, StructCustomizationUtils);

		const TSharedPtr<IPropertyHandle> MsgTag = StructHandle->GetChildHandle(TEXT("InternalMessageTag"));
		MsgTag->MarkHiddenByCustomization();

		ForwardMetadata(MsgTag);

		// Display inner property as the struct itself
		StructBuilder.AddProperty(MsgTag.ToSharedRef())
			.DisplayName(StructHandle->GetPropertyDisplayName())
			.ToolTip(StructHandle->GetToolTipText());
	}
};