// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Toolbar/ActorBaker.h"
#include "EditorDialogLibrary.h"
#include "Helpers/LoggingMacros.h"
#include "Actors/Procedural/BakerHelpers.h"
#include "Actors/Procedural/ToroProcGenBase.h"
#include "Actors/Procedural/ToroProcMeshBase.h"
#include "Subsystems/EditorActorSubsystem.h"

void FActorBaker::ExecuteAction()
{
	if (UEditorActorSubsystem* Subsystem = GEditor ? GEditor->GetEditorSubsystem<UEditorActorSubsystem>() : nullptr)
	{
		const TArray<AActor*> Actors = Subsystem->GetSelectedLevelActors();
		if (Actors.IsEmpty()) return;

		if (UEditorDialogLibrary::ShowMessage(FText::FromString(TEXT("Are you sure you want to Bake Selected Meshes?")),
			FText::FromString(TEXT("This will gather mesh components from the selected actors and bake them into individual static mesh actors.")),
			EAppMsgType::OkCancel, EAppReturnType::Cancel, EAppMsgCategory::Info) != EAppReturnType::Ok)
		{
			return;
		}
		
		const EAppReturnType::Type SrcRetType = UEditorDialogLibrary::ShowMessage(FText::FromString(TEXT("Do you want to destroy source actors?")),
			FText::FromString(TEXT("If 'Yes' source actors will be destroyed, otherwise, they will be kept.\nDefault: Yes")),
			EAppMsgType::YesNoCancel, EAppReturnType::Yes, EAppMsgCategory::Info);

		if (SrcRetType == EAppReturnType::Cancel) return;
		const bool bDestroySourceActors = SrcRetType == EAppReturnType::Yes;

		UE_LOG_MESSAGE(LogToroEditor, 2.0f, TEXT("%s"), TEXT("Starting Bake!"));

		TArray<AActor*> ActorsToDelete;
		ActorsToDelete.Reserve(Actors.Num());

		// Transaction scope
		{
			const FScopedTransaction Transaction(NSLOCTEXT("ToroEditor", "BakeStaticMeshes", "Bake Static Meshes"));
			for (int32 i = 0; i < Actors.Num(); i++)
			{
				if (!Actors[i] || Actors[i]->IsEditorOnly()) continue;
				UE_LOG_MESSAGE(LogToroEditor, 2.0f, TEXT(">	Baking [%d/%d] %s"),
					i + 1, Actors.Num(), *Actors[i]->GetName());

				if (Actors[i]->IsA<AStaticMeshActor>() || Actors[i]->IsA<AToroProcMeshBase>())
				{
					Subsystem->SetActorSelectionState(Actors[i], true);
					continue;
				}
				if (AToroProcGenBase* ProcActor = Cast<AToroProcGenBase>(Actors[i]))
				{
					Subsystem->SetActorSelectionState(Actors[i], false);
					ProcActor->BakeInternal(false, true, nullptr);
					ActorsToDelete.Add(ProcActor);
					continue;
				}

				TArray<UStaticMeshComponent*> MeshComps;
				Actors[i]->GetComponents<UStaticMeshComponent>(MeshComps);
				if (MeshComps.IsEmpty()) continue;

				for (int32 j = 0; j < MeshComps.Num(); j++)
				{
					if (!MeshComps[j] || MeshComps[j]->IsEditorOnly() || !MeshComps[j]->GetStaticMesh()) continue;

					TArray<AActor*> ResultActors;
					ResultActors.Append(BakerHelpers::BakeComponent(Subsystem, MeshComps[j], j));
					for (AActor* Actor : ResultActors)
					{
						Subsystem->SetActorSelectionState(Actor, true);
					}
				}

				ActorsToDelete.Add(Actors[i]);
				Subsystem->SetActorSelectionState(Actors[i], false);
			}
		}

		// Transaction scope
		{
			const FScopedTransaction Transaction(NSLOCTEXT("ToroEditor", "DestroySourceActors", "Destroy Source Actors (Undo to revert)"));
			if (bDestroySourceActors)
			{
				for (AActor* Actor : ActorsToDelete)
				{
					Subsystem->SetActorTransform(Actor, FTransform::Identity);
				}
				Subsystem->DestroyActors(ActorsToDelete);
			}
		}

		UE_LOG_MESSAGE(LogToroEditor, 2.0f, TEXT("%s"), TEXT("Bake Finished!"));
	}
}
