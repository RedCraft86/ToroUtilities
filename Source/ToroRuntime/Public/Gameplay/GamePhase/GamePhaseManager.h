// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

// Fixes AudioMixerQuantizedCommands.h(31): error C2838: 'PlaySoundW': illegal qualified name in member declaration
// for whatever reason. It's actual magic.
//#include "Windows/WindowsHWrapper.h"

#include "GamePhaseData.h"
#include "LoadingScreenWidget.h"
#include "Framework/ToroGameState.h"
#include "Components/ActorComponent.h"
#include "Helpers/ClassGetterMacros.h"
#include "GamePhaseManager.generated.h"

namespace MenuThemeTags
{
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(MenuTheme)
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(Default)
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(Ending)
	TORORUNTIME_API CREATE_TAG_VERIFIER(MenuTheme)
}

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UGamePhaseManager final : public UActorComponent
{
	GENERATED_BODY()

public:

	UGamePhaseManager();
	
	GLOBAL_COMPONENT_GETTER(UGamePhaseManager, AToroGameState, GamePhase)

	UFUNCTION(BlueprintCallable, Category = Statics, meta = (WorldContext = "ContextObject"))
	static UGamePhaseManager* GetPhaseManager(const UObject* ContextObject)
	{
		return Get(ContextObject);
	}

	UFUNCTION(BlueprintCallable, Category = GamePhase)
		void LoadSequence();

	UFUNCTION(BlueprintCallable, Category = GamePhase)
		void StepSequence(const uint8 InIndex);

	UFUNCTION(BlueprintCallable, Category = GamePhase)
		void ChangePhase(UToroGamePhaseNode* NewPhase);

	UFUNCTION(BlueprintCallable, Category = GamePhase, meta = (AutoCreateRefTerm = "InSequence"))
		void SetSequence(const TArray<uint8>& InSequence);

	UFUNCTION(BlueprintPure, Category = GamePhase)
		bool IsLoading() const { return bLoading; }

private:

	bool bLoading;
	float PhaseTime;
	uint8 UnloadTasks;
	TArray<uint8> Sequence;
	TSet<TSoftObjectPtr<UWorld>> LoadLevels;
	TSet<TSoftObjectPtr<UWorld>> UnloadLevels;
	
	TObjectPtr<class UNarrativeManager> Narrative;
	TObjectPtr<class UInventoryManager> Inventory;
	TObjectPtr<class UToroSaveManager> SaveManager;
	TObjectPtr<class UWorldMusicManager> MusicManager;
	TObjectPtr<class AToroPlayerCharacter> PlayerChar;
	TObjectPtr<class AMasterPostProcess> PostProcessing;
	
	UPROPERTY(Transient) TObjectPtr<UToroGamePhaseGraph> Graph;
	UPROPERTY(Transient) TObjectPtr<UToroGamePhaseNode> ThisPhase;

	TObjectPtr<ULoadingScreenWidget> LoadingWidget;
	ULoadingScreenWidget* GetLoadingWidget();

	bool IsValidManager() const;
	void UnloadLevel(const TSoftObjectPtr<UWorld>& InLevel);
	void LoadLevel(const TSoftObjectPtr<UWorld>& InLevel);
	
	UFUNCTION() void OnMainLevelLoaded();
	UFUNCTION() void OnUnloadLevel();

	void FadeToBlack() const;
	void FadeFromBlack() const;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
