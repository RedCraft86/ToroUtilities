// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InteractionData.h"
#include "UObject/Interface.h"
#include "Framework/ToroPlayerCharacter.h"
#include "InteractionInterface.generated.h"

UINTERFACE()
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class TORORUNTIME_API IInteractionInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = Interaction, DisplayName = "Begin Interact")
		void OnBeginInteract(AToroPlayerCharacter* Player, const FHitResult& Hit);
	virtual void OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& Hit) {}

	UFUNCTION(BlueprintNativeEvent, Category = Interaction, DisplayName = "End Interact")
		void OnEndInteract(AToroPlayerCharacter* Player);
	virtual void OnEndInteract_Implementation(AToroPlayerCharacter* Player) {}

	UFUNCTION(BlueprintNativeEvent, Category = Interaction, DisplayName = "Pawn Interact")
		void OnPawnInteract(APawn* Pawn, const FHitResult& Hit);
	virtual void OnPawnInteract_Implementation(APawn* Pawn, const FHitResult& Hit) {}

	UFUNCTION(BlueprintNativeEvent, Category = Interaction)
		void SetMarkerState(const bool bVisible);
	virtual void SetMarkerState_Implementation(const bool bVisible) {}

	UFUNCTION(BlueprintNativeEvent, Category = Interaction)
		bool GetInteractInfo(const FHitResult& Hit, FInteractionInfo& Info);
	virtual bool GetInteractInfo_Implementation(const FHitResult& Hit, FInteractionInfo& Info)
	{
		return false;
	}

	static bool ImplementedBy(const UObject* Target) 
	{ 
		return IsValid(Target) && Target->Implements<UInteractionInterface>(); 
	}

	static void BeginInteract(UObject* Target, AToroPlayerCharacter* Player, const FHitResult& Hit)
	{
		if (ImplementedBy(Target)) Execute_OnBeginInteract(Target, Player, Hit);
	}

	static void EndInteract(UObject* Target, AToroPlayerCharacter* Player)
	{
		if (ImplementedBy(Target)) Execute_OnEndInteract(Target, Player);
	}

	static void PawnInteract(UObject* Target, APawn* Pawn, const FHitResult& Hit)
	{
		if (ImplementedBy(Target)) Execute_OnPawnInteract(Target, Pawn, Hit);
	}

	static void SetMarkerState(UObject* Target, const bool bState)
	{
		if (ImplementedBy(Target)) Execute_SetMarkerState(Target, bState);
	}

	static bool GetInteractInfo(UObject* Target, const FHitResult& Hit, FInteractionInfo& Info)
	{
		return ImplementedBy(Target) && Execute_GetInteractInfo(Target, Hit, Info);
	}
};