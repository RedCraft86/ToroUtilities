// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

UENUM(BlueprintInternalUseOnly)
enum class EToroFoundPins : uint8
{
	Found,
	NotFound
};

UENUM(BlueprintInternalUseOnly)
enum class EToroValidityPins : uint8
{
	Valid,
	Invalid
};

UENUM(BlueprintInternalUseOnly)
enum class EToroSuccessPins : uint8
{
	Success,
	Failed
};