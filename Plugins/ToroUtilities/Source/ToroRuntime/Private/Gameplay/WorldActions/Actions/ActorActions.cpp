// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/WorldActions/Actions/ActorActions.h"
#include "Interfaces/ActorStateInterface.h"

void FWATask_Actors::ValidateTargets(TSet<TSoftObjectPtr<AActor>>& InTargets)
{
	for (auto It = InTargets.CreateIterator(); It; ++It)
	{
		if (It->IsNull()) It.RemoveCurrent();
	}
}

void FWTActor_Visibility::RunAction()
{
	for (TSoftObjectPtr<AActor>& Actor : Targets)
	{
		if (AActor* ActorPtr = Actor.LoadSynchronous())
		{
			ActorPtr->SetActorHiddenInGame(bToggle ? !ActorPtr->IsHidden() : !bVisible);
		}
	}
}

void FWTActor_Visibility::OnBeginPlay()
{
	ValidateTargets(Targets);
}

void FWTActor_Collision::RunAction()
{
	for (TSoftObjectPtr<AActor>& Actor : Targets)
	{
		if (AActor* ActorPtr = Actor.LoadSynchronous())
		{
			ActorPtr->SetActorEnableCollision(bToggle ? !ActorPtr->GetActorEnableCollision() : bHasCollision);
		}
	}
}

void FWTActor_Collision::OnBeginPlay()
{
	ValidateTargets(Targets);
}

void FWTActor_Tags::RunAction()
{
	for (TSoftObjectPtr<AActor>& Actor : Targets)
	{
		if (AActor* ActorPtr = Actor.LoadSynchronous())
		{
			for (const FName& Tag : Add)
			{
				if (!Tag.IsNone()) ActorPtr->Tags.AddUnique(Tag);
			}
			for (const FName& Tag : Remove)
			{
				ActorPtr->Tags.Remove(Tag);
			}
		}
	}
}

void FWTActor_Tags::OnBeginPlay()
{
	Add.Remove(NAME_None);
	Remove.Remove(NAME_None);
	ValidateTargets(Targets);
}

void FWTActor_Enabled::RunAction()
{
	for (TSoftObjectPtr<AActor>& Actor : Targets)
	{
		if (AActor* ActorPtr = Actor.LoadSynchronous())
		{
			IActorStateInterface::SetEnabled(ActorPtr, bToggle
				? !IActorStateInterface::IsEnabled(ActorPtr) : bEnabled);
		}
	}
}

void FWTActor_Enabled::OnBeginPlay()
{
	ValidateTargets(Targets);
}

void FWATask_StaticMeshes::ValidateTargets(TSet<TSoftObjectPtr<AStaticMeshActor>>& InTargets)
{
	for (auto It = InTargets.CreateIterator(); It; ++It)
	{
		if (It->IsNull()) It.RemoveCurrent();
	}
}

void FWTActor_Materials::RunAction()
{
	for (TSoftObjectPtr<AStaticMeshActor>& Actor : Targets)
	{
		AStaticMeshActor* ActorPtr = Actor.LoadSynchronous();
		if (UStaticMeshComponent* MeshComp = ActorPtr->GetStaticMeshComponent())
		{
			for (const TPair<uint8, TSoftObjectPtr<UMaterialInterface>>& Pair : Materials)
			{
				if (UMaterialInterface* Material = Pair.Value.LoadSynchronous())
				{
					MeshComp->SetMaterial(Pair.Key, Material);
				}
			}
		}
	}
}

void FWTActor_Materials::OnBeginPlay()
{
	ValidateTargets(Targets);
}

void FWTActor_PrimitiveData::RunAction()
{
	for (TSoftObjectPtr<AStaticMeshActor>& Actor : Targets)
	{
		AStaticMeshActor* ActorPtr = Actor.LoadSynchronous();
		if (UStaticMeshComponent* MeshComp = ActorPtr->GetStaticMeshComponent())
		{
			for (const TPair<uint8, float>& Pair : Data)
			{
				bOverrideDefault
					? MeshComp->SetDefaultCustomPrimitiveDataFloat(Pair.Key, Pair.Value)
					: MeshComp->SetCustomPrimitiveDataFloat(Pair.Key, Pair.Value);
			}
		}
	}
}

void FWTActor_PrimitiveData::OnBeginPlay()
{
	ValidateTargets(Targets);
}
