// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroCharacter.h"
#include "DataTypes/LightingData.h"
#include "Helpers/GameplayTagMacros.h"
#include "Helpers/ClassGetterMacros.h"
#include "UserSettings/ToroUserSettings.h"
#include "ToroPlayerCharacter.generated.h"

class AToroGameMode;
class AToroGameState;
class AToroPlayerController;

namespace PlayerLockTags
{
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(PlayerLock)
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(Loading)
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(Gameplay)

	TORORUNTIME_API CREATE_TAG_VERIFIER(PlayerLock)
}

UCLASS(Abstract)
class TORORUNTIME_API AToroPlayerCharacter : public AToroCharacter
{
	GENERATED_BODY()

	friend class AToroPlayerController;

public:

	AToroPlayerCharacter();

	PLAYER_CLASS_GETTER(AToroPlayerCharacter, GetPlayerPawn);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UPointLightComponent> PlayerLight;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UAudioComponent> FootstepAudio;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class USpringArmComponent> CameraArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UCameraComponent> PlayerCamera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class USceneComponent> EquipmentRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UInteractionManager> Interaction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Tick, meta = (ClampMin = 0.05f, UIMin = 0.05f))
		float SlowTickInterval;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (Categories = "PlayerLock"))
		FGameplayTagContainer LockTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, AdvancedDisplay)
		FPointLightProperties LightSettings;

	UFUNCTION(BlueprintPure, Category = Player)
		float GetSpeed() const { return GetVelocity().Size2D(); }
	
	UFUNCTION(BlueprintPure, Category = Player)
		bool IsMoving() const { return GetSpeed() > 50.0f; }
	
	UFUNCTION(BlueprintPure, Category = Player)
		bool IsPaused() const { return GetWorldSettings()->GetPauserPlayerState() != nullptr; }

	UFUNCTION(BlueprintPure, Category = Player)
		virtual bool IsKillLocked();

	UFUNCTION(BlueprintPure, Category = Player)
		virtual bool IsControlLocked();

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void AddLockTag(UPARAM(meta = (Categories = "PlayerLock")) const FGameplayTag InTag);

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void RemoveLockTag(UPARAM(meta = (Categories = "PlayerLock")) const FGameplayTag InTag);

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void ClearLockTags();

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void SetLockOnTarget(UObject* InTarget);

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void ClearLockOnTarget();

	UFUNCTION(BlueprintPure, Category = Player)
		UObject* GetLockOnTarget() const { return LockOnTarget.Get(); }

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void SetLightSettings(const FPointLightProperties& InSettings);

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void PlayFootstep(USoundBase* InSound);

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual bool GetStandingSurface(TEnumAsByte<EPhysicalSurface>& Surface, const ECollisionChannel TraceChannel);

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Pawn)
		void SetControlRotation(const FRotator& InRotator) const;

	template <typename T = AToroPlayerController>
	T* GetPlayerController() const { return GetController<T>(); }

	template <typename T = AToroGameMode>
	T* GetGameMode() const { return GetWorld()->GetAuthGameMode<T>(); }

	template <typename T = AToroGameState>
	T* GetGameState() const { return GetWorld()->GetGameState<T>(); }

	virtual bool GetViewTarget_Implementation(FVector& Location) const override;
	virtual void GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) const override;
	virtual void Teleport(const FVector& InLocation, const FRotator& InRotation) override;

protected:

	float SlowTickTime;
	TWeakObjectPtr<UObject> LockOnTarget;

	virtual void OnCinematic(AActor* InActor) {}
	virtual void OnSettingsUpdate(const ESettingApplyType ApplyType);
	virtual float GetCapsuleCameraOffset() const;

	virtual void SlowTick() {}
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
