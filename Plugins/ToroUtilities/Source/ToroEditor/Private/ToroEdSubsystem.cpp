// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroEdSubsystem.h"
#include "ToroEditorSettings.h"

void UToroEdSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	const UToroEditorSettings* Settings = UToroEditorSettings::Get();
	for (const TPair<FString, FString>& Command : Settings->StartupCommands)
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(this,
			*FString::Printf(TEXT("%s %s"), *Command.Key, *Command.Value));
	}
}
