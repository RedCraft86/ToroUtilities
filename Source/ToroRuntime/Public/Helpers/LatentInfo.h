// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "Engine/LatentActionManager.h"

/**
 * Creates latent action information with sequential IDs for game-thread calls.
 * IDs wrap back to 1000 upon reaching the maximum uint16 value, so they can eventually repeat.
 */
class TORORUNTIME_API FLatentInfo final
{
	static inline uint16 LatentId = 1000;

public:

	/**
	 * Creates latent action information with a fresh ID; must be called on the game thread.
	 * @param Target the UObject at which a UFUNCTION with FuncName will be present to use as a callback after the latent action.
	 * @param FuncName the UFUNCTION to be called with the Target once the latent action finishes.
	 */
	[[nodiscard]] static FLatentActionInfo Make(UObject* Target = nullptr, const TCHAR* FuncName = TEXT(""))
	{
		checkf(IsInGameThread(), TEXT("FLatentInfo::Make(...) should only be called on the Game Thread."));
		return FLatentActionInfo(0, NextId(), FuncName, Target);
	}

	/**
	 * Returns the next ID; must be called on the game thread.
	 */
	[[nodiscard]] static uint16 NextId()
	{
		checkf(IsInGameThread(), TEXT("FLatentInfo::NextId() should only be called on the Game Thread."));
		if (LatentId == TNumericLimits<uint16>::Max())
		{
			LatentId = 1000;
		}

		return LatentId++;
	}
};