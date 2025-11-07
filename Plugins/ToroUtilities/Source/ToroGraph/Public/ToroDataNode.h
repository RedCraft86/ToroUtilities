// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "ToroDataNode.generated.h"

class UToroDataEdge;
class UToroDataGraph;

UCLASS(NotBlueprintable, BlueprintType)
class TOROGRAPH_API UToroDataNode : public UDataAsset
{
	GENERATED_BODY()

public:

	UToroDataNode();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Node, meta = (SortPriority = -1))
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Node, meta = (MultiLine = true, SortPriority = -1))
		FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Node)
		FGuid NodeID;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Node, AdvancedDisplay)
		FLinearColor BackgroundColor;

	UPROPERTY(VisibleAnywhere, Category = Node, AdvancedDisplay)
		FText ContextMenuName;

	UPROPERTY(VisibleAnywhere, Category = Node, AdvancedDisplay)
		uint8 ParentLimit;

	UPROPERTY(VisibleAnywhere, Category = Node, AdvancedDisplay)
		uint8 ChildLimit;

	UPROPERTY(VisibleAnywhere, Category = Node, AdvancedDisplay)
		TSubclassOf<UToroDataGraph> CompatibleGraph;
#endif

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Node, AdvancedDisplay)
		TObjectPtr<UToroDataGraph> OwningGraph;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Node, AdvancedDisplay, meta = (DisplayThumbnail = false))
		TArray<TObjectPtr<UToroDataNode>> ParentNodes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Node, AdvancedDisplay, meta = (DisplayThumbnail = false))
		TArray<TObjectPtr<UToroDataNode>> ChildNodes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Node, AdvancedDisplay, meta = (DisplayThumbnail = false))
		TMap<TObjectPtr<UToroDataNode>, TObjectPtr<UToroDataEdge>> Edges;

	UFUNCTION(BlueprintPure, Category = "DataGraphs|Node")
		bool IsRootNode() const { return ParentNodes.IsEmpty(); }

	UFUNCTION(BlueprintPure, Category = "DataGraphs|Node")
		bool IsLeafNode() const { return ChildNodes.IsEmpty(); }

	UFUNCTION(BlueprintPure, Category = "DataGraphs|Node")
		UToroDataEdge* GetEdge(UToroDataNode* ChildNode) const;

#if WITH_EDITOR
	virtual FText GetNodeTitle() const;
	virtual FText GetNodeDescription() const;
	virtual void SetNodeTitle(const FText& NewTitle);
	virtual bool CanConnectFrom(UToroDataNode* Other, uint8 NumParentNodes, FString* ErrorMsg = nullptr);
	virtual bool CanConnectTo(UToroDataNode* Other, uint8 NumChildNodes, FString* ErrorMsg = nullptr);
#endif
};
