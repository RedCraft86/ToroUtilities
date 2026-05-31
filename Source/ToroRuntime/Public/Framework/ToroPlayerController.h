// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Helpers/WorldGetter.h"
#include "DataTypes/GameInputConfig.h"
#include "GameFramework/PlayerController.h"
#include "ToroPlayerController.generated.h"

UCLASS(NotPlaceable, Blueprintable, BlueprintType)
class TORORUNTIME_API AToroPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AToroPlayerController();

	template<typename T = AToroPlayerController>
	[[nodiscard]] static T* Get(const UObject* ContextObject)
	{
		const UWorld* World = FWorldGetter::Get(ContextObject);
		return IsValid(World) ? World->GetFirstPlayerController<T>() : nullptr;
	}

	UFUNCTION(BlueprintCallable, Category = Input)
		virtual void SetInputConfig(const FGameInputConfig& InConfig);

	UFUNCTION(BlueprintPure, Category = Input)
		const FGameInputConfig& GetInputConfig() const { return InputConfig; }

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(Transient)
		FGameInputConfig InputConfig;

	virtual void BeginPlay() override;
};
