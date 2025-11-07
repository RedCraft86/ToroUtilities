// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

class FToroGraphAssetEditor;

class FToroGraphToolbar : public TSharedFromThis<FToroGraphToolbar>
{
public:
	
	explicit FToroGraphToolbar(const TSharedPtr<FToroGraphAssetEditor>& InEditor)
		: GraphAssetEditor(InEditor)
	{}

	void AddDataGraphToolbar(TSharedPtr<FExtender> Extender);

private:
	
	TWeakPtr<FToroGraphAssetEditor> GraphAssetEditor;
	void FillDataGraphToolbar(FToolBarBuilder& ToolbarBuilder) const;
};

