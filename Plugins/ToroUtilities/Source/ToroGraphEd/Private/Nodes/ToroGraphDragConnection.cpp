// Copyright (C) RedCraft86. All Rights Reserved.

#include "Nodes/ToroGraphDragConnection.h"
#include "Nodes/EdToroGraphNode.h"
#include "SGraphPanel.h"

TSharedRef<FToroGraphDragConnection> FToroGraphDragConnection::New(
	const TSharedRef<SGraphPanel>& InGraphPanel, const TArray<FGraphPinHandle>& InStartingPins)
{
	TSharedRef<FToroGraphDragConnection> Operation = MakeShareable(new FToroGraphDragConnection(InGraphPanel, InStartingPins));
	Operation->Construct();
	return Operation;
}

void FToroGraphDragConnection::OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent)
{
	GraphPanel->OnStopMakingConnection();
	FGraphEditorDragDropAction::OnDrop(bDropWasHandled, MouseEvent);
}

void FToroGraphDragConnection::HoverTargetChanged()
{
	TArray<FPinConnectionResponse> UniqueMessages;
	if (const UEdGraphPin* TargetPinObj = GetHoveredPin())
	{
		TArray<UEdGraphPin*> ValidSourcePins;
		ValidateGraphPinList(ValidSourcePins);
		for (const UEdGraphPin* StartingPinObj : ValidSourcePins)
		{
			const UEdGraph* GraphObj = StartingPinObj->GetOwningNode()->GetGraph();
			const FPinConnectionResponse Response = GraphObj->GetSchema()->CanCreateConnection(StartingPinObj, TargetPinObj);

			if (Response.Response == CONNECT_RESPONSE_DISALLOW)
			{
				TSharedPtr<SGraphNode> NodeWidget = TargetPinObj->GetOwningNode()->DEPRECATED_NodeWidget.Pin();
				if (NodeWidget.IsValid())
				{
					NodeWidget->NotifyDisallowedPinConnection(StartingPinObj, TargetPinObj);
				}
			}

			UniqueMessages.AddUnique(Response);
		}
	}
	else if (const UEdToroGraphNode* TargetNodeObj = Cast<UEdToroGraphNode>(GetHoveredNode()))
	{
		TArray<UEdGraphPin*> ValidSourcePins;
		ValidateGraphPinList(ValidSourcePins);
		for (const UEdGraphPin* StartingPinObj : ValidSourcePins)
		{
			FText ResponseText;
			FPinConnectionResponse Response;			
			const UEdGraphSchema *Schema = StartingPinObj->GetSchema();
			const UEdGraphPin *TargetPin = TargetNodeObj->GetInputPin();
			if (Schema && TargetPin)
			{
				Response = Schema->CanCreateConnection(StartingPinObj, TargetPin);
				if (Response.Response == CONNECT_RESPONSE_DISALLOW)
				{
					TSharedPtr<SGraphNode> NodeWidget = TargetPin->GetOwningNode()->DEPRECATED_NodeWidget.Pin();
					if (NodeWidget.IsValid())
					{
						NodeWidget->NotifyDisallowedPinConnection(StartingPinObj, TargetPinObj);
					}
				}
			}
			else
			{
				Response = FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW,
					NSLOCTEXT("ToroGraphEd", "PinError", "Not a valid UEdToroGraphNode"));
			}

			UniqueMessages.AddUnique(Response);
		}
	}
	else if (const UEdGraph* CurrentHoveredGraph = GetHoveredGraph())
	{
		TArray<UEdGraphPin*> ValidSourcePins;
		ValidateGraphPinList(ValidSourcePins);
		for (UEdGraphPin* StartingPinObj : ValidSourcePins)
		{
			FPinConnectionResponse Response = CurrentHoveredGraph->GetSchema()->CanCreateNewNodes(StartingPinObj);
			if (!Response.Message.IsEmpty())
			{
				UniqueMessages.AddUnique(Response);
			}
		}
	}

	if (UniqueMessages.IsEmpty())
	{
		SetSimpleFeedbackMessage(FAppStyle::GetBrush(TEXT("Graph.ConnectorFeedback.NewNode")),
			FLinearColor::White, NSLOCTEXT("GraphEditor.Feedback", "PlaceNewNode", "Place a new node."));
	}
	else
	{
		const TSharedRef<SVerticalBox> FeedbackBox = SNew(SVerticalBox);
		for (auto ResponseIt = UniqueMessages.CreateConstIterator(); ResponseIt; ++ResponseIt)
		{

			const FSlateBrush* StatusSymbol = NULL;
			switch (ResponseIt->Response)
			{
			case CONNECT_RESPONSE_MAKE:
			case CONNECT_RESPONSE_BREAK_OTHERS_A:
			case CONNECT_RESPONSE_BREAK_OTHERS_B:
			case CONNECT_RESPONSE_BREAK_OTHERS_AB:
				StatusSymbol = FAppStyle::GetBrush(TEXT("Graph.ConnectorFeedback.OK"));
				break;

			case CONNECT_RESPONSE_MAKE_WITH_CONVERSION_NODE:
				StatusSymbol = FAppStyle::GetBrush(TEXT("Graph.ConnectorFeedback.ViaCast"));
				break;

			case CONNECT_RESPONSE_DISALLOW:
			default:
				StatusSymbol = FAppStyle::GetBrush(TEXT("Graph.ConnectorFeedback.Error"));
				break;
			}

			FeedbackBox->AddSlot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(3.0f)
					.VAlign(VAlign_Center)
					[
						SNew(SImage).Image(StatusSymbol)
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock).Text(ResponseIt->Message)
					]
				];
		}

		SetFeedbackMessage(FeedbackBox);
	}
}

