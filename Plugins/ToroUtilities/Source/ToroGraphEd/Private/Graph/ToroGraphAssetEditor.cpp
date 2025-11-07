// Copyright (C) RedCraft86. All Rights Reserved.

#include "Graph/ToroGraphAssetEditor.h"

#include "EdGraphUtilities.h"
#include "EditorClassUtils.h"
#include "Graph/ToroGraphToolbar.h"

#include "GraphEditorActions.h"
#include "ToroGraphEd.h"
#include "ToroGraphLayout.h"
#include "Framework/Commands/GenericCommands.h"
#include "Graph/EdToroGraph.h"
#include "Graph/EdToroGraphSchema.h"
#include "Graph/ToroGraphCommands.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Nodes/EdToroGraphEdge.h"
#include "Nodes/EdToroGraphNode.h"
#include "UObject/ObjectSaveContext.h"
#include "Windows/WindowsPlatformApplicationMisc.h"

#define LOCTEXT_NAMESPACE "ToroGraphEd"

FToroGraphAssetEditor::FToroGraphAssetEditor()
{
	EditingGraph = nullptr;
	OnPackageSavedDelegateHandle = UPackage::PackageSavedWithContextEvent.AddRaw(
		this, &FToroGraphAssetEditor::OnPackageSavedWithContext);
}

FToroGraphAssetEditor::~FToroGraphAssetEditor()
{
	UPackage::PackageSavedWithContextEvent.Remove(OnPackageSavedDelegateHandle);
}

void FToroGraphAssetEditor::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UToroDataGraph* Graph)
{
	EditingGraph = Graph;
	CreateEdGraph();

	FGenericCommands::Register();
	FGraphEditorCommands::Register();
	FToroGraphCommands::Register();

	if (!ToolbarBuilder.IsValid())
	{
		ToolbarBuilder = MakeShareable(new FToroGraphToolbar(SharedThis(this)));
	}

	BindCommands();
	CreateInternalWidgets();
	const TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarBuilder->AddDataGraphToolbar(ToolbarExtender);

	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_DataGraphEditor_Layout_v2")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)->SetSizeCoefficient(0.9f)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.65f)
					->AddTab(ToroGraphStatics::ViewportID, ETabState::OpenedTab)->SetHideTabWell(true)
				)
				->Split
				(
				FTabManager::NewStack()
					->SetSizeCoefficient(0.55f)
					->AddTab(ToroGraphStatics::PropertyID, ETabState::OpenedTab)->SetHideTabWell(true)
				)
			)
		);

	InitAssetEditor(Mode, InitToolkitHost, ToroGraphStatics::EditorAppName,
		StandaloneDefaultLayout, true, true, EditingGraph);

	RegenerateMenusAndToolbars();
}

void FToroGraphAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu", "ToroGraph Editor"));
	const TSharedRef<FWorkspaceItem> WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
	
	InTabManager->RegisterTabSpawner(ToroGraphStatics::ViewportID, FOnSpawnTab::CreateSP(
		this, &FToroGraphAssetEditor::SpawnTab_Viewport))
		.SetDisplayName(LOCTEXT("GraphCanvasTab", "Viewport"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.EventGraph_16x"));

	InTabManager->RegisterTabSpawner(ToroGraphStatics::PropertyID, FOnSpawnTab::CreateSP(
		this, &FToroGraphAssetEditor::SpawnTab_Details))
		.SetDisplayName(LOCTEXT("DetailsTab", "Property"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FToroGraphAssetEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	InTabManager->UnregisterTabSpawner(ToroGraphStatics::ViewportID);
	InTabManager->UnregisterTabSpawner(ToroGraphStatics::PropertyID);
}

void FToroGraphAssetEditor::PostRegenerateMenusAndToolbars()
{
	TArray<UObject*> Objs = GetEditingObjects();
	
	TArray<UClass*> ClassList;
	for (const UObject* Obj : Objs)
	{
		check(Obj);
		ClassList.Add(Obj->GetClass());
	}

	const UClass* CommonBaseClass = UClass::FindCommonBase(ClassList);
	const bool bNotAllSame = Objs.Num() > 0 && Objs[0]->GetClass() != CommonBaseClass;

	if (!CommonBaseClass) return;

	if (CommonBaseClass->ClassGeneratedBy)
	{
		const TSharedRef<SHorizontalBox> MenuOverlayBox = SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.ColorAndOpacity(FSlateColor::UseSubduedForeground())
			.ShadowOffset(FVector2D::UnitVector)
			.Text(NSLOCTEXT("GenericEditor", "BlueprintEditor_ParentClass", "Parent class: "))
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(SSpacer)
			.Size(FVector2D(2.0f, 1.0f))
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.ShadowOffset(FVector2D::UnitVector)
			.Text(FText::FromName(CommonBaseClass->ClassGeneratedBy->GetFName()))
			.TextStyle(FAppStyle::Get(), "Common.InheritedFromBlueprintTextStyle")
			.ToolTipText(NSLOCTEXT("GenericEditor", "ParentClassToolTip", "The class that the current Blueprint is based on. The parent provides the base definition, which the current Blueprint extends."))
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
			.VAlign(VAlign_Center)
			.ButtonStyle(FAppStyle::Get(), "HoverHintOnly")
			.OnClicked(this, &FToroGraphAssetEditor::OnFindBlueprintParent, CommonBaseClass->ClassGeneratedBy)
			.ToolTipText(NSLOCTEXT("GenericEditor", "FindParentInCBToolTip", "Find parent in Content Browser"))
			.ContentPadding(4.0f)
			.ForegroundColor(FSlateColor::UseForeground())
			[
				SNew(SImage)
				.Image(FAppStyle::GetBrush("Icons.Search"))
			]
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
			.VAlign(VAlign_Center)
			.ButtonStyle(FAppStyle::Get(), "HoverHintOnly")
			.OnClicked(this, &FToroGraphAssetEditor::OnEditParentClass, CommonBaseClass->ClassGeneratedBy)
			.ToolTipText(NSLOCTEXT("GenericEditor", "EditParentClassToolTip", "Open parent in editor"))
			.ContentPadding(4.0f)
			.ForegroundColor(FSlateColor::UseForeground())
			[
				SNew(SImage)
				.Image(FAppStyle::GetBrush("Icons.Edit"))
			]
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(SSpacer)
			.Size(FVector2D(8.0f, 1.0f))
		];
		SetMenuOverlay(MenuOverlayBox);
	}
	else
	{
		const TSharedRef<SHorizontalBox> MenuOverlayBox = SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.ColorAndOpacity(FSlateColor::UseSubduedForeground())
			.ShadowOffset(FVector2D::UnitVector)
			.Text(bNotAllSame ? NSLOCTEXT("GenericEditor", "SimpleAssetEditor_AssetType_Varied", "Common Asset Type: ") : LOCTEXT("SimpleAssetEditor_AssetType", "Asset Type: "))
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		.Padding(0.0f, 0.0f, 8.0f, 0.0f)
		[
			FEditorClassUtils::GetSourceLink(CommonBaseClass)
		];
		SetMenuOverlay(MenuOverlayBox);
	}
}

FName FToroGraphAssetEditor::GetToolkitFName() const
{
	return FName("FToroGraphEditor");
}

FText FToroGraphAssetEditor::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "ToroGraph Editor");
}

FText FToroGraphAssetEditor::GetToolkitName() const
{
	return FText::FromString(EditingGraph->GetName());
}

FText FToroGraphAssetEditor::GetToolkitToolTipText() const
{
	return GetToolTipTextForObject(EditingGraph);
}

FLinearColor FToroGraphAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor::White;
}

FString FToroGraphAssetEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("FToroGraphEditor");
}

FString FToroGraphAssetEditor::GetDocumentationLink() const
{
	return TEXT("https://github.com/jinyuliao/GenericGraph");
}

void FToroGraphAssetEditor::SaveAsset_Execute()
{
	if (EditingGraph) RebuildDataGraph();
	FAssetEditorToolkit::SaveAsset_Execute();
}

void FToroGraphAssetEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(EditingGraph);
	Collector.AddReferencedObject(EditingGraph->EdGraph);
}

void FToroGraphAssetEditor::RegisterToolbarTab(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}

TSharedRef<SDockTab> FToroGraphAssetEditor::SpawnTab_Viewport(const FSpawnTabArgs& Args) const
{
	check(Args.GetTabId() == ToroGraphStatics::ViewportID);
	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab).Label(LOCTEXT("ViewportLabel", "Viewport"));
	if (ViewportWidget.IsValid()) SpawnedTab->SetContent(ViewportWidget.ToSharedRef());
	return SpawnedTab;
}

TSharedRef<SDockTab> FToroGraphAssetEditor::SpawnTab_Details(const FSpawnTabArgs& Args) const
{
	check(Args.GetTabId() == ToroGraphStatics::PropertyID);
	return SNew(SDockTab).Label(LOCTEXT("DetailsLabel", "Property"))[ PropertyWidget.ToSharedRef() ];
}

