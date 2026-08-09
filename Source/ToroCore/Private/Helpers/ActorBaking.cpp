// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "Helpers/ActorBaking.h"

#if WITH_EDITOR
#include "ToroCore.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/SplineMeshActor.h"
#include "Animation/SkeletalMeshActor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Dialogs/DlgPickAssetPath.h"
#include "ProceduralMeshConversion.h"
#include "PhysicsEngine/BodySetup.h"
#include "EditorDirectories.h"
#include "AssetToolsModule.h"
#include "Actors/ISMActors.h"

#define LOCTEXT_NAMESPACE "ToroCore"

namespace
{
	AActor* ProcessSpawnedActor(AActor* Actor, const AActor* Owner, const FString& Name)
	{
		if (Owner)
		{
			Actor->SetActorHiddenInGame(Owner->IsHidden());
			Actor->SetFolderPath(*(Owner->GetFolderPath().ToString() / *Owner->GetActorLabel()));
		}
		else
		{
			Actor->SetFolderPath(*FString::Printf(TEXT("Spawned_%s"),
				*FDateTime::Now().ToString(TEXT("%s%S%M%H%d%m%Y")).Left(5)
			));
		}

		Actor->SetActorLabel(FString::Printf(TEXT("%s_%s"), *Name, *FGuid::NewGuid().ToString().Left(5)));
		return Actor;
	}
}

void FActorBaking::DestroyActors(const TArray<AActor*>& Actors)
{
	if (!Actors.IsEmpty())
	{
		if (UEditorActorSubsystem* Subsystem = ActorSubsystem.Get())
		{
			const FScopedTransaction Transaction(LOCTEXT("DestroyActors", "Destroy Actors"));
			Subsystem->DestroyActors(Actors);
		}
	}
}

TArray<AActor*> FActorBaking::SplitActors(const TArray<AActor*>& Sources, const bool bDestroySources)
{
	TArray<AActor*> Actors;
	if (Sources.IsEmpty() || !IsValid(ActorSubsystem.Get()))
	{
		return Actors;
	}

	const int32 SourceCount = Sources.Num();
	UE_LOG(LogToroCore, Display, TEXT("[SplitActors] Starting split for %d actors..."), SourceCount);
	{
		const FScopedTransaction Transaction(LOCTEXT("SplitProcessSources", "Split Actors"));

		Actors.Reserve(SourceCount);
		for (int32 i = 0; i < SourceCount; i++)
		{
			if (!Sources[i])
			{
				continue;
			}

			TArray<UMeshComponent*> Components;
			Sources[i]->GetComponents<UMeshComponent>(Components);
			for (const UMeshComponent* Comp : Components)
			{
				Actors.Append(ConvertMeshComponent(Comp));
			}

			ActorSubsystem->SetActorSelectionState(Sources[i], false);
			UE_LOG(LogToroCore, Display, TEXT("[SplitActors] \t Processed [%d/%d] actors"), i + 1, SourceCount);
		}
	}

	ActorSubsystem->SetSelectedLevelActors(Actors);
	if (bDestroySources)
	{
		DestroyActors(Sources);
	}

	UE_LOG(LogToroCore, Display, TEXT("[SplitActors] Finished splitting with %d actors created!"), Actors.Num());
	return Actors;
}

TArray<AActor*> FActorBaking::InstanceActors(const TArray<AActor*>& Sources, const bool bUseHISM, const bool bDestroySources)
{
	TArray<AActor*> Actors;
	if (Sources.IsEmpty() || !IsValid(ActorSubsystem.Get()))
	{
		return Actors;
	}

	const int32 SourceCount = Sources.Num();
	UE_LOG(LogToroCore, Display, TEXT("[InstanceActors] Starting instance for %d actors..."), SourceCount);
	{
		const FScopedTransaction Transaction(LOCTEXT("InstanceProcessSources", "Instance Actors"));

		Actors.Reserve(SourceCount);

		TArray<UMeshComponent*> AllComponents;
		AllComponents.Reserve(SourceCount);
		for (int32 i = 0; i < SourceCount; i++)
		{
			if (!Sources[i])
			{
				continue;
			}

			TArray<UMeshComponent*> Components;
			Sources[i]->GetComponents<UMeshComponent>(Components);
			AllComponents.Append(Components);

			UE_LOG(LogToroCore, Display, TEXT("[InstanceActors] \t Processed [%d/%d] actors"), i + 1, SourceCount);
		}

		const TMap<FStaticMeshProperties, TArray<FTransform>> Instances = GroupSimilarMeshes(AllComponents);
		const int32 InstCount = Instances.Num();

		UE_LOG(LogToroCore, Display, TEXT("[InstanceActors] Found %d instances (batches) from the actors"), InstCount);

		int32 BatchCount = 0;
		for (const TPair<FStaticMeshProperties, TArray<FTransform>>& Instance : Instances)
		{
			BatchCount++;
			if (Instance.Value.Num() == 1)
			{
				if (AStaticMeshActor* Actor = SpawnStaticMesh<AStaticMeshActor>(Instance.Value[0]))
				{
					Instance.Key.ToMeshComponent(Actor->GetStaticMeshComponent());
					Actors.Add(ProcessSpawnedActor(Actor, nullptr, Instance.Key.StaticMesh.GetAssetName()));
				}
			}
			else
			{
				if (AInstancedStaticMeshActor* Actor = bUseHISM
					? SpawnStaticMesh<AHierarchicalInstancedStaticMeshActor>(Instance.Key.Transform)
					: SpawnStaticMesh<AInstancedStaticMeshActor>(Instance.Key.Transform))
				{
					UInstancedStaticMeshComponent* Component = Actor->GetMeshComponent<>();
					Instance.Key.ToMeshComponent(Component);

					Component->AddInstances(Instance.Value, false, true, true);
					Actor->CopyInstancesFromComponent();

					Actors.Add(ProcessSpawnedActor(Actor, nullptr, Instance.Key.StaticMesh.GetAssetName()));
				}
			}

			UE_LOG(LogToroCore, Display, TEXT("[InstanceActors] \t Processed [%d/%d] batches"), BatchCount, InstCount);
		}

		const int32 RemainComps = AllComponents.Num();
		UE_LOG(LogToroCore, Display, TEXT("[InstanceActors] %d cannot be instanced, splitting instead"), RemainComps);

		for (int32 i = 0; i < AllComponents.Num(); i++)
		{
			Actors.Append(ConvertMeshComponent(AllComponents[i]));
			UE_LOG(LogToroCore, Display, TEXT("[InstanceActors] \t Splitting [%d/%d] components"), i + 1, RemainComps);
		}
	}

	ActorSubsystem->SetSelectedLevelActors(Actors);
	if (bDestroySources)
	{
		DestroyActors(Sources);
	}

	UE_LOG(LogToroCore, Display, TEXT("[InstanceActors] Finished instancing with %d actors created!"), Actors.Num());
	return Actors;
}

TArray<AActor*> FActorBaking::ConvertMeshComponent(const UMeshComponent* Src)
{
	TArray<AActor*> Actors;
	if (!Src)
	{
		return Actors;
	}

	if (const UInstancedStaticMeshComponent* ISM = Cast<UInstancedStaticMeshComponent>(Src))
	{
		const int32 NumInst = ISM->GetNumInstances();
		Actors.Reserve(NumInst);

		for (int32 i = 0; i < NumInst; i++)
		{
			FTransform T;
			if (!ISM->GetInstanceTransform(i, T, true))
			{
				continue;
			}

			if (AActor* Actor = ConvertStaticMesh(ISM))
			{
				Actor->SetActorTransform(T, false, nullptr, ETeleportType::ResetPhysics);
				Actors.Add(Actor);
			}
		}
	}
	else if (const UStaticMeshComponent* SM = Cast<UStaticMeshComponent>(Src))
	{
		Actors.Add(ConvertStaticMesh(SM));
	}
	else if (const USplineMeshComponent* SMC = Cast<USplineMeshComponent>(Src))
	{
		Actors.Add(ConvertSplineMesh(SMC));
	}
	else if (const USkeletalMeshComponent* SKMC = Cast<USkeletalMeshComponent>(Src))
	{
		Actors.Add(ConvertSkeletalMesh(SKMC));
	}
	else if (const UProceduralMeshComponent* PMC = Cast<UProceduralMeshComponent>(Src))
	{
		Actors.Add(ConvertProceduralMesh(PMC));
	}

	return Actors;
}

AActor* FActorBaking::ConvertStaticMesh(const UStaticMeshComponent* Src)
{
	FStaticMeshProperties Data;
	Data.FromMeshComponent(Src);
	if (!Data.IsValid())
	{
		return nullptr;
	}

	if (AStaticMeshActor* Actor = SpawnStaticMesh<AStaticMeshActor>(Src->GetComponentTransform()))
	{
		UStaticMeshComponent* Comp = Actor->GetStaticMeshComponent();
		Comp->SetAffectDynamicIndirectLighting(Src->bAffectDynamicIndirectLighting);
		Comp->SetOverlayMaterial(Src->GetOverlayMaterial());
		Data.ToMeshComponent(Comp);

		return ProcessSpawnedActor(Actor, Src->GetOwner(), Data.StaticMesh.GetAssetName());
	}

	return nullptr;
}

