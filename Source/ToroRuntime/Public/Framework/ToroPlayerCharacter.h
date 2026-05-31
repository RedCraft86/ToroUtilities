// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Helpers/WorldGetter.h"
#include "Actors/ToroCharacter.h"
#include "DataTypes/LightProperties.h"
#include "GameFramework/WorldSettings.h"
#include "ToroPlayerCharacter.generated.h"

UCLASS(Blueprintable, BlueprintType, PrioritizeCategories = (Settings))
class TORORUNTIME_API AToroPlayerCharacter : public AToroCharacter
{
	GENERATED_BODY()

public:

	AToroPlayerCharacter();

	template<typename T = AToroPlayerCharacter>
	[[nodiscard]] static T* Get(const UObject* ContextObject)
	{
		const UWorld* World = FWorldGetter::Get(ContextObject);
		const APlayerController* PC = IsValid(World) ? World->GetFirstPlayerController() : nullptr;
		return IsValid(PC) ? PC->GetPawn<T>() : nullptr;
	}

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void SetLightSettings(const FPointLightProperties& InSettings);

	UFUNCTION(BlueprintPure, Category = Player)
		float GetSpeed() const { return GetVelocity().Size2D(); }

	UFUNCTION(BlueprintPure, Category = Player)
		bool IsMoving() const { return GetSpeed() > 50.0f; }

	UFUNCTION(BlueprintPure, Category = Player)
		bool IsPaused() const { return GetWorldSettings()->GetPauserPlayerState() != nullptr; }

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void PlayFootstep(USoundBase* InSound, float InVolume = 1.0f);

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual bool GetFloorSurface(TEnumAsByte<EPhysicalSurface>& Surface, const ECollisionChannel TraceChannel);

	template <typename T = class AToroGameMode>
	T* GetGameMode() const { return GetWorld()->GetAuthGameMode<T>(); }

	template <typename T = class AToroGameState>
	T* GetGameState() const { return GetWorld()->GetGameState<T>(); }

	virtual void Teleport(const FVector& InLocation, const FRotator& InRotation) override;

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UPointLightComponent> AmbientLight;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UAudioComponent> PlayerFootstep;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Tick, meta = (ClampMin = 0.05f, UIMin = 0.05f))
		float SlowTickInterval;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, AdvancedDisplay)
		FPointLightProperties LightSettings;

	float SlowTickTime;

	void FixFootstepPosition() const;

	virtual void SlowTick();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
