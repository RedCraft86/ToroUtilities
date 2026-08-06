// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "ActorLayout.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Input/SButton.h"
#include "Editor.h"

namespace
{
	class SActorLayoutDialog final : public SCompoundWidget
	{
	public:

		SLATE_BEGIN_ARGS(SActorLayoutDialog){}
		SLATE_END_ARGS()

		SActorLayoutDialog() {}
		// ReSharper disable once CppDeclaratorNeverUsed
		void Construct(const FArguments& InArgs)
		{
			const FMargin GridPadding(6.f, 3.f);
			this->ChildSlot
			    .Padding(0.f)
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
							.Text(INVTEXT("Per Row"))
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
							.Text(INVTEXT("Yaw Offset"))
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
							.Value_Static(&FActorLayout::GetPerRow)
							.OnValueChanged_Static(&FActorLayout::SetPerRow)
						]
						+SGridPanel::Slot(1, 1)
						.Padding(GridPadding)
						.HAlign(HAlign_Fill)
						[
							SNew(SSpinBox<float>)
							.Delta(1.0f)
							.MinValue(1.0f)
							.MinSliderValue(1.0f)
							.MinFractionalDigits(0)
							.MaxFractionalDigits(0)
							.Value_Static(&FActorLayout::GetOffsetX)
							.OnValueChanged_Static(&FActorLayout::SetOffsetX)
						]
						+SGridPanel::Slot(1, 2)
						.Padding(GridPadding)
						.HAlign(HAlign_Fill)
						[
							SNew(SSpinBox<float>)
							.Delta(1.0f)
							.MinValue(1.0f)
							.MinSliderValue(1.0f)
							.MinFractionalDigits(0)
							.MaxFractionalDigits(0)
							.Value_Static(&FActorLayout::GetOffsetY)
							.OnValueChanged_Static(&FActorLayout::SetOffsetY)
						]
						+SGridPanel::Slot(1, 3)
						.Padding(GridPadding)
						.HAlign(HAlign_Fill)
						[
							SNew(SSpinBox<float>)
							.Delta(90.0f)
							.MaxValue(90.0f)
							.MinValue(-90.0f)
							.MaxSliderValue(90.0f)
							.MinSliderValue(-90.0f)
							.MinFractionalDigits(0)
							.MaxFractionalDigits(0)
							.Value_Static(&FActorLayout::GetYaw)
							.OnValueChanged_Static(&FActorLayout::SetYaw)
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
			FActorLayout::LayoutActors();
			return FReply::Handled();
		}
	};
}

void FActorLayout::LayoutActors()
{
	if (Window)
	{
		Window->RequestDestroyWindow();
		Window.Reset();
	}

	if (UEditorActorSubsystem* Subsystem = GEditor ? GEditor->GetEditorSubsystem<UEditorActorSubsystem>() : nullptr)
	{
		TArray<AActor*> Actors = Subsystem->GetSelectedLevelActors();
		Actors.Remove(nullptr);
		if (Actors.Num() < 2)
		{
			return;
		}

		Actors.Sort([](const AActor& A, const AActor& B)
		{
			auto GetSize = [](const AActor& Actor) -> float
			{
				FVector Origin, Extent;
				Actor.GetActorBounds(false, Origin, Extent, true);
				return Extent.Size();
			};

			return GetSize(A) > GetSize(B);
		});

		FVector Unused, MaxActorSize;
		Actors[0]->GetActorBounds(false, Unused, MaxActorSize, true);
		MaxActorSize *= 2.0f;

		const FVector CellSize = MaxActorSize + FVector(GetOffsetX(), GetOffsetY(), 0);

		int32 MultiX = 0, MultiY = 0;
		for (AActor* Actor : Actors)
		{
			Actor->SetActorLocation(CellSize * FVector(MultiX, MultiY, 0));

			FRotator Rotation = Actor->GetActorRotation();
			Rotation.Yaw = GetYaw();
			Actor->SetActorRotation(Rotation);

			MultiY++;
			if (MultiY >= GetPerRow())
			{
				MultiX++;
				MultiY = 0;
			}
		}
	}

}

void FActorLayout::Execute()
{
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