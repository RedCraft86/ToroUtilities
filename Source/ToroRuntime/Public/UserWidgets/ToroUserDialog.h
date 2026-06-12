// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "UE5Coro.h"
#include "ToroWidgetBase.h"
#include "CommonButtonBase.h"
#include "ToroMasterWidget.h"
#include "Components/StackBox.h"
#include "Components/TextBlock.h"
#include "Buttons/CommonLabeledButton.h"
#include "ToroUserDialog.generated.h"

/**
 * Represents a single interactable option (button) within a User Dialog.
 */
USTRUCT(BlueprintType)
struct TORORUNTIME_API FToroUserDialogEntry final
{
	GENERATED_BODY()

public:

	/** The machine-readable name returned when this button is clicked (e.g., "Confirm", "Cancel"). */
	UPROPERTY(EditAnywhere, Category = Entry)
		FName Identifier;

	/** The localized text displayed on the button face. */
	UPROPERTY(EditAnywhere, Category = Entry)
		FText DisplayText;

	FToroUserDialogEntry()
		: Identifier(NAME_None), DisplayText(FText::GetEmpty())
	{}

	FToroUserDialogEntry(const FName& InIdentifier, const FText& InDisplayText)
		: Identifier(InIdentifier), DisplayText(InDisplayText)
	{}

	FORCEINLINE bool IsValidEntry() const { return !Identifier.IsNone(); }
	FORCEINLINE FText GetDisplayText() const
	{
		return DisplayText.IsEmptyOrWhitespace() ? FText::FromName(Identifier) : DisplayText;
	}
};

/**
 * A specialized modal widget used for displaying prompts, alerts, and confirmations.
 * Supports dynamic button generation and custom layouts (Horizontal/Vertical).
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class TORORUNTIME_API UToroUserDialog final : public UToroUserWidget
{
	GENERATED_BODY()

public:

	UToroUserDialog(const FObjectInitializer& ObjectInit);

	/** 
	 * Factory method to instantiate a new User Dialog.
	 * Ideal setup would be: Create -> Bind Events -> Push
	 * 
	 * @param ContextObject   The world context for spawning the widget.
	 * @param Title           The header text for the dialog.
	 * @param Message         The body text providing details to the user.
	 * @param Buttons         An array of entries defining the buttons to generate.
	 * @param ButtonLayout    Whether buttons should be stacked vertically or horizontally.
	 * @return A configured UToroUserDialog instance, ready to be pushed to the screen.
	 */
	UFUNCTION(BlueprintCallable, Category = UserWidget)
		static UToroUserDialog* CreateUserDialog(const UObject* ContextObject, const FText& Title, const FText& Message, 
			const TArray<FToroUserDialogEntry>& Buttons, const TEnumAsByte<EOrientation> ButtonLayout);

	/** 
	 * Adds this dialog to the Master Widget's overlay, making it visible and focused.
	 */
	UFUNCTION(BlueprintCallable, Category = UserWidget)
		void PushUserDialog();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDialogResultDelegateBP, const FName, Identifier);
	UPROPERTY(BlueprintAssignable, DisplayName = "On Selected")
		FDialogResultDelegateBP OnResultSelectedBP;

	DECLARE_MULTICAST_DELEGATE_OneParam(FDialogResultDelegate, const FName);
	FDialogResultDelegate OnResultSelected;

protected:

	/** Label for the dialog header. REQUIREMENT: A TextBlock named 'TitleLabel'. */
	UPROPERTY(BlueprintReadOnly, Category = Subobjects, meta = (BindWidget))
		TObjectPtr<UTextBlock> TitleLabel;

	/** Label for the dialog body. REQUIREMENT: A TextBlock named 'MessageLabel'. */
	UPROPERTY(BlueprintReadOnly, Category = Subobjects, meta = (BindWidget))
		TObjectPtr<UTextBlock> MessageLabel;

	/** Container for dynamic buttons. REQUIREMENT: A StackBox named 'ButtonContainer'. */
	UPROPERTY(BlueprintReadOnly, Category = Subobjects, meta = (BindWidget))
		TObjectPtr<UStackBox> ButtonContainer;

	/** Spacing between generated buttons. */
	UPROPERTY(EditAnywhere, Category = Appearance, meta = (ClampMin = 0.0f))
		float EntryPadding;

	UPROPERTY(Transient)
		TObjectPtr<UToroMasterWidget> MasterWidget;

	UPROPERTY(Transient)
		TMap<UCommonLabeledButton*, FName> ButtonToIdentifier;

	void OnButtonClicked(UCommonLabeledButton* Button);

	void ConstructDialog(UToroMasterWidget* Master, const FText& TitleText, const FText& MessageText, 
		const TArray<FToroUserDialogEntry>& Entries, const TEnumAsByte<EOrientation> Layout);
};
