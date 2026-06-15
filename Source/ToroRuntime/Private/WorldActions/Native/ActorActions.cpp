// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "WorldActions/Native/ActorActions.h"
#include "Components/StaticMeshComponent.h"

void UWorldAction_Visibility::OnExecute_Implementation()
{
	for (TSoftObjectPtr<AActor>& SoftPtr : Targets)
	{
		if (AActor* ActorPtr = SoftPtr.LoadSynchronous())
		{
			ActorPtr->SetActorHiddenInGame(Type == EWorldActionApplyType::Set 
				? !bVisible : !ActorPtr->IsHidden());
		}
	}
}

void UWorldAction_Collision::OnExecute_Implementation()
{
	for (TSoftObjectPtr<AActor>& SoftPtr : Targets)
	{
		if (AActor* ActorPtr = SoftPtr.LoadSynchronous())
		{
			ActorPtr->SetActorEnableCollision(Type == EWorldActionApplyType::Set 
				? bCollisionEnabled : !ActorPtr->GetActorEnableCollision());
		}
	}
}

void UWorldAction_ActorEnabled::OnExecute_Implementation()
{
	for (TSoftObjectPtr<AActor>& SoftPtr : Targets)
	{
		if (AActor* ActorPtr = SoftPtr.LoadSynchronous())
		{
			IEnabledState::SetEnabled(ActorPtr, Type == EWorldActionApplyType::Set 
				? bEnabled : !IEnabledState::IsEnabled(ActorPtr));
		}
	}
}

void UWorldAction_ActorTags::OnExecute_Implementation()
{
	for (TSoftObjectPtr<AActor>& SoftPtr : Targets)
	{
		if (AActor* ActorPtr = SoftPtr.LoadSynchronous())
		{
			for (const FName& Tag : AddTags)
			{
				ActorPtr->Tags.Add(Tag);
			}
			for (const FName& Tag : RemoveTags)
			{
				ActorPtr->Tags.Remove(Tag);
			}
		}
	}
}

void UWorldAction_PrimitiveData::OnExecute_Implementation()
{
	for (TSoftObjectPtr<AStaticMeshActor>& SoftPtr : Targets)
	{
		const AStaticMeshActor* ActorPtr = SoftPtr.LoadSynchronous();
		if (UStaticMeshComponent* MeshComp = ActorPtr ? ActorPtr->GetStaticMeshComponent() : nullptr)
		{
			for (const TPair<uint8, float>& Data : PrimitiveData)
			{
				bOverrideDefault
					? MeshComp->SetDefaultCustomPrimitiveDataFloat(Data.Key, Data.Value)
					: MeshComp->SetCustomPrimitiveDataFloat(Data.Key, Data.Value);
			}
		}
	}
}