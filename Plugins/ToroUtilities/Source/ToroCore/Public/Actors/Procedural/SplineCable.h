// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSplineGenBase.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "SplineCable.generated.h"

UCLASS(NotBlueprintable)
class TOROCORE_API ASplineCable final : public AToroSplineGenBase
{
	GENERATED_BODY()

public:

	ASplineCable(): AttachmentCount(0), AttachmentLoopMode(EGeneratorLoopMode::Forward) {}

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TSet<TObjectPtr<USplineMeshComponent>> SplineMeshComponents;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TArray<TObjectPtr<UInstancedStaticMeshComponent>> AttachMeshComponents;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UStaticMeshComponent> StartMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UStaticMeshComponent> EndMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Settings")
		FPrimitiveCollision Collision;
	
	UPROPERTY(EditAnywhere, Category = "Settings|Cable")
		FSplineMeshData CableMesh;

	UPROPERTY(EditAnywhere, Category = "Settings|Cable")
		FTransformMeshData StartCap;

	UPROPERTY(EditAnywhere, Category = "Settings|Cable")
		FTransformMeshData EndCap;

	UPROPERTY(EditAnywhere, Category = "Settings|Attachments")
		uint8 AttachmentCount;

	UPROPERTY(EditAnywhere, Category = "Settings|Attachments")
		EGeneratorLoopMode AttachmentLoopMode;

	UPROPERTY(EditAnywhere, Category = "Settings|Attachments")
		TArray<FTransformMeshData> AttachmentSample;

protected:

	TArray<int32> AttachmentIndices;

	void DestroyComponents();
	virtual void Construct() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
