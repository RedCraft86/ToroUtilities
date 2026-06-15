// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

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
		return false;
	}

	FCollisionResponseTemplate Template;
	if (Utils::GetProfile(InName, &Template))
	{
		ProfileName = Template.Name;
		CollisionEnabled = Template.CollisionEnabled;
		ObjectType = Template.ObjectType;

		Utils::ForEachChannel([this, Template](const ECollisionChannel Channel, const int32, const UEnum*)
		{
			ResponsesMap.Add(Channel, Template.ResponseToChannels.GetResponse(Channel));
		}, false);

		return true;
	}

	ProfileName = UCollisionProfile::CustomCollisionProfileName;
	FillDefaultResponses();
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

void FPrimitiveCollision::SetAllResponses(const ECollisionResponse InResponse)
{
	ProfileName = UCollisionProfile::CustomCollisionProfileName;
	Utils::ForEachChannel([this, InResponse](const ECollisionChannel Channel, const int32, const UEnum*)
	{
		ResponsesMap.Add(Channel, InResponse);
	}, false);
}

void FPrimitiveCollision::SetResponse(const ECollisionChannel InChannel, const ECollisionResponse InResponse)
{
	ProfileName = UCollisionProfile::CustomCollisionProfileName;
	ResponsesMap.Add(InChannel, InResponse);
}

ECollisionResponse FPrimitiveCollision::GetResponse(const ECollisionChannel InChannel) const
{
	return ResponsesMap.FindRef(InChannel, ECR_Ignore);
}

void FPrimitiveCollision::FillDefaultResponses(const ECollisionResponse InResponses, const bool bResetAll)
{
	Utils::ForEachChannel([this, InResponses, bResetAll](const ECollisionChannel Channel, const int32, const UEnum*)
	{
		if (!ResponsesMap.Contains(Channel) || bResetAll)
		{
			ResponsesMap.Add(Channel, InResponses);
		}
	}, false);
}

void FPrimitiveCollision::SetResponses(const TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& Map)
{
	ProfileName = UCollisionProfile::CustomCollisionProfileName;
	ResponsesMap = Map;
}

void FPrimitiveCollision::FromPrimitiveComponent(const UPrimitiveComponent* Target)
{
	if (Target && !SetProfileName(Target->GetCollisionProfileName()))
	{
		ProfileName = UCollisionProfile::CustomCollisionProfileName;
		CollisionEnabled = Target->GetCollisionEnabled();
		ObjectType = Target->GetCollisionObjectType();

		Utils::ForEachChannel([this, Target](const ECollisionChannel Channel, const int32, const UEnum*)
		{
			ResponsesMap.Add(Channel, Target->GetCollisionResponseToChannel(Channel));
		}, false);
	}
}

void FPrimitiveCollision::ToPrimitiveComponent(UPrimitiveComponent* Target) const
{
	if (!Target) return;
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
			Target->SetCollisionResponseToChannel(Channel, ResponsesMap.FindRef(Channel, ECR_Ignore));
		}, false);
	}
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
