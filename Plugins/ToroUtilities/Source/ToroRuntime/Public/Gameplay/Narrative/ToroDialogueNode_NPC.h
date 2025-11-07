// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "DialogueSM.h"
#include "ToroDialogueNode_NPC.generated.h"

UCLASS()
class TORORUNTIME_API UToroDialogueNode_NPC final : public UDialogueNode_NPC
{
	GENERATED_BODY()

public:

	AActor* GetLookTarget() const { return LookTarget.LoadSynchronous(); }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details - NPC Dialogue Node")
		TSoftObjectPtr<AActor> LookTarget;
};
