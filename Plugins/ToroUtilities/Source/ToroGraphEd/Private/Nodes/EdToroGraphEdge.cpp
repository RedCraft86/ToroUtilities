// Copyright (C) RedCraft86. All Rights Reserved.

#include "Nodes/EdToroGraphEdge.h"
#include "Graph/EdToroGraph.h"
#include "Nodes/EdToroGraphNode.h"

UEdToroGraphEdge::UEdToroGraphEdge(): SEdge(nullptr)
{
	GraphEdge = nullptr;
	bCanRenameNode = true;
}

void UEdToroGraphEdge::SetEdge(UToroDataEdge* Edge)
{
	GraphEdge = Edge;
}

UEdToroGraph* UEdToroGraphEdge::GetEdGraph() const
{
	return Cast<UEdToroGraph>(GetGraph());
}

void UEdToroGraphEdge::CreateConnections(const UEdToroGraphNode* Start, const UEdToroGraphNode* End)
{
	Pins[0]->Modify();
	Pins[0]->LinkedTo.Empty();

	Start->GetOutputPin()->Modify();
	Pins[0]->MakeLinkTo(Start->GetOutputPin());

	Pins[1]->Modify();
	Pins[1]->LinkedTo.Empty();

	End->GetInputPin()->Modify();
	Pins[1]->MakeLinkTo(End->GetInputPin());
}

UEdToroGraphNode* UEdToroGraphEdge::GetEndNode()
{
	return Pins[1]->LinkedTo.IsEmpty() ? nullptr : Cast<UEdToroGraphNode>(Pins[1]->LinkedTo[0]->GetOwningNode());
}

UEdToroGraphNode* UEdToroGraphEdge::GetStartNode()
{
	return Pins[0]->LinkedTo.IsEmpty() ? nullptr : Cast<UEdToroGraphNode>(Pins[0]->LinkedTo[0]->GetOwningNode());
}

void UEdToroGraphEdge::PrepareForCopying()
{
	GraphEdge->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
}

void UEdToroGraphEdge::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, TEXT("Edge"), FName(), TEXT("In"))->bHidden = true;
	CreatePin(EGPD_Output, TEXT("Edge"), FName(), TEXT("Out"))->bHidden = true;
}

void UEdToroGraphEdge::PinConnectionListChanged(UEdGraphPin* Pin)
{
	if (Pin->LinkedTo.IsEmpty())
	{
		Modify();
		if (UEdGraph* ParentGraph = GetGraph())
		{
			ParentGraph->Modify();
		}
		DestroyNode();
	}
}

FText UEdToroGraphEdge::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return GraphEdge ? GraphEdge->GetNodeTitle() : Super::GetNodeTitle(TitleType);
}
