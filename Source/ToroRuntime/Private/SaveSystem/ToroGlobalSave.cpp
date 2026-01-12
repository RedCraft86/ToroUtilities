// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "SaveSystem/ToroGlobalSave.h"
#include "Helpers/WindowsHelpers.h"
#include "ToroRuntime.h"

UToroGlobalSave::UToroGlobalSave()
{
	bSingleSlot = true;
}

void UToroGlobalSave::DeleteData()
{
#if !WITH_EDITOR
	WindowsHelpers::OpenDialogue(TEXT("Report this to the dev!"),
		TEXT("Attempting to delete global persistent data, which should not be possible."),
		EWindowsDialogueType::Ok, EWindowsDialogueIcon::Error);
#else
	UE_LOG(LogToroRuntime, Error, TEXT("Cannot delete global persistent data!"));
#endif
}

void UToroGlobalSave::SerializeData(FArchive& Ar)
{
	Ar << Cutscenes;
	Ar << Tutorials;
	Ar << Themes;
	Ar << Content;
}
