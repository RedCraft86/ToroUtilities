// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "Libraries/ToroActorLibrary.h"
#include "LevelSequencePlayer.h"

void UToroActorLibrary::GetBoundingBoxVertices(TSet<FVector>& OutVerts, FVector& Origin, FVector& Extent, 
	const AActor* Target, const FVector Scale, const bool bOnlyColliding, const bool bChildActors)
{
	OutVerts.Empty(8);
	Origin = FVector::ZeroVector;
	Extent = FVector::ZeroVector;

	if (IsValid(Target))
	{
		Target->GetActorBounds(bOnlyColliding, Origin, Extent, bChildActors);

		const FVector Multipliers[8] {
			FVector(0, 0, 0),
			FVector(1, 0, 0), 
			FVector(0, 1, 0), 
			FVector(0, 0, 1),
			FVector(1, 1, 0), 
			FVector(0, 1, 1), 
			FVector(1, 0, 1), 
			FVector(1, 1, 1)
		};

		for (int i = 0; i < 8; i++)
		{
			OutVerts.Add(Origin + (Multipliers[i] * 2 - 1) * Extent * Scale);
		}
	}
}

void UToroActorLibrary::AddActorTag(AActor* Target, const FName InTag)
{
	if (IsValid(Target))
	{
		Target->Tags.AddUnique(InTag);
	}
}

void UToroActorLibrary::RemoveActorTag(AActor* Target, const FName InTag)
{
	if (IsValid(Target))
	{
		Target->Tags.Remove(InTag);
	}
}

void UToroActorLibrary::PlayLevelSequence(const ALevelSequenceActor* Target, const float PlayRate)
{
	if (ULevelSequencePlayer* SequencePlayer = Target ? Target->GetSequencePlayer() : nullptr)
	{
		SequencePlayer->SetPlayRate(PlayRate);
		SequencePlayer->Play();
	}
}

void UToroActorLibrary::ReverseLevelSequence(const ALevelSequenceActor* Target, const float PlayRate)
{
	if (ULevelSequencePlayer* SequencePlayer = Target ? Target->GetSequencePlayer() : nullptr)
	{
		SequencePlayer->SetPlayRate(PlayRate);
		SequencePlayer->PlayReverse();
	}
}

void UToroActorLibrary::StopLevelSequence(const ALevelSequenceActor* Target, const ESequenceStopType StopType)
{
	if (ULevelSequencePlayer* SequencePlayer = Target ? Target->GetSequencePlayer() : nullptr)
	{
		switch (StopType)
		{
		case ESequenceStopType::Default:
			SequencePlayer->Stop();
			break;

		case ESequenceStopType::AtCurrentTime:
			SequencePlayer->StopAtCurrentTime();
			break;

		case ESequenceStopType::SkipToEnd:
			SequencePlayer->GoToEndAndStop();
			break;
		}
	}
}

void UToroActorLibrary::PlayAmbientSound(const AAmbientSound* Target, float StartTime)
{
	if (Target && Target->GetAudioComponent())
	{
		Target->GetAudioComponent()->Play(StartTime);
	}
}

void UToroActorLibrary::StopAmbientSound(const AAmbientSound* Target)
{
	if (Target && Target->GetAudioComponent())
	{
		Target->GetAudioComponent()->Stop();
	}
}

void UToroActorLibrary::FadeInAmbientSound(const AAmbientSound* Target, float Duration, 
	float TargetLevel, float StartTime, const EAudioFaderCurve FadeCurve)
{
	if (Target && Target->GetAudioComponent())
	{
		Target->GetAudioComponent()->FadeIn(Duration, TargetLevel, StartTime, FadeCurve);
	}
}

void UToroActorLibrary::FadeOutAmbientSound(const AAmbientSound* Target, float Duration, 
	float TargetLevel, const EAudioFaderCurve FadeCurve)
{
	if (Target && Target->GetAudioComponent())
	{
		Target->GetAudioComponent()->FadeOut(Duration, TargetLevel, FadeCurve);
	}
}
