// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MiscObjects/ToroDataAsset.h"
#include "ToroDataGraph.generated.h"

class UToroDataNode;
class UToroDataEdge;

UCLASS(NotBlueprintable, BlueprintType)
class TOROGRAPH_API UToroDataGraph : public UToroDataAsset
{
	GENERATED_BODY()

public:

	UToroDataGraph();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Graph)
	FGuid GraphID;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, Category = Graph, AdvancedDisplay)
		bool bCyclical;

	UPROPERTY(VisibleAnywhere, Category = Graph, AdvancedDisplay)
		TSubclassOf<UToroDataNode> CompatibleNode;

	UPROPERTY(VisibleAnywhere, Category = Graph, AdvancedDisplay)
		TSubclassOf<UToroDataEdge> CompatibleEdge;

	UPROPERTY() TObjectPtr<UEdGraph> EdGraph;
#endif

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Graph, AdvancedDisplay, meta = (DisplayThumbnail = false))
		TArray<TObjectPtr<UToroDataNode>> AllNodes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Graph, AdvancedDisplay, meta = (DisplayThumbnail = false))
		TArray<TObjectPtr<UToroDataNode>> RootNodes;

	UFUNCTION(BlueprintPure, Category = DataGraphs)
		int32 GetNodeDepth() const;

	UFUNCTION(BlueprintPure, Category = DataGraphs)
		UToroDataNode* GetNodeByID(const FGuid& InID, const bool bRootFallback = true) const;

	UFUNCTION(BlueprintPure, Category = DataGraphs)
		TArray<uint8> ValidateSequence(const TArray<uint8>& InSequence, const bool bRootFallback = true) const;

	UFUNCTION(BlueprintPure, Category = DataGraphs)
		TArray<UToroDataNode*> GetNodesInSequence(const TArray<uint8>& InSequence, const bool bRootFallback = true) const;

	UFUNCTION(BlueprintPure, Category = DataGraphs)
		UToroDataNode* GetLeafInSequence(const TArray<uint8>& InSequence, const bool bRootFallback = true) const;

	UFUNCTION(BlueprintPure, Category = DataGraphs)
		TArray<uint8> GetSequenceFromNode(UToroDataNode* InNode) const;

	template<typename T>
	TArray<T*> GetNodesInSequence(const TArray<uint8>& InSequence, const bool bRootFallback) const
	{
		TArray<UToroDataNode*> Nodes = GetNodesInSequence(InSequence, bRootFallback);
		TArray<T*> OutNodes;
		OutNodes.Reserve(Nodes.Num());
		for (const UToroDataNode* Node : Nodes)
		{
			OutNodes.Add(Cast<T>(Node));
		}

		return OutNodes;
	}

	template<typename T>
	T* GetLeafInSequence(const TArray<uint8>& InSequence, const bool bRootFallback) const
	{
		return Cast<T>(GetLeafInSequence(InSequence, bRootFallback));
	}

#if WITH_EDITOR
	void ClearGraph();
	virtual FText GetDescription() const override;
#endif
};
