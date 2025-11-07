// Copyright (C) RedCraft86. All Rights Reserved.

#include "Graph/ToroGraphToolbar.h"
#include "Graph/ToroGraphAssetEditor.h"
#include "Graph/ToroGraphCommands.h"
#include "ToroGraphEd.h"

#define LOCTEXT_NAMESPACE "ToroGraphEd"

void FToroGraphToolbar::AddDataGraphToolbar(TSharedPtr<FExtender> Extender)
{
	check(GraphAssetEditor.IsValid());
	const TSharedPtr<FToroGraphAssetEditor> GraphEditorPtr = GraphAssetEditor.Pin();

	const TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension("Asset",
		EExtensionHook::After, GraphEditorPtr->GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateSP(this, &FToroGraphToolbar::FillDataGraphToolbar));
	
	GraphEditorPtr->AddToolbarExtender(ToolbarExtender);
}

void FToroGraphToolbar::FillDataGraphToolbar(FToolBarBuilder& ToolbarBuilder) const
{
	check(GraphAssetEditor.IsValid());
	ToolbarBuilder.BeginSection("Util");
	{
		ToolbarBuilder.AddToolBarButton(FToroGraphCommands::Get().AutoArrange,
			NAME_None,
			LOCTEXT("AutoArrange_Label", "Auto Arrange"),
			LOCTEXT("AutoArrange_ToolTip", "Auto arrange nodes. Not perfect but still handy."),
			FSlateIcon(FToroGraphEdStyle::GetStyleSetName(), "ToroGraph.AutoArrange"));
	}
	ToolbarBuilder.EndSection();
}

#undef LOCTEXT_NAMESPACE