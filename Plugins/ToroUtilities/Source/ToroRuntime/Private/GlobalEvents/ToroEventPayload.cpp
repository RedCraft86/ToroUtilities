// Copyright (C) RedCraft86. All Rights Reserved.

#include "GlobalEvents/ToroEventPayload.h"

void UToroEventPayload::AddString(const FName Key, const FString& Value)
{
	Strings.Add(Key, Value);
}

void UToroEventPayload::AddFloat(const FName Key, const float Value)
{
	Floats.Add(Key, Value);
}

FString UToroEventPayload::GetString(const FName Key) const
{
	return Strings.FindRef(Key);
}

float UToroEventPayload::GetFloat(const FName Key) const
{
	return Floats.FindRef(Key);
}
