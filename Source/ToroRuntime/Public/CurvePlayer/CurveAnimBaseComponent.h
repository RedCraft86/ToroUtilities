// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "DataTypes/InlineCurves.h"
#include "Components/ToroComponents.h"
#include "CurveAnimBaseComponent.generated.h"

UENUM(BlueprintType)
enum class ECurvePlayerState : uint8
{
	Stopped,
	Forward,
	Reverse
};

/**
 * Abstract base class for component-based curve players.
 * Handles the core logic for playback states, looping, play rates, and time management.
 * 
 * Derived classes must implement BroadcastEval to send type-specific data (Float, Vector, etc.).
 */
UCLASS(Abstract, NotBlueprintable)
class TORORUNTIME_API UCurveAnimBaseComponent : public UToroActorComponent
{
	GENERATED_BODY()

public:

	UCurveAnimBaseComponent();

	/** If true, the curve will restart automatically upon reaching the boundary. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayPriority = -1))
		bool bLooping;

	/** Multiplier for playback speed. 1.0 is default. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 0.05f, UIMin = 0.05f, DisplayPriority = -1))
		float PlayRate;

	/** Stops playback. If bReset is true, current time returns to the start of the curve. */
	UFUNCTION(BlueprintCallable, Category = CurvePlayer)
		void StopPlaying(const bool bReset = false);

	/** Starts playback in the forward direction. */
	UFUNCTION(BlueprintCallable, Category = CurvePlayer)
		void PlayForward(const bool bFromStart = false);

	/** Starts playback in the reverse direction. */
	UFUNCTION(BlueprintCallable, Category = CurvePlayer)
		void PlayReverse(const bool bFromEnd = false);

	/** Manually sets the evaluation time. bNotify determines if evaluation events are fired. */
	UFUNCTION(BlueprintCallable, Category = CurvePlayer)
		void SetPlaybackTime(const float InTime, const bool bNotify = true);

	/** Snaps current time to the curve's minimum time. */
	UFUNCTION(BlueprintCallable, Category = CurvePlayer)
		void SnapToStart(const bool bNotify = true) { SetPlaybackTime(TimeRange.X, bNotify); }

	/** Snaps current time to the curve's maximum time. */
	UFUNCTION(BlueprintCallable, Category = CurvePlayer)
		void SnapToEnd(const bool bNotify = true) { SetPlaybackTime(TimeRange.Y, bNotify); }

	/** @return The current internal playback time. */
	UFUNCTION(BlueprintPure, Category = CurvePlayer)
		float GetPlaybackTime() const { return CurrentTime; }

	/** @return Whether the player is stopped, forward, or reversing. */
	UFUNCTION(BlueprintPure, Category = CurvePlayer)
		ECurvePlayerState GetPlayState() const { return PlayState; }

	/** @return True if the player is currently moving. */
	UFUNCTION(BlueprintPure, Category = CurvePlayer)
		bool IsPlaying() const { return PlayState != ECurvePlayerState::Stopped; }

	/** @return True if CurrentTime is approximately equal to curve start. */
	UFUNCTION(BlueprintPure, Category = CurvePlayer)
		bool IsAtStart() const { return FMath::IsNearlyEqual(TimeRange.X, CurrentTime); }

	/** @return True if CurrentTime is approximately equal to curve end. */
	UFUNCTION(BlueprintPure, Category = CurvePlayer)
		bool IsAtEnd() const { return FMath::IsNearlyEqual(TimeRange.Y, CurrentTime); }

	/** Checks if the curve has a valid duration. */
	UFUNCTION(BlueprintPure, Category = CurvePlayer)
		virtual bool IsValidCurve() const { return TimeRange.Y - TimeRange.X > KINDA_SMALL_NUMBER; }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChangedDelegateBP, const ECurvePlayerState, NewState);
	/** Broadcasts when the playback state changes (e.g., from Forward to Stopped). */
	UPROPERTY(BlueprintAssignable, DisplayName = "On State Changed")
		FOnStateChangedDelegateBP OnStateChangedBP;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnStateChangedDelegate, const ECurvePlayerState);
	FOnStateChangedDelegate OnStateChanged;

protected:

	float CurrentTime;
	FVector2D TimeRange;
	ECurvePlayerState PlayState;

	virtual void BroadcastEval() const PURE_VIRTUAL(UCurvePlayerComponent::BroadcastEval,);

	void BroadcastState() const;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
};
