// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Libraries/ToroConsoleLibrary.h"

void UToroConsoleLibrary::SetCVarBool(const FString& InName, const bool InValue)
{
	if (IConsoleVariable* CVar = FindCVar(InName))
	{
		CVar->SetWithCurrentPriority(InValue, NAME_None, MaxPriority, MinPriority);
	}
}

void UToroConsoleLibrary::SetCVarInt(const FString& InName, const int32 InValue)
{
	if (IConsoleVariable* CVar = FindCVar(InName))
	{
		CVar->SetWithCurrentPriority(InValue, NAME_None, MaxPriority, MinPriority);
	}
}

void UToroConsoleLibrary::SetCVarFloat(const FString& InName, const float InValue)
{
	if (IConsoleVariable* CVar = FindCVar(InName))
	{
		CVar->SetWithCurrentPriority(InValue, NAME_None, MaxPriority, MinPriority);
	}
}

void UToroConsoleLibrary::SetCVarString(const FString& InName, const FString& InValue)
{
	if (IConsoleVariable* CVar = FindCVar(InName))
	{
		CVar->SetWithCurrentPriority(*InValue, NAME_None, MaxPriority, MinPriority);
	}
}

bool UToroConsoleLibrary::GetCVarBool(const FString& InName)
{
	const IConsoleVariable* CVar = FindCVar(InName);
	return CVar ? CVar->GetBool() : false;
}

int32 UToroConsoleLibrary::GetCVarInt(const FString& InName)
{
	const IConsoleVariable* CVar = FindCVar(InName);
	return CVar ? CVar->GetInt() : 0;
}

float UToroConsoleLibrary::GetCVarFloat(const FString& InName)
{
	const IConsoleVariable* CVar = FindCVar(InName);
	return CVar ? CVar->GetFloat() : 0.0f;
}

FString UToroConsoleLibrary::GetCVarString(const FString& InName)
{
	const IConsoleVariable* CVar = FindCVar(InName);
	return CVar ? CVar->GetString() : FString();
}

IConsoleVariable* UToroConsoleLibrary::FindCVar(const FString& InName)
{
	return IConsoleManager::Get().FindConsoleVariable(*InName);
}
