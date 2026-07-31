// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma	once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpleCooldown.generated.h"

/**
 * A utility struct used to throttle logic within a tick function.
 * This allows specific blocks of code to execute at a lower frequency while 
 * allowing the parent ticker to tick every frame for higher frequency tasks.
 */
USTRUCT(BlueprintType)
struct TOROCORE_API FSimpleCooldown final
{
	GENERATED_BODY()

	/** Duration in seconds between successful cooldowns. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cooldown)
		float Interval;

	/** Remaining time in seconds until the cooldown finsishes. */
	UPROPERTY(VisibleInstanceOnly, Category = Cooldown)
		float Cooldown;

	FSimpleCooldown(): Interval(0.1f), Cooldown(0.0f) {}
	FSimpleCooldown(const float InInterval): Interval(InInterval), Cooldown(0.0f) {}

	/**
	 * Updates the cooldown and checks if the cooldown has finished.
	 * @param DeltaTime Elapsed time since last frame to tick down the internal timer.
	 * @return true if the cooldown has ended this frame, false otherwise.
	 */
	bool Tick(const float DeltaTime)
	{
		if ((Cooldown -= DeltaTime) <= 0.0f)
		{
			Cooldown = Interval;
			return true;
		}

		return false;
	}

	/**
	 * Sets the cooldown to 0 so the next Tick call considers the cooldown finished.
	 */
	void Ready()
	{
		Cooldown = 0.0f;
	}

	/**
	 * Resets the cooldown to the interval.
	 */
	void Reset()
	{
		Cooldown = Interval;
	}
};

/**
 * Blueprint function wrappers for SimpleCooldown struct functions
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API USimpleCooldownLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Updates the cooldown and checks if the cooldown has finished.
	 * @param InCooldown The simple cooldown struct to modify.
	 * @param DeltaTime Elapsed time since last frame to tick down the internal timer.
	 * @return true if the cooldown has ended this frame, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = SimpleCooldown)
	static bool TickCooldown(UPARAM(ref) FSimpleCooldown& InCooldown, const float DeltaTime)
	{
		return InCooldown.Tick(DeltaTime);
	}

	/**
	 * Sets the cooldown to 0 so the next Tick call considers the cooldown finished.
	 * @param InCooldown The simple cooldown struct to modify.
	 */
	UFUNCTION(BlueprintCallable, Category = SimpleCooldown)
	static void ForceCooldownReady(UPARAM(ref) FSimpleCooldown& InCooldown)
	{
		InCooldown.Ready();
	}

	/**
	 * Resets the cooldown to the interval.
	 * @param InCooldown The simple cooldown struct to modify.
	 */
	UFUNCTION(BlueprintCallable, Category = SimpleCooldown)
	static void ResetCooldown(UPARAM(ref) FSimpleCooldown& InCooldown)
	{
		InCooldown.Reset();
	}
};