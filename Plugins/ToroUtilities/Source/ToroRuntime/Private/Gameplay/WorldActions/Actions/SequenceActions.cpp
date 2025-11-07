// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/WorldActions/Actions/SequenceActions.h"
#include "LevelSequencePlayer.h"

void FWTSequence_Basic::RunAction()
{
	for (TSoftObjectPtr<ALevelSequenceActor>& Actor : Targets)
	{
		const ALevelSequenceActor* ActorPtr = Actor.LoadSynchronous();
		if (ULevelSequencePlayer* Player = ActorPtr ? ActorPtr->GetSequencePlayer() : nullptr)
		{
			if (bStopPlaying)
			{
				Player->GoToEndAndStop();
			}
			else if (!FMath::IsNearlyZero(PlayRate, 0.05f))
			{
				Player->SetPlayRate(FMath::Abs(PlayRate));
				PlayRate < 0.0f ? Player->PlayReverse() : Player->Play();
			}
		}
	}
}

void FWTSequence_Basic::OnBeginPlay()
{
	for (auto It = Targets.CreateIterator(); It; ++It)
	{
		if (It->IsNull()) It.RemoveCurrent();
	}
}

void FWTSequence_Cutscene::RunAction()
{
	if (AToroCutsceneActor* Actor = Target.LoadSynchronous())
	{
		FMath::IsNearlyZero(FadeTime, 0.1f)
			? Actor->PlayCutscene() : Actor->PlayAfterFade(FadeTime);
	}
}
