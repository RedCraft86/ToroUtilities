// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "MiscEnums.generated.h"

UENUM(BlueprintType)
enum class EAudioComponentAction : uint8
{
	Play,
	Stop,
	Pause,
	Unpause
};
