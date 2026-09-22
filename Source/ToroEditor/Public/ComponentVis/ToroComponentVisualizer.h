// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "ComponentVisualizer.h"
#include "ToroVisualizerHelpers.h"

/**
 * Base component visualizer with optional world-space and HUD drawing hooks.
 */
class TOROEDITOR_API FToroComponentVisualizer : public FComponentVisualizer
{
	/**
	 * Draws world-space visualization for a component.
	 * @param Component Component being visualized.
	 * @param View Scene view used to render the component.
	 * @param PDI Primitive drawing interface receiving world-space elements.
	 */
	virtual void DrawVisualization(const UActorComponent* Component,
		const FSceneView* View, FPrimitiveDrawInterface* PDI) override
	{}

	/**
	 * Draws screen-space visualization for a component.
	 * @param Component Component being visualized.
	 * @param Viewport Viewport receiving the HUD elements.
	 * @param View Scene view used to project world positions.
	 * @param Canvas Canvas receiving screen-space elements.
	 */
	virtual void DrawVisualizationHUD(const UActorComponent* Component,
		const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override
	{}
};
