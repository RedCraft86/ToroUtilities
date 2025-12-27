// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

/**
 * Utility class for retrieving a reference to the current UWorld instance.
 * 
 * Found world is <b>cached</b> for efficient subsequent <c>FWorldGetter::Get()</c> calls.
 * Said cache is automatically cleared upon level/map change to refresh references.
 * The cache can also be explicitly cleared with <c>FWorldGetter::Reset()</c>.
 */
class TOROCORE_API FWorldGetter final
{
public:

	[[nodiscard]] static UWorld* Get(const UObject* Context = nullptr)
	{
		if (CachedWorld.IsValid())
		{
			return CachedWorld.Get();
		}

		if (GEngine)
		{
			CachedWorld = GEngine->GetWorldFromContextObject(Context, EGetWorldErrorMode::ReturnNull);
			if (!CachedWorld.IsValid())
			{
				CachedWorld = GEngine->GetCurrentPlayWorld();
			}
		}
		else
		{
			CachedWorld = GWorld.GetReference();
		}

		return CachedWorld.Get();
	}

	static void SetWorld(UWorld* InWorld)
	{
		if (InWorld) CachedWorld = InWorld;
	}

	static void Reset()
	{
		CachedWorld.Reset();
	}

private:
	
	inline static TWeakObjectPtr<UWorld> CachedWorld = nullptr;
};