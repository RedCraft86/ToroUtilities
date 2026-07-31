// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "Engine/LatentActionManager.h"

/**
 * A static utility class for creating FLatentActionInfo structs.
 * 
 * <b>Usage Example:</b>
 * <pre>
 *     FLatentActionInfo Info = FLatentMaker::Make(GET_MEMBER_NAME_CHECKED(UMyClass, AfterDelay), this);
 *     UKismetSystemLibrary::Delay(GetWorld(), 2.0f, Info);
 * </pre>
 */
class FLatentInfo
{
	static inline uint16 LatentId = 1000; 

public:
	
	/**
	 * Creates a fully initialized FLatentActionInfo struct.
	 * @param FuncName The name of the function to execute once the latent action completes.
	 * @param Target   The object on which to call the completion function.
	 * @return A unique FLatentActionInfo configured for the current execution context.
	 * @note This method is not thread-safe. Latent actions should generally be initialized on the Game Thread.
	 */
	[[nodiscard]] static FLatentActionInfo Make(const TCHAR* FuncName = TEXT(""), UObject* Target = nullptr)
	{
		checkf(IsInGameThread(), TEXT("FLatentInfo::Make(...) should only be called on the Game Thread."))
		return FLatentActionInfo(0, NextId(), FuncName, Target);
	}

	/**
	 * Increments and returns the next available LinkID. 
	 * Automatically wraps back to 1000 if the <c>UINT16_MAX</c> limit 
	 * is reached to maintain a consistent range of safe IDs that avoid potential collisions with BP code.
	 * @return A unique unsigned 16-bit integer ID.
	 * @note This method is not thread-safe. Latent actions should generally be initialized on the Game Thread.
	 */
	[[nodiscard]] static uint16 NextId()
	{
		checkf(IsInGameThread(), TEXT("FLatentInfo::NextId() should only be called on the Game Thread."))
		if (LatentId == UINT16_MAX)
		{
			LatentId = 1000;
		}
		return LatentId++;
	}
};