// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/WorldActions/Actions/SoundActions.h"
#include "Kismet/GameplayStatics.h"

void FWTSounds_Simple::RunAction()
{
	UGameplayStatics::PlaySound2D(GetWorldContext(), Sound, Volume, Pitch);
}

void FWTSounds_World::RunAction()
{
	for (TSoftObjectPtr<AAmbientSound>& Actor : Targets)
	{
		const AAmbientSound* ActorPtr = Actor.LoadSynchronous();
		if (UAudioComponent* AudioComp = ActorPtr ? ActorPtr->GetAudioComponent() : nullptr)
		{
			if (bFade)
			{
				bStopSound ? AudioComp->FadeOut(FadeTime, 0.0f, FadeCurve)
					: AudioComp->FadeIn(FadeTime, 1.0f, StartTime, FadeCurve);
			}
			else
			{
				bStopSound ? AudioComp->Stop() : AudioComp->Play(StartTime);
			}
		}
	}
}

void FWTSounds_World::OnBeginPlay()
{
	for (auto It = Targets.CreateIterator(); It; ++It)
	{
		if (It->IsNull()) It.RemoveCurrent();
	}
}
