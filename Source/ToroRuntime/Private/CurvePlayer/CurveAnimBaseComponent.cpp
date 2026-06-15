// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "CurvePlayer/CurveAnimBaseComponent.h"
#include "GameFramework/Actor.h"
#include "ToroRuntime.h"

UCurveAnimBaseComponent::UCurveAnimBaseComponent(): bLooping(false), PlayRate(1.0f)
	, CurrentTime(0.0f), TimeRange(FVector2D::ZeroVector), PlayState(ECurvePlayerState::Stopped)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UCurveAnimBaseComponent::StopPlaying(const bool bReset)
{
	if (PlayState != ECurvePlayerState::Stopped)
	{
		PlayState = ECurvePlayerState::Stopped;
		SetComponentTickEnabled(false);
		if (bReset)
		{
			SetPlaybackTime(TimeRange.X);
		}

		BroadcastState();
	}
}

void UCurveAnimBaseComponent::PlayForward(const bool bFromStart)
{
	if (!IsValidCurve())
	{
		StopPlaying(true);
	}
	else if (PlayState != ECurvePlayerState::Forward)
	{
		PlayState = ECurvePlayerState::Forward;
		BroadcastState();

		// If explicitly set OR if the animation cannot loop, but has already played
		if (bFromStart || (IsAtEnd() && !bLooping))
		{
			SetPlaybackTime(TimeRange.X);
		}

		SetComponentTickEnabled(true);
	}
}

void UCurveAnimBaseComponent::PlayReverse(const bool bFromEnd)
{
	if (!IsValidCurve())
	{
		StopPlaying(true);
	}
	else if (PlayState != ECurvePlayerState::Reverse)
	{
		PlayState = ECurvePlayerState::Reverse;
		BroadcastState();

		// If explicitly set OR if the animation cannot loop, but has already reversed
		if (bFromEnd || (IsAtStart() && !bLooping))
		{
			SetPlaybackTime(TimeRange.Y);
		}

		SetComponentTickEnabled(true);
	}
}

void UCurveAnimBaseComponent::SetPlaybackTime(const float InTime, const bool bNotify)
{
	if (!FMath::IsNearlyEqual(CurrentTime, InTime) && IsValidCurve())
	{
		CurrentTime = FMath::Clamp(InTime, TimeRange.X, TimeRange.Y);
		if (bNotify) BroadcastEval();
	}
}

void UCurveAnimBaseComponent::BroadcastState() const
{
	OnStateChanged.Broadcast(PlayState);
	OnStateChangedBP.Broadcast(PlayState);
}

void UCurveAnimBaseComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeCurve();
}

void UCurveAnimBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	if (PlayState == ECurvePlayerState::Stopped || !IsValidCurve())
	{
		SetComponentTickEnabled(false);
		return;
	}

	float Delta = PlayRate * DeltaTime;
	if (Delta >= (TimeRange.Y - TimeRange.X))
	{
		SetComponentTickEnabled(false);
		SetPlaybackTime(TimeRange.X);
		UE_LOG(LogToroRuntime, Warning, 
			TEXT("CurveAnim::Tick [%s--%s] Time Delta is greater than or equal to the length of the animation."),
			*GetName(), *GetNameSafe(GetOwner()))
		return;
	}

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
			StopPlaying(false);
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
			StopPlaying(false);
		}
	}
	else
	{
		CurrentTime = TargetTime;
	}

	BroadcastEval();
}
