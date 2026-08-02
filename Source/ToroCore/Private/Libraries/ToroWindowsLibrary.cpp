// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

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
	FString ClipboardContent;
#if PLATFORM_WINDOWS
	FWindowsPlatformApplicationMisc::ClipboardPaste(ClipboardContent);
#endif
	return ClipboardContent;
}

EWindowsDialogueResult UToroWindowsLibrary::OpenDialogue(const FString& Title, const FString& Message,
	const EWindowsDialogueType Type, const EWindowsDialogueIcon Icon)
{
	EWindowsDialogueResult Result = EWindowsDialogueResult::None;
#if PLATFORM_WINDOWS
	Result = static_cast<EWindowsDialogueResult>(MessageBox(nullptr, *Message, *Title,
		static_cast<uint8>(Type) | static_cast<uint8>(Icon)
	));
#endif
	return Result;
}
