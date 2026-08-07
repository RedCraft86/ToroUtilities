// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "Widgets/CommonLabeledButton.h"
#include "Components/ButtonSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Engine/Font.h"

UCommonLabeledButton::UCommonLabeledButton(const FObjectInitializer& ObjectInit)
{
	LabelText = INVTEXT("Button Label");
	LabelStyle = nullptr;

	LabelColor = FLinearColor::Black;
	if (!IsRunningCommandlet() && !IsRunningDedicatedServer())
	{
		static ConstructorHelpers::FObjectFinder<UFont> RobotoFinder(*UWidget::GetDefaultFontName());
		if (RobotoFinder.Succeeded())
		{
			LabelFont = FSlateFontInfo(RobotoFinder.Object, 24, FName("Bold"));
		}
	}

	LabelPadding = 2.0f;
	LabelHorizontalAlignment = HAlign_Center;
	LabelVerticalAlignment = VAlign_Center;

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
	ApplyLabelSettings();
}

void UCommonLabeledButton::SetLabelStyle(const TSubclassOf<UCommonTextStyle> InStyle)
{
	LabelStyle = InStyle;
	ApplyLabelSettings();
}

void UCommonLabeledButton::SetLabelPadding(const FMargin& InPadding)
{
	LabelPadding = InPadding;
	ApplyLabelSettings();
}

void UCommonLabeledButton::SetLabelHorizontalAlignment(const EHorizontalAlignment InAlignment)
{
	LabelHorizontalAlignment = InAlignment;
	ApplyLabelSettings();
}

void UCommonLabeledButton::SetLabelVerticalAlignment(const EVerticalAlignment InAlignment)
{
	LabelVerticalAlignment = InAlignment;
	ApplyLabelSettings();
}

void UCommonLabeledButton::SetLabelFont(const FSlateFontInfo& InFont)
{
	LabelFont = InFont;
	ApplyLabelSettings();
}

void UCommonLabeledButton::SetLabelStrikeBrush(const FSlateBrush& InBrush)
{
	LabelStrikeBrush = InBrush;
	ApplyLabelSettings();
}

void UCommonLabeledButton::SetLabelShadowOffset(const FVector2D& InOffset)
{
	LabelShadowOffset = InOffset;
	ApplyLabelSettings();
}

void UCommonLabeledButton::SetLabelShadowColor(const FLinearColor& InColor)
{
	LabelShadowColor = InColor;
	ApplyLabelSettings();
}

void UCommonLabeledButton::SetLabelTransformPolicy(const ETextTransformPolicy InPolicy)
{
	LabelTransformPolicy = InPolicy;
	ApplyLabelSettings();
}

void UCommonLabeledButton::SetLabelJustification(const ETextJustify::Type InJustification)
{
	LabelJustification = InJustification;
	ApplyLabelSettings();
}

void UCommonLabeledButton::SetLabelAutoWrapText(const bool bInAutoWrap)
{
	bLabelAutoWrapText = bInAutoWrap;
	ApplyLabelSettings();
}

void UCommonLabeledButton::SetLabelWrapTextAt(const float InWrapAt)
{
	LabelWrapTextAt = InWrapAt;
	ApplyLabelSettings();
}

void UCommonLabeledButton::SetLabelWrappingPolicy(const ETextWrappingPolicy InPolicy)
{
	LabelWrappingPolicy = InPolicy;
	ApplyLabelSettings();
}

void UCommonLabeledButton::ApplyLabelSettings()
{
	if (const UCommonTextStyle* LabelStyleCDO = GetLabelStyleCDO())
	{
		LabelFont = LabelStyleCDO->Font;
		LabelColor = LabelStyleCDO->Color;
		LabelStrikeBrush = LabelStyleCDO->StrikeBrush;
		LabelShadowOffset = LabelStyleCDO->ShadowOffset;
		LabelShadowColor = LabelStyleCDO->ShadowColor;
	}

	if (ButtonLabel)
	{
		if (LabelStyle)
		{
			ButtonLabel->SetStyle(LabelStyle);
		}
		else
		{
			ButtonLabel->SetColorAndOpacity(LabelColor);
			ButtonLabel->SetFont(LabelFont);
			
			ButtonLabel->SetStrikeBrush(LabelStrikeBrush);
			ButtonLabel->SetShadowOffset(LabelShadowOffset);
			ButtonLabel->SetShadowColorAndOpacity(LabelShadowColor);
			ButtonLabel->SetTextTransformPolicy(LabelTransformPolicy);
			ButtonLabel->SetJustification(LabelJustification);

			ButtonLabel->SetAutoWrapText(bLabelAutoWrapText);
			ButtonLabel->SetWrapTextAt(LabelWrapTextAt);
			ButtonLabel->SetWrappingPolicy(LabelWrappingPolicy);
		}

		if (UButtonSlot* ButtonSlot = Cast<UButtonSlot>(ButtonLabel->Slot))
		{
			ButtonSlot->SetPadding(LabelPadding);
			ButtonSlot->SetHorizontalAlignment(LabelHorizontalAlignment);
			ButtonSlot->SetVerticalAlignment(LabelVerticalAlignment);
		}
	}
}

const UCommonTextStyle* UCommonLabeledButton::GetLabelStyleCDO() const
{
	return LabelStyle ? Cast<UCommonTextStyle>(LabelStyle->GetDefaultObject(false)) : nullptr;
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
		if (ButtonLabel)
		{
			WidgetTree->RootWidget = ButtonLabel;
			ButtonLabel->SetScrollingEnabled(false);
			ButtonLabel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}

	// Call to base class for button spawning
	return Super::Initialize();
}

void UCommonLabeledButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	ApplyLabelSettings();
	if (ButtonLabel)
	{
		ButtonLabel->SetText(LabelText);
	}
}
