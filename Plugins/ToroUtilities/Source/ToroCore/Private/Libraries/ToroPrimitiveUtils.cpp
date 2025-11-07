// Copyright (C) RedCraft86. All Rights Reserved.

#include "Libraries/ToroPrimitiveUtils.h"
#include "Helpers/CollisionHelpers.h"

void UToroPrimitiveUtils::FillMaterials(FStaticMeshData& Target)
{
	Target.FillMaterials();
}

bool UToroPrimitiveUtils::IsValid_MeshData(const FStaticMeshData& Target)
{
	return Target.IsValidData();
}

FStaticMeshData UToroPrimitiveUtils::GetStaticMeshData(const UStaticMeshComponent* Target)
{
	FStaticMeshData Result;
	Result.FromStaticMeshComponent(Target);
	return Result;
}

void UToroPrimitiveUtils::SetStaticMeshData(UStaticMeshComponent* Target, const FStaticMeshData& Data)
{
	Data.ToStaticMeshComponent(Target);
}

FPrimitiveCollision UToroPrimitiveUtils::Make_PrimitiveCollision(const FName InProfileName,
	const TEnumAsByte<ECollisionEnabled::Type> InCollisionEnabled, const TEnumAsByte<ECollisionChannel> InObjectType,
	const TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& InResponses)
{
	return CollisionHelpers::IsValidProfile(InProfileName)
		? FPrimitiveCollision(InProfileName)
		: FPrimitiveCollision(InCollisionEnabled, InObjectType, InResponses);
}

void UToroPrimitiveUtils::Break_PrimitiveCollision(const FPrimitiveCollision& Target, FName& ProfileName,
	TEnumAsByte<ECollisionEnabled::Type>& CollisionEnabled, TEnumAsByte<ECollisionChannel>& ObjectType,
	TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& Responses)
{
	ProfileName = Target.GetProfileName();
	CollisionEnabled = Target.GetCollisionEnabled();
	ObjectType = Target.GetObjectType();
	Responses = Target.GetResponsesMap();
}

void UToroPrimitiveUtils::SetCollisionProfileName(FPrimitiveCollision& Target, const FName InProfileName)
{
	Target.SetProfileName(InProfileName);
}

FName UToroPrimitiveUtils::GetCollisionProfileName(const FPrimitiveCollision& Target)
{
	return Target.GetProfileName();
}

void UToroPrimitiveUtils::SetAllCollisionResponses(FPrimitiveCollision& Target, const ECollisionResponse InResponse)
{
	Target.SetAllResponses(InResponse);
}

void UToroPrimitiveUtils::SetCollisionResponse(FPrimitiveCollision& Target, const ECollisionChannel InChannel, const ECollisionResponse InResponse)
{
	Target.SetResponse(InChannel, InResponse);
}

ECollisionResponse UToroPrimitiveUtils::GetCollisionResponse(const FPrimitiveCollision& Target, const ECollisionChannel InChannel)
{
	return Target.GetResponse(InChannel);
}

void UToroPrimitiveUtils::SetCollisionEnabled(FPrimitiveCollision& Target, const ECollisionEnabled::Type InEnabled)
{
	Target.SetCollisionEnabled(InEnabled);
}

ECollisionEnabled::Type UToroPrimitiveUtils::GetCollisionEnabled(const FPrimitiveCollision& Target)
{
	return Target.GetCollisionEnabled();
}

void UToroPrimitiveUtils::SetCollisionObjectType(FPrimitiveCollision& Target, const ECollisionChannel InObjectType)
{
	Target.SetObjectType(InObjectType);
}

ECollisionChannel UToroPrimitiveUtils::GetCollisionObjectType(const FPrimitiveCollision& Target)
{
	return Target.GetObjectType();
}

FPrimitiveCollision UToroPrimitiveUtils::GetPrimitiveCollision(const UPrimitiveComponent* Target)
{
	FPrimitiveCollision Result;
	Result.FromPrimitiveComponent(Target);
	return Result;
}

void UToroPrimitiveUtils::SetPrimitiveCollision(UPrimitiveComponent* Target, const FPrimitiveCollision& Data)
{
	Data.ToPrimitiveComponent(Target);
}
