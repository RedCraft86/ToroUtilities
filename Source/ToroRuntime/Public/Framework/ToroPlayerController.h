// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Helpers/WorldGetter.h"
#include "DataTypes/GameInputConfig.h"
#include "GameFramework/PlayerController.h"
#include "ToroPlayerController.generated.h"

/**
 * The interface between the player and the pawn. Manages Input Configurations.
 */
UCLASS(NotPlaceable, Blueprintable, BlueprintType)
class TORORUNTIME_API AToroPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AToroPlayerController();

	template<typename T = AToroPlayerController>
	[[nodiscard]] static T* Get(const UObject* ContextObject)
	{
		static_assert(TIsDerivedFrom<T, AToroPlayerController>::Value, 
			"T must derive from AToroPlayerController");

		const UWorld* World = FWorldGetter::Get(ContextObject);
		return IsValid(World) ? World->GetFirstPlayerController<T>() : nullptr;
	}

	/** Updates the input routing and cursor behavior through a single struct containing all settings. */
	UFUNCTION(BlueprintCallable, Category = Input)
		virtual void SetInputConfig(const FGameInputConfig& InConfig);

	/** Returns the currently active input configuration. */
	UFUNCTION(BlueprintPure, Category = Input)
		const FGameInputConfig& GetInputConfig() const { return InputConfig; }

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	/** Cached input configuration last applied. */
	UPROPERTY(Transient)
		FGameInputConfig InputConfig;

	virtual void BeginPlay() override;
};
