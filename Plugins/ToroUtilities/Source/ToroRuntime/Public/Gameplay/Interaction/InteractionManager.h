// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InteractionData.h"
#include "InteractionWidget.h"
#include "Components/ActorComponent.h"
#include "Helpers/ClassGetterMacros.h"
#include "Framework/ToroPlayerCharacter.h"
#include "InteractionManager.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UInteractionManager : public UActorComponent
{
	GENERATED_BODY()

public:

	UInteractionManager();
	
	PLAYER_COMPONENT_GETTER(UInteractionManager, AToroPlayerCharacter, Interaction)

	UFUNCTION(BlueprintCallable, Category = Statics, meta = (WorldContext = "ContextObject"))
	static UInteractionManager* GetInteractionManager(const UObject* ContextObject, const int32 PlayerIdx = 0)
	{
		return Get(ContextObject, PlayerIdx);
	}

	UFUNCTION(BlueprintCallable, Category = Interaction)
		void SetEnabled(const bool bInEnabled);

	UFUNCTION(BlueprintPure, Category = Interaction)
		bool IsEnabled() const { return bEnabled; }

	UFUNCTION(BlueprintCallable, Category = Interaction)
		void SetInteracting(const bool bInInteracting);

	UFUNCTION(BlueprintPure, Category = Interaction)
		bool IsInteracting() const { return bInteracting; }

	DECLARE_DELEGATE_RetVal(FHitResult, FHandleTrace);
	FHandleTrace HandleTrace;

private:

	bool bEnabled;
	bool bInteracting;
	FInteractionCache InteractCache;
	TObjectPtr<AToroPlayerCharacter> PlayerChar;

	TObjectPtr<UInteractionWidget> InteractionWidget;
	UInteractionWidget* GetInteractionWidget();

	void CleanupCache();
	bool CanInteract() const { return bEnabled && PlayerChar && HandleTrace.IsBound(); }

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
};
