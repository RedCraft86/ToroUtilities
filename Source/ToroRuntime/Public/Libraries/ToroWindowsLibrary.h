// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroWindowsLibrary.generated.h"

/**
 * Selects the buttons shown by a Windows dialog.
 */
UENUM(BlueprintType)
enum class EWinDialogueType : uint8
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
 * Selects the icon shown by a Windows dialog.
 */
UENUM(BlueprintType)
enum class EWinDialogueIcon : uint8
{
	None		= 0,
	Error		= 16,
	Warning		= 48,
	Info		= 64
};

/**
 * Identifies the button selected in a Windows dialog.
 */
UENUM(BlueprintType)
enum class EWinDialogueResult : uint8
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
 * Provides clipboard access and a native Windows dialog.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TORORUNTIME_API UToroWindowsLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Copies text to the system clipboard.
	 * @param Value Text to copy.
	 */
	UFUNCTION(BlueprintCallable, Category = "Windows|Clipboard")
		static void CopyStringToClipboard(const FString& Value);

	/**
	 * Reads text from the system clipboard.
	 */
	UFUNCTION(BlueprintCallable, Category = "Windows|Clipboard")
		static FString GetStringFromClipboard();

	/**
	 * Opens a native Windows message dialog.
	 * @param Title Dialog title.
	 * @param Message Dialog body text.
	 * @param Type Button layout.
	 * @param Icon Dialog icon.
	 */
	UFUNCTION(BlueprintCallable, Category = "Windows")
		static EWinDialogueResult OpenDialogue(const FString& Title, const FString& Message,
			const EWinDialogueType Type = EWinDialogueType::Ok, const EWinDialogueIcon Icon = EWinDialogueIcon::Info);
};
