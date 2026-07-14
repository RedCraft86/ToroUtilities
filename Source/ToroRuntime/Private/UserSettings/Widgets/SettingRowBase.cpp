// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "UserSettings/Widgets/SettingRowBase.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Overlay.h"
#include "CommonButtonBase.h"
#include "CommonTextBlock.h"
#include "CommonBorder.h"
#if WITH_EDITOR
#include "Editor/WidgetCompilerLog.h"
#endif

USettingRowBase::USettingRowBase(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
	BorderStyle = LoadClass<UCommonBorderStyle>(nullptr, 
		TEXT("/ToroUtilities/Widgets/Styles/Style_ToroBorder_01.Style_ToroBorder_01_C")
	);

	LabelStyle = LoadClass<UCommonTextStyle>(nullptr, 
		TEXT("/ToroUtilities/Widgets/Styles/Style_ToroText_02.Style_ToroText_02_C")
	);

	RevertStyle = LoadClass<UCommonButtonStyle>(nullptr, 
		TEXT("/ToroUtilities/Widgets/Styles/Style_ToroButton_Revert.Style_ToroButton_Revert_C")
	);
}

bool USettingRowBase::Initialize()
{
	const bool bInitializedThisFrame = Super::Initialize();
	if (bInitializedThisFrame)
	{
		if (WidgetTree)
		{
			Background = WidgetTree->ConstructWidget<UCommonBorder>(UCommonBorder::StaticClass(), TEXT("Background"));
			if (Background) { WidgetTree->RootWidget = Background; }
		}

		if (Background)
		{
			RowContainer = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("RowContainer"));
			if (RowContainer) { Background->AddChild(RowContainer); }
		}

		if (RowContainer)
		{
			RowLabel = WidgetTree->ConstructWidget<UCommonTextBlock>(UCommonTextBlock::StaticClass(), TEXT("LabelText"));
			if (UHorizontalBoxSlot* HBSlot = Cast<UHorizontalBoxSlot>(RowContainer->AddChild(RowLabel)))
			{
				HBSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
				HBSlot->SetHorizontalAlignment(HAlign_Fill);
			}

			ContentContainer = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("ContentContainer"));
			if (UHorizontalBoxSlot* HBSlot = Cast<UHorizontalBoxSlot>(RowContainer->AddChild(ContentContainer)))
			{
				HBSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
				HBSlot->SetHorizontalAlignment(HAlign_Fill);
			}
		}

		if (ContentContainer)
		{
			ConstructSettings(ContentContainer);
			RevertButton = WidgetTree->ConstructWidget<UCommonButtonBase>(UCommonButtonBase::StaticClass(), TEXT("RevertButton"));
			ContentContainer->AddChild(RevertButton);
		}
	}

	return bInitializedThisFrame;
}

void USettingRowBase::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (Background)
	{
		Background->SetStyle(BorderStyle);
	}

	if (RowLabel)
	{
		RowLabel->SetStyle(LabelStyle);
	}

	if (RevertButton)
	{
		RevertButton->SetStyle(RevertStyle);
	}
}

void USettingRowBase::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	OnHovered.Broadcast(Provider);
}

#if WITH_EDITOR
void USettingRowBase::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);
	if (!AllowedStruct || !Provider.GetScriptStruct()->IsChildOf(AllowedStruct))
	{
		CompileLog.Error(FText::Format(INVTEXT("{0}'s provider isn't of allowed type: {1}!"),
			FText::FromString(GetName()), FText::FromString(GetNameSafe(AllowedStruct))));
	}
}
#endif