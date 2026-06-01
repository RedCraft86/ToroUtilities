// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "StateEnums.generated.h"

UENUM(BlueprintInternalUseOnly)
enum class EToroFoundState : uint8
{
	Found,
	NotFound
};

UENUM(BlueprintInternalUseOnly)
enum class EToroValidityState : uint8
{
	Valid,
	Invalid
};

UENUM(BlueprintInternalUseOnly)
enum class EToroSuccessState : uint8
{
	Success,
	Failed
};