// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroProcGenBase.h"
#include "ProceduralMeshComponent.h"
#include "ToroProcMeshBase.generated.h"

UCLASS(Abstract, NotBlueprintable)
class TOROCORE_API AToroProcMeshBase : public AToroProcGenBase
{
	GENERATED_BODY()

public:

	AToroProcMeshBase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UProceduralMeshComponent> ProceduralMesh;

#if WITH_EDITOR
protected:
	TSet<int32> EditorSections = {1};
	virtual void BakeInternal(const bool bRemoveSource, const bool bSelectActors, TArray<AActor*>* OutActors) override;
#endif
};
