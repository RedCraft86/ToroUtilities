// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "DataTypes/MeshProperties.h"

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

bool FStaticMeshProperties::FromMeshComponent(const UStaticMeshComponent* Target, const bool bIncludeTransform)
{
	if (!Target)
	{
		return false;
	}

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
	return true;
}

bool FStaticMeshProperties::ToMeshComponent(UStaticMeshComponent* Target) const
{
	if (!Target)
	{
		return false;
	}

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

	return true;
}

bool FStaticMeshProperties::Equals(const FStaticMeshProperties& Other, const bool bCheckTransform) const
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

bool FSplineMeshProperties::FromMeshComponent(const UStaticMeshComponent* Target, const bool bIncludeTransform)
{
	if (const USplineMeshComponent* SplineMesh = Cast<USplineMeshComponent>(Target))
	{
		ForwardAxis = SplineMesh->GetForwardAxis();
	}
	else
	{
		return false;
	}

	return FStaticMeshProperties::FromMeshComponent(Target, bIncludeTransform);
}

bool FSplineMeshProperties::ToMeshComponent(UStaticMeshComponent* Target) const
{
	if (USplineMeshComponent* SplineMesh = Cast<USplineMeshComponent>(Target))
	{
		SplineMesh->SetForwardAxis(ForwardAxis);
	}
	else
	{
		return false;
	}

	return FStaticMeshProperties::ToMeshComponent(Target);
}

bool FSplineMeshProperties::Equals(const FSplineMeshProperties& Other, const bool bCheckTransform) const
{
	return ForwardAxis == Other.ForwardAxis && Equals(static_cast<FStaticMeshProperties>(Other), bCheckTransform);
}
