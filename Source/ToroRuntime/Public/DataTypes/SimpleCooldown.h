// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpleCooldown.generated.h"

/**
 * Lightweight repeating cooldown driven by an external tick.
 *
 * Call Tick() with elapsed time until it returns true. When triggered, the
 * cooldown automatically resets to Interval and begins another cycle.
 */
USTRUCT(BlueprintType)
struct TORORUNTIME_API FSimpleCooldown final
{
	GENERATED_BODY()

	/**
	 * Duration of each cooldown cycle in seconds.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cooldown)
		float Interval;

	/**
	 * Time remaining in the current cooldown cycle in seconds.
	 */
	UPROPERTY(VisibleInstanceOnly, Category = Cooldown)
		float Cooldown;

	/**
	 * Creates a ready cooldown with a default interval of 0.1 seconds.
	 */
	FSimpleCooldown()
		: Interval(0.1f), Cooldown(0.0f)
	{}

	/**
	 * Creates a ready cooldown with the specified interval.
	 * @param InInterval Duration of each cooldown cycle in seconds.
	 */
	FSimpleCooldown(const float InInterval)
		: Interval(InInterval), Cooldown(0.0f)
	{}

	/**
	 * Restarts the cooldown using the current interval.
	 */
	void Reset();

	/**
	 * Makes the cooldown trigger on the next tick.
	 */
	void Ready();

	/**
	 * Advances the cooldown and restarts it when the interval elapses.
	 *
	 * A call can produce at most one trigger, regardless of the supplied delta.
	 *
	 * @param DeltaTime Elapsed time in seconds.
	 * @return true when the cooldown elapses during this call; otherwise, false.
	 */
	[[nodiscard]] bool Tick(const float DeltaTime);
};

/**
 * Blueprint-accessible operations for FSimpleCooldown.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TORORUNTIME_API USimpleCooldownLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Restarts a cooldown using its current interval.
	 * @param InCooldown Cooldown to reset.
	 */
	UFUNCTION(BlueprintCallable, Category = SimpleCooldown)
		static void ResetCooldown(UPARAM(ref) FSimpleCooldown& InCooldown);

	/**
	 * Makes a cooldown trigger on its next tick.
	 * @param InCooldown Cooldown to mark as ready.
	 */
	UFUNCTION(BlueprintCallable, Category = SimpleCooldown)
		static void ReadyCooldown(UPARAM(ref) FSimpleCooldown& InCooldown);

	/**
	 * Advances a cooldown and restarts it when its interval elapses.
	 * @param InCooldown Cooldown to advance.
	 * @param DeltaTime Elapsed time in seconds.
	 * @return true when the cooldown elapses during this call; otherwise, false.
	 */
	UFUNCTION(BlueprintCallable, Category = SimpleCooldown)
		static bool TickCooldown(UPARAM(ref) FSimpleCooldown& InCooldown, const float DeltaTime);
};