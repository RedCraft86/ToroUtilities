// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "MiscObjects/UDSSetterObject.h"
#include "Kismet/GameplayStatics.h"

namespace WeatherTags
{
	DEFINE_GAMEPLAY_TAG(Weather)
	DEFINE_GAMEPLAY_TAG_CHILD(Weather, ClearSkies)
	DEFINE_GAMEPLAY_TAG_CHILD(Weather, Cloudy)
	DEFINE_GAMEPLAY_TAG_CHILD(Weather, Overcast)
	DEFINE_GAMEPLAY_TAG_CHILD(Weather, PartlyCloudy)
	DEFINE_GAMEPLAY_TAG_CHILD(Weather, FogThick)
	DEFINE_GAMEPLAY_TAG_CHILD(Weather, FogThin)
	DEFINE_GAMEPLAY_TAG_CHILD(Weather, Rain)
	DEFINE_GAMEPLAY_TAG_CHILD(Weather, RainLight)
	DEFINE_GAMEPLAY_TAG_CHILD(Weather, RainFoggy)
	DEFINE_GAMEPLAY_TAG_CHILD(Weather, Thunderstorm)
}

void UUDSSetterObject::SetSettings(const UObject* ContextObject, const FUDSSettings& InSettings)
{
	if (AActor* Actor = UGameplayStatics::GetActorOfClass(ContextObject, TargetClass))
	{
		ApplySettings(Actor, InSettings);
	}
}
