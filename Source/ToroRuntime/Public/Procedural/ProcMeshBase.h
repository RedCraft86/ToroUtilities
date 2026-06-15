// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

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