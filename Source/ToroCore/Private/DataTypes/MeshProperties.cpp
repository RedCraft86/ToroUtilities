// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "DataTypes/MeshProperties.h"
#include "Engine/StaticMesh.h"

void FStaticMeshProperties::FillEmptyMaterials()
{
	if (UStaticMesh* SM = StaticMesh.LoadSynchronous())
	{
		const TArray<FStaticMaterial>& Mats = SM->GetStaticMaterials();
		Materials.Reserve(Mats.Num());

		for (int32 i = 0; i < Mats.Num(); i++)
		{
			if (!Materials.IsValidIndex(i))
			{
				Materials.Add(Mats[i].MaterialInterface);
			}
			else if (Materials[i].IsNull())
			{
				Materials[i] = Mats[i].MaterialInterface;
			}
		}
	}
	else
	{
		Materials.Empty();
	}
}

void FStaticMeshProperties::FromMeshComponent(const UStaticMeshComponent* Target, const bool bIncludeTransform)
{
	if (Target)
	{
		StaticMesh = Target->GetStaticMesh();
		bCastShadows = Target->CastShadow;
		bUseTransform = bIncludeTransform;
		Transform = bUseTransform ? Target->GetComponentTransform() : FTransform::Identity;
		OverlayMaterial = Target->GetOverlayMaterial();

		Materials.Empty(Target->GetNumMaterials());
		TArray<UMaterialInterface*> Mats = Target->GetMaterials();
		for (UMaterialInterface* Mat : Mats)
		{
			Materials.Add(Mat);
		}

		FillEmptyMaterials();
	}
}

void FStaticMeshProperties::ToMeshComponent(UStaticMeshComponent* Target)
{
	if (Target)
	{
		FillEmptyMaterials();
		Target->SetStaticMesh(StaticMesh.LoadSynchronous());
		Target->SetCastShadow(bCastShadows);
		if (bUseTransform)
		{
			Target->SetWorldTransform(Transform);
		}

		Target->SetOverlayMaterial(OverlayMaterial.LoadSynchronous());
		for (int32 i = 0; i < Materials.Num(); i++)
		{
			Target->SetMaterial(i, Materials[i].LoadSynchronous());
		}
	}
}

bool FStaticMeshProperties::EqualsTo(const FStaticMeshProperties& Other, const bool bCheckTransform) const
{
	if (StaticMesh != Other.StaticMesh 
		|| bCastShadows != Other.bCastShadows 
		|| OverlayMaterial != Other.OverlayMaterial
		|| Materials.Num() != Other.Materials.Num())
	{
		return false;
	}

	if (bCheckTransform && (bUseTransform != Other.bUseTransform || !Transform.Equals(Other.Transform)))
	{
		return false;
	}

	for (int32 i = 0; i < Materials.Num(); i++)
	{
		if (Materials[i] != Other.Materials[i])
		{
			return false;
		}
	}

	return true;
}

void FSplineMeshProperties::FromMeshComponent(const UStaticMeshComponent* Target, const bool bIncludeTransform)
{
	FStaticMeshProperties::FromMeshComponent(Target, bIncludeTransform);
	if (const USplineMeshComponent* SplineMesh = Cast<USplineMeshComponent>(Target))
	{
		ForwardAxis = SplineMesh->GetForwardAxis();
	}
}

void FSplineMeshProperties::ToMeshComponent(UStaticMeshComponent* Target)
{
	FStaticMeshProperties::ToMeshComponent(Target);
	if (USplineMeshComponent* SplineMesh = Cast<USplineMeshComponent>(Target))
	{
		SplineMesh->SetForwardAxis(ForwardAxis);
	}
}

bool FSplineMeshProperties::EqualsTo(const FSplineMeshProperties& Other, const bool bCheckTransform) const
{
	return ForwardAxis == Other.ForwardAxis && FStaticMeshProperties::EqualsTo(*Other, bCheckTransform);
}
