// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "DetailsPanel/ToroStructCustomization.h"
#include "Types/RelativePath.h"
#include "ConfigKeyCache.h"
#include "ToroSetup.h"

#define STRUCT_NAME FTSetupRelativePath
class FTSetupRelativePathDetails final : public FToroStructCustomization
{
	static inline FSlateIcon CacheIcon = FSlateIcon("EditorStyle", "Zen.Server.Restart");

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructHandle, FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		FToroStructCustomization::CustomizeHeader(StructHandle, HeaderRow, CustomizationUtils);

		GET_STRUCT_PROPERTY_VAR_NS(Path, Path);

		static const FName META_FileTypes(TEXT("FileTypes"));
		const FString FileTypes = StructHandle->HasMetaData(META_FileTypes)
			? StructHandle->GetMetaData(META_FileTypes) : FString();

		const bool bIsIniFile = FileTypes.Contains(TEXT(".ini"));

		HeaderRow
		.NameContent()
		[
			StructHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.FillWidth(1.0f)
			[
				Path->CreatePropertyValueWidget()
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.Padding(1.0f, 0.0f, 0.0f, 0.0f)
			.AutoWidth()
			[
				PropertyCustomizationHelpers::MakeEditButton(
					FSimpleDelegate::CreateLambda([StructHandle, FileTypes, Path]()
					{
						const FString BaseDirectory = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
						FString FilePath = FToroSetupModule::PickFileOrDir(BaseDirectory,
							FileTypes.IsEmpty() ? TEXT("Select Directory") : TEXT("Select File"), FileTypes);

						FPaths::NormalizeDirectoryName(FilePath);

						if (!FPaths::IsUnderDirectory(FilePath, BaseDirectory))
						{
							UE_LOG(LogToroSetup, Warning,
								TEXT("Path \"%s\" is not something inside \"%s\""),
								*FilePath, *BaseDirectory
							)
							return;
						}

						FilePath.RightChopInline(BaseDirectory.Len() - 1);
						if (!FilePath.IsEmpty())
						{
							StructHandle->NotifyPreChange();
							Path->SetValue(FilePath);
							StructHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
						}
					}), INVTEXT("Browse")
				)
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.Padding(1.0f, 0.0f, 0.0f, 0.0f)
			.AutoWidth()
			[
				PropertyCustomizationHelpers::MakeCustomButton(CacheIcon.GetIcon(),
					FSimpleDelegate::CreateLambda([Path]()
					{
						FString FilePath;
						Path->GetValue(FilePath);
						FTSetupConfigKeyCache::CacheFile(FilePath);
					}), INVTEXT("Re-Cache INI File Keys"), bIsIniFile,
					bIsIniFile ? EVisibility::Visible : EVisibility::Hidden
				)
			]
		];
	}
};
#undef STRUCT_NAME