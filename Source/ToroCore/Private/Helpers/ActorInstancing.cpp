// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "Helpers/ActorInstancing.h"
#if WITH_EDITOR
#include "Actors/ISMActors.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Helpers/ActorBaking.h"

#define LOCTEXT_NAMESPACE "ToroCore"

TArray<AActor*> FActorInstancing::InstanceActors(const TArray<AActor*>& Sources, const bool bUseHISM, const bool bDestroySources)
{
	UEditorActorSubsystem* Subsystem = FActorBaking::ActorSubsystem.Get();
	if (!Subsystem || Sources.IsEmpty()) return TArray<AActor*>();

	UE_LOG(LogToroCore, Display, TEXT("[InstanceActors] Starting Instancing for %d actors..."), Sources.Num())

	TArray<AActor*> Actors;
	// Transaction Scope
	{
		const FScopedTransaction Transaction(LOCTEXT("InstanceProcessSources", "Instance Actors"));

		TArray<UStaticMeshComponent*> AllComponents;
		for (int32 i = 0; i < Sources.Num(); i++)
		{
			if (!Sources[i]) continue;
			TArray<UStaticMeshComponent*> Components;
			Sources[i]->GetComponents<UStaticMeshComponent>(Components);
			AllComponents.Append(Components);

			Subsystem->SetActorSelectionState(Sources[i], false);
			UE_LOG(LogToroCore, Display, TEXT("[InstanceActors] \t Processed [%d/%d] actors"), i + 1, Sources.Num())
		}

		TMap<FStaticMeshProperties, TArray<FTransform>> Instances = ExtractInstances(AllComponents);
		UE_LOG(LogToroCore, Display, TEXT("[InstanceActors] Found %d unique instances from the actors"), Instances.Num())

		int32 Idx = 0;
		for (TPair<FStaticMeshProperties, TArray<FTransform>>& Instance : Instances)
		{
			Idx++;
			if (Instance.Value.Num() == 1)
			{
				AStaticMeshActor* Actor = FActorBaking::SpawnStaticMesh<AStaticMeshActor>(Instance.Value[0]);
				Instance.Key.ToMeshComponent(Actor->GetStaticMeshComponent());

				Actor->SetActorLabel(FString::Printf(TEXT("%s_Inst_%d"),
					*Instance.Key.StaticMesh->GetName(), Idx));
				Actors.Add(Actor);
			}
			else
			{
				AInstancedStaticMeshActor* Actor = bUseHISM 
					? FActorBaking::SpawnStaticMesh<AHierarchicalInstancedStaticMeshActor>(Instance.Key.Transform)
					: FActorBaking::SpawnStaticMesh<AInstancedStaticMeshActor>(Instance.Key.Transform);

				UInstancedStaticMeshComponent* Component = Actor->GetMeshComponent<UInstancedStaticMeshComponent>();
				Instance.Key.ToMeshComponent(Component);

				Component->AddInstances(Instance.Value, false, true);
				Actor->CopyInstancesFromComponent();

				Actor->SetActorLabel(FString::Printf(TEXT("%s_Inst_%d"),
					*Instance.Key.StaticMesh->GetName(), Idx));
				Actors.Add(Actor);
			}

			UE_LOG(LogToroCore, Display, TEXT("[InstanceActors] \t Created [%d/%d] instances"), Idx, Instances.Num())
		}

		UE_LOG(LogToroCore, Display, TEXT("[InstanceActors] %d non-instancable components remain, baking instead"), AllComponents.Num())

		for (int32 i = 0; i < AllComponents.Num(); i++)
		{
			Idx++;
			Actors.Append(FActorBaking::BakeComponent(AllComponents[i], Idx));
			UE_LOG(LogToroCore, Display, TEXT("[InstanceActors] \t Baked [%d/%d] remaining components"), i, AllComponents.Num())
		}
	}

	const FString UniqueGUID = FGuid::NewGuid().ToString();
	for (AActor* Actor : Actors)
	{
		Actor->SetFolderPath(*(UniqueGUID + TEXT("_INSTANCED")));
	}

	Subsystem->SetSelectedLevelActors(Actors);
	if (bDestroySources)
	{
		const FScopedTransaction Transaction(LOCTEXT("InstanceDestroySources", "Destroy Source Actors"));
		Subsystem->DestroyActors(Sources);
	}

	UE_LOG(LogToroCore, Display, TEXT("[InstanceActors] Finished Instancing %d actors!"), Sources.Num())
	return Actors;
}

TMap<FStaticMeshProperties, TArray<FTransform>> FActorInstancing::ExtractInstances(TArray<UStaticMeshComponent*>& Components)
{
	TMap<FStaticMeshProperties, TArray<FTransform>> Result;
	for (auto It = Components.CreateIterator(); It; ++It)
	{
		FStaticMeshProperties MeshData;
		UStaticMeshComponent* Comp = *It;
		if (!Comp)
		{
			It.RemoveCurrent();
		}
		else if (Comp->IsA<USplineMeshComponent>())
		{
			// Handled by caller
			continue;
		}
		else if (UInstancedStaticMeshComponent* ISMComp = Cast<UInstancedStaticMeshComponent>(Comp))
		{
			MeshData.FromMeshComponent(ISMComp, true);
			if (!MeshData.IsValid()) continue;

			TArray<FTransform>& Transforms = Result.FindOrAdd(MeshData);

			const int32 NumInstances = ISMComp->GetNumInstances();
			Transforms.Reserve(Transforms.Num() + NumInstances);
			for (int32 Idx = 0; Idx < NumInstances; Idx++)
			{
				FTransform T;
				ISMComp->GetInstanceTransform(Idx, T, true);
				if (!Transforms.ContainsByPredicate([T](const FTransform& Element)
					{
						return T.Equals(Element);
					}))
				{
					Transforms.Emplace(MoveTemp(T));
				}
			}

			It.RemoveCurrent();
		}
		else if (UStaticMeshComponent* SMComp = Cast<UStaticMeshComponent>(Comp))
		{
			MeshData.FromMeshComponent(SMComp, true);
			if (!MeshData.IsValid()) continue;

			FTransform T = SMComp->GetComponentTransform();
			TArray<FTransform>& Transforms = Result.FindOrAdd(MeshData);
			if (!Transforms.ContainsByPredicate([T](const FTransform& Element)
				{
					return T.Equals(Element);
				}))
			{
				Transforms.Emplace(MoveTemp(T));
			}

			It.RemoveCurrent();
		}
	}

	return Result;
}

#undef LOCTEXT_NAMESPACE
#endif