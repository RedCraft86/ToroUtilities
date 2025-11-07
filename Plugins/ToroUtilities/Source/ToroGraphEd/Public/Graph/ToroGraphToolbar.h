// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

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

