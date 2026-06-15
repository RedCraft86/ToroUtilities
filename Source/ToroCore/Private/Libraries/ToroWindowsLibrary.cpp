// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "Libraries/ToroWindowsLibrary.h"
#include "Framework/Application/SlateApplication.h"
#if PLATFORM_WINDOWS
#include "shlobj.h" // <- This causes some serious errors to happen if included in .h due to conflicts with UE macros
#endif

EWindowsDialogueResult UToroWindowsLibrary::OpenDialogue(const FString& Title, const FString& Message,
	const EWindowsDialogueType Type, const EWindowsDialogueIcon Icon)
{
#if PLATFORM_WINDOWS
	return static_cast<EWindowsDialogueResult>(MessageBox(nullptr, *Message, *Title,
		static_cast<uint8>(Type) | static_cast<uint8>(Icon)));
#endif
}

void UToroWindowsLibrary::VirtualKey(const FKey InKey, const EVirtualKeyTriggerType TriggerType, const bool bRepeat)
{
	if (!FSlateApplication::IsInitialized() || !InKey.IsValid()) return;
	const TSharedPtr<GenericApplication> PlatformApp = FSlateApplication::Get().GetPlatformApplication();
	const TSharedRef<FGenericApplicationMessageHandler> MsgHandler = PlatformApp->GetMessageHandler();
	if (InKey.IsMouseButton())
	{
		const EMouseButtons::Type MouseButton = GetMouseButton(InKey);
		switch(TriggerType)
		{
		case EVirtualKeyTriggerType::Click:
			MsgHandler->OnMouseDown(PlatformApp->GetWindowUnderCursor(), MouseButton);
			MsgHandler->OnMouseUp(MouseButton);
			break;

		case EVirtualKeyTriggerType::DoubleClick:
			MsgHandler->OnMouseDown(PlatformApp->GetWindowUnderCursor(), MouseButton);
			MsgHandler->OnMouseUp(MouseButton);
			MsgHandler->OnMouseDown(PlatformApp->GetWindowUnderCursor(), MouseButton);
			MsgHandler->OnMouseUp(MouseButton);
			break;

		case EVirtualKeyTriggerType::Press:
			MsgHandler->OnMouseDown(PlatformApp->GetWindowUnderCursor(), MouseButton);
			break;

		case EVirtualKeyTriggerType::Release:
			MsgHandler->OnMouseUp(MouseButton);
			break;
		}
	}
	else
	{
		int32 KeyCode, CharCode = 0;
		GetKeyCode(InKey, KeyCode, CharCode);
		switch(TriggerType)
		{
		case EVirtualKeyTriggerType::Click:
			MsgHandler->OnKeyDown(KeyCode, CharCode, false);
			MsgHandler->OnKeyChar(static_cast<TCHAR>(CharCode), false);
			MsgHandler->OnKeyUp(KeyCode, CharCode, false);
			break;

		case EVirtualKeyTriggerType::DoubleClick:
			MsgHandler->OnKeyDown(KeyCode, CharCode, false);
			MsgHandler->OnKeyChar(static_cast<TCHAR>(CharCode), false);
			MsgHandler->OnKeyUp(KeyCode, CharCode, false);
			MsgHandler->OnKeyDown(KeyCode, CharCode, false);
			MsgHandler->OnKeyChar(static_cast<TCHAR>(CharCode), false);
			MsgHandler->OnKeyUp(KeyCode, CharCode, false);
			break;

		case EVirtualKeyTriggerType::Press:
			MsgHandler->OnKeyDown(KeyCode, CharCode, bRepeat);
			MsgHandler->OnKeyChar(static_cast<TCHAR>(CharCode), bRepeat);
			break;

		case EVirtualKeyTriggerType::Release:
			MsgHandler->OnKeyUp(KeyCode, CharCode, false);
			break;
		}
	}
}

EMouseButtons::Type UToroWindowsLibrary::GetMouseButton(const FKey& InKey)
{
	if (InKey.IsMouseButton())
	{
		if (InKey == EKeys::LeftMouseButton)	return EMouseButtons::Left;
		if (InKey == EKeys::RightMouseButton)	return EMouseButtons::Right;
		if (InKey == EKeys::MiddleMouseButton)	return EMouseButtons::Middle;
		if (InKey == EKeys::ThumbMouseButton)	return EMouseButtons::Thumb01;
		if (InKey == EKeys::ThumbMouseButton2)	return EMouseButtons::Thumb02;
	}

	return EMouseButtons::Invalid;
}

void UToroWindowsLibrary::GetKeyCode(const FKey& InKey, int32& KeyCode, int32& CharCode)
{
	const uint32* KeyCodePtr;
	const uint32* CharCodePtr;
	FInputKeyManager::Get().GetCodesFromKey(InKey, KeyCodePtr, CharCodePtr);
	KeyCode = KeyCodePtr ? static_cast<int32>(*KeyCodePtr) : 0;
	CharCode = CharCodePtr ? static_cast<int32>(*CharCodePtr) : 0;
}
