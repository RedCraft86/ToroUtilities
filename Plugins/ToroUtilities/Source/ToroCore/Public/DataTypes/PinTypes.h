// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "PinTypes.generated.h"

UENUM(BlueprintInternalUseOnly)
enum class EToroFoundPins : uint8
{
	Found,
	NotFound
};

UENUM(BlueprintInternalUseOnly)
enum class EToroValidPins : uint8
{
	Valid,
	NotValid
};