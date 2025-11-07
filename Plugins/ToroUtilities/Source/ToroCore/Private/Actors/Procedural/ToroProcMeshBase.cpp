// Copyright (C) RedCraft86. All Rights Reserved.

#include "Actors/Procedural/ToroProcMeshBase.h"
#if WITH_EDITOR
#include "AssetToolsModule.h"
#include "EditorDirectories.h"
#include "PhysicsEngine/BodySetup.h"
#include "ProceduralMeshConversion.h"
#include "Dialogs/DlgPickAssetPath.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "Engine/StaticMeshActor.h"
#endif

AToroProcMeshBase::AToroProcMeshBase()
{
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("MeshComponent");
	ProceduralMesh->SetupAttachment(GetRootComponent());
	ProceduralMesh->bAffectDynamicIndirectLighting = false;
	ProceduralMesh->bUseComplexAsSimpleCollision = false;
	ProceduralMesh->SetCastShadow(false);
}

#if WITH_EDITOR
void AToroProcMeshBase::BakeInternal(const bool bRemoveSource, const bool bSelectActors, TArray<AActor*>* OutActors)
{
	for (const int32 i : EditorSections)
	{
		ProceduralMesh->ClearMeshSection(i);
	}
	
	FString DefaultPath;
	const FString DefaultDirectory = FEditorDirectories::Get().GetLastDirectory(ELastDirectory::NEW_ASSET);
	FPackageName::TryConvertFilenameToLongPackageName(DefaultDirectory, DefaultPath);
	if (DefaultPath.IsEmpty()) DefaultPath = TEXT("/Game/Meshes");
	FString PackageName = DefaultPath / TEXT("SM_ProcMesh");
	
	FString Name;
	const FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	AssetToolsModule.Get().CreateUniqueAssetName(PackageName, TEXT("_00"), PackageName, Name);

	const TSharedPtr<SDlgPickAssetPath> PickAssetPathWidget = SNew(SDlgPickAssetPath)
		.Title(NSLOCTEXT("ProceduralMeshComponentDetails", "ConvertToStaticMeshPickName", "Choose New StaticMesh Location"))
		.DefaultAssetPath(FText::FromString(PackageName));

	UStaticMesh* StaticMesh = nullptr;
	if (PickAssetPathWidget->ShowModal() == EAppReturnType::Ok)
	{
		FString UserPackageName = PickAssetPathWidget->GetFullAssetPath().ToString();
		FName MeshName(*FPackageName::GetLongPackageAssetName(UserPackageName));
		if (MeshName == NAME_None)
		{
			UserPackageName = PackageName;
			MeshName = *Name;
		}
		
		FMeshDescription MeshDescription = BuildMeshDescription(ProceduralMesh);
		if (MeshDescription.Polygons().Num() > 0)
		{
			UPackage* Package = CreatePackage(*UserPackageName); check(Package);
			StaticMesh = NewObject<UStaticMesh>(Package, MeshName, RF_Public | RF_Standalone);
			StaticMesh->InitResources();
			StaticMesh->SetLightingGuid();

			FStaticMeshSourceModel& SrcModel = StaticMesh->AddSourceModel();
			SrcModel.BuildSettings.bRecomputeNormals = false;
			SrcModel.BuildSettings.bRecomputeTangents = false;
			SrcModel.BuildSettings.bRemoveDegenerates = false;
			SrcModel.BuildSettings.bUseHighPrecisionTangentBasis = false;
			SrcModel.BuildSettings.bUseFullPrecisionUVs = false;
			SrcModel.BuildSettings.bGenerateLightmapUVs = true;
			SrcModel.BuildSettings.SrcLightmapIndex = 0;
			SrcModel.BuildSettings.DstLightmapIndex = 1;
			StaticMesh->CreateMeshDescription(0, MoveTemp(MeshDescription));
			StaticMesh->CommitMeshDescription(0);

			if (!ProceduralMesh->bUseComplexAsSimpleCollision)
			{
				StaticMesh->CreateBodySetup();
				UBodySetup* NewBodySetup = StaticMesh->GetBodySetup();
				NewBodySetup->BodySetupGuid = FGuid::NewGuid();
				NewBodySetup->AggGeom.ConvexElems = ProceduralMesh->ProcMeshBodySetup->AggGeom.ConvexElems;
				NewBodySetup->bGenerateMirroredCollision = false;
				NewBodySetup->bDoubleSidedGeometry = true;
				NewBodySetup->CollisionTraceFlag = CTF_UseDefault;
				NewBodySetup->CreatePhysicsMeshes();
			}

			TMap<int32, UMaterialInterface*> UniqueMaterials;
			const int32 NumSections = ProceduralMesh->GetNumSections();
			for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++)
			{
				if (EditorSections.Contains(SectionIdx)) continue;
				UniqueMaterials.Add(SectionIdx, ProceduralMesh->GetMaterial(SectionIdx));
			}

			for (const TPair<int32, UMaterialInterface*>& Material : UniqueMaterials)
			{
				StaticMesh->GetStaticMaterials().Add(FStaticMaterial(Material.Value,
						*FString::Printf(TEXT("Section_%d"), Material.Key)));
			}

			StaticMesh->ImportVersion = EImportStaticMeshVersion::LastVersion;
			StaticMesh->Build(false);
			StaticMesh->PostEditChange();

			FAssetRegistryModule::AssetCreated(StaticMesh);
		}
	}

	if (!StaticMesh) return;
	if (UEditorActorSubsystem* Subsystem = GEditor ? GEditor->GetEditorSubsystem<UEditorActorSubsystem>() : nullptr)
	{
		const FScopedTransaction Transaction(NSLOCTEXT("ToroCore", "BakeProceduralMesh", "Bake Procedural Mesh"));
		if (AStaticMeshActor* NewActor = Cast<AStaticMeshActor>(Subsystem->SpawnActorFromClass(
			AStaticMeshActor::StaticClass(), GetActorLocation(), GetActorRotation())))
		{
			NewActor->SetFolderPath(GetFolderPath());
			NewActor->SetActorLabel(GetActorLabel());
			NewActor->SetActorScale3D(GetActorScale3D());
			NewActor->GetStaticMeshComponent()->SetStaticMesh(StaticMesh);
			if (OutActors)
			{
				OutActors->Empty(1);
				OutActors->Add(NewActor);
			}
			if (bSelectActors)
			{
				Subsystem->SetSelectedLevelActors({NewActor});
			}
			if (bRemoveSource)
			{
				Subsystem->SetActorSelectionState(this, false);
				Subsystem->DestroyActor(this);
			}
		}
		return;
	}

	Construct();
}
#endif