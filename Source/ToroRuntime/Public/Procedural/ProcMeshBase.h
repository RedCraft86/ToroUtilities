// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "ProceduralBase.h"
#include "ProceduralMeshComponent.h"
#include "ProcMeshBase.generated.h"

/**
 * Specialized procedural actor that includes a Procedural Mesh Component.
 */
UCLASS(Abstract)
class TORORUNTIME_API AProcMeshBase : public AProceduralBase
{
	GENERATED_BODY()

public:

	AProcMeshBase();

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UProceduralMeshComponent> MeshComponent;

#if WITH_EDITOR
	virtual void BakeActor() override;
#endif
};