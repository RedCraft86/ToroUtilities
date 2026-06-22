// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "WorldActions/Native/MiscActions.h"
#include "Framework/ToroGameInstance.h"
#include "Libraries/ToroWorldLibrary.h"
#include "AsyncGameplayMessageSystem.h"
#include "AsyncMessageWorldSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Asset/FlowAssetParams.h"
#include "LevelSequencePlayer.h"
#include "Helpers/WorldGetter.h"
#include "Helpers/LatentInfo.h"
#include "FlowSubsystem.h"

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
		if (bReverse)
		{
			if (bRestart)
			{
				SequencePlayer->GoToEndAndStop();
			}

			SequencePlayer->Play();
		}
		else
		{
			if (bRestart)
			{
				SequencePlayer->RewindForReplay();
			}

			SequencePlayer->Play();
		}
	}
}

void UWorldAction_RemoteEvent::OnExecute_Implementation()
{
	UToroWorldLibrary::CallRemoteEvent(this, EventName);
}

void UWorldAction_AsyncMessage::OnExecute_Implementation()
{
	if (MessageId.IsValid())
	{
		const TSharedPtr<FAsyncGameplayMessageSystem> System = UAsyncMessageWorldSubsystem::
			GetSharedMessageSystem<FAsyncGameplayMessageSystem>(FWorldGetter::Get(WorldContext.Get()));
		if (System.IsValid())
		{
			System->QueueMessageForBroadcast(MessageId, Payload);
		}
	}
}

void UWorldAction_RootFlow::OnExecute_Implementation()
{
	if (!FlowAsset)
	{
		return;
	}

	const UGameInstance* GI = UToroGameInstance::Get<UToroGameInstance>(WorldContext.Get());
	if (UFlowSubsystem* Subsystem = GI ? GI->GetSubsystem<UFlowSubsystem>() : nullptr)
	{
		Subsystem->StartRootFlow(WorldContext.IsValid() ? WorldContext.Get() : this, 
			FlowAsset, FlowParams.ResolveFlowAssetParams(), false);
	}
}
