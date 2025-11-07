// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "TutorialDatabase.h"
#include "TutorialWidget.h"
#include "Components/ActorComponent.h"
#include "Framework/ToroPlayerState.h"
#include "Helpers/ClassGetterMacros.h"
#include "SaveSystem/ToroSaveManager.h"
#include "TutorialManager.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UTutorialManager final : public UActorComponent
{
	GENERATED_BODY()

public:

	UTutorialManager() {}
	
	PLAYER_COMPONENT_GETTER(UTutorialManager, AToroPlayerState, Tutorials)

	UFUNCTION(BlueprintCallable, Category = Statics, meta = (WorldContext = "ContextObject"))
	static UTutorialManager* GetTutorialManager(const UObject* ContextObject, const int32 PlayerIdx = 0)
	{
		return Get(ContextObject, PlayerIdx);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Tutorials)
		void QueueTutorial(UPARAM(meta = (Categories = "Tutorial")) const FGameplayTag Key);

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Tutorials)
		void QueueTutorials(UPARAM(meta = (Categories = "Tutorial")) const TArray<FGameplayTag>& Keys);

	UFUNCTION(BlueprintPure, Category = Tutorials)
		bool IsShowingTutorial() const;

	void MarkTutorialsSeen();
	const FTutorialEntry* GetTutorialData(const FGameplayTag& Key) const;
	const TArray<FGameplayTag>& GetTutorials() const { return Tutorials; }
	
private:

	UPROPERTY(Transient)
		TObjectPtr<UTutorialDatabase> Database;

	TArray<FGameplayTag> Tutorials;
	TObjectPtr<UToroSaveManager> SaveManager;

	TObjectPtr<UTutorialWidget> TutorialWidget;
	UTutorialWidget* GetTutorialWidget();

	void QueueInternal(const FGameplayTag& Key);
	virtual void BeginPlay() override;
};
