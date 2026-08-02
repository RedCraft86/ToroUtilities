// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroWindowsLibrary.generated.h"

/**
 * The button configuration for a standard Windows dialogue box.
 * Values correspond to Windows API MessageBox constants (MB_OK, etc.).
 */
UENUM(BlueprintType)
enum class EWindowsDialogueType : uint8
{
	Ok						= 0	UMETA(DisplayName = "OK"),
	OkCancel				= 1	UMETA(DisplayName = "OK, Cancel"),
	AbortRetryIgnore		= 2	UMETA(DisplayName = "Abort, Retry, and Ignore"),
	YesNoCancel				= 3	UMETA(DisplayName = "Yes, No, Cancel"),
	YesNo					= 4	UMETA(DisplayName = "Yes, No"),
	RetryCancel				= 5	UMETA(DisplayName = "Retry, Cancel"),
	CancelTryAgainContinue	= 6	UMETA(DisplayName = "Cancel, Try Again, Continue")
};

/**
 * The system icon displayed on the windows dialogue box.
 */
UENUM(BlueprintType)
enum class EWindowsDialogueIcon : uint8
{
	None		= 0,
	Error		= 16,
	Warning		= 48,
	Info		= 64
};

/**
 * The button clicked by the user in a dialogue box.
 * Values correspond to Windows API return codes (IDOK, IDCANCEL, etc.).
 */
UENUM(BlueprintType)
enum class EWindowsDialogueResult : uint8
{
	None		= 0,
	OK			= 1,
	Cancel		= 2,
	Abort		= 3,
	Retry		= 4,
	Ignore		= 5,
	Yes			= 6,
	No			= 7,
	Close		= 8,
	Help		= 9,
	TryAgain	= 10,
	Continue	= 11
};

/**
 * A utility library providing functions for the Windows system.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API UToroWindowsLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Copy the provided string to the clipboard.
	 * @param Value The string to copy to the clipboard.
	 */
	UFUNCTION(BlueprintCallable, Category = "Windows|Clipboard")
		static void CopyStringToClipboard(const FString& Value);

	/**
	 * Gets the content of the clipboard as a string.
	 * @return The string found in the clipboard.
	 */
	UFUNCTION(BlueprintCallable, Category = "Windows|Clipboard")
		static FString GetStringFromClipboard();

	/**
	 * Spawns a modal Windows system dialogue box.
	 * @warning This is a <b>blocking</b> call. The game thread will <b>freeze</b> until
	 * the user interacts with the dialogue box. Use with caution during gameplay.
	 * @param Title The text displayed in the dialogue box window frame.
	 * @param Message The primary body text of the message.
	 * @param Type The combination of buttons to display.
	 * @param Icon The system icon to display next to the message.
	 * @return The button the user clicked to close the dialogue.
	 */
	UFUNCTION(BlueprintCallable, Category = "Windows")
		static EWindowsDialogueResult OpenDialogue(const FString& Title, const FString& Message,
			const EWindowsDialogueType Type = EWindowsDialogueType::Ok,
			const EWindowsDialogueIcon Icon = EWindowsDialogueIcon::Info);
};
