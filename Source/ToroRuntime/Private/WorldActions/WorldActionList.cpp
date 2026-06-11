// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "WorldActions/WorldActionList.h"

void FWorldActionList::Execute(UObject* WorldContext) const
{
	ForEachAction([WorldContext](UWorldActionBase* Action) {
		Action->SetWorldContext(WorldContext);
		Action->RunAction();
	});
}

void FWorldActionList::Initialize(UObject* WorldContext) const
{
	ForEachAction([WorldContext](UWorldActionBase* Action) {
		Action->SetWorldContext(WorldContext);
		Action->OnInitialize();
	});
}

void FWorldActionList::ForEachAction(const TFunction<void(UWorldActionBase*)>& Func) const
{
	if (!Func) return;
	for (const TObjectPtr<UWorldActionBase>& Action : Actions)
	{
		if (Action) Func(Action);
	}
}
