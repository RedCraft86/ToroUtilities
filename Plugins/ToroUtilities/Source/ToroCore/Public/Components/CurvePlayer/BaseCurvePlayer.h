// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "BaseCurvePlayer.generated.h"

UENUM(BlueprintType)
enum class ECurvePlayerState : uint8
{
	Stopped,
	Forward,
	Reverse
};

UCLASS(NotBlueprintable, ClassGroup = (Animation), meta = (BlueprintSpawnableComponent))
class TOROCORE_API UBaseCurvePlayer : public UActorComponent
{
	GENERATED_BODY()

public:

	UBaseCurvePlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayPriority = -1))
		bool bLooping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 0.05f, UIMin = 0.05f, DisplayPriority = -1))
		float PlayRate;

	UFUNCTION(BlueprintCallable, Category = CurvePlayer)
		virtual void Stop(const bool bReset = false);

	UFUNCTION(BlueprintCallable, Category = CurvePlayer)
		virtual void Play(const float StartTime = 0.0f);

	UFUNCTION(BlueprintCallable, Category = CurvePlayer)
		virtual void Reverse(const bool bFromEnd = false);

	UFUNCTION(BlueprintCallable, Category = CurvePlayer)
		virtual void SetPlaybackTime(const float InTime);

	UFUNCTION(BlueprintCallable, Category = CurvePlayer)
		virtual void SnapToStart() { SetPlaybackTime(TimeRange.X); }

	UFUNCTION(BlueprintCallable, Category = CurvePlayer)
		virtual void SnapToEnd() { SetPlaybackTime(TimeRange.Y); }

	UFUNCTION(BlueprintPure, Category = CurvePlayer)
		virtual float GetTime() const { return CurrentTime; }

	UFUNCTION(BlueprintPure, Category = CurvePlayer)
		virtual ECurvePlayerState GetPlayState() const { return PlayState; }

	UFUNCTION(BlueprintPure, Category = CurvePlayer)
		virtual bool IsPlaying() const { return PlayState != ECurvePlayerState::Stopped; }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChangedBP, const ECurvePlayerState, NewState);
	UPROPERTY(BlueprintAssignable, DisplayName = "On State Changed")
		FOnStateChangedBP OnStateChangedBP;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnStateChanged, const ECurvePlayerState);
	FOnStateChanged OnStateChanged;

protected:

	float CurrentTime;
	FVector2D TimeRange;
	ECurvePlayerState PlayState;

	virtual void BroadcastState() const;
	virtual void BroadcastEval() const {}

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
};
