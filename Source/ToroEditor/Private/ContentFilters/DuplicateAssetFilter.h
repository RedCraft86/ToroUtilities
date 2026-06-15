// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "FrontendFilterBase.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "ContentBrowserFrontEndFilterExtension.h"
#include "DuplicateAssetFilter.generated.h"

#define LOCTEXT_NAMESPACE "ToroEditor"

class FFrontendFilter_DuplicateAsset final : public FFrontendFilter
{
public:

	FFrontendFilter_DuplicateAsset(const TSharedPtr<FFrontendFilterCategory>& InCategory)
		: FFrontendFilter(InCategory), bActive(false)
	{
		if (const FAssetRegistryModule* Module = FModuleManager::LoadModulePtr<FAssetRegistryModule>("AssetRegistry"))
		{
			IAssetRegistry& AssetRegistry = Module->Get();
			OnAssetAdded = AssetRegistry.OnAssetAdded().AddRaw(this, &FFrontendFilter_DuplicateAsset::OnAssetIO);
			OnAssetRemoved = AssetRegistry.OnAssetRemoved().AddRaw(this, &FFrontendFilter_DuplicateAsset::OnAssetIO);
			OnAssetRenamed = AssetRegistry.OnAssetRenamed().AddRaw(this, &FFrontendFilter_DuplicateAsset::OnAssetRename);
		}
	}

	virtual ~FFrontendFilter_DuplicateAsset() override
	{
		Duplicates.Empty();
		SourcePaths.Empty();
		if (const FAssetRegistryModule* Module = FModuleManager::GetModulePtr<FAssetRegistryModule>("AssetRegistry"))
		{
			IAssetRegistry& AssetRegistry = Module->Get();
			AssetRegistry.OnAssetAdded().Remove(OnAssetAdded);
			AssetRegistry.OnAssetRemoved().Remove(OnAssetRemoved);
			AssetRegistry.OnAssetRenamed().Remove(OnAssetRenamed);
		}
	}

private:

	bool bActive;
	TArray<FName> SourcePaths;
	TMap<FString, TSet<FString>> Duplicates;
	FDelegateHandle OnAssetAdded, OnAssetRemoved, OnAssetRenamed;

	virtual FLinearColor GetColor() const override { return FLinearColor::Red; }
	virtual FString GetName() const override { return TEXT("DuplicateAssetFilter"); }
	virtual FText GetDisplayName() const override { return LOCTEXT("DuplicateAssetFilterName", "Duplicate Assets"); }
	virtual FText GetToolTipText() const override { return LOCTEXT("DuplicateAssetFilterTip", "Filters assets that share the same names and class."); }
	virtual void LoadSettings(const FString& IniFilename, const FString& IniSection, const FString& SettingsString) override
	{
		SetActive(false); // Disable if left on to prevent engine startup stalls
	}

	virtual void SetCurrentFilter(TArrayView<const FName> InSourcePaths, const FContentBrowserDataFilter& InBaseFilter) override
	{
		SourcePaths = InSourcePaths;
		PopulateDuplicateNames();
	}

	virtual void ActiveStateChanged(bool InActive) override
	{
		bActive = InActive;
		PopulateDuplicateNames();
	}

	virtual bool PassesFilter(const FContentBrowserItem& InItem) const override
	{
		if (const TSet<FString>* Paths = Duplicates.Find(GetAssetKey(InItem)))
		{
			if (Paths->Num() > 1 && Paths->Contains(InItem.GetInternalPath().ToString()))
			{
				return true;
			}
		}

		return false;
	}

	void PopulateDuplicateNames()
	{
		Duplicates.Empty();
		if (!bActive) return;
		if (const FAssetRegistryModule* Module = FModuleManager::LoadModulePtr<FAssetRegistryModule>("AssetRegistry"))
		{
			TArray<FAssetData> AllAssets;
			Module->Get().GetAssetsByPaths(SourcePaths, AllAssets, true);
			for (const FAssetData& Asset : AllAssets)
			{
				Duplicates.FindOrAdd(GetAssetKey(Asset)).Add(Asset.GetSoftObjectPath().ToString());
			}
		}
	}

	void OnAssetIO(const FAssetData& Asset) { PopulateDuplicateNames(); }
	void OnAssetRename(const FAssetData& Asset, const FString& Name) { PopulateDuplicateNames(); }

	static FString GetAssetKey(const FAssetData& Asset)
	{
		const FString Path = Asset.AssetClassPath.ToString();
		const FString Name = Asset.AssetName.ToString();

		FString Type;
		Path.Split(TEXT("."), nullptr, &Type, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		return FString::Printf(TEXT("%s.%s"), *Type, *Name);
	}

	static FString GetAssetKey(const FContentBrowserItem& Asset)
	{
		FAssetData AssetData;
		Asset.Legacy_TryGetAssetData(AssetData);
		return AssetData.IsValid() ? GetAssetKey(AssetData) : Asset.GetItemName().ToString();
	}
};

UCLASS()
class UDuplicateAssetFilter final : public UContentBrowserFrontEndFilterExtension
{
	GENERATED_BODY()

	virtual void AddFrontEndFilterExtensions(TSharedPtr<FFrontendFilterCategory> DefaultCategory, TArray<TSharedRef<FFrontendFilter>>& InOutFilterList) const override
	{
		InOutFilterList.Add(MakeShareable(new FFrontendFilter_DuplicateAsset(DefaultCategory)));
	}
};

#undef LOCTEXT_NAMESPACE