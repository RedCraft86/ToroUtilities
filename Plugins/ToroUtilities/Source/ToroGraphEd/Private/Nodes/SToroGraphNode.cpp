// Copyright (C) RedCraft86. All Rights Reserved.

#include "Nodes/SToroGraphNode.h"
#include "Nodes/EdToroGraphNode.h"
#include "Nodes/ToroGraphDragConnection.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"
#include "GraphEditorSettings.h"
#include "SCommentBubble.h"
#include "ToroGraphEd.h"
#include "SGraphPin.h"

#define LOCTEXT_NAMESPACE "ToroGraphEd"

class SToroGraphPin final : public SGraphPin
{
public:
	
	SLATE_BEGIN_ARGS(SToroGraphPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
	{
		SetCursor(EMouseCursor::Default);
		bShowLabel = true;
		GraphPinObj = InPin;
		check(GraphPinObj);
		const UEdGraphSchema* Schema = GraphPinObj->GetSchema();
		check(Schema);
		SBorder::Construct(SBorder::FArguments()
			.BorderBackgroundColor(ToroGraphStatics::DefaultPinColor)
			.BorderImage(FAppStyle::GetBrush(TEXT("Graph.StateNode.Body")))
			.OnMouseButtonDown(this, &SToroGraphPin::OnPinMouseDown)
			.Cursor(this, &SToroGraphPin::GetPinCursor)
			.Padding(FMargin(5.0f))
		);
	}

protected:

	virtual TSharedRef<SWidget>	GetDefaultValueWidget() override
	{
		return SNew(STextBlock);
	}

	virtual TSharedRef<FDragDropOperation> SpawnPinDragEvent(const TSharedRef<SGraphPanel>& InGraphPanel,
		const TArray<TSharedRef<SGraphPin>>& InStartingPins) override
	{
		TArray<FGraphPinHandle> PinHandles;
		PinHandles.Reserve(InStartingPins.Num());
		for (const TSharedRef<SGraphPin>& PinWidget : InStartingPins)
		{
			PinHandles.Add(PinWidget->GetPinObj());
		}

		return FToroGraphDragConnection::New(InGraphPanel, PinHandles);
	}

};

void SToroGraphNode::Construct(const FArguments& InArgs, UEdToroGraphNode* InNode)
{
	GraphNode = InNode;
	InNode->SNode = this;
	UpdateGraphNode();
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SToroGraphNode::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();
	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	TSharedPtr<SErrorText> ErrorText;
	TSharedPtr<SVerticalBox> NodeBody;
	const FSlateBrush *NodeTypeIcon = FAppStyle::GetBrush(TEXT("BTEditor.Graph.BTNode.Icon"));
	constexpr FLinearColor TitleShadowColor(0.6f, 0.6f, 0.6f);
	const TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

	FSlateFontInfo IdentifierFont = FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("MonospacedText").Font;
	IdentifierFont.Size = 9.0f;

	ContentScale.Bind(this, &SGraphNode::GetContentScale);
	GetOrAddSlot(ENodeZone::Center)
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Center)
	[
		SNew(SBorder)
		.Padding(0.0f)
		.BorderImage(FAppStyle::GetBrush("Graph.StateNode.Body"))
		.BorderBackgroundColor(this, &SToroGraphNode::GetBackgroundColor)
		[
			SNew(SOverlay)
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.FillHeight(1)
				[
					SAssignNew(LeftNodeBox, SVerticalBox)
				]
				+ SVerticalBox::Slot()
				.FillHeight(1)
				[
					SAssignNew(RightNodeBox, SVerticalBox)
				]
			]
			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Padding(8.0f)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::GetBrush("Graph.StateNode.ColorSpill"))
				.BorderBackgroundColor(TitleShadowColor)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.Visibility(EVisibility::SelfHitTestInvisible)
				.Padding(6.0f)
				[
					SAssignNew(NodeBody, SVerticalBox)
					.Visibility(EVisibility::Visible)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.AutoWidth()
						[
							SAssignNew(ErrorText, SErrorText)
							.BackgroundColor(this, &SToroGraphNode::GetErrorColor)
							.ToolTipText(this, &SToroGraphNode::GetErrorMsgToolTip)
						]
						+SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(VAlign_Center)
						[
							SNew(SImage)
							.Image(NodeTypeIcon)
						]
						+SHorizontalBox::Slot()
						.Padding(FMargin(4.0f, 0.0f, 4.0f, 0.0f))
						[
							SNew(SVerticalBox)
							+SVerticalBox::Slot()
							.AutoHeight()
							[
								SAssignNew(InlineEditableText, SInlineEditableTextBlock)
								.Style(FAppStyle::Get(), "Graph.StateNode.NodeTitleInlineEditableText")
								.Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
								.OnVerifyTextChanged(this, &SToroGraphNode::OnVerifyNameTextChanged)
								.OnTextCommitted(this, &SToroGraphNode::OnNameCommitted)
								.IsReadOnly(this, &SToroGraphNode::IsNameReadOnly)
								.IsSelected(this, &SToroGraphNode::IsSelectedExclusively)
							]
							+SVerticalBox::Slot()
							.AutoHeight()
							[
								NodeTitle.ToSharedRef()
							]
						]
					]
					+SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(STextBlock)
						.AutoWrapText(true)
						.WrapTextAt(325.0f)
						.Text(this, &SToroGraphNode::GetDescription)
					]
					+SVerticalBox::Slot()
					.Padding(0.0f, 10.0f, 0.0f, 0.0f)
					.AutoHeight()
					[
						SNew(STextBlock)
						.Font(IdentifierFont)
						.Text(this, &SToroGraphNode::GetIdentifierText)
					]
				]
			]
		]
	];

	TSharedPtr<SCommentBubble> CommentBubble;
	const FSlateColor CommentColor = GetDefault<UGraphEditorSettings>()->DefaultCommentNodeTitleColor;
	
	SAssignNew(CommentBubble, SCommentBubble)
		.GraphNode(GraphNode)
		.Text(this, &SGraphNode::GetNodeComment)
		.OnTextCommitted(this, &SGraphNode::OnCommentTextCommitted)
		.ColorAndOpacity(CommentColor)
		.AllowPinning(true)
		.EnableTitleBarBubble(true)
		.EnableBubbleCtrls(true)
		.GraphLOD(this, &SGraphNode::GetCurrentLOD)
		.IsGraphNodeHovered(this, &SGraphNode::IsHovered);
	
	GetOrAddSlot(ENodeZone::TopCenter)
		.SlotOffset2f(TAttribute<FVector2f>(CommentBubble.Get(), &SCommentBubble::GetOffset2f))
		.SlotSize2f(TAttribute<FVector2f>(CommentBubble.Get(), &SCommentBubble::GetSize2f))
		.AllowScaling(TAttribute<bool>(CommentBubble.Get(), &SCommentBubble::IsScalingAllowed))
		.VAlign(VAlign_Top)
		[
			CommentBubble.ToSharedRef()
		];

	ErrorReporting = ErrorText;
	ErrorReporting->SetError(ErrorMsg);
	CreatePinWidgets();
}

