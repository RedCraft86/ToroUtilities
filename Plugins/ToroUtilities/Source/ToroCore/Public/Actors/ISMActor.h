// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "ISMActor.generated.h"

class UInstancedStaticMeshComponent;

UCLASS(meta = (ChildCanTick = true))
class TOROCORE_API AInstancedStaticMeshActor : public AStaticMeshActor
{
	GENERATED_BODY()

public:

	AInstancedStaticMeshActor(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer.SetDefaultSubobjectClass<UInstancedStaticMeshComponent>(StaticMeshComponentName))
	{
		bCanBeInCluster = GetClass() == AInstancedStaticMeshActor::StaticClass();
#if WITH_EDITOR
		if (UStaticMeshComponent* MeshComp = GetStaticMeshComponent())
		{
			const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
			if (MeshFinder.Succeeded()) MeshComp->SetStaticMesh(MeshFinder.Object);
	
			const ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(
				TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
			if (MaterialFinder.Succeeded()) MeshComp->SetMaterial(0, MaterialFinder.Object);
		}
#endif
	}

	template<typename T = UInstancedStaticMeshComponent>
	T* GetMeshComponent() const { return Cast<T>(GetStaticMeshComponent()); }

#if WITH_EDITORONLY_DATA
	UPROPERTY(Category = StaticMesh, EditAnywhere, meta = (MakeEditWidget, DisplayPriority = 1))
		TArray<FTransform> Instances = {FTransform::Identity};
protected:
	virtual void OnConstruction(const FTransform& Transform) override
	{
		Super::OnConstruction(Transform);
		if (UInstancedStaticMeshComponent* ISMComponent = GetMeshComponent<UInstancedStaticMeshComponent>())
		{
			ISMComponent->ClearInstances();
			ISMComponent->AddInstances(Instances, false);
		}
	}
#endif
};

UCLASS(meta = (ChildCanTick = true))
class TOROCORE_API AHierarchicalInstancedStaticMeshActor : public AInstancedStaticMeshActor
{
	GENERATED_BODY()

public:

	AHierarchicalInstancedStaticMeshActor(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer.SetDefaultSubobjectClass<UHierarchicalInstancedStaticMeshComponent>(StaticMeshComponentName))
	{
		bCanBeInCluster = GetClass() == AHierarchicalInstancedStaticMeshActor::StaticClass();
	}
};