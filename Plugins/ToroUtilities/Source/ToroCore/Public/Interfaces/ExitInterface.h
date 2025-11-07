// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "ExitInterface.generated.h"

UINTERFACE()
class UExitInterface : public UInterface
{
	GENERATED_BODY()
};

class TOROCORE_API IExitInterface
{
	GENERATED_BODY()

public:

	/* Universal generic exit command */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = ExitInterface)
		void Exit();
	virtual void Exit_Implementation() {}

	/* Universal generic return command */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = ExitInterface)
		void ReturnToWidget(UUserWidget* FromWidget);
	virtual void ReturnToWidget_Implementation(UUserWidget* FromWidget) {}

	static bool ImplementedBy(const UObject* Target) 
	{ 
		return IsValid(Target) && Target->Implements<UExitInterface>(); 
	}

	static void Exit(UObject* Target)
	{
		if (ImplementedBy(Target)) Execute_Exit(Target);
	}

	static void ReturnToWidget(UObject* Target, UUserWidget* FromWidget)
	{
		if (ImplementedBy(Target)) Execute_ReturnToWidget(Target, FromWidget);
	}
};