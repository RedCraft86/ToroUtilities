// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "DataTypes/MeshProperties.h"

/**
 * A utility class focused on actor-to-instance conversion and optimization.
 * 
 * This class facilitates the process of taking a selection of individual Actors 
 * and collapsing them into a single InstancedStaticMeshComponent (ISM) or 
 * HierarchicalInstancedStaticMeshComponent (HISM) to reduce draw calls.
 * 
 * All functions are Editor-Only.
 */
class TOROCORE_API FActorInstancing final
{
#if WITH_EDITOR
public:

	/** 
	 * Converts an array of individual Actors into optimized instances.
	 * @param Sources         The actors to be converted.
	 * @param bUseHISM        If true, uses Hierarchical ISMs (better for LODs).
	 * @param bDestroySources If true, original actors are removed once instanced.
	 * @return                The newly created Instanced Actors.
	 */
	static TArray<AActor*> InstanceActors(const TArray<AActor*>& Sources, const bool bUseHISM, const bool bDestroySources);

	/** 
	 * Analyzes a collection of components and groups them by their shared properties 
	 * (Mesh asset, materials, etc.) to prepare for instancing.
	 * @return A map where each unique mesh configuration points to its collection of world transforms.
	 */
	static TMap<FStaticMeshProperties, TArray<FTransform>> ExtractInstances(TArray<UStaticMeshComponent*>& Components);
#endif
};
