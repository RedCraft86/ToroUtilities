// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "WorldActions/Native/SoundActions.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void UWorldAction_Sound2D::OnExecute_Implementation()
{
	if (USoundBase* SoundPtr = Sound.LoadSynchronous())
	{
		UGameplayStatics::PlaySound2D(this, SoundPtr, Volume, Pitch, StartTime);
	}
}

void UWorldAction_Sound3D::OnExecute_Implementation()
{
	const AAmbientSound* ActorPtr = SoundActor.LoadSynchronous();
	if (UAudioComponent* AudioComp = ActorPtr ? ActorPtr->GetAudioComponent() : nullptr)
	{
		if (Action == EAudioComponentAction::Play)
		{
			if (FadeTime >= 0.1f)
			{
				AudioComp->FadeIn(FadeTime, StartTime);
			}
			else
			{
				AudioComp->Play(StartTime);
			}
		}
		else if (Action == EAudioComponentAction::Stop)
		{
			if (FadeTime >= 0.1f)
			{
				AudioComp->FadeOut(FadeTime, 0.0f);
			}
			else
			{
				AudioComp->Stop();
			}
		}
		else // Action == EAudioComponentAction::Pause || Action == EAudioComponentAction::Unpause
		{
			AudioComp->SetPaused(Action == EAudioComponentAction::Pause);
		}
	}
}