// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "Buttons/CommonLabeledButton.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/ButtonSlot.h"
#include "Blueprint/WidgetTree.h"
#include "CommonTextBlock.h"
#include "Engine/Font.h"

UCommonLabeledButton::UCommonLabeledButton(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
	LabelText = INVTEXT("Button Label");
	LabelColor = FLinearColor::Black;
	LabelStyle = nullptr;

	LabelPadding = 2.0f;
	LabelHorizontalAlignment = HAlign_Center;
	LabelVerticalAlignment = VAlign_Center;

	if (!IsRunningCommandlet() && !IsRunningDedicatedServer())
	{
		static ConstructorHelpers::FObjectFinder<UFont> RobotoFinder(*UWidget::GetDefaultFontName());
		if (RobotoFinder.Succeeded()) LabelFont = FSlateFontInfo(RobotoFinder.Object, 24, FName("Bold"));
	}

	LabelStrikeBrush.TintColor = FLinearColor::Black;
	LabelShadowOffset = FVector2D::UnitVector;
	LabelShadowColor = FLinearColor::Transparent;
	LabelTransformPolicy = ETextTransformPolicy::None;
	LabelJustification = ETextJustify::Center;

	bLabelAutoWrapText = false;
	LabelWrapTextAt = 0.0f;
	LabelWrappingPolicy = ETextWrappingPolicy::DefaultWrapping;

	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	ForegroundColor = FLinearColor::Black;
	PRAGMA_ENABLE_DEPRECATION_WARNINGS

	SetStyle(LoadClass<UCommonButtonStyle>(nullptr, TEXT("/ToroUtilities/Widgets/BS_UserDialog.BS_UserDialog_C")));
	SetLabelStyle(LoadClass<UCommonTextStyle>(nullptr, TEXT("/ToroUtilities/Widgets/TS_LabeledButton.TS_LabeledButton_C")));
}

void UCommonLabeledButton::SetLabelContentText(const FText InText)
{
	LabelText = InText;
	if (ButtonLabel)
	{
		ButtonLabel->SetText(InText);
	}
}

void UCommonLabeledButton::SetLabelColor(const FLinearColor& InColor)
{
	LabelColor = InColor;
	if (ButtonLabel)
	{
		ButtonLabel->SetColorAndOpacity(InColor);
	}
}

void UCommonLabeledButton::SetLabelStyle(const TSubclassOf<UCommonTextStyle> InStyle)
{
	LabelStyle = InStyle;
	if (ButtonLabel)
	{
		ButtonLabel->SetStyle(InStyle);
	}
}

void UCommonLabeledButton::SetLabelPadding(const FMargin& InPadding)
{
	LabelPadding = InPadding;
	if (UButtonSlot* ButtonSlot = Cast<UButtonSlot>(ButtonLabel->Slot))
	{
		ButtonSlot->SetPadding(InPadding);
	}
}

void UCommonLabeledButton::SetLabelHorizontalAlignment(const EHorizontalAlignment InAlignment)
{
	LabelHorizontalAlignment = InAlignment;
	if (UButtonSlot* ButtonSlot = Cast<UButtonSlot>(ButtonLabel->Slot))
	{
		ButtonSlot->SetHorizontalAlignment(InAlignment);
	}
}

void UCommonLabeledButton::SetLabelVerticalAlignment(const EVerticalAlignment InAlignment)
{
	LabelVerticalAlignment = InAlignment;
	if (UButtonSlot* ButtonSlot = Cast<UButtonSlot>(ButtonLabel->Slot))
	{
		ButtonSlot->SetVerticalAlignment(InAlignment);
	}
}

void UCommonLabeledButton::SetLabelFont(const FSlateFontInfo& InFont)
{
	LabelFont = InFont;
	if (ButtonLabel)
	{
		ButtonLabel->SetFont(InFont);
	}
}

void UCommonLabeledButton::SetLabelStrikeBrush(const FSlateBrush& InBrush)
{
	LabelStrikeBrush = InBrush;
	if (ButtonLabel)
	{
		ButtonLabel->SetStrikeBrush(InBrush);
	}
}

