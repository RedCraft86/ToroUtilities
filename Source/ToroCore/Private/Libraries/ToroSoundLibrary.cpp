// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "Libraries/ToroSoundLibrary.h"
#include "Sound/AmbientSound.h"

void UToroSoundLibrary::PlayAmbientSound(const AAmbientSound* Target, const float StartTime)
{
	if (UAudioComponent* Audio = Target ? Target->GetAudioComponent() : nullptr)
	{
		Audio->Play(StartTime);
	}
}

void UToroSoundLibrary::StopAmbientSound(const AAmbientSound* Target, const float Delay)
{
	if (UAudioComponent* Audio = Target ? Target->GetAudioComponent() : nullptr)
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

void UToroSoundLibrary::FadeInAmbientSound(const AAmbientSound* Target, const float Duration,
	const float TargetLevel, const float StartTime, const EAudioFaderCurve FadeCurve)
{
	if (UAudioComponent* Audio = Target ? Target->GetAudioComponent() : nullptr)
	{
		Audio->FadeIn(Duration, TargetLevel, StartTime, FadeCurve);
	}
}

void UToroSoundLibrary::FadeOutAmbientSound(const AAmbientSound* Target, const float Duration,
	const float TargetLevel, const EAudioFaderCurve FadeCurve)
{
	if (UAudioComponent* Audio = Target ? Target->GetAudioComponent() : nullptr)
	{
		Audio->FadeOut(Duration, TargetLevel, FadeCurve);
	}
}
