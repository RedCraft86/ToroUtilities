// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#if WITH_EDITOR
#include "Components/Editor/EdIconComponent.h"
#endif
#include "GameFramework/Volume.h"
#include "Interfaces/IEnabledState.h"
#include "Interfaces/IObjectIdentity.h"
#include "ToroVolume.generated.h"

/**
 * Specialized AVolume base providing identity and state tracking.
 * Features:
 * - Persistent Unique Identity (GUID).
 * - Unified Toggle logic (Enable/Disable).
 * - Editor-only ticking capabilities for procedural setup.
 * - Editor-only visualization options for shading and coloring the volume bounds.
 */
UCLASS(Abstract, NotBlueprintable, BlueprintType, PrioritizeCategories = (Settings, Tools), meta = (ChildCanTick = true))
class TOROCORE_API AToroVolume : public AVolume, public IObjectIdentity, public IEnabledState
{
	GENERATED_BODY()

public:

	AToroVolume();

	/** Event broadcasted to Blueprints when the Enabled state is toggled. */
	UPROPERTY(BlueprintAssignable, DisplayName = "On Enable State Changed")
		FEnableStateDelegateBP OnEnableStateChangedBP;

	/** Overridable Blueprint event for handling custom logic when enabled/disabled. */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Enable State Changed")
		void EnableStateChangedBP(const bool bState);

	FEnableStateDelegate OnEnableStateChanged;

	virtual FGameObjectId GetGameObjectId_Implementation() const override { return UniqueId; }
	virtual bool GetEnabledState_Implementation() const override { return bEnabled; }
	virtual void SetEnabledState_Implementation(const bool bInEnabled) override;

protected:

	/** Whether this actor is currently "Active" in the world. */
	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayPriority = -100));
		bool bEnabled;

	/** 
	 * Unique Identifier for save systems and cross-actor referencing.
	 * Marked as Transient for duplication/export to ensure IDs remain unique.
	 */
	UPROPERTY(EditAnywhere, Category = Settings, NonPIEDuplicateTransient, TextExportTransient, NonTransactional, meta = (DisplayPriority = -100))
		FGameObjectId UniqueId;

#if WITH_EDITORONLY_DATA
	/** If true, the Tick function will run inside the Level Editor viewport. */
	UPROPERTY(EditDefaultsOnly, Category = Tick)
		bool bTickInEditor;

	/** Toggle for the editor icon visibility. */
	UPROPERTY(EditAnywhere, Category = Actor)
		bool bDisplayIcon;

	/** If true, the volume will be drawn as a solid/shaded box in the editor. */
	UPROPERTY(EditAnywhere, Category = "Actor|Volume", DisplayName = "Shaded")
		bool bShadedVolume;

	/** Enables custom color overrides for the volume wireframe. */
	UPROPERTY(EditAnywhere, Category = "Actor|Volume", DisplayName = "Colored", meta = (InlineEditConditionToggle))
		bool bColoredVolume;

	/** The color used for the volume bounds if bColoredVolume is true. */
	UPROPERTY(EditAnywhere, Category = "Actor|Volume", DisplayName = "Color", meta = (EditCondition = "bColoredVolume", HideAlphaChannel = true))
		FColor VolumeColor;
#endif

#if WITH_EDITOR
	DECLARE_ICON_COMPONENT
#endif

	void BroadcastStateChanged();
	virtual void EnableStateChanged(const bool bState);

	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual bool ShouldTickIfViewportsOnly() const override { return bTickInEditor; }
#endif
};
