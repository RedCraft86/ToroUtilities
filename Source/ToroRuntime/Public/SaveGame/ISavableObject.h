// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "UObject/Interface.h"
#include "ISavableObject.generated.h"

/**
 * Interface to notify objects of save/load operations. (UObject Wrapper)
 */
UINTERFACE()
class USavableObject : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface to notify objects of save/load operations.
 */
class TORORUNTIME_API ISavableObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = SaveGame, DisplayName = "Pre-Save")
		void OnPreSave();
	virtual void OnPreSave_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, Category = SaveGame, DisplayName = "Post-Save")
		void OnPostSave();
	virtual void OnPostSave_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, Category = SaveGame, DisplayName = "Pre-Load")
		void OnPreLoad();
	virtual void OnPreLoad_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, Category = SaveGame, DisplayName = "Post-Load")
		void OnPostLoad();
	virtual void OnPostLoad_Implementation() = 0;

	static void NotifyPreSave(UObject* Target)
	{
		if (Target && Target->Implements<USavableObject>())
		{
			Execute_OnPreSave(Target);
		}
	}

	static void NotifyPostSave(UObject* Target)
	{
		if (Target && Target->Implements<USavableObject>())
		{
			Execute_OnPostSave(Target);
		}
	}

	static void NotifyPreLoad(UObject* Target)
	{
		if (Target && Target->Implements<USavableObject>())
		{
			Execute_OnPreLoad(Target);
		}
	}

	static void NotifyPostLoad(UObject* Target)
	{
		if (Target && Target->Implements<USavableObject>())
		{
			Execute_OnPostLoad(Target);
		}
	}
};