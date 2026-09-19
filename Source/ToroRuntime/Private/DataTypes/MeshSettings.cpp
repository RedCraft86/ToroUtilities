// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "DataTypes/MeshSettings.h"

bool FStaticMeshSettings::Equals(const FStaticMeshSettings& Other, const bool bCheckTransform) const
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

void FStaticMeshSettings::FillEmptyMaterials()
{
	if (UStaticMesh* SM = StaticMesh.LoadSynchronous())
	{
		const TArray<FStaticMaterial>& Mats = SM->GetStaticMaterials();
		Materials.SetNum(Mats.Num());

		for (int32 i = 0; i < Mats.Num(); i++)
		{
			if (Materials[i].IsNull())
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

void FStaticMeshSettings::FromMeshComponent(const UStaticMeshComponent* Target, const bool bIncludeTransform)
{
	if (Target)
	{
		StaticMesh		= Target->GetStaticMesh();
		bCastShadows	= Target->CastShadow;
		bUseTransform	= bIncludeTransform;
		Transform		= bUseTransform ? Target->GetComponentTransform() : FTransform::Identity;
		OverlayMaterial	= Target->GetOverlayMaterial();

		TArray<UMaterialInterface*> Mats = Target->GetMaterials();
		Materials.SetNum(Mats.Num());

		for (int32 i = 0; i < Mats.Num(); i++)
		{
			Materials[i] = Mats[i];
		}

		FillEmptyMaterials();
	}
}

void FStaticMeshSettings::ToMeshComponent(UStaticMeshComponent* Target) const
{
	if (Target)
	{
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

bool FSplineMeshSettings::Equals(const FSplineMeshSettings& Other, const bool bCheckTransform) const
{
	return ForwardAxis == Other.ForwardAxis && Equals(static_cast<FStaticMeshSettings>(Other), bCheckTransform);
}

void FSplineMeshSettings::FromMeshComponent(const UStaticMeshComponent* Target, const bool bIncludeTransform)
{
	if (const USplineMeshComponent* SplineMesh = Cast<USplineMeshComponent>(Target))
	{
		ForwardAxis = SplineMesh->GetForwardAxis();
	}

	FStaticMeshSettings::FromMeshComponent(Target, bIncludeTransform);
}

void FSplineMeshSettings::ToMeshComponent(UStaticMeshComponent* Target) const
{
	FStaticMeshSettings::ToMeshComponent(Target);
	if (USplineMeshComponent* SplineMesh = Cast<USplineMeshComponent>(Target))
	{
		SplineMesh->SetForwardAxis(ForwardAxis);
	}
}

bool UMeshSettingsLibrary::IsValidStaticMeshSettings(const FStaticMeshSettings& Settings)
{
	return Settings.IsValid();
}

bool UMeshSettingsLibrary::IsStaticMeshSettingsEqual(const FStaticMeshSettings& A, const FStaticMeshSettings& B, const bool bCheckTransform)
{
	return A.Equals(B, bCheckTransform);
}

void UMeshSettingsLibrary::FillEmptyStaticMeshMaterials(FStaticMeshSettings& Settings)
{
	Settings.FillEmptyMaterials();
}

void UMeshSettingsLibrary::GetStaticMeshSettings(FStaticMeshSettings& OutData, const UStaticMeshComponent* Target, const bool bIncludeTransform)
{
	OutData.FromMeshComponent(Target, bIncludeTransform);
}

void UMeshSettingsLibrary::SetStaticMeshSettings(UStaticMeshComponent* Target, FStaticMeshSettings& Settings)
{
	Settings.ToMeshComponent(Target);
}

bool UMeshSettingsLibrary::IsValidSplineMeshSettings(const FSplineMeshSettings& Settings)
{
	return Settings.IsValid();
}

bool UMeshSettingsLibrary::IsSplineMeshSettingsEqual(const FSplineMeshSettings& A, const FSplineMeshSettings& B, const bool bCheckTransform)
{
	return A.Equals(B, bCheckTransform);
}

void UMeshSettingsLibrary::FillEmptySplineMeshMaterials(FSplineMeshSettings& Settings)
{
	Settings.FillEmptyMaterials();
}

void UMeshSettingsLibrary::GetSplineMeshSettings(FSplineMeshSettings& OutData, const USplineMeshComponent* Target, const bool bIncludeTransform)
{
	OutData.FromMeshComponent(Target, bIncludeTransform);
}

void UMeshSettingsLibrary::SetSplineMeshSettings(USplineMeshComponent* Target, FSplineMeshSettings& Settings)
{
	Settings.ToMeshComponent(Target);
}
