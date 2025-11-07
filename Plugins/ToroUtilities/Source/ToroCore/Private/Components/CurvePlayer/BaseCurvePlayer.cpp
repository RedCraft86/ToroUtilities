// Copyright (C) RedCraft86. All Rights Reserved.

#include "Components/CurvePlayer/BaseCurvePlayer.h"

UBaseCurvePlayer::UBaseCurvePlayer()
	: bLooping(false), PlayRate(1.0f), CurrentTime(0.0f), TimeRange(0.0f), PlayState(ECurvePlayerState::Stopped)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UBaseCurvePlayer::Stop(const bool bReset)
{
	PlayState = ECurvePlayerState::Stopped;
	if (bReset) CurrentTime = TimeRange.X;
	SetComponentTickEnabled(false);
	BroadcastEval();
	BroadcastState();
}

void UBaseCurvePlayer::Play(const float StartTime)
{
	PlayState = ECurvePlayerState::Forward;
	CurrentTime = FMath::Clamp(StartTime, TimeRange.X, TimeRange.Y);
	SetComponentTickEnabled(true);
	BroadcastState();
}

void UBaseCurvePlayer::Reverse(const bool bFromEnd)
{
	PlayState = ECurvePlayerState::Reverse;
	if (bFromEnd) CurrentTime = TimeRange.Y;
	SetComponentTickEnabled(true);
	BroadcastState();
}

void UBaseCurvePlayer::SetPlaybackTime(const float InTime)
{
	CurrentTime = FMath::Clamp(InTime, TimeRange.X, TimeRange.Y);
	BroadcastState();
}

void UBaseCurvePlayer::BroadcastState() const
{
	OnStateChanged.Broadcast(PlayState);
	OnStateChangedBP.Broadcast(PlayState);
}

void UBaseCurvePlayer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	if (PlayState == ECurvePlayerState::Stopped)
	{
		SetComponentTickEnabled(false);
		return;
	}

	float Delta = PlayRate * DeltaTime;
	if (PlayState == ECurvePlayerState::Reverse)
	{
		Delta *= -1.0f;
	}

	const float TargetTime = CurrentTime + Delta;
	if (TargetTime < TimeRange.X)
	{
		if (bLooping)
		{
			// Preserve the overflow when wrapping backwards
			const float Overflow = TimeRange.X - TargetTime;
			CurrentTime = TimeRange.Y - Overflow;
		}
		else
		{
			CurrentTime = TimeRange.X;
			Stop();
		}
	}
	else if (TargetTime > TimeRange.Y)
	{
		if (bLooping)
		{
			// Preserve the overflow when wrapping forwards
			const float Overflow = TargetTime - TimeRange.Y;
			CurrentTime = TimeRange.X + Overflow;
		}
		else
		{
			CurrentTime = TimeRange.Y;
			Stop();
		}
	}
	else
	{
		CurrentTime = TargetTime;
	}

	BroadcastEval();
}