void UCommonLabeledButton::SetLabelShadowOffset(const FVector2D& InOffset)
{
	LabelShadowOffset = InOffset;
	if (ButtonLabel)
	{
		ButtonLabel->SetShadowOffset(InOffset);
	}
}

void UCommonLabeledButton::SetLabelShadowColor(const FLinearColor& InColor)
{
	LabelShadowColor = InColor;
	if (ButtonLabel)
	{
		ButtonLabel->SetShadowColorAndOpacity(InColor);
	}
}

void UCommonLabeledButton::SetLabelTransformPolicy(const ETextTransformPolicy InPolicy)
{
	LabelTransformPolicy = InPolicy;
	if (ButtonLabel)
	{
		ButtonLabel->SetTextTransformPolicy(InPolicy);
	}
}

void UCommonLabeledButton::SetLabelJustification(const ETextJustify::Type InJustification)
{
	LabelJustification = InJustification;
	if (ButtonLabel)
	{
		ButtonLabel->SetJustification(InJustification);
	}
}

void UCommonLabeledButton::SetLabelAutoWrapText(const bool bInAutoWrap)
{
	bLabelAutoWrapText = bInAutoWrap;
	if (ButtonLabel)
	{
		ButtonLabel->SetAutoWrapText(bInAutoWrap);
	}
}

void UCommonLabeledButton::SetLabelWrapTextAt(const float InWrapAt)
{
	LabelWrapTextAt = InWrapAt;
	if (ButtonLabel)
	{
		ButtonLabel->SetWrapTextAt(InWrapAt);
	}
}

void UCommonLabeledButton::SetLabelWrappingPolicy(const ETextWrappingPolicy InPolicy)
{
	LabelWrappingPolicy = InPolicy;
	if (ButtonLabel)
	{
		ButtonLabel->SetWrappingPolicy(InPolicy);
	}
}

bool UCommonLabeledButton::Initialize()
{
	// Need to create the tree ourselves to assign a root widget for UCommonButtonBase's Initialize call
	if (!WidgetTree)
	{
		WidgetTree = NewObject<UWidgetTree>(this, TEXT("WidgetTree"), RF_Transient);
	}

	// Add the label and set it as root
	if (WidgetTree)
	{
		ButtonLabel = WidgetTree->ConstructWidget<UCommonTextBlock>(UCommonTextBlock::StaticClass(), TEXT("InternalLabel"));
		if (ButtonLabel) { WidgetTree->RootWidget = ButtonLabel; }
	}

	// Call to base class for button spawning
	return Super::Initialize();
}

void UCommonLabeledButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (ButtonLabel)
	{
		ButtonLabel->SetText(LabelText);
		ButtonLabel->SetColorAndOpacity(LabelColor);
		ButtonLabel->SetStyle(LabelStyle);

		ButtonLabel->SetFont(LabelFont);
		ButtonLabel->SetStrikeBrush(LabelStrikeBrush);
		ButtonLabel->SetShadowOffset(LabelShadowOffset);
		ButtonLabel->SetShadowColorAndOpacity(LabelShadowColor);
		ButtonLabel->SetTextTransformPolicy(LabelTransformPolicy);
		ButtonLabel->SetJustification(LabelJustification);

		ButtonLabel->SetAutoWrapText(bLabelAutoWrapText);
		ButtonLabel->SetWrapTextAt(LabelWrapTextAt);
		ButtonLabel->SetWrappingPolicy(LabelWrappingPolicy);

		ButtonLabel->SetScrollingEnabled(false);
		ButtonLabel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (UButtonSlot* ButtonSlot = Cast<UButtonSlot>(ButtonLabel->Slot))
		{
			ButtonSlot->SetPadding(LabelPadding);
			ButtonSlot->SetHorizontalAlignment(LabelHorizontalAlignment);
			ButtonSlot->SetVerticalAlignment(LabelVerticalAlignment);
		}
	}
}
