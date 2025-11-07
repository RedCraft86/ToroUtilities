// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroEditorSettings.h"

UToroEditorSettings::UToroEditorSettings()
	: StartupCommands({
		{TEXT("r.VSyncEditor"), TEXT("1")},
		{TEXT("r.Streaming.PoolSize"), TEXT("3000")}
	}), ALRoot(TEXT("D:/UnrealEngine/AssetProject/"))
	, ALDirs({TEXT("Content/AssetPacks"), TEXT("Saved/Collections")})
{
	CategoryName = TEXT("Project");
	SectionName = TEXT("ToroEditor");
}

TMap<FString, FString> UToroEditorSettings::GetAssetLibraryPaths() const
{
	static FString ProjectPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());

	TMap<FString, FString> Paths;
	for (const FString& Dir : ALDirs)
	{
		Paths.Add(FPaths::Combine(ALRoot.Path, Dir), FPaths::Combine(ProjectPath, Dir));
	}
	return Paths;
}

void UToroEditorSettings::FixAssetLibPaths()
{
	FPaths::NormalizeDirectoryName(ALRoot.Path);
	if (!FPaths::DirectoryExists(ALRoot.Path))
	{
		ALRoot.Path = TEXT("");
	}

	TArray<FString> Files;
	IFileManager::Get().FindFiles(Files, *ALRoot.Path, TEXT(".uproject"));
	if (Files.Num() != 1)
	{
		ALRoot.Path = TEXT("");
	}
}

void UToroEditorSettings::PostInitProperties()
{
	Super::PostInitProperties();
	FixAssetLibPaths();
}

void UToroEditorSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	FixAssetLibPaths();
}