AActor* FActorBaking::ConvertSplineMesh(const USplineMeshComponent* Src)
{
	FStaticMeshProperties Data;
	Data.FromMeshComponent(Src);
	if (!Data.IsValid())
	{
		return nullptr;
	}

	if (ASplineMeshActor* Actor = SpawnStaticMesh<ASplineMeshActor>(Src->GetComponentTransform()))
	{
		const FVector StartPos	= Src->GetStartPosition();
		const FVector StartTan	= Src->GetStartTangent();
		const FVector EndPos	= Src->GetEndPosition();
		const FVector EndTan	= Src->GetEndTangent();

		USplineMeshComponent* Comp = Actor->GetSplineMeshComponent();
		Comp->SetForwardAxis(Src->GetForwardAxis(), false);
		Comp->SetStartAndEnd(StartPos, StartTan, EndPos, EndTan, true);
		Comp->SetAffectDynamicIndirectLighting(Src->bAffectDynamicIndirectLighting);
		Comp->SetOverlayMaterial(Src->GetOverlayMaterial());
		Data.ToMeshComponent(Comp);

		return ProcessSpawnedActor(Actor, Src->GetOwner(), Data.StaticMesh.GetAssetName());
	}

	return nullptr;
}

AActor* FActorBaking::ConvertSkeletalMesh(const USkeletalMeshComponent* Src)
{
	if (!Src || !Src->GetSkeletalMeshAsset())
	{
		return nullptr;
	}

	if (ASkeletalMeshActor* Actor = SpawnStaticMesh<ASkeletalMeshActor>(Src->GetComponentTransform()))
	{
		USkeletalMeshComponent* Comp = Actor->GetSkeletalMeshComponent();
		Comp->SetSkeletalMeshAsset(Src->GetSkeletalMeshAsset());

		Comp->SetPlayRate(Src->GetPlayRate());
		Comp->GlobalAnimRateScale = Src->GlobalAnimRateScale;

		Comp->SetAnimationMode(Src->GetAnimationMode());
		if (const UAnimInstance* AnimInst = Src->GetAnimInstance())
		{
			Comp->SetAnimInstanceClass(AnimInst->GetClass());
		}
		if (const UAnimSingleNodeInstance* Node = Src->GetSingleNodeInstance(); Node && Node->GetAnimationAsset())
		{
			Comp->PlayAnimation(Node->GetAnimationAsset(), Node->IsLooping());
		}

		return ProcessSpawnedActor(Actor, Src->GetOwner(), Comp->GetSkeletalMeshAsset()->GetName());
	}

	return nullptr;
}

