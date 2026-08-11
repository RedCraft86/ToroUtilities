// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "DataTypes/PrimitiveCollision.h"
#include "Components/PrimitiveComponent.h"

bool FPrimitiveCollision::HasValidProfileName() const
{
	return Utils::GetProfile(ProfileName);
}

bool FPrimitiveCollision::SetProfileName(const FName& InName)
{
	if (InName == ProfileName)
	{
		return true;
	}

	FCollisionResponseTemplate Template;
	if (Utils::GetProfile(InName, &Template))
	{
		ProfileName = Template.Name;
		CollisionEnabled = Template.CollisionEnabled;
		ObjectType = Template.ObjectType;

		Utils::ForEachChannel([this, Template](const ECollisionChannel Channel, const int32, const UEnum*)
		{
			Responses.Add(Channel, Template.ResponseToChannels.GetResponse(Channel));
		}, false);

		return true;
	}

	FillAllResponses();
	return false;
}

void FPrimitiveCollision::SetCollisionEnabled(const ECollisionEnabled::Type InEnabled)
{
	ProfileName = UCollisionProfile::CustomCollisionProfileName;
	CollisionEnabled = InEnabled;
}

void FPrimitiveCollision::SetObjectType(const ECollisionChannel InObjectType)
{
	ProfileName = UCollisionProfile::CustomCollisionProfileName;
	ObjectType = InObjectType;
}

void FPrimitiveCollision::SetResponse(const ECollisionChannel InChannel, const ECollisionResponse InResponse)
{
	ProfileName = UCollisionProfile::CustomCollisionProfileName;
	Responses.Add(InChannel, InResponse);
}

void FPrimitiveCollision::SetAllResponses(const ECollisionResponse InResponse)
{
	ProfileName = UCollisionProfile::CustomCollisionProfileName;
	Utils::ForEachChannel([this, InResponse](const ECollisionChannel Channel, const int32, const UEnum*)
	{
		Responses.Add(Channel, InResponse);
	}, false);
}

void FPrimitiveCollision::FillAllResponses(const ECollisionResponse InResponse, const bool bResetAll)
{
	ProfileName = UCollisionProfile::CustomCollisionProfileName;
	Utils::ForEachChannel([this, InResponse, bResetAll](const ECollisionChannel Channel, const int32, const UEnum*)
	{
		if (!Responses.Contains(Channel) || bResetAll)
		{
			Responses.Add(Channel, InResponse);
		}
	}, false);
}

void FPrimitiveCollision::SetResponses(const TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& InResponses)
{
	ProfileName = UCollisionProfile::CustomCollisionProfileName;
	Responses = InResponses;
}

bool FPrimitiveCollision::FromPrimitiveComponent(const UPrimitiveComponent* Target)
{
	if (!Target)
	{
		return false;
	}

	if (!SetProfileName(Target->GetCollisionProfileName()))
	{
		ProfileName = UCollisionProfile::CustomCollisionProfileName;
		CollisionEnabled = Target->GetCollisionEnabled();
		ObjectType = Target->GetCollisionObjectType();

		Utils::ForEachChannel([this, Target](const ECollisionChannel Channel, const int32, const UEnum*)
		{
			Responses.Add(Channel, Target->GetCollisionResponseToChannel(Channel));
		}, false);
	}

	return true;
}

bool FPrimitiveCollision::ToPrimitiveComponent(UPrimitiveComponent* Target) const
{
	if (!Target)
	{
		return false;
	}

	if (Utils::GetProfile(ProfileName))
	{
		Target->SetCollisionProfileName(ProfileName);
	}
	else
	{
		Target->SetCollisionEnabled(CollisionEnabled);
		Target->SetCollisionObjectType(ObjectType);

		Utils::ForEachChannel([this, Target](const ECollisionChannel Channel, const int32, const UEnum*)
		{
			Target->SetCollisionResponseToChannel(Channel, Responses.FindRef(Channel, ECR_Ignore));
		}, false);
	}

	return true;
}

bool FPrimitiveCollision::Equals(const FPrimitiveCollision& Other) const
{
	if (CollisionEnabled != Other.CollisionEnabled
		|| ObjectType != Other.ObjectType
		|| Responses.Num() != Other.Responses.Num())
	{
		return false;
	}

	for (const TPair<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& Response : Responses)
	{
		if (!Other.Responses.Contains(Response.Key) || Response.Value != Other.Responses[Response.Key])
		{
			return false;
		}
	}

	return true;
}

bool FPrimitiveCollision::Utils::GetProfile(const FName& ProfileName, FCollisionResponseTemplate* OutData)
{
	if (ProfileName.IsNone() || ProfileName == UCollisionProfile::CustomCollisionProfileName)
	{
		return false;
	}

	const UCollisionProfile* Profile = UCollisionProfile::Get();
	if (!Profile)
	{
		return false;
	}

	if (OutData)
	{
		return Profile->GetProfileTemplate(ProfileName, *OutData);
	}

	FCollisionResponseTemplate Template;
	return Profile->GetProfileTemplate(ProfileName, Template);
}

void FPrimitiveCollision::Utils::ForEachChannel(const TFunction<void(const ECollisionChannel, const int32, const UEnum*)>& Func, const bool bIgnoreHidden)
{
	if (const UEnum* EnumPtr = StaticEnum<ECollisionChannel>())
	{
		for (int32 i = 0; i < EnumPtr->NumEnums(); i++)
		{
#if WITH_EDITOR
			if (bIgnoreHidden && EnumPtr->HasMetaData(TEXT("Hidden"), i))
			{
				continue;
			}
#endif

			const TEnumAsByte<ECollisionChannel> EnumVal(i);
			if (EnumVal != ECC_OverlapAll_Deprecated && EnumVal != ECC_MAX && Func)
			{
				Func(EnumVal, i, EnumPtr);
			}
		}
	}
}
