// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "CoreMinimal.h"
#include "Engine/LatentActionManager.h"

/**
 * A static utility class for generating FLatentActionInfo structures.
 * 
 * Latent actions in Blueprints require a unique LinkID to track their state in the 
 * LatentActionManager. This utility automates the generation of these IDs to prevent 
 * collisions and simplify the boilerplate required when calling latent functions from C++.
 *
 * Usage:
 *     FLatentActionInfo LatentInfo = FLatentMaker::Make(GET_FUNCTION_NAME_CHECKED(UMyClass, OnComplete), this);
 *     UKismetSystemLibrary::Delay(GetWorld(), 2.0f, LatentInfo);
 */
class FLatentInfo
{
	static inline uint16 LatentId = 1000; 

public:
	
	/**
	 * Creates a fully initialized FLatentActionInfo struct.
	 * 
	 * @param FuncName The name of the function to execute once the latent action completes.
	 * @param Target   The object on which to call the completion function.
	 * @return A unique FLatentActionInfo configured for the current execution context.
	 * @note This method is not thread-safe. Latent actions should generally be initialized on the Game Thread.
	 */
	[[nodiscard]] static FLatentActionInfo Make(const TCHAR* FuncName = TEXT(""), UObject* Target = nullptr)
	{
		return FLatentActionInfo(0, NextId(), FuncName, Target);
	}
	 
	/**
	 * Increments and returns the next available LinkID. 
	 * Automatically wraps back to 1000 if the <c>UINT16_MAX</c> limit 
	 * is reached to maintain a consistent range of safe IDs.
	 * 
	 * @return A unique unsigned 16-bit integer ID.
	 * @note This method is not thread-safe. Latent actions should generally be initialized on the Game Thread.
	 */
	[[nodiscard]] static uint16 NextId()
	{
		checkf(IsInGameThread(), TEXT("FLatentInfo::NextId() should only be called on the Game Thread."))
		if (LatentId == UINT16_MAX)
		{
			// Wrap back to the safe start range to avoid engine collisions
			LatentId = 1000;
		}
		return LatentId++;
	}
};