void FToroGraphAssetEditor::CreateInternalWidgets()
{
	ViewportWidget = CreateViewportWidget();

	FDetailsViewArgs Args;
	Args.bHideSelectionTip = true;
	Args.NotifyHook = this;

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyWidget = PropertyModule.CreateDetailView(Args);
	PropertyWidget->SetObject(EditingGraph);
	PropertyWidget->OnFinishedChangingProperties().AddSP(
		this, &FToroGraphAssetEditor::OnFinishedChangingProperties);
}

TSharedRef<SGraphEditor> FToroGraphAssetEditor::CreateViewportWidget()
{
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("AppearanceCornerText_DataGraph", "Data Graph");

	CreateCommandList();

	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(
		this, &FToroGraphAssetEditor::OnSelectedNodesChanged);

	return SNew(SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.IsEditable(true)
		.Appearance(AppearanceInfo)
		.GraphToEdit(EditingGraph->EdGraph)
		.GraphEvents(InEvents)
		.AutoExpandActionMenu(true)
		.ShowGraphStateOverlay(false);
}

void FToroGraphAssetEditor::BindCommands()
{
	ToolkitCommands->MapAction(FToroGraphCommands::Get().AutoArrange,
		FExecuteAction::CreateSP(this, &FToroGraphAssetEditor::AutoArrange),
		FCanExecuteAction::CreateSP(this, &FToroGraphAssetEditor::CanAutoArrange)
	);
}

void FToroGraphAssetEditor::CreateEdGraph() const
{
	if (!EditingGraph->EdGraph)
	{
		EditingGraph->EdGraph = CastChecked<UEdToroGraph>(FBlueprintEditorUtils::CreateNewGraph(EditingGraph,
			NAME_None, UEdToroGraph::StaticClass(), UEdToroGraphSchema::StaticClass()));

		EditingGraph->EdGraph->bAllowDeletion = false;
		const UEdGraphSchema* Schema = EditingGraph->EdGraph->GetSchema();
		Schema->CreateDefaultNodesForGraph(*EditingGraph->EdGraph);
	}
}

void FToroGraphAssetEditor::CreateCommandList()
{
	if (GraphEditorCommands.IsValid()) return;
	GraphEditorCommands = MakeShareable(new FUICommandList);

	GraphEditorCommands->MapAction(FToroGraphCommands::Get().AutoArrange,
		FExecuteAction::CreateRaw(this, &FToroGraphAssetEditor::AutoArrange),
		FCanExecuteAction::CreateRaw(this, &FToroGraphAssetEditor::CanAutoArrange)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().SelectAll,
		FExecuteAction::CreateRaw(this, &FToroGraphAssetEditor::SelectAllNodes),
		FCanExecuteAction::CreateRaw(this, &FToroGraphAssetEditor::CanSelectAllNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
		FExecuteAction::CreateRaw(this, &FToroGraphAssetEditor::DeleteSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FToroGraphAssetEditor::CanDeleteNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Copy,
		FExecuteAction::CreateRaw(this, &FToroGraphAssetEditor::CopySelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FToroGraphAssetEditor::CanCopyNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Cut,
		FExecuteAction::CreateRaw(this, &FToroGraphAssetEditor::CutSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FToroGraphAssetEditor::CanCutNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Paste,
		FExecuteAction::CreateRaw(this, &FToroGraphAssetEditor::PasteNodes),
		FCanExecuteAction::CreateRaw(this, &FToroGraphAssetEditor::CanPasteNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Duplicate,
		FExecuteAction::CreateRaw(this, &FToroGraphAssetEditor::DuplicateNodes),
		FCanExecuteAction::CreateRaw(this, &FToroGraphAssetEditor::CanDuplicateNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Rename,
		FExecuteAction::CreateSP(this, &FToroGraphAssetEditor::OnRenameNode),
		FCanExecuteAction::CreateSP(this, &FToroGraphAssetEditor::CanRenameNodes)
	);
}

void FToroGraphAssetEditor::RebuildDataGraph() const
{
	if (EditingGraph == nullptr)
	{
		UE_LOG(LogToroGraphEd, Warning, TEXT("FToroGraphAssetEditor::RebuildDataGraph EditingGraph is nullptr"));
		return;
	}

	UEdToroGraph* EdGraph = Cast<UEdToroGraph>(EditingGraph->EdGraph);
	check(EdGraph);
	EdGraph->RebuildDataGraph();
}

TSharedPtr<SGraphEditor> FToroGraphAssetEditor::GetCurrGraphEditor() const
{
	return ViewportWidget;
}

FGraphPanelSelectionSet FToroGraphAssetEditor::GetSelectedNodes() const
{
	return GetCurrGraphEditor().IsValid() ? GetCurrGraphEditor()->GetSelectedNodes() : FGraphPanelSelectionSet{};
}

void FToroGraphAssetEditor::SelectAllNodes() const
{
	if (GetCurrGraphEditor().IsValid()) GetCurrGraphEditor()->SelectAllNodes();
}

bool FToroGraphAssetEditor::CanSelectAllNodes() const
{
	return true;
}

void FToroGraphAssetEditor::DeleteSelectedNodes() const
{
	const TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	const FScopedTransaction Transaction(FGenericCommands::Get().Delete->GetDescription());

	CurrentGraphEditor->GetCurrentGraph()->Modify();

	const FGraphPanelSelectionSet SelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		UEdGraphNode* EdNode = Cast<UEdGraphNode>(*NodeIt);
		if (EdNode == nullptr || !EdNode->CanUserDeleteNode())
			continue;
		
		if (UEdToroGraphNode* EdNode_Node = Cast<UEdToroGraphNode>(EdNode))
		{
			EdNode_Node->Modify();
			if (const UEdGraphSchema* Schema = EdNode_Node->GetSchema()) Schema->BreakNodeLinks(*EdNode_Node);
			EdNode_Node->DestroyNode();
		}
		else
		{
			EdNode->Modify();
			EdNode->DestroyNode();
		}
	}
}

bool FToroGraphAssetEditor::CanDeleteNodes() const
{
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		if (const UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter); Node && Node->CanUserDeleteNode())
		{
			return true;
		}
	}

	return false;
}

void FToroGraphAssetEditor::DeleteSelectedDuplicateNodes() const
{
	const TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid()) return;

	const FGraphPanelSelectionSet OldSelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode())
		{
			CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}

	DeleteSelectedNodes();

	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter))
		{
			CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}
}

void FToroGraphAssetEditor::CutSelectedNodes() const
{
	CopySelectedNodes();
	DeleteSelectedDuplicateNodes();
}

bool FToroGraphAssetEditor::CanCutNodes() const
{
	return CanCopyNodes() && CanDeleteNodes();
}

void FToroGraphAssetEditor::CopySelectedNodes() const
{
	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();

	FString ExportedText;

	for (FGraphPanelSelectionSet::TIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node == nullptr)
		{
			SelectedIter.RemoveCurrent();
			continue;
		}

		if (UEdToroGraphEdge* EdNode_Edge = Cast<UEdToroGraphEdge>(*SelectedIter))
		{
			const UEdToroGraphNode* StartNode = EdNode_Edge->GetStartNode();
			const UEdToroGraphNode* EndNode = EdNode_Edge->GetEndNode();
			if (!SelectedNodes.Contains(StartNode) || !SelectedNodes.Contains(EndNode))
			{
				SelectedIter.RemoveCurrent();
				continue;
			}
		}

		Node->PrepareForCopying();
	}

	FEdGraphUtilities::ExportNodesToText(SelectedNodes, ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
}

bool FToroGraphAssetEditor::CanCopyNodes() const
{
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		if (const UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter); Node && Node->CanDuplicateNode())
		{
			return true;
		}
	}

	return false;
}

void FToroGraphAssetEditor::PasteNodes() const
{
	if (GetCurrGraphEditor().IsValid()) PasteNodesHere(GetCurrGraphEditor()->GetPasteLocation2f());
}

void FToroGraphAssetEditor::PasteNodesHere(const FVector2f& Location) const
{
	const TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid()) return;

	UEdGraph* EdGraph = CurrentGraphEditor->GetCurrentGraph();
	{
		const FScopedTransaction Transaction(FGenericCommands::Get().Paste->GetDescription());
		EdGraph->Modify();

		CurrentGraphEditor->ClearSelectionSet();

		FString TextToImport;
		FPlatformApplicationMisc::ClipboardPaste(TextToImport);

		TSet<UEdGraphNode*> PastedNodes;
		FEdGraphUtilities::ImportNodesFromText(EdGraph, TextToImport, PastedNodes);

		FVector2D AvgNodePosition(0.0f, 0.0f);
		for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
		{
			const UEdGraphNode* Node = *It;
			AvgNodePosition.X += Node->NodePosX;
			AvgNodePosition.Y += Node->NodePosY;
		}

		const float InvNumNodes = 1.0f / (float)PastedNodes.Num();
		AvgNodePosition.X *= InvNumNodes;
		AvgNodePosition.Y *= InvNumNodes;

		for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
		{
			UEdGraphNode* Node = *It;
			CurrentGraphEditor->SetNodeSelection(Node, true);
			Node->NodePosX = Node->NodePosX - AvgNodePosition.X + Location.X;
			Node->NodePosY = Node->NodePosY - AvgNodePosition.Y + Location.Y;
			Node->SnapToGrid(16);
			Node->CreateNewGuid();
		}
	}

	CurrentGraphEditor->NotifyGraphChanged();
	if (UObject* GraphOwner = EdGraph->GetOuter())
	{
		GraphOwner->PostEditChange();
		GraphOwner->MarkPackageDirty();
	}
}

bool FToroGraphAssetEditor::CanPasteNodes() const
{
	const TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid()) return false;

	FString ClipboardContent;
	FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);
	return FEdGraphUtilities::CanImportNodesFromText(CurrentGraphEditor->GetCurrentGraph(), ClipboardContent);
}

void FToroGraphAssetEditor::DuplicateNodes() const
{
	CopySelectedNodes();
	PasteNodes();
}

bool FToroGraphAssetEditor::CanDuplicateNodes() const
{
	return CanCopyNodes();
}

void FToroGraphAssetEditor::GraphSettings() const
{
	PropertyWidget->SetObject(EditingGraph);
}

bool FToroGraphAssetEditor::CanGraphSettings() const
{
	return true;
}

void FToroGraphAssetEditor::AutoArrange() const
{
	UEdToroGraph* EdGraph = Cast<UEdToroGraph>(EditingGraph->EdGraph);
	check(EdGraph);
	
	const FScopedTransaction Transaction(LOCTEXT("EdToroGraphArrange", "ToroGraph Editor: Auto Arrange"));
	
	EdGraph->Modify();
	
	if (UToroGraphLayout* LayoutStrategy = UToroGraphLayout::Get())
	{
		LayoutStrategy->Layout(EdGraph);
	}
	else
	{
		UE_LOG(LogToroGraphEd, Error, TEXT("FToroGraphAssetEditor::AutoArrange Layout is null."));
	}
}

bool FToroGraphAssetEditor::CanAutoArrange() const
{
	return EditingGraph && Cast<UEdToroGraph>(EditingGraph->EdGraph);
}

void FToroGraphAssetEditor::OnRenameNode() const
{
	if (GetCurrGraphEditor().IsValid())
	{
		const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
		for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
		{
			const UEdGraphNode* SelectedNode = Cast<UEdGraphNode>(*NodeIt);
			if (SelectedNode && SelectedNode->bCanRenameNode)
			{
				GetCurrGraphEditor()->IsNodeTitleVisible(SelectedNode, true);
				break;
			}
		}
	}
}

bool FToroGraphAssetEditor::CanRenameNodes() const
{
	const UEdToroGraph* EdGraph = Cast<UEdToroGraph>(EditingGraph->EdGraph);
	check(EdGraph);

	const UToroDataGraph* Graph = EdGraph->GetGraph();
	check(Graph)
	
	return GetSelectedNodes().Num() == 1;
}

void FToroGraphAssetEditor::OnSelectedNodesChanged(const TSet<UObject*>& NewSelection)
{
	TArray<UObject*> Selection;
	for (UObject* SelectionEntry : NewSelection)
	{
		Selection.Add(SelectionEntry);
	}

	if (Selection.IsEmpty()) 
	{
		PropertyWidget->SetObject(EditingGraph);

	}
	else
	{
		PropertyWidget->SetObjects(Selection);
	}
}

void FToroGraphAssetEditor::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
	if (EditingGraph) EditingGraph->EdGraph->GetSchema()->ForceVisualizationCacheClear();
}

void FToroGraphAssetEditor::OnPackageSavedWithContext(const FString& PackageFileName, UPackage* Package, FObjectPostSaveContext ObjectSaveContext)
{
	RebuildDataGraph();
}

FReply FToroGraphAssetEditor::OnFindBlueprintParent(TObjectPtr<UObject> SyncToClass) const
{
	if (SyncToClass) GEditor->SyncBrowserToObjects({SyncToClass});
	return FReply::Handled();
}

FReply FToroGraphAssetEditor::OnEditParentClass(TObjectPtr<UObject> EditClass) const
{
	if (EditClass) GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(EditClass);
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