AActor* FActorBaking::ConvertProceduralMesh(const UProceduralMeshComponent* Src)
{
	FString DefaultPath;
	const FString DefaultDirectory = FEditorDirectories::Get().GetLastDirectory(ELastDirectory::NEW_ASSET);
	FPackageName::TryConvertFilenameToLongPackageName(DefaultDirectory, DefaultPath);

	FString PackageName = (DefaultPath.IsEmpty() ? TEXT("/Game/Meshes") : DefaultPath);
	PackageName /= Src->GetOwner() ? Src->GetOwner()->GetActorLabel() : TEXT("SM_ProcMesh");

	FString AssetName;
	const FAssetToolsModule& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
	AssetTools.Get().CreateUniqueAssetName(PackageName, TEXT("_0"), PackageName, AssetName);

	const TSharedPtr<SDlgPickAssetPath> PathWidget = SNew(SDlgPickAssetPath)
		.Title(LOCTEXT("SaveProceduralMeshLocation", "Choose New StaticMesh Location"))
		.DefaultAssetPath(FText::FromString(PackageName));

	UStaticMesh* StaticMesh = nullptr;
	if (PathWidget->ShowModal() == EAppReturnType::Ok)
	{
		FString UserPackageName = PathWidget->GetFullAssetPath().ToString();
		FName MeshName = *FPackageName::GetLongPackageAssetName(UserPackageName);
		if (MeshName.IsNone())
		{
			UserPackageName = PackageName;
			MeshName = *AssetName;
		}

		FMeshDescription MeshDescription = BuildMeshDescription(const_cast<UProceduralMeshComponent*>(Src));
		if (MeshDescription.Polygons().Num() > 0)
		{
			UPackage* Package = CreatePackage(*UserPackageName);
			check(Package);

			StaticMesh = NewObject<UStaticMesh>(Package, MeshName, RF_Public | RF_Standalone);
			StaticMesh->InitResources();
			StaticMesh->SetLightingGuid();

			FStaticMeshSourceModel& SrcModel = StaticMesh->AddSourceModel();
			SrcModel.BuildSettings.bUseHighPrecisionTangentBasis = false;
			SrcModel.BuildSettings.bUseFullPrecisionUVs = false;
			SrcModel.BuildSettings.bGenerateLightmapUVs = true;
			SrcModel.BuildSettings.bRemoveDegenerates = false;
			SrcModel.BuildSettings.bRecomputeTangents = false;
			SrcModel.BuildSettings.bRecomputeNormals = false;
			SrcModel.BuildSettings.SrcLightmapIndex = 0;
			SrcModel.BuildSettings.DstLightmapIndex = 1;

			StaticMesh->CreateMeshDescription(0, MoveTemp(MeshDescription));
			StaticMesh->CommitMeshDescription(0);

			if (!Src->bUseComplexAsSimpleCollision)
			{
				StaticMesh->CreateBodySetup();
				UBodySetup* NewBodySetup = StaticMesh->GetBodySetup();
				NewBodySetup->AggGeom.ConvexElems = Src->ProcMeshBodySetup->AggGeom.ConvexElems;
				NewBodySetup->CollisionTraceFlag = CTF_UseDefault;
				NewBodySetup->bGenerateMirroredCollision = false;
				NewBodySetup->BodySetupGuid = FGuid::NewGuid();
				NewBodySetup->bDoubleSidedGeometry = true;
				NewBodySetup->CreatePhysicsMeshes();
			}

			TMap<int32, UMaterialInterface*> UniqueMaterials;
			const int32 NumSections = Src->GetNumSections();
			for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++)
			{
				UniqueMaterials.Add(SectionIdx, Src->GetMaterial(SectionIdx));
			}

			for (const TPair<int32, UMaterialInterface*>& Material : UniqueMaterials)
			{
				StaticMesh->GetStaticMaterials().Add(FStaticMaterial(
					Material.Value, *FString::Printf(TEXT("Section_%d"), Material.Key)
				));
			}

			StaticMesh->SetImportVersion(EImportStaticMeshVersion::LastVersion);
			StaticMesh->Build(false);
			StaticMesh->PostEditChange();

			FAssetRegistryModule::AssetCreated(StaticMesh);
		}
	}

	if (UEditorActorSubsystem* Subsystem = ActorSubsystem.Get(); Subsystem && StaticMesh)
	{
		const FScopedTransaction Transaction(LOCTEXT("SaveProceduralMesh", "Save Procedural Mesh"));
		if (AStaticMeshActor* Actor = SpawnStaticMesh<AStaticMeshActor>(Src->GetComponentTransform()))
		{
			Actor->GetStaticMeshComponent()->SetStaticMesh(StaticMesh);
			Subsystem->SetSelectedLevelActors({Actor});
			return Actor;
		}
	}

	return nullptr;
}

TMap<FStaticMeshProperties, TArray<FTransform>> FActorBaking::GroupSimilarMeshes(TArray<UMeshComponent*>& Srcs)
{
	TMap<FStaticMeshProperties, TArray<FTransform>> Groups;
	for (auto It = Srcs.CreateIterator(); It; ++It)
	{
		FStaticMeshProperties MeshData;
		UMeshComponent* Comp = *It;
		if (!Comp)
		{
			It.RemoveCurrent();
		}
		else if (Comp->IsA<USplineMeshComponent>())
		{
			// InstanceActors(...); will handle this
			continue;
		}
		else if (const UInstancedStaticMeshComponent* ISM = Cast<UInstancedStaticMeshComponent>(Comp))
		{
			It.RemoveCurrent();

			MeshData.FromMeshComponent(ISM, true);
			if (!MeshData.IsValid())
			{
				continue;
			}

			TArray<FTransform>& Instances = Groups.FindOrAdd(MeshData);
			const int32 NumInst = ISM->GetNumInstances();
			Instances.Reserve(Instances.Num() + NumInst);
			for (int32 i = 0; i < NumInst; i++)
			{
				FTransform T;
				if (!ISM->GetInstanceTransform(i, T, true))
				{
					continue;
				}

				if (!Instances.ContainsByPredicate([T](const FTransform& Element)
					{
						return T.Equals(Element);
					}))
				{
					Instances.Add(T);
				}
			}
		}
		else if (const UStaticMeshComponent* SM = Cast<UStaticMeshComponent>(Comp))
		{
			It.RemoveCurrent();

			MeshData.FromMeshComponent(SM, true);
			if (!MeshData.IsValid())
			{
				continue;
			}

			const FTransform T = SM->GetComponentTransform();
			TArray<FTransform>& Instances = Groups.FindOrAdd(MeshData);
			if (!Instances.ContainsByPredicate([T](const FTransform& Element)
				{
					return T.Equals(Element);
				}))
			{
				Instances.Add(T);
			}
		}
	}

	return Groups;
}

#undef LOCTEXT_NAMESPACE
#endif