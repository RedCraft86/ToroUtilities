// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "ActorStateInterface.generated.h"

#define DECLARE_ENABLE_STATE_HELPERS \
	bool IsEnabled() const { return Execute_GetEnabledState(this); } \
	void SetEnabled(const bool bInEnabled) { Execute_SetEnabledState(this, bInEnabled); }

UINTERFACE()
class UActorStateInterface : public UInterface
{
	GENERATED_BODY()
};

class TOROCORE_API IActorStateInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = ActorState, DisplayName = "Set Enabled")
		void SetEnabledState(const bool bInEnabled);
	virtual void SetEnabledState_Implementation(const bool bInEnabled) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = ActorState, DisplayName = "Is Enabled")
		bool GetEnabledState() const;
	virtual bool GetEnabledState_Implementation() const { return false; }

	static bool ImplementedBy(const UObject* Target) 
	{ 
		return IsValid(Target) && Target->Implements<UActorStateInterface>(); 
	}

	static void SetEnabled(UObject* Target, const bool bInEnabled)
	{
		if (ImplementedBy(Target)) Execute_SetEnabledState(Target, bInEnabled);
	}

	static bool IsEnabled(const UObject* Target)
	{
		return ImplementedBy(Target) && Execute_GetEnabledState(Target);
	}
};