// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "SGraphNode.h"

class SToroGraphNode final : public SGraphNode
{
public:
	
	SLATE_BEGIN_ARGS(SToroGraphNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, class UEdToroGraphNode* InNode);

	virtual void UpdateGraphNode() override;
	virtual void CreatePinWidgets() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	
	FText GetDescription() const;
	FText GetIdentifierText() const;
	FSlateColor GetBackgroundColor() const;
	void OnNameCommitted(const FText& InText, ETextCommit::Type CommitInfo);
};
