// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Helpers/ActorBaker.h"
#if WITH_EDITOR
#include "AssetToolsModule.h"
#include "EditorDirectories.h"
#include "MeshDescription.h"
#include "ProceduralMeshConversion.h"
#include "DataTypes/MeshProperties.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Dialogs/DlgPickAssetPath.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMeshActor.h"
#include "PhysicsEngine/BodySetup.h"

#define LOCTEXT_NAMESPACE "ToroCore"

TArray<AActor*> FActorBaker::BakeActors(const TArray<AActor*>& Sources, const bool bDestroySources)
{
	UEditorActorSubsystem* Subsystem = ActorSubsystem.Get();
	if (!Subsystem || Sources.IsEmpty()) return TArray<AActor*>();

	UE_LOG(LogToroCore, Display, TEXT("[BakeActors] Starting Bake for %d actors..."), Sources.Num())

	TArray<AActor*> Actors;
	// Transaction Scope
	{
		const FScopedTransaction Transaction(LOCTEXT("BakeProcessSources", "Bake Actors"));
		for (int32 i = 0; i < Sources.Num(); i++)
		{
			if (!Sources[i]) continue;
			TArray<UMeshComponent*> Components;
			Sources[i]->GetComponents<UMeshComponent>(Components);
			for (const UMeshComponent* Component : Components)
			{
				if (!Component) continue;
				Actors.Append(BakeComponent(Component, i + FMath::RandRange(1, 99)));
			}

			Subsystem->SetActorSelectionState(Sources[i], false);
			UE_LOG(LogToroCore, Display, TEXT("[BakeActors] \t Processed [%d/%d] actors"), i + 1, Sources.Num())
		}
	}

	Subsystem->SetSelectedLevelActors(Actors);
	if (bDestroySources)
	{
		const FScopedTransaction Transaction(LOCTEXT("BakeDestroySources", "Destroy Source Actors"));
		Subsystem->DestroyActors(Sources);
	}

	UE_LOG(LogToroCore, Display, TEXT("[BakeActors] Finished baking %d actors!"), Sources.Num())
	return Actors;
}

TArray<AActor*> FActorBaker::BakeComponent(const UMeshComponent* Source, const int32 Index)
{
	if (!Source)
	{
		return TArray<AActor*>();
	}

	TArray<AActor*> Actors;
	const int32 Idx = Index < 0 ? FMath::RandRange(1, 99) : Index;
	if (const UStaticMeshComponent* SM = Cast<UStaticMeshComponent>(Source))
	{
		if (AActor* Actor = BakeStaticMesh(SM))
		{
			Actor->SetActorLabel(FString::Printf(TEXT("%s_%d"),
			   *SM->GetStaticMesh()->GetName(), Idx));

			Actors.Add(Actor);
		}
	}
	else if (const UInstancedStaticMeshComponent* ISM = Cast<UInstancedStaticMeshComponent>(Source))
	{
		Actors.Reserve(ISM->GetNumInstances());
		for (int i = 0; i < ISM->GetNumInstances(); i++)
		{
			FTransform T;
			if (ISM->GetInstanceTransform(i, T, true))
			{
				if (AActor* Actor = BakeStaticMesh(ISM))
				{
					Actor->SetActorTransform(T, false, nullptr, ETeleportType::ResetPhysics);
					Actor->SetActorLabel(FString::Printf(TEXT("%s_%d_Inst_%d"),
						*ISM->GetStaticMesh()->GetName(), Idx, i));

					Actors.Add(Actor);
				}
			}
		}
	}
	else if (const USplineMeshComponent* SMC = Cast<USplineMeshComponent>(Source))
	{
		if (AActor* Actor = BakeSplineMesh(SMC))
		{
			Actor->SetActorLabel(FString::Printf(TEXT("%s_%d"),
				*SMC->GetStaticMesh()->GetName(), Idx));

			Actors.Add(Actor);
		}
	}
	else if (const USkeletalMeshComponent* SKMC = Cast<USkeletalMeshComponent>(Source))
	{
		if (AActor* Actor = BakeSkeletalMesh(SKMC))
		{
			Actor->SetActorLabel(FString::Printf(TEXT("%s_%d"),
				*SKMC->GetSkeletalMeshAsset()->GetName(), Idx));

			Actors.Add(Actor);
		}
	}

	return Actors;
}

AActor* FActorBaker::BakeProceduralMesh(UProceduralMeshComponent* Source, const FName& Name)
{
	FString DefaultPath;
	const FString DefaultDirectory = FEditorDirectories::Get().GetLastDirectory(ELastDirectory::NEW_ASSET);
	FPackageName::TryConvertFilenameToLongPackageName(DefaultDirectory, DefaultPath);
	FString PackageName = (DefaultPath.IsEmpty() ? TEXT("/Game/Meshes") : DefaultPath);
	PackageName /= (Name.IsNone() ? TEXT("SM_ProcMesh") : Name.ToString());

	FString AssetName;
	const FAssetToolsModule& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
	AssetTools.Get().CreateUniqueAssetName(PackageName, TEXT("_0"), PackageName, AssetName);

	const TSharedPtr<SDlgPickAssetPath> PathWidget = SNew(SDlgPickAssetPath)
		.Title(LOCTEXT("BakeProceduralMeshLocation", "Choose New StaticMesh Location"))
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

		FMeshDescription MeshDescription = BuildMeshDescription(Source);
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

			if (!Source->bUseComplexAsSimpleCollision)
			{
				StaticMesh->CreateBodySetup();
				UBodySetup* NewBodySetup = StaticMesh->GetBodySetup();
				NewBodySetup->AggGeom.ConvexElems = Source->ProcMeshBodySetup->AggGeom.ConvexElems;
				NewBodySetup->CollisionTraceFlag = CTF_UseDefault;
				NewBodySetup->bGenerateMirroredCollision = false;
				NewBodySetup->BodySetupGuid = FGuid::NewGuid();
				NewBodySetup->bDoubleSidedGeometry = true;
				NewBodySetup->CreatePhysicsMeshes();
			}

			TMap<int32, UMaterialInterface*> UniqueMaterials;
			const int32 NumSections = Source->GetNumSections();
			for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++)
			{
				UniqueMaterials.Add(SectionIdx, Source->GetMaterial(SectionIdx));
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
		const FScopedTransaction Transaction(LOCTEXT("BakeProceduralMesh", "Bake Procedural Mesh"));
		if (AStaticMeshActor* Actor = SpawnStaticMesh<AStaticMeshActor>(Source->GetComponentTransform()))
		{
			Actor->GetStaticMeshComponent()->SetStaticMesh(StaticMesh);
			Subsystem->SetSelectedLevelActors({Actor});
			return Actor;
		}
	}

	return nullptr;
}

AStaticMeshActor* FActorBaker::BakeStaticMesh(const UStaticMeshComponent* Src)
{
	FStaticMeshProperties Data;
	Data.FromMeshComponent(Src);
	if (!Data.IsValid()) return nullptr;
	if (AStaticMeshActor* Actor = SpawnStaticMesh<AStaticMeshActor>(Src->GetComponentTransform()))
	{
		UStaticMeshComponent* Comp = Actor->GetStaticMeshComponent();
		Comp->SetAffectDynamicIndirectLighting(Src->bAffectDynamicIndirectLighting);
		Comp->SetOverlayMaterial(Src->GetOverlayMaterial());
		Data.ToMeshComponent(Comp);

		const AActor* Owner = Src->GetOwner();
		Actor->SetActorHiddenInGame(Owner->IsHidden());
		Actor->SetFolderPath(*(Owner->GetFolderPath().ToString() / Owner->GetActorLabel() + TEXT("_BAKED")));

		return Actor;
	}
	return nullptr;
}

ASplineMeshActor* FActorBaker::BakeSplineMesh(const USplineMeshComponent* Src)
{
	FStaticMeshProperties Data;
	Data.FromMeshComponent(Src);
	if (!Data.IsValid()) return nullptr;
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

		const AActor* Owner = Src->GetOwner();
		Actor->SetActorHiddenInGame(Owner->IsHidden());
		Actor->SetFolderPath(*(Owner->GetFolderPath().ToString() / Owner->GetActorLabel() + TEXT("_BAKED")));

		return Actor;
	}
	return nullptr;
}

ASkeletalMeshActor* FActorBaker::BakeSkeletalMesh(const USkeletalMeshComponent* Src)
{
	if (!Src || !Src->GetSkeletalMeshAsset())
	{
		return nullptr;
	}

	ASkeletalMeshActor* Actor = SpawnStaticMesh<ASkeletalMeshActor>(Src->GetComponentTransform());
	if (USkeletalMeshComponent* Comp = Actor ? Actor->GetSkeletalMeshComponent() : nullptr)
	{
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
	}

	return Actor;
}

#undef LOCTEXT_NAMESPACE
#endif