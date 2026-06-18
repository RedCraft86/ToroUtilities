// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "IDetailPropertyRow.h"
#include "DetailsPanel/ToroStructCustomization.h"
#include "ProjectLinkTypes.h"
#include "ProjectLink.h"

#define STRUCT_NAME FPLRelativePath
class FRelativePathDetails final : public FToroStructCustomization
{
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		GET_STRUCT_PROPERTY_VAR_NS(Path, Path);

		static const FName META_FileTypes(TEXT("FileTypes"));
		const FString FileTypes = StructHandle->HasMetaData(META_FileTypes) 
			? StructHandle->GetMetaData(META_FileTypes) : FString();

		StructBuilder.AddCustomRow(INVTEXT("Path"))
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
						FString FilePath = FProjectLinkModule::PickFileOrDir(BaseDirectory, 
							FileTypes.IsEmpty() ? TEXT("Select Directory") : TEXT("Select File"), FileTypes);

						FPaths::NormalizeDirectoryName(FilePath);

						if (!FPaths::IsUnderDirectory(FilePath, BaseDirectory))
						{
							UE_LOG(LogTemp, Warning, 
								TEXT("Path \"%s\" is not something inside \"%s\""), 
								*FilePath, *BaseDirectory)
							return;
						}

						FilePath.RightChopInline(BaseDirectory.Len() - 1);
						if (!FilePath.IsEmpty())
						{
							StructHandle->NotifyPreChange();
							Path->SetValue(FilePath);
							StructHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
						}
					})
				)
			]
		];
	}
};
#undef STRUCT_NAME