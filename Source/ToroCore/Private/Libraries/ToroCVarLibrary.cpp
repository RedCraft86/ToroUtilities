// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "Libraries/ToroCVarLibrary.h"

void UToroCVarLibrary::SetCVarBool(const FString& InName, const bool InValue)
{
	if (IConsoleVariable* CVar = FindCVar(InName))
	{
		CVar->SetWithCurrentPriority(InValue, NAME_None, MaxPriority, MinPriority);
	}
}

void UToroCVarLibrary::SetCVarInt(const FString& InName, const int32 InValue)
{
	if (IConsoleVariable* CVar = FindCVar(InName))
	{
		CVar->SetWithCurrentPriority(InValue, NAME_None, MaxPriority, MinPriority);
	}
}

void UToroCVarLibrary::SetCVarFloat(const FString& InName, const float InValue)
{
	if (IConsoleVariable* CVar = FindCVar(InName))
	{
		CVar->SetWithCurrentPriority(InValue, NAME_None, MaxPriority, MinPriority);
	}
}

void UToroCVarLibrary::SetCVarString(const FString& InName, const FString& InValue)
{
	if (IConsoleVariable* CVar = FindCVar(InName))
	{
		CVar->SetWithCurrentPriority(*InValue, NAME_None, MaxPriority, MinPriority);
	}
}

bool UToroCVarLibrary::GetCVarBool(const FString& InName, const bool bInDefault)
{
	const IConsoleVariable* CVar = FindCVar(InName);
	return CVar ? CVar->GetBool() : bInDefault;
}

int32 UToroCVarLibrary::GetCVarInt(const FString& InName, const int32 InDefault)
{
	const IConsoleVariable* CVar = FindCVar(InName);
	return CVar ? CVar->GetInt() : InDefault;
}

float UToroCVarLibrary::GetCVarFloat(const FString& InName, const float InDefault)
{
	const IConsoleVariable* CVar = FindCVar(InName);
	return CVar ? CVar->GetFloat() : InDefault;
}

FString UToroCVarLibrary::GetCVarString(const FString& InName, const FString& InDefault)
{
	const IConsoleVariable* CVar = FindCVar(InName);
	return CVar ? CVar->GetString() : InDefault;
}

IConsoleVariable* UToroCVarLibrary::FindCVar(const FString& InName)
{
	return IConsoleManager::Get().FindConsoleVariable(*InName);
}
