// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GraphEditorDragDropAction.h"

struct FGraphPinHandle;

class FToroGraphDragConnection final : public FGraphEditorDragDropAction
{
public:

	DRAG_DROP_OPERATOR_TYPE(FDragConnection, FGraphEditorDragDropAction)

	static TSharedRef<FToroGraphDragConnection> New(const TSharedRef<SGraphPanel>& InGraphPanel, const TArray<FGraphPinHandle>& InStartingPins);

	// FDragDropOperation interface
	virtual void OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent) override;
	// End of FDragDropOperation interface

	// FGraphEditorDragDropAction interface
	virtual void HoverTargetChanged() override;
	virtual FReply DroppedOnPin(const FVector2f& ScreenPosition, const FVector2f& GraphPosition) override;
	virtual FReply DroppedOnNode(const FVector2f& ScreenPosition, const FVector2f& GraphPosition) override;
	virtual FReply DroppedOnPanel(const TSharedRef<SWidget>& Panel, const FVector2f& ScreenPosition, const FVector2f& GraphPosition, UEdGraph& Graph) override;
	virtual void OnDragged(const FDragDropEvent& DragDropEvent) override;
	// End of FGraphEditorDragDropAction interface

	void ValidateGraphPinList(TArray<UEdGraphPin*>& OutValidPins);

private:
	
	FVector2f DecoratorAdjust;
	TSharedPtr<SGraphPanel> GraphPanel;
	TArray<FGraphPinHandle> DraggingPins;

	FToroGraphDragConnection(const TSharedRef<SGraphPanel>& InGraphPanel, const TArray<FGraphPinHandle>& DraggedPins);
};