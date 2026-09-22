// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "ActorLayout.h"
#include "Widgets/Input/SSpinBox.h"
#include "Helpers/ActorBaking.h"

namespace
{
	class SActorLayoutDialog final : public SCompoundWidget
	{
	public:

		SLATE_BEGIN_ARGS(SActorLayoutDialog){}
		SLATE_END_ARGS()

		SActorLayoutDialog() {}

		// ReSharper disable once CppDeclaratorNeverUsed
		void Construct(const FArguments&)
		{
			const FMargin GridPadding(6.f, 3.f);
			this->ChildSlot.Padding(0.f)
			[
				SNew(SBorder)
				.Padding(0.f)
				.BorderImage(FAppStyle::Get().GetBrush("ToolPanel.GroupBorder"))
				[
					SNew(SVerticalBox)
					+SVerticalBox::Slot()
					.Padding(6.0)
					.HAlign(HAlign_Fill)
					[
						SNew(SGridPanel).FillColumn(1, 1.0f)
						+SGridPanel::Slot(0, 0)
						.VAlign(VAlign_Center)
						.Padding(GridPadding)
						[
							SNew(STextBlock)
							.Text(INVTEXT("Max Columns"))
						]
						+SGridPanel::Slot(0, 1)
						.VAlign(VAlign_Center)
						.Padding(GridPadding)
						[
							SNew(STextBlock)
							.Text(INVTEXT("X Offset"))
						]
						+SGridPanel::Slot(0, 2)
						.VAlign(VAlign_Center)
						.Padding(GridPadding)
						[
							SNew(STextBlock)
							.Text(INVTEXT("Y Offset"))
						]
						+SGridPanel::Slot(0, 3)
						.VAlign(VAlign_Center)
						.Padding(GridPadding)
						[
							SNew(STextBlock)
							.Text(INVTEXT("Yaw Value"))
						]
						+SGridPanel::Slot(1, 0)
						.Padding(GridPadding)
						.HAlign(HAlign_Fill)
						[
							SNew(SSpinBox<float>)
							.Delta(1.0f)
							.MinValue(1.0f)
							.MinSliderValue(1.0f)
							.MinFractionalDigits(0)
							.MaxFractionalDigits(0)
							.Value_Lambda([]()
							{
								return FActorLayoutButton::MaxColumns.GetValue();
							})
							.OnValueChanged_Lambda([](float Value)
							{
								FActorLayoutButton::MaxColumns.SetValue(Value);
							})
						]
						+SGridPanel::Slot(1, 1)
						.Padding(GridPadding)
						.HAlign(HAlign_Fill)
						[
							SNew(SSpinBox<float>)
							.Delta(10.0f)
							.MinValue(1.0f)
							.MinSliderValue(1.0f)
							.MinFractionalDigits(0)
							.MaxFractionalDigits(0)
							.Value_Lambda([]()
							{
								return FActorLayoutButton::XOffset.GetValue();
							})
							.OnValueChanged_Lambda([](float Value)
							{
								FActorLayoutButton::XOffset.SetValue(Value);
							})
						]
						+SGridPanel::Slot(1, 2)
						.Padding(GridPadding)
						.HAlign(HAlign_Fill)
						[
							SNew(SSpinBox<float>)
							.Delta(10.0f)
							.MinValue(1.0f)
							.MinSliderValue(1.0f)
							.MinFractionalDigits(0)
							.MaxFractionalDigits(0)
							.Value_Lambda([]()
							{
								return FActorLayoutButton::YOffset.GetValue();
							})
							.OnValueChanged_Lambda([](float Value)
							{
								FActorLayoutButton::YOffset.SetValue(Value);
							})
						]
						+SGridPanel::Slot(1, 3)
						.Padding(GridPadding)
						.HAlign(HAlign_Fill)
						[
							SNew(SSpinBox<float>)
							.Delta(10.0f)
							.MaxValue(360.0f)
							.MinValue(0.0f)
							.MaxSliderValue(360.0f)
							.MinSliderValue(0.0f)
							.MinFractionalDigits(0)
							.MaxFractionalDigits(0)
							.Value_Lambda([]()
							{
								return FActorLayoutButton::YawValue.GetValue();
							})
							.OnValueChanged_Lambda([](float Value)
							{
								FActorLayoutButton::YawValue.SetValue(Value);
							})
						]
					]
					+SVerticalBox::Slot()
					.Padding(16.f)
					.AutoHeight()
					[
						SNew(SButton)
						.ButtonStyle(&FAppStyle::Get().GetWidgetStyle<FButtonStyle>("PrimaryButton"))
						.OnClicked_Static(&SActorLayoutDialog::OnLayoutClicked)
						.ToolTipText(INVTEXT("Organize Actors"))
						.Text(INVTEXT("Layout Actors"))
					]
				]
			];
		}

		static FReply OnLayoutClicked()
		{
			FActorBaking::LayoutActors(
				FActorBaking::GetSelectedActors(),
				FActorLayoutButton::MaxColumns.GetValue(),
				FActorLayoutButton::YawValue.GetValue(),
				FVector2D(
					FActorLayoutButton::XOffset.GetValue(),
					FActorLayoutButton::YOffset.GetValue()
				)
			);

			return FReply::Handled();
		}
	};
}

void FActorLayoutButton::Execute()
{
	if (FActorBaking::GetSelectedActors().IsEmpty())
	{
		return;
	}

	if (!Window)
	{
		Window = SNew(SWindow)
		   .Title(INVTEXT("Actor Layout Settings"))
		   .ClientSize(FVector2D(300, 175))
		   .SupportsMinimize(false)
		   .SupportsMaximize(false)
		   .FocusWhenFirstShown(true)
		   [
			   SNew(SActorLayoutDialog)
		   ];

		FSlateApplication::Get().AddWindow(Window.ToSharedRef());
	}

	Window->BringToFront();
}
