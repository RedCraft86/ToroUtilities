// Copyright (C) RedCraft86. All Rights Reserved.

#include "Libraries/ToroConsoleLibrary.h"

TMap<FString, IConsoleVariable*> UToroConsoleLibrary::CachedCVars = {};

void UToroConsoleLibrary::SetCVarBool(const FString& InName, const bool InValue)
{
	if (IConsoleVariable* CVar = FindCVar(InName))
	{
		CVar->SetWithCurrentPriority(InValue);
	}
}

void UToroConsoleLibrary::SetCVarInt(const FString& InName, const int32 InValue)
{
	if (IConsoleVariable* CVar = FindCVar(InName))
	{
		CVar->SetWithCurrentPriority(InValue);
	}
}

void UToroConsoleLibrary::SetCVarFloat(const FString& InName, const float InValue)
{
	if (IConsoleVariable* CVar = FindCVar(InName))
	{
		CVar->SetWithCurrentPriority(InValue);
	}
}

void UToroConsoleLibrary::SetCVarString(const FString& InName, const FString& InValue)
{
	if (IConsoleVariable* CVar = FindCVar(InName))
	{
		CVar->SetWithCurrentPriority(*InValue);
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
	return CVar ? CVar->GetString() : TEXT("");
}

IConsoleVariable* UToroConsoleLibrary::FindCVar(const FString& InName)
{
	if (IConsoleVariable** CVar = CachedCVars.Find(InName); CVar && *CVar)
	{
		return *CVar;
	}
	if (IConsoleVariable* NewCVar = IConsoleManager::Get().FindConsoleVariable(*InName))
	{
		CachedCVars.Add(InName, NewCVar);
		return NewCVar;
	}
	return nullptr;
}
