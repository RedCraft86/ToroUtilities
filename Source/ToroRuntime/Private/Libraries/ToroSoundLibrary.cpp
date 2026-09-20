// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "Libraries/ToroSoundLibrary.h"
#include "Sound/AmbientSound.h"

void UToroSoundLibrary::PlayAmbientSound(const AAmbientSound* Target, const float StartTime)
{
	if (UAudioComponent* Audio = IsValid(Target) ? Target->GetAudioComponent() : nullptr)
	{
		Audio->Play(StartTime);
	}
}

void UToroSoundLibrary::StopAmbientSound(const AAmbientSound* Target, const float Delay)
{
	if (UAudioComponent* Audio = IsValid(Target) ? Target->GetAudioComponent() : nullptr)
	{
		if (Delay > UE_KINDA_SMALL_NUMBER)
		{
			Audio->StopDelayed(Delay);
		}
		else
		{
			Audio->Stop();
		}
	}
}

FVoidCoroutine UToroSoundLibrary::FadeInAmbientSound(FLatentActionInfo LatentInfo, const AAmbientSound* Target,
	const float Duration, const float TargetLevel, const float StartTime, const EAudioFaderCurve FadeCurve)
{
	if (UAudioComponent* Audio = IsValid(Target) ? Target->GetAudioComponent() : nullptr)
	{
		Audio->FadeIn(Duration, TargetLevel, StartTime, FadeCurve);
		co_await UE5Coro::Latent::Seconds(Duration);
	}

	co_return;
}

FVoidCoroutine UToroSoundLibrary::FadeOutAmbientSound(FLatentActionInfo LatentInfo, const AAmbientSound* Target,
	const float Duration, const float TargetLevel, const EAudioFaderCurve FadeCurve)
{
	if (UAudioComponent* Audio = IsValid(Target) ? Target->GetAudioComponent() : nullptr)
	{
		Audio->FadeOut(Duration, TargetLevel, FadeCurve);
		co_await UE5Coro::Latent::Seconds(Duration);
	}

	co_return;
}
