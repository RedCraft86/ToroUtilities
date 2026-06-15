// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

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