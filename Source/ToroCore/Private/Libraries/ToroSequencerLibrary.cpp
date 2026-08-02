// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "Libraries/ToroSequencerLibrary.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"

void UToroSequencerLibrary::PlayLevelSequence(const ALevelSequenceActor* Target, const float PlayRate)
{
	if (ULevelSequencePlayer* SequencePlayer = Target ? Target->GetSequencePlayer() : nullptr)
	{
		SequencePlayer->SetPlayRate(PlayRate);
		SequencePlayer->Play();
	}
}

void UToroSequencerLibrary::ReverseLevelSequence(const ALevelSequenceActor* Target, const float PlayRate)
{
	if (ULevelSequencePlayer* SequencePlayer = Target ? Target->GetSequencePlayer() : nullptr)
	{
		SequencePlayer->SetPlayRate(PlayRate);
		SequencePlayer->PlayReverse();
	}
}

void UToroSequencerLibrary::StopLevelSequence(const ALevelSequenceActor* Target, const EToroSequenceStopType StopType)
{
	if (ULevelSequencePlayer* SequencePlayer = Target ? Target->GetSequencePlayer() : nullptr)
	{
		switch (StopType)
		{
			case EToroSequenceStopType::Default:
				SequencePlayer->Stop();
			break;

			case EToroSequenceStopType::Reset:
				SequencePlayer->StopAtCurrentTime();
				SequencePlayer->SetPlaybackPosition(FMovieSceneSequencePlaybackParams(
					SequencePlayer->GetStartTime().Time, EUpdatePositionMethod::Jump
				));
			break;

			case EToroSequenceStopType::CurrentTime:
				SequencePlayer->StopAtCurrentTime();
			break;

			case EToroSequenceStopType::SkipToEnd:
				SequencePlayer->StopAtCurrentTime();
				SequencePlayer->SetPlaybackPosition(FMovieSceneSequencePlaybackParams(
					SequencePlayer->GetEndTime().Time, EUpdatePositionMethod::Scrub
				));
			break;
		}
	}
}
