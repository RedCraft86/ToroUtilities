// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Helpers/WorldGetter.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"
#include "ToroPlayerCameraManager.generated.h"

/**
 * Handles per-frame camera updates, post-process blending, and camera shakes.
 * @note This class also allows camera movement when paused.
 */
UCLASS(NotPlaceable, Blueprintable, BlueprintType)
class TORORUNTIME_API AToroPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:

	AToroPlayerCameraManager();

	template<typename T = AToroPlayerCameraManager>
	[[nodiscard]] static T* Get(const UObject* ContextObject)
	{
		const UWorld* World = FWorldGetter::Get(ContextObject);
		const APlayerController* PC = IsValid(World) ? World->GetFirstPlayerController() : nullptr;
		return IsValid(PC) ? Cast<T>(PC->PlayerCameraManager) : nullptr;
	}

protected:

	virtual void Tick(float DeltaSeconds) override;
};
