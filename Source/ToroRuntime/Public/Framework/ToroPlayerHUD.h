// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "GameFramework/HUD.h"
#include "Helpers/WorldGetter.h"
#include "GameFramework/PlayerController.h"
#include "UserWidgets/ToroMasterWidget.h"
#include "ToroPlayerHUD.generated.h"

/**
 * Manages the lifetime and visibility of the UToroMasterWidget.
 */
UCLASS(NotPlaceable, Blueprintable, BlueprintType, PrioritizeCategories = (Settings), meta = (ChildCanTick = true))
class TORORUNTIME_API AToroPlayerHUD : public AHUD
{
	GENERATED_BODY()

public:

	AToroPlayerHUD();

	template<typename T = AToroPlayerHUD>
	[[nodiscard]] static T* Get(const UObject* ContextObject)
	{
		static_assert(TIsDerivedFrom<T, AToroPlayerHUD>::Value, 
			"T must derive from AToroPlayerHUD");

		const UWorld* World = FWorldGetter::Get(ContextObject);
		const APlayerController* PC = IsValid(World) ? World->GetFirstPlayerController() : nullptr;
		return IsValid(PC) ? PC->GetHUD<T>() : nullptr;
	}

	/** Pushes the Settings Widget to the master stack. */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = HUD)
		void PushSettings() const;

	/** Returns the master UI widget container. */
	UFUNCTION(BlueprintPure, Category = HUD)
		UToroMasterWidget* GetMasterWidget() { return MasterWidget; }

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(Transient)
		TObjectPtr<UToroMasterWidget> MasterWidget;

	virtual void BeginPlay() override;
};
