// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpleCooldown.generated.h"

/**
 * A utility struct used to throttle logic within a Tick function.
 * 
 * This allows specific blocks of code to execute at a lower frequency (Interval) 
 * while allowing the parent Actor to continue ticking every frame for high-frequency 
 * tasks like movement or input.
 */
USTRUCT(BlueprintType)
struct TOROCORE_API FSimpleCooldown final
{
	GENERATED_BODY()

	/** 
	 * The duration (in seconds) between successful pulses.
	 * Higher values result in the gated logic running less frequently.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cooldown)
		float Interval;

	/** 
	 * The remaining time (in seconds) until the next interval is reached.
	 * When this value reaches <= 0, the next call to TickCooldown returns true.
	 */
	UPROPERTY(VisibleInstanceOnly, Category = Cooldown) 
		float Cooldown;

	FSimpleCooldown(): Interval(0.1f), Cooldown(0.0f) {}
	FSimpleCooldown(const float Value): Interval(Value), Cooldown(0.0f) {}

	/**
	 * Updates the internal timer and checks if the interval has elapsed.
	 * 
	 * @param DeltaTime  The elapsed time since the last frame (usually from AActor::Tick).
	 * @return           True if the cooldown has expired this frame, triggering the interval; 
	 *                   False otherwise. If true, the internal timer is reset to Interval.
	 */
	bool TickCooldown(const float DeltaTime)
	{
		if ((Cooldown -= DeltaTime) < 0.0f)
		{
			Cooldown = Interval;
			return true;
		}
		return false;
	}

	/** 
	 * Immediately resets the cooldown, ensuring the next call to TickCooldown 
	 * returns true and triggers the gated logic. 
	 */
	void ForceReady()
	{
		Cooldown = 0.0f;
	}

	/** 
	 * Resets the cooldown to the interval value so the next call to TickCooldown 
	 * will have to wait the whole interval before it returns true.
	 */
	void ResetTime()
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
	 * Updates the internal timer and checks if the interval has elapsed.
	 * 
	 * @param InCooldown The simple cooldown struct to modify.
	 * @param DeltaTime  The elapsed time since the last frame (usually from AActor::Tick).
	 * @return           True if the cooldown has expired this frame, triggering the interval; 
	 *                   False otherwise. If true, the internal timer is reset to Interval.
	 */
	UFUNCTION(BlueprintCallable, Category = "SimpleCooldown")
	static bool TickCooldown(UPARAM(ref) FSimpleCooldown& InCooldown, const float DeltaTime)
	{
		return InCooldown.TickCooldown(DeltaTime);
	}

	/** 
	 * Immediately resets the cooldown, ensuring the next call to TickCooldown 
	 * returns true and triggers the gated logic. 
	 * @param InCooldown The simple cooldown struct to modify.
	 */
	UFUNCTION(BlueprintCallable, Category = "SimpleCooldown")
	static void ForceCooldownReady(UPARAM(ref) FSimpleCooldown& InCooldown)
	{
		InCooldown.ForceReady();
	}

	/** 
	 * Resets the cooldown to the interval value so the next call to TickCooldown 
	 * will have to wait the whole interval before it returns true.
	 * @param InCooldown The simple cooldown struct to modify.
	 */
	UFUNCTION(BlueprintCallable, Category = "SimpleCooldown")
	static void ResetCooldown(UPARAM(ref) FSimpleCooldown& InCooldown)
	{
		InCooldown.ResetTime();
	}
};