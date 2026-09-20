// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "Libraries/ToroSequenceLibrary.h"
#include "MovieSceneSequencePlayer.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"

void UToroSequenceLibrary::PlayLevelSequence(const ALevelSequenceActor* Target, const float PlayRate)
{
	if (ULevelSequencePlayer* SequencePlayer = IsValid(Target) ? Target->GetSequencePlayer() : nullptr)
	{
		SequencePlayer->SetPlayRate(PlayRate);
		SequencePlayer->Play();
	}
}

void UToroSequenceLibrary::ReverseLevelSequence(const ALevelSequenceActor* Target, const float PlayRate)
{
	if (ULevelSequencePlayer* SequencePlayer = IsValid(Target) ? Target->GetSequencePlayer() : nullptr)
	{
		SequencePlayer->SetPlayRate(PlayRate);
		SequencePlayer->PlayReverse();
	}
}

void UToroSequenceLibrary::StopLevelSequence(const ALevelSequenceActor* Target, const EToroSequenceStopType StopType)
{
	if (ULevelSequencePlayer* SequencePlayer = IsValid(Target) ? Target->GetSequencePlayer() : nullptr)
	{
		switch (StopType)
		{
		case EToroSequenceStopType::Default:
			SequencePlayer->Stop();
			break;

		case EToroSequenceStopType::Reset:
			SequencePlayer->Pause();
			SequencePlayer->SetPlaybackPosition(FMovieSceneSequencePlaybackParams(
				SequencePlayer->GetStartTime().Time, EUpdatePositionMethod::Jump
			));
			SequencePlayer->Stop();
			break;

		case EToroSequenceStopType::CurrentTime:
			SequencePlayer->StopAtCurrentTime();
			break;

		case EToroSequenceStopType::SkipToEnd:
			SequencePlayer->GoToEndAndStop();
			break;
		}
	}
}
