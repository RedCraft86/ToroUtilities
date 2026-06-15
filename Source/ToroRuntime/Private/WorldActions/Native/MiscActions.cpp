// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "WorldActions/Native/MiscActions.h"
#include "Libraries/ToroWorldLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ToroRuntimeSubsystem.h"
#include "LevelSequencePlayer.h"
#include "Helpers/LatentInfo.h"
#include "ToroGlobals.h"

void UWorldAction_LevelStreaming::OnExecute_Implementation()
{
	for (const TPair<TSoftObjectPtr<UWorld>, bool>& Level : Load)
	{
		if (Level.Key.IsNull()) continue;
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, 
			Level.Key, Level.Value, false, FLatentInfo::Make());
	}
	for (const TSoftObjectPtr<UWorld>& Level : Unload)
	{
		if (Level.IsNull()) continue;
		UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this,
			Level, FLatentInfo::Make(), false);
	}
}

void UWorldAction_LevelSequence::OnExecute_Implementation()
{
	const ALevelSequenceActor* ActorPtr = Sequence.LoadSynchronous();
	if (ULevelSequencePlayer* SequencePlayer = ActorPtr ? ActorPtr->GetSequencePlayer() : nullptr)
	{
		SequencePlayer->RewindForReplay();
		SequencePlayer->Play();
	}
}

void UWorldAction_RemoteEvent::OnExecute_Implementation()
{
	UToroWorldLibrary::CallRemoteEvent(this, EventName);
}

void UWorldAction_GlobalMetadata::OnExecute_Implementation()
{
	if (ToroGameplayTags::Flag::IsValidTag(Key))
	{
		if (UToroRuntimeSubsystem* Subsystem = UToroRuntimeSubsystem::Get(this))
		{
			Subsystem->AddGlobalMetadata(Key, Value);
		}
	}
}

void UWorldAction_GlobalEvent::OnExecute_Implementation()
{
	if (ToroGameplayTags::Event::IsValidTag(Key))
	{
		AActor* InstigatorPtr = Instigator.Get();
		if (!InstigatorPtr)
		{
			InstigatorPtr = Cast<AActor>(WorldContext.Get());
		}

		if (UToroRuntimeSubsystem* Subsystem = UToroRuntimeSubsystem::Get(this))
		{
			Subsystem->InvokeGlobalEvent(Key, Payload, InstigatorPtr);
		}
	}
}