// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataTypes/PrimitiveData.h"
#include "Helpers/CollisionHelpers.h"

void FStaticMeshData::FillMaterials()
{
	if (Mesh.IsNull())
	{
		Materials.Empty();
	}
	else if (UStaticMesh* SM = Mesh.LoadSynchronous())
	{
		if (const TArray<FStaticMaterial>& Mats = SM->GetStaticMaterials(); Materials.Num() < Mats.Num())
		{
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
	}
}

void FStaticMeshData::FromStaticMeshComponent(const UStaticMeshComponent* InComponent)
{
	if (IsValid(InComponent))
	{
		Mesh = InComponent->GetStaticMesh();
		bCastShadows = InComponent->CastShadow;
		TArray<UMaterialInterface*> Mats = InComponent->GetMaterials();
		Materials.Empty(Mats.Num());
		for (UMaterialInterface* Material : Mats)
		{
			Materials.Add(TObjectPtr<UMaterialInterface>(Material));
		}

		FillMaterials();
	}
}

void FStaticMeshData::ToStaticMeshComponent(UStaticMeshComponent* InComponent) const
{
	if (IsValid(InComponent))
	{
		FStaticMeshData TempCopy = *this;
		TempCopy.FillMaterials();
		InComponent->SetStaticMesh(TempCopy.Mesh.LoadSynchronous());
		InComponent->SetCastShadow(TempCopy.bCastShadows);
		for (int i = 0; i < TempCopy.Materials.Num(); i++)
		{
			InComponent->SetMaterial(i, TempCopy.Materials[i].LoadSynchronous());
		}
	}
}

void FPrimitiveCollision::SetProfileName(const FName& InName)
{
	const UCollisionProfile* Profile = UCollisionProfile::Get();
	if (FCollisionResponseTemplate ProfileData; Profile->GetProfileTemplate(InName, ProfileData))
	{
		CollisionProfileName = ProfileData.Name;
		CollisionEnabled = ProfileData.CollisionEnabled;
		ObjectType = ProfileData.ObjectType;
		CollisionHelpers::ForEachChannel([this, ProfileData](const ECollisionChannel Channel)
		{
			Responses.Add(Channel, ProfileData.ResponseToChannels.GetResponse(Channel));
		}, false);
	}
	else
	{
		CollisionProfileName = UCollisionProfile::CustomCollisionProfileName;
	}

	FillResponsesMap();
}

void FPrimitiveCollision::SetCollisionEnabled(const ECollisionEnabled::Type InCollisionEnabled)
{
	CollisionProfileName = UCollisionProfile::CustomCollisionProfileName;
	CollisionEnabled = InCollisionEnabled;
}

void FPrimitiveCollision::SetObjectType(const ECollisionChannel InObjectType)
{
	CollisionProfileName = UCollisionProfile::CustomCollisionProfileName;
	ObjectType = InObjectType;
}

void FPrimitiveCollision::SetAllResponses(const ECollisionResponse InResponse)
{
	CollisionProfileName = UCollisionProfile::CustomCollisionProfileName;
	CollisionHelpers::ForEachChannel([this, InResponse](const ECollisionChannel Channel)
	{
		Responses.Add(Channel, InResponse);
	}, false);
}

void FPrimitiveCollision::SetResponse(const ECollisionChannel InChannel, const ECollisionResponse InResponse)
{
	CollisionProfileName = UCollisionProfile::CustomCollisionProfileName;
	Responses.Add(InChannel, InResponse);
}

ECollisionResponse FPrimitiveCollision::GetResponse(const ECollisionChannel InChannel) const
{
	return Responses.Contains(InChannel) ? (ECollisionResponse)Responses.FindRef(InChannel) : ECR_Ignore;
}

void FPrimitiveCollision::FillResponsesMap(const ECollisionResponse InResponses)
{
	CollisionHelpers::ForEachChannel([this, InResponses](const ECollisionChannel Channel)
	{
		if (!Responses.Contains(Channel))
		{
			Responses.Add(Channel, CollisionHelpers::IsHiddenChannel(Channel) ? ECR_Ignore : InResponses);
		}
	}, false);
}

void FPrimitiveCollision::FromPrimitiveComponent(const UPrimitiveComponent* InComponent)
{
	if (!IsValid(InComponent)) return;
	if (CollisionHelpers::IsValidProfile(InComponent->GetCollisionProfileName()))
	{
		SetProfileName(InComponent->GetCollisionProfileName());
#if !WITH_EDITOR
	}
	else
	{
#else
		CollisionProfileName = UCollisionProfile::CustomCollisionProfileName;
#endif
		CollisionEnabled = InComponent->GetCollisionEnabled();
		ObjectType = InComponent->GetCollisionObjectType();
		CollisionHelpers::ForEachChannel([this, InComponent](const ECollisionChannel Channel)
		{
			Responses.Add(Channel, InComponent->GetCollisionResponseToChannel(Channel));
		}, false);
	}
}

void FPrimitiveCollision::ToPrimitiveComponent(UPrimitiveComponent* InComponent) const
{
	if (!IsValid(InComponent)) return;
	if (CollisionHelpers::IsValidProfile(CollisionProfileName))
	{
		InComponent->SetCollisionProfileName(CollisionProfileName);
#if !WITH_EDITOR
	}
	else
	{
#endif
		InComponent->SetCollisionEnabled(CollisionEnabled);
		InComponent->SetCollisionObjectType(ObjectType);
		CollisionHelpers::ForEachChannel([this, InComponent](const ECollisionChannel Channel)
		{
			InComponent->SetCollisionResponseToChannel(Channel, Responses.FindRef(Channel));
		}, false);
	}
}
