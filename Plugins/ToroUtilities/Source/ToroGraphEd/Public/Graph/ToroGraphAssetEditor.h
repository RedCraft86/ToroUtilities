// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroDataGraph.h"

class FToroGraphToolbar;

class FToroGraphAssetEditor final : public FAssetEditorToolkit, public FNotifyHook, public FGCObject
{
public:

	FToroGraphAssetEditor();
	virtual ~FToroGraphAssetEditor() override;

	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UToroDataGraph* Graph);

	// IToolkit interface
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void PostRegenerateMenusAndToolbars() override;
	// ~IToolkit interface

	// FAssetEditorToolkit
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FString GetDocumentationLink() const override;
	virtual void SaveAsset_Execute() override;
	// ~FAssetEditorToolkit

	// FSerializableObject interface
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	// ~FSerializableObject interface

	// FGCObject interface
	virtual FString GetReferencerName() const { return TEXT("FAssetEditor_LTToroDataGraph"); }
	// ~FGCObject interface

	void RegisterToolbarTab(const TSharedRef<FTabManager>& InTabManager);
	TSharedPtr<FToroGraphToolbar> GetToolbarBuilder() { return ToolbarBuilder; }

private:

	TSharedPtr<IDetailsView> PropertyWidget;
	TSharedPtr<SGraphEditor> ViewportWidget;
	TSharedPtr<FToroGraphToolbar> ToolbarBuilder;
	TSharedPtr<FUICommandList> GraphEditorCommands;
	FDelegateHandle OnPackageSavedDelegateHandle;
	TObjectPtr<UToroDataGraph> EditingGraph;

	TSharedRef<SDockTab> SpawnTab_Viewport(const FSpawnTabArgs& Args) const;
	TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& Args) const;

	void CreateInternalWidgets();
	TSharedRef<SGraphEditor> CreateViewportWidget();

	void BindCommands();
	void CreateEdGraph() const;
	void CreateCommandList();
	void RebuildDataGraph() const;
	TSharedPtr<SGraphEditor> GetCurrGraphEditor() const;
	FGraphPanelSelectionSet GetSelectedNodes() const;

	// Delegates for graph editor commands
	void SelectAllNodes() const;
	bool CanSelectAllNodes() const;
	void DeleteSelectedNodes() const;
	bool CanDeleteNodes() const;
	void DeleteSelectedDuplicateNodes() const;
	void CutSelectedNodes() const;
	bool CanCutNodes() const;
	void CopySelectedNodes() const;
	bool CanCopyNodes() const;
	void PasteNodes() const;
	void PasteNodesHere(const FVector2f& Location) const;
	bool CanPasteNodes() const;
	void DuplicateNodes() const;
	bool CanDuplicateNodes() const;

	void GraphSettings() const;
	bool CanGraphSettings() const;

	void AutoArrange() const;
	bool CanAutoArrange() const;

	void OnRenameNode() const;
	bool CanRenameNodes() const;

	void OnSelectedNodesChanged(const TSet<UObject*>& NewSelection);
	void OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent);
	void OnPackageSavedWithContext(const FString& PackageFileName, UPackage* Package, FObjectPostSaveContext ObjectSaveContext);
	FReply OnFindBlueprintParent(TObjectPtr<UObject> SyncToClass) const;
	FReply OnEditParentClass(TObjectPtr<UObject> EditClass) const;
};