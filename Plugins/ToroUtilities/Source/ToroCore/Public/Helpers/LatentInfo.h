// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

namespace LatentInfo
{
	// Start at an offset in case we have other startup tasks on lower IDs
	inline uint16 LatentID = 1000;

	inline uint16 GetID() { return LatentID++; }

	inline FLatentActionInfo Make()
	{
		return FLatentActionInfo(0, GetID(), TEXT(""), nullptr);
	}

	inline FLatentActionInfo Make(const TCHAR* FuncName, UObject* Target)
	{
		return FLatentActionInfo(0, GetID(), FuncName, Target);
	}
}