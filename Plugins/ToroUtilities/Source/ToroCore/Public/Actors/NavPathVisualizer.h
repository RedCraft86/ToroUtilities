// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/EditorShapeComponent.h"
#include "Components/BillboardComponent.h"
#include "NavPathVisualizer.generated.h"

UCLASS(MinimalAPI, NotBlueprintable, NotBlueprintType, HideCategories = (Rendering, HLOD, Replication, Collision, Physics, Networking, Input, Actor, WorldPartition, LevelInstance, Cooking, DataLayers))
class ANavPathVisualizer final : public AActor
{
	GENERATED_BODY()

public:
	
	ANavPathVisualizer(): bRefresh(false), PathPoints({FVector{0.0f}})
	{
		PrimaryActorTick.bCanEverTick = false;

		SceneRoot = CreateDefaultSubobject<UBillboardComponent>("DefaultIconBillboard");
		SceneRoot->SetSprite(LoadObject<UTexture2D>(nullptr,
			TEXT("/Engine/EditorResources/EmptyActor.EmptyActor")));
		SceneRoot->bIsScreenSizeScaled = true;
		SceneRoot->SetHiddenInGame(true);
		SetRootComponent(SceneRoot);
	
#if WITH_EDITOR
		if (EditorShapes = CreateEditorOnlyDefaultSubobject<UEditorShapeComponent>("EditorShapes"); EditorShapes)
		{
			FWireStringData Data; Data.String = TEXT("NavPath Visualizer");
			EditorShapes->WireStrings.Add(TEXT("Label"), Data);
		}

		bRunConstructionScriptOnDrag = true;
#endif
		bIsEditorOnlyActor = true;
		SetCanBeDamaged(false);
	}

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bRefresh;
	
	UPROPERTY(EditAnywhere, Category = Settings, meta = (MakeEditWidget = true))
		TArray<FVector> PathPoints;
	
private:

	UPROPERTY() TObjectPtr<UBillboardComponent> SceneRoot;
#if WITH_EDITORONLY_DATA
	UPROPERTY() TObjectPtr<UEditorShapeComponent> EditorShapes;
#endif
	
	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		K2_DestroyActor();
	}
	
	virtual void OnConstruction(const FTransform& Transform) override
	{
		Super::OnConstruction(Transform);
		SetActorScale3D(FVector{1.0f});
#if WITH_EDITOR
		if (EditorShapes)
		{
			EditorShapes->WireNavPath.Targets = PathPoints;
			EditorShapes->UpdateNavPoints();
		}
#endif
		bRefresh = false;
	}
};