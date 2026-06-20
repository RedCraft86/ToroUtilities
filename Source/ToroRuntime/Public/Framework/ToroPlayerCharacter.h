// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Helpers/WorldGetter.h"
#include "Actors/ToroCharacter.h"
#include "DataTypes/LightProperties.h"
#include "GameFramework/WorldSettings.h"
#include "ToroPlayerCharacter.generated.h"

/**
 * The primary playable agent. Includes specialized 'Slow Tick' for non-critical performance-heavy logic.
 */
UCLASS(Blueprintable, BlueprintType, PrioritizeCategories = (Settings))
class TORORUNTIME_API AToroPlayerCharacter : public AToroCharacter
{
	GENERATED_BODY()

public:

	AToroPlayerCharacter();

	template<typename T = AToroPlayerCharacter>
	[[nodiscard]] static T* Get(const UObject* ContextObject)
	{
		static_assert(TIsDerivedFrom<T, AToroPlayerCharacter>::Value, 
			TEXT("T must derive from AToroPlayerCharacter"));

		const UWorld* World = FWorldGetter::Get(ContextObject);
		const APlayerController* PC = IsValid(World) ? World->GetFirstPlayerController() : nullptr;
		return IsValid(PC) ? PC->GetPawn<T>() : nullptr;
	}

	/** Updates the ambient light properties attached to the player. */
	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void SetLightSettings(const FPointLightProperties& InSettings);

	/** Returns current horizontal movement speed. */
	UFUNCTION(BlueprintPure, Category = Player)
		float GetSpeed() const { return GetVelocity().Size2D(); }

	/** True if the character's horizontal velocity exceeds the movement threshold of 50 units. */
	UFUNCTION(BlueprintPure, Category = Player)
		bool IsMoving() const { return GetSpeed() > 50.0f; }

	/** Checks if the game world is currently paused. */
	UFUNCTION(BlueprintPure, Category = Player)
		bool IsPaused() const { return GetWorldSettings()->GetPauserPlayerState() != nullptr; }

	/** Triggers a footstep sound effect at the player's current foot position. */
	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void PlayFootstep(USoundBase* InSound, float InVolume = 1.0f);

	/** Performs a line trace downwards to determine the physical surface type for footsteps/VFX. */
	UFUNCTION(BlueprintCallable, Category = Player)
		virtual bool GetFloorSurface(TEnumAsByte<EPhysicalSurface>& Surface, const ECollisionChannel TraceChannel);

	/** Helper for accessing GameMode as a specific type. */
	template <typename T = class AToroGameMode>
	T* GetGameMode() const { return GetWorld()->GetAuthGameMode<T>(); }

	/** Helper for accessing GameState as a specific type. */
	template <typename T = class AToroGameState>
	T* GetGameState() const { return GetWorld()->GetGameState<T>(); }

	virtual void Teleport(const FVector& InLocation, const FRotator& InRotation) override;

protected:

	/** Ambient point light attached to the player for visibility in dark environments. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UPointLightComponent> AmbientLight;

	/** Audio component used for triggering local player movement sounds. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UAudioComponent> PlayerFootstep;

	/** Frequency for 'SlowTick' execution. High values improve performance. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Tick, meta = (ClampMin = 0.05f, UIMin = 0.05f))
		float SlowTickInterval;

	/** Designer-facing properties for the AmbientLight component. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, AdvancedDisplay)
		FPointLightProperties LightSettings;

	float SlowTickTime;

	/** 
	 * Updates the PlayerFootstep component position to be at capsule half height below origin. 
	 * Mostly due to it potentially getting messed up during crouching.
	 */
	void FixFootstepPosition() const;

	/** Executed at the rate defined by SlowTickInterval for non-render-critical logic. */
	virtual void SlowTick();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
