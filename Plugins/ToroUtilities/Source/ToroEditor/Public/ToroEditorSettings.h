// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "Helpers/ClassGetterMacros.h"
#include "ToroEditorSettings.generated.h"

UCLASS(Config = Editor, DefaultConfig, DisplayName = "ToroEditor")
class TOROEDITOR_API UToroEditorSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroEditorSettings();
	
	SETTING_GETTER(UToroEditorSettings)

	UPROPERTY(Config, EditAnywhere, Category = Editor)
		TMap<FString, FString> StartupCommands;
	
	UPROPERTY(Config, EditAnywhere, Category = AssetLibrary, DisplayName = "Root Path")
		FDirectoryPath ALRoot;
	
	UPROPERTY(Config, EditAnywhere, Category = AssetLibrary, DisplayName = "Directories")
		TSet<FString> ALDirs;

	TMap<FString, FString> GetAssetLibraryPaths() const;

private:

	void FixAssetLibPaths();
	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