FReply FToroGraphDragConnection::DroppedOnPin(const FVector2f& ScreenPosition, const FVector2f& GraphPosition)
{
	TArray<UEdGraphPin*> ValidSourcePins;
	ValidateGraphPinList(ValidSourcePins);
	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "GraphEd_CreateConnection", "Create Pin Link"));

	bool bError = false;
	TSet<UEdGraphNode*> NodeList;
	UEdGraphPin* PinB = GetHoveredPin();
	for (UEdGraphPin* PinA : ValidSourcePins)
	{
		if (PinA && PinB)
		{
			const UEdGraph* GraphObj = PinA->GetOwningNode()->GetGraph();
			if (GraphObj->GetSchema()->TryCreateConnection(PinA, PinB))
			{
				if (!PinA->IsPendingKill())
				{
					NodeList.Add(PinA->GetOwningNode());
				}
				if (!PinB->IsPendingKill())
				{
					NodeList.Add(PinB->GetOwningNode());
				}
			}
		}
		else
		{
			bError = true;
		}
	}

	for (auto It = NodeList.CreateConstIterator(); It; ++It)
	{
		UEdGraphNode* Node = (*It);
		Node->NodeConnectionListChanged();
	}

	return bError ? FReply::Unhandled() : FReply::Handled();
}

FReply FToroGraphDragConnection::DroppedOnNode(const FVector2f& ScreenPosition, const FVector2f& GraphPosition)
{
	bool bHandledPinDropOnNode = false;
	if (UEdGraphNode* NodeOver = GetHoveredNode())
	{
		TArray<UEdGraphPin*> ValidSourcePins;
		ValidateGraphPinList(ValidSourcePins);
		if (ValidSourcePins.Num())
		{
			for (UEdGraphPin* SourcePin : ValidSourcePins)
			{
				FText ResponseText;
				if (SourcePin->GetOwningNode() != NodeOver && SourcePin->GetSchema()->SupportsDropPinOnNode(
					NodeOver, SourcePin->PinType, SourcePin->Direction, ResponseText))
				{
					bHandledPinDropOnNode = true;
					const FName PinName = SourcePin->PinFriendlyName.IsEmpty()
						? SourcePin->PinName : *SourcePin->PinFriendlyName.ToString();
					
					const FScopedTransaction Transaction(SourcePin->Direction == EGPD_Output
						? NSLOCTEXT("UnrealEd", "AddInParam", "Add In Parameter")
						: NSLOCTEXT("UnrealEd", "AddOutParam", "Add Out Parameter"));
					
					UEdGraphPin* EdGraphPin = NodeOver->GetSchema()->DropPinOnNode(
						GetHoveredNode(), PinName, SourcePin->PinType, SourcePin->Direction);
					
					if (SourcePin->GetOwningNodeUnchecked() && EdGraphPin)
					{
						SourcePin->Modify();
						EdGraphPin->Modify();
						SourcePin->GetSchema()->TryCreateConnection(SourcePin, EdGraphPin);
					}
				}

				if (!bHandledPinDropOnNode && !ResponseText.IsEmpty())
				{
					bHandledPinDropOnNode = true;
				}
			}
		}
	}
	
	return bHandledPinDropOnNode ? FReply::Handled() : FReply::Unhandled();
}

FReply FToroGraphDragConnection::DroppedOnPanel(const TSharedRef<SWidget>& Panel,
	const FVector2f& ScreenPosition, const FVector2f& GraphPosition, UEdGraph& Graph)
{
	TArray<UEdGraphPin*> PinObjects;
	ValidateGraphPinList(PinObjects);

	const TSharedPtr<SWidget> WidgetToFocus = GraphPanel->SummonContextMenu(ScreenPosition,
		GraphPosition, nullptr, nullptr, PinObjects);

	return !WidgetToFocus.IsValid() ? FReply::Handled() : FReply::Handled().SetUserFocus(
		WidgetToFocus.ToSharedRef(),EFocusCause::SetDirectly);
}

void FToroGraphDragConnection::OnDragged(const FDragDropEvent& DragDropEvent)
{
	const FVector2f TargetPosition = DragDropEvent.GetScreenSpacePosition();
	CursorDecoratorWindow->MoveWindowTo(DragDropEvent.GetScreenSpacePosition() + DecoratorAdjust);
	GraphPanel->RequestDeferredPan(TargetPosition);
}

void FToroGraphDragConnection::ValidateGraphPinList(TArray<UEdGraphPin*>& OutValidPins)
{
	OutValidPins.Empty(DraggingPins.Num());
	for (const FGraphPinHandle& PinHandle : DraggingPins)
	{
		if (UEdGraphPin* GraphPin = PinHandle.GetPinObj(*GraphPanel))
		{
			OutValidPins.Add(GraphPin);
		}
	}
}

FToroGraphDragConnection::FToroGraphDragConnection(const TSharedRef<SGraphPanel>& InGraphPanel, const TArray<FGraphPinHandle>& DraggedPins)
	: DecoratorAdjust(FSlateApplication::Get().GetCursorSize()), GraphPanel(InGraphPanel), DraggingPins(DraggedPins)
{
	if (!DraggingPins.IsEmpty())
	{
		const UEdGraphPin* PinObj = TArray<FGraphPinHandle>::TConstIterator(DraggedPins)->GetPinObj(*InGraphPanel);
		if (PinObj && PinObj->Direction == EGPD_Input)
		{
			DecoratorAdjust *= FVector2f(-1.0f, 1.0f);
		}
	}

	for (const FGraphPinHandle& DraggedPin : DraggedPins)
	{
		InGraphPanel->OnBeginMakingConnection(DraggedPin);
	}
}
