// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroGraphEd.h"
#include "Interfaces/IPluginManager.h"
#include "Nodes/ToroGraphNodeFactory.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateStyleMacros.h"
#include "EdGraphUtilities.h"

DEFINE_LOG_CATEGORY(LogToroGraphEd)

#define LOCTEXT_NAMESPACE "FToroGraphEdModule"

void FToroGraphEdModule::StartupModule()
{
	FToroGraphEdStyle::Init();
	DataGraphNodeFactory = MakeShareable(new FToroGraphNodeFactory());
	if (DataGraphNodeFactory.IsValid())
	{
		FEdGraphUtilities::RegisterVisualNodeFactory(DataGraphNodeFactory);
	}
}

void FToroGraphEdModule::ShutdownModule()
{
	FToroGraphEdStyle::Shutdown();
	if (DataGraphNodeFactory.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualNodeFactory(DataGraphNodeFactory);
		DataGraphNodeFactory.Reset();
	}
}

#define RootToContentDir StyleSet->RootToContentDir
TSharedPtr<FSlateStyleSet> FToroGraphEdStyle::StyleSet = nullptr;
void FToroGraphEdStyle::Init()
{
	if (StyleSet.IsValid()) return;
	StyleSet = MakeShareable(new FSlateStyleSet("ToroGraphEdStyle"));
	StyleSet->SetContentRoot(IPluginManager::Get().FindPlugin(
		TEXT("ToroUtilities"))->GetBaseDir() / TEXT("Resources"));
    
	StyleSet->Set("ClassThumbnail.ToroDataGraph",
		new IMAGE_BRUSH_SVG(TEXT("DataGraph"), FVector2D(64.0f)));

	StyleSet->Set("ToroGraph.AutoArrange",
		new IMAGE_BRUSH_SVG(TEXT("DataGraphArrange"), FVector2D(20.0f)));
	
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

void FToroGraphEdStyle::Shutdown()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}
#undef RootToContentDir

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FToroGraphEdModule, ToroGraphEd)