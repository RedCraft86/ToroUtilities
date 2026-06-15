// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "ComponentVisualizer.h"
#include "ToroVisualizerHelpers.h"

class TOROEDITOR_API FToroComponentVisualizer : public FComponentVisualizer
{
	virtual void DrawVisualization(const UActorComponent* Component,
		const FSceneView* View, FPrimitiveDrawInterface* PDI) override
	{}

	virtual void DrawVisualizationHUD(const UActorComponent* Component,
		const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override
	{}
};