// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "InputCoreTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GenericPlatform/GenericApplicationMessageHandler.h"
#include "ToroWindowsLibrary.generated.h"

/**
 * Defines the button configuration for a standard Windows dialogue box.
 * Values correspond directly to WinAPI MessageBox constants (MB_OK, etc.).
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
 * Defines the system icon displayed in the dialogue box.
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
 * Represents the button clicked by the user in a dialogue box.
 * Values correspond to WinAPI return codes (IDOK, IDCANCEL, etc.).
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
 * Defines the simulation method for virtual input.
 */
UENUM(BlueprintType)
enum class EVirtualKeyTriggerType : uint8
{
	Click,
	DoubleClick,
	Press,
	Release
};

/**
 * A utility library providing low-level Windows integration.
 * 
 * @warning The functions in this library are platform-specific. They will only 
 * function as intended on Windows builds and may do nothing or crash on other platforms.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API UToroWindowsLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Spawns a modal Windows system dialogue box. 
	 * 
	 * @warning This is a BLOCKING call. The game thread will freeze entirely until 
	 * the user interacts with the dialogue box. Use with caution during gameplay.
	 * 
	 * @param Title    The text displayed in the dialogue box window frame.
	 * @param Message  The primary body text of the message.
	 * @param Type     The combination of buttons to display.
	 * @param Icon     The system icon to display next to the message.
	 * @return         The button the user clicked to close the dialogue.
	 */
	UFUNCTION(BlueprintCallable, Category = Windows)
		static EWindowsDialogueResult OpenDialogue(const FString& Title, const FString& Message,
			const EWindowsDialogueType Type = EWindowsDialogueType::Ok,
			const EWindowsDialogueIcon Icon = EWindowsDialogueIcon::Info);

	/**
	 * Simulates hardware-level input for a specific key or mouse button.
	 * 
	 * @param InKey        The FKey to simulate (e.g., 'LeftMouseButton', 'A', 'Enter').
	 * @param TriggerType  Whether to Click, Press, or Release the key.
	 * @param bRepeat      If true, the key will be treated as if it is being held down (Keyboard only).
	 */
	UFUNCTION(BlueprintCallable, Category = Windows)
		static void VirtualKey(const FKey InKey, const EVirtualKeyTriggerType TriggerType, const bool bRepeat);

	/**
	 * Maps an FKey to the internal Windows mouse button representation.
	 * @return The corresponding EMouseButtons type.
	 */
	static EMouseButtons::Type GetMouseButton(const FKey& InKey);

	/**
	 * Extracts the hardware KeyCode and CharCode from an FKey for use in low-level OS events.
	 * @param InKey      The key to inspect.
	 * @param KeyCode    The virtual key code (Output).
	 * @param CharCode   The character code (Output).
	 */
	static void GetKeyCode(const FKey& InKey, int32& KeyCode, int32& CharCode);
};
