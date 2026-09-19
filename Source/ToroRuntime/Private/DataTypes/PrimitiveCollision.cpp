// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "DataTypes/PrimitiveCollision.h"

bool FPrimitiveCollision::Equals(const FPrimitiveCollision& Other) const
{
	if (CollisionEnabled != Other.CollisionEnabled
		|| ObjectType != Other.ObjectType
		|| Responses.Num() != Other.Responses.Num())
	{
		return false;
	}

	for (const TPair<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& ChannelToResponse : Responses)
	{
		if (!Other.Responses.Contains(ChannelToResponse.Key)
			|| ChannelToResponse.Value != Other.Responses[ChannelToResponse.Key])
		{
			return false;
		}
	}

	return true;
}

bool FPrimitiveCollision::HasValidProfileName() const
{
	return Utils::GetProfileData(ProfileName);
}

bool FPrimitiveCollision::SetProfileName(const FName& InName)
{
	FCollisionResponseTemplate Template;
	if (Utils::GetProfileData(InName, &Template))
	{
		ProfileName = Template.Name;
		CollisionEnabled = Template.CollisionEnabled;
		ObjectType = Template.ObjectType;

		Responses.Reset();
		Utils::ForEachChannel([this, Template](const ECollisionChannel Channel, const int32, const UEnum*)
		{
			Responses.Add(Channel, Template.ResponseToChannels.GetResponse(Channel));
		}, false);

		return true;
	}

	FillResponses();
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

void FPrimitiveCollision::FillResponses(const ECollisionResponse InResponse)
{
	ProfileName = UCollisionProfile::CustomCollisionProfileName;
	Utils::ForEachChannel([this, InResponse](const ECollisionChannel Channel, const int32, const UEnum*)
	{
		if (!Responses.Contains(Channel))
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

void FPrimitiveCollision::FromPrimitiveComponent(const UPrimitiveComponent* Target)
{
	if (Target)
	{
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
	}
}

void FPrimitiveCollision::ToPrimitiveComponent(UPrimitiveComponent* Target) const
{
	if (Target)
	{
		if (HasValidProfileName())
		{
			Target->SetCollisionProfileName(ProfileName);
		}
		else
		{
			Target->SetCollisionEnabled(CollisionEnabled);
			Target->SetCollisionObjectType(ObjectType);

			Utils::ForEachChannel([this, Target](const ECollisionChannel Channel, const int32, const UEnum*)
			{
				const TEnumAsByte<ECollisionResponse>* Response = Responses.Find(Channel);
				Target->SetCollisionResponseToChannel(Channel, Response ? Response->GetValue() : ECR_Ignore);
			}, false);
		}
	}
}

bool FPrimitiveCollision::Utils::GetProfileData(const FName& ProfileName, FCollisionResponseTemplate* OutData)
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

	FCollisionResponseTemplate Unused;
	return Profile->GetProfileTemplate(ProfileName, Unused);
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

void UPrimitiveCollisionLibrary::SetCollisionProfileName(FPrimitiveCollision& Settings, const FName ProfileName)
{
	Settings.SetProfileName(ProfileName);
}

FName UPrimitiveCollisionLibrary::GetCollisionProfileName(const FPrimitiveCollision& Settings)
{
	return Settings.GetProfileName();
}

void UPrimitiveCollisionLibrary::SetCollisionEnabled(FPrimitiveCollision& Settings, const ECollisionEnabled::Type InEnabled)
{
	Settings.SetCollisionEnabled(InEnabled);
}

ECollisionEnabled::Type UPrimitiveCollisionLibrary::GetCollisionEnabled(const FPrimitiveCollision& Settings)
{
	return Settings.GetCollisionEnabled();
}

void UPrimitiveCollisionLibrary::SetCollisionObjectType(FPrimitiveCollision& Settings, const ECollisionChannel InObjectType)
{
	Settings.SetObjectType(InObjectType);
}

ECollisionChannel UPrimitiveCollisionLibrary::GetCollisionObjectType(const FPrimitiveCollision& Settings)
{
	return Settings.GetObjectType();
}

void UPrimitiveCollisionLibrary::SetCollisionResponseToAllChannels(FPrimitiveCollision& Settings, const ECollisionResponse InResponse)
{
	Settings.SetAllResponses(InResponse);
}

void UPrimitiveCollisionLibrary::SetCollisionResponseToChannel(FPrimitiveCollision& Settings, const ECollisionChannel InChannel, const ECollisionResponse InResponse)
{
	Settings.SetResponse(InChannel, InResponse);
}

ECollisionResponse UPrimitiveCollisionLibrary::GetCollisionResponseToChannel(const FPrimitiveCollision& Settings, const ECollisionChannel InChannel)
{
	return Settings.GetResponse(InChannel);
}

void UPrimitiveCollisionLibrary::GetPrimitiveCollision(FPrimitiveCollision& OutData, const UPrimitiveComponent* Target)
{
	OutData.FromPrimitiveComponent(Target);
}

void UPrimitiveCollisionLibrary::SetPrimitiveCollision(UPrimitiveComponent* Target, FPrimitiveCollision& Settings)
{
	Settings.ToPrimitiveComponent(Target);
}