void SToroGraphNode::CreatePinWidgets()
{
	UEdToroGraphNode* StateNode = CastChecked<UEdToroGraphNode>(GraphNode);
	for (int32 PinIdx = 0; PinIdx < StateNode->Pins.Num(); PinIdx++)
	{
		if (UEdGraphPin* MyPin = StateNode->Pins[PinIdx]; !MyPin->bHidden)
		{
			TSharedPtr<SGraphPin> NewPin = SNew(SToroGraphPin, MyPin);
			AddPin(NewPin.ToSharedRef());
		}
	}
}

void SToroGraphNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));
	const UEdGraphPin* PinObj = PinToAdd->GetPinObj();
	if (PinObj && PinObj->bAdvancedView)
	{
		PinToAdd->SetVisibility(TAttribute<EVisibility>(PinToAdd, &SGraphPin::IsPinVisibleAsAdvanced));
	}

	TSharedPtr<SVerticalBox> PinBox;
	if (PinToAdd->GetDirection() == EGPD_Input)
	{
		PinBox = LeftNodeBox;
		InputPins.Add(PinToAdd);
	}
	else
	{
		PinBox = RightNodeBox;
		OutputPins.Add(PinToAdd);
	}

	if (PinBox)
	{
		PinBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillHeight(1.0f)
			[
				PinToAdd
			];
	}
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FText SToroGraphNode::GetDescription() const
{
	const UEdToroGraphNode* Node = CastChecked<UEdToroGraphNode>(GraphNode);
	return Node ? Node->GetDescription() : INVTEXT("Unknown Node");
}

FText SToroGraphNode::GetIdentifierText() const
{
	const UEdToroGraphNode* Node = CastChecked<UEdToroGraphNode>(GraphNode);
	return Node ? Node->GetIdentifierText() : INVTEXT("Unknown ID");
}

FSlateColor SToroGraphNode::GetBackgroundColor() const
{
	const UEdToroGraphNode* Node = CastChecked<UEdToroGraphNode>(GraphNode);
	return Node ? Node->GetBackgroundColor() : ToroGraphStatics::InvalidBgColor;
}

void SToroGraphNode::OnNameCommitted(const FText& InText, ETextCommit::Type CommitInfo)
{
	OnNameTextCommited(InText, CommitInfo);
	UEdToroGraphNode* Node = CastChecked<UEdToroGraphNode>(GraphNode);
	if (Node && Node->GraphNode)
	{
		const FScopedTransaction Transaction(LOCTEXT("RenameNode", "ToroGraph Editor: Rename Node"));
		Node->Modify();
		Node->GraphNode->Modify();
		Node->GraphNode->SetNodeTitle(InText);
		UpdateGraphNode();
	}
}

#undef LOCTEXT_NAMESPACE