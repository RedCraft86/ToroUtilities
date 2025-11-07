// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "EdGraphUtilities.h"

class FToroGraphNodeFactory final : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<SGraphNode> CreateNode(UEdGraphNode* Node) const override;
};