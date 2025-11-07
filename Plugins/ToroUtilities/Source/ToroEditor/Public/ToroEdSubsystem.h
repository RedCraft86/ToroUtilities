// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "EditorSubsystem.h"
#include "Helpers/ClassGetterMacros.h"
#include "ToroEdSubsystem.generated.h"

UCLASS()
class TOROEDITOR_API UToroEdSubsystem final : public UEditorSubsystem
{
	GENERATED_BODY()

public:

	UToroEdSubsystem() {}

	EDITOR_SUBSYSTEM_GETTER(UToroEdSubsystem)

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
