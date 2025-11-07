// Copyright (C) RedCraft86. All Rights Reserved.

#include "Toolbar/ActorLayout.h"
#include "Helpers/LoggingMacros.h"
#include "Widgets/Input/SSpinBox.h"
#include "Subsystems/EditorActorSubsystem.h"

class SActorLayoutDialog final : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SActorLayoutDialog){}
	SLATE_END_ARGS()

	SActorLayoutDialog() {}
	void Construct(const FArguments& InArgs)
	{
		FMargin GridPadding(6.f, 3.f);
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
						.Text(NSLOCTEXT("ToroEditor", "LayoutPerRow", "Per Row"))
					]
					+SGridPanel::Slot(0, 1)
					.VAlign(VAlign_Center)
					.Padding(GridPadding)
					[
						SNew(STextBlock)
						.Text(NSLOCTEXT("ToroEditor", "LayoutMinX", "Min X Offset"))
					]
					+SGridPanel::Slot(0, 2)
					.VAlign(VAlign_Center)
					.Padding(GridPadding)
					[
						SNew(STextBlock)
						.Text(NSLOCTEXT("ToroEditor", "LayoutMinY", "Min Y Offset"))
					]
					+SGridPanel::Slot(0, 3)
					.VAlign(VAlign_Center)
					.Padding(GridPadding)
					[
						SNew(STextBlock)
						.Text(NSLOCTEXT("ToroEditor", "LayoutYaw", "Yaw Offset"))
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
						.Value_Static(&FActorLayout::GetMinX)
						.OnValueChanged_Static(&FActorLayout::SetMinX)
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
						.Value_Static(&FActorLayout::GetMinY)
						.OnValueChanged_Static(&FActorLayout::SetMinY)
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
					.ToolTipText(NSLOCTEXT("ToroEditor", "ToroEditorTooltip", "Organize Actors"))
					.OnClicked_Static(&SActorLayoutDialog::OnLayoutClicked)
					.Text(NSLOCTEXT("ToroEditor", "ToroEditor", "Layout Actors"))
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

void FActorLayout::ExecuteAction()
{
	if (!Window)
	{
		Window = SNew(SWindow)
		   .Title(NSLOCTEXT("ToroEditor", "ActorLayoutDialog", "Actor Layout Settings"))
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
		if (Actors.IsEmpty()) return;

		UE_LOG_MESSAGE(LogToroEditor, 2.0f, TEXT("%s"), TEXT("Starting Layout!"));

		Actors.RemoveAll([](const AActor* Elem) { return Elem == nullptr; });
		Actors.Sort([](const AActor& A, const AActor& B)
		{
			const FBox BoxA = A.GetComponentsBoundingBox(false, true);
			const FBox BoxB = B.GetComponentsBoundingBox(false, true);
			return BoxA.GetSize().Size2D() > BoxB.GetSize().Size2D();
		});

		// Transaction scope
		{
			const FScopedTransaction Transaction(NSLOCTEXT("ToroEditor", "LayoutActors", "Layout Actors"));

			for (AActor* Actor : Actors)
			{
				Actor->SetActorRotation({0.0f, (float)Yaw, 0.0f});
			}

			int i = 0;
			FVector2D Offsets = {};
			AActor* This = Actors[i];
			const int32 Len = Actors.Num();
			const AActor* LastHead = nullptr;
			const int32 NumRows = FMath::CeilToInt((float)Len / PerRow);
			for (int r = 0; r < NumRows; r++)
			{
				if (!This) break;
				Offsets.Y = 0.0f;
				const AActor* Last = nullptr;
				FBox ThisBox = This->GetComponentsBoundingBox();
				FBox HeadBox = LastHead ? LastHead->GetComponentsBoundingBox() : FBox();
				Offsets.X -= r == 0 ? 0 : FMath::Max(MinX, ThisBox.GetExtent().X * 2.0f +
					(LastHead ? HeadBox.GetExtent().X : 0.0f));

				for (int c = 0; c < PerRow; c++)
				{
					UE_LOG_MESSAGE(LogToroEditor, 2.0f, TEXT(">	Laying Out [%d/%d] %s"), i + 1, Len, *This->GetName());
					if (Last)
					{
						ThisBox = This->GetComponentsBoundingBox();
						FBox LastBox = Last->GetComponentsBoundingBox();
						Offsets.Y += FMath::Max(MinY,  ThisBox.GetExtent().Y * 2.0f + LastBox.GetExtent().Y);
						This->SetActorLocation({Offsets.X, Offsets.Y, 0.0f});
					}
					else
					{
						LastHead = This;
						This->SetActorLocation({Offsets.X, 0.0f, 0.0f});
					}

					i++;
					if (i < Len)
					{
						Last = This;
						This = Actors[i];
						This->Modify();
					}
					else
					{
						r = NumRows;
						break;
					}
				}
			}
		}
		UE_LOG_MESSAGE(LogToroEditor, 2.0f, TEXT("%s"), TEXT("Layout Finished!"));
	}
}
