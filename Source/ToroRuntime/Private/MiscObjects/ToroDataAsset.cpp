// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "MiscObjects/ToroDataAsset.h"

UToroDataAsset::UToroDataAsset()
{
}

#if WITH_EDITOR
EDataValidationResult UToroDataAsset::IsDataValid(FDataValidationContext& Context) const
{
	return Super::IsDataValid(Context);
}
#endif
