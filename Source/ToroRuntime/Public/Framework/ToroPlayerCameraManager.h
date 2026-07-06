// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
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
		static_assert(TIsDerivedFrom<T, AToroPlayerCameraManager>::Value, 
			"T must derive from AToroPlayerCameraManager");

		const UWorld* World = FWorldGetter::Get(ContextObject);
		const APlayerController* PC = IsValid(World) ? World->GetFirstPlayerController() : nullptr;
		return IsValid(PC) ? Cast<T>(PC->PlayerCameraManager) : nullptr;
	}

	/**
	 * Checks if an actor is in view by running frustum checks and line trace occlusion checks
	 * @param Target Actor to test.
	 * @param BoxScale Size of the actor's bounding box. (Must be >= 0.2f)
	 * @param MaxSamples Num of random points within the bounding box that should be traced. (If 0, only origin point)
	 * @return True if the actor is in view.
	 */
	UFUNCTION(BlueprintPure, Category = PlayerCamera)
		bool IsActorSeen(AActor* Target, const float BoxScale = 0.8f, const uint8 MaxSamples = 16) const;

	/**
	 * Does a single line trace from the camera, making sure to ignore core actors such as the view target and self.
	 * @param HitResult The resulting hits.
	 * @param Target End of trace point.
	 * @param TraceChannel Channel to trace on.
	 * @param ModifyParams Any additional modifications for CollisionQueryParam.
	 * @return True if a hit occured.
	 */
	bool LineTraceSingleFromView(FHitResult& HitResult, const FVector& Target, 
		const ECollisionChannel TraceChannel = ECC_Visibility, 
		const TFunction<void(FCollisionQueryParams&)>& ModifyParams = nullptr) const;

	/**
	 * Does a multi line trace from the camera, making sure to ignore core actors such as the view target and self.
	 * @param HitResults The resulting hits.
	 * @param Target End of trace point.
	 * @param TraceChannel Channel to trace on.
	 * @param ModifyParams Any additional modifications for CollisionQueryParam.
	 * @return True if a hit occured.
	 */
	bool LineTraceMultiFromView(TArray<FHitResult>& HitResults, const FVector& Target, 
		const ECollisionChannel TraceChannel = ECC_Visibility, 
		const TFunction<void(FCollisionQueryParams&)>& ModifyParams = nullptr) const;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
