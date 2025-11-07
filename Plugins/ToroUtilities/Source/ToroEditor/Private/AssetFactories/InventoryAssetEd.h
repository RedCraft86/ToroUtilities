// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroEditor.h"
#include "AssetFactories/ToroAssetFactory.h"
#include "AssetFactories/ToroAssetDefinition.h"
#include "Gameplay/Inventory/InventoryAsset.h"
#include "InventoryAssetEd.generated.h"

UCLASS()
class TOROEDITOR_API UInventoryAssetFactory final : public UToroAssetFactory
{
	GENERATED_BODY()

public:

	UInventoryAssetFactory()
	{
		AssetName = TEXT("InventoryAsset");
		SupportedClass = UInventoryAsset::StaticClass();
	}
};

UCLASS()
class TOROEDITOR_API UInventoryAssetDefinition final : public UToroAssetDefinition
{
	GENERATED_BODY()

public:

	virtual FLinearColor GetAssetColor() const override { return FColor(100, 25, 160); }
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UInventoryAsset::StaticClass(); }
	virtual FText GetAssetDisplayName() const override
	{
		return NSLOCTEXT("ToroEditor", "InventoryAssetName", "Inventory Asset");
	}

	virtual const FSlateBrush* GetIconBrush(const FAssetData& InAssetData, const FName InClassName) const override
	{
		return FToroEditorStyle::Get().GetBrush("ClassThumbnail.InventoryAsset");
	}

	virtual const FSlateBrush* GetThumbnailBrush(const FAssetData& InAssetData, const FName InClassName) const override
	{
		UInventoryAsset* Asset = Cast<UInventoryAsset>(InAssetData.GetAsset());
		if (UTexture2D* Icon = Asset ? Asset->Thumbnail.LoadSynchronous() : nullptr)
		{
			FSlateBrush& Brush = const_cast<UInventoryAssetDefinition*>(this)->AddThumbnail(Asset);
			Brush.ImageSize = FIntPoint(Icon->GetSizeX(), Icon->GetSizeY());
			Brush.SetResourceObject(Icon);
			return &Brush;
		}
		return nullptr;
	}

private:

	UPROPERTY(Transient)
	TMap<TSoftObjectPtr<UInventoryAsset>, FSlateBrush> Thumbnails;

	FSlateBrush& AddThumbnail(const TSoftObjectPtr<UInventoryAsset>& InAsset)
	{
		FSlateBrush& Brush = Thumbnails.FindOrAdd(InAsset);
		Brush.DrawAs = ESlateBrushDrawType::Type::RoundedBox;
		Brush.OutlineSettings.RoundingType = ESlateBrushRoundingType::Type::FixedRadius;
		Brush.OutlineSettings.CornerRadii = FVector4(10.0f, 10.0f, 10.0f, 10.0f);
		Brush.OutlineSettings.Width = 4.0f;
		return Brush;
	}
};