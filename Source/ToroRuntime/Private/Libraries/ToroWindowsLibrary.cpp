// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "Libraries/ToroWindowsLibrary.h"
#include "Framework/Application/SlateApplication.h"
#if PLATFORM_WINDOWS
#include "Windows/WindowsPlatformApplicationMisc.h"
#include "shlobj.h" // <- This fella causes serious errors if included in a .h due to conflicts with UE macros
#endif

void UToroWindowsLibrary::CopyStringToClipboard(const FString& Value)
{
#if PLATFORM_WINDOWS
	FWindowsPlatformApplicationMisc::ClipboardCopy(*Value);
#endif
}

FString UToroWindowsLibrary::GetStringFromClipboard()
{
	FString Content;
#if PLATFORM_WINDOWS
	FWindowsPlatformApplicationMisc::ClipboardPaste(Content);
#endif
	return Content;
}

EWinDialogueResult UToroWindowsLibrary::OpenDialogue(const FString& Title, const FString& Message,
	const EWinDialogueType Type, const EWinDialogueIcon Icon)
{
	EWinDialogueResult Result = EWinDialogueResult::None;
#if PLATFORM_WINDOWS
	Result = static_cast<EWinDialogueResult>(MessageBox(nullptr, *Message, *Title, static_cast<uint8>(Type) | static_cast<uint8>(Icon)));
#endif
	return Result;
}
