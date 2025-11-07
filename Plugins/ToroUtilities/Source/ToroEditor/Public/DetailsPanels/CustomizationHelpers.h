// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DetailLayoutBuilder.h"
#include "PropertyCustomizationHelpers.h"

#define CLASS_PROPERTY(Class, Member) DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(Class, Member))
#define CLASS_PROPERTY_NS(Member) CLASS_PROPERTY(CLASSNAME, Member)

#define CLASS_PROPERTY_VAR_NS(Member, VarName) CLASS_PROPERTY_VAR(CLASSNAME, Member, VarName)
#define CLASS_PROPERTY_VAR(Class, Member, VarName) \
	TSharedRef<IPropertyHandle> VarName = CLASS_PROPERTY(Class, Member); \
	VarName->MarkHiddenByCustomization();

#define STRUCT_PROPERTY_NS(Member) STRUCT_PROPERTY(STRUCTNAME, Member)
#define STRUCT_PROPERTY(Struct, Member) \
	StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(Struct, Member)).ToSharedRef()

#define STRUCT_PROPERTY_VAR_NS(Member, VarName) STRUCT_PROPERTY_VAR(STRUCTNAME, Member, VarName)
#define STRUCT_PROPERTY_VAR(Struct, Member, VarName) \
	TSharedPtr<IPropertyHandle> VarName = STRUCT_PROPERTY(Struct, Member); \
	VarName->MarkHiddenByCustomization();

#define SIMPLE_RESET_TO_DEFAULT(Property) \
	FResetToDefaultOverride::Create(\
	TAttribute<bool>::CreateLambda([Property]() -> bool { \
		return Property->DiffersFromDefault(); \
	}), FSimpleDelegate::CreateLambda([Property]() { \
		Property->ResetToDefault(); \
	}))

#define SIMPLE_LABEL(InText) \
	SNew(STextBlock).Text(InText).Font(IDetailLayoutBuilder::GetDetailFont())

namespace CustomizationHelpers
{
	TOROEDITOR_API inline bool IsPropertyFromChildClass(const TSharedRef<IPropertyHandle>& InHandle, const UClass* BaseClass)
	{
		if (!BaseClass) return false;
		const FProperty* Property = InHandle->GetProperty();
		const UClass* OwningClass = Property ? Property->GetOwnerClass() : nullptr;
		return OwningClass ? BaseClass->IsChildOf(OwningClass) : false;
	}

	TOROEDITOR_API inline TSharedRef<SWidget> ConstructVerticalVec2D(const TSharedRef<IPropertyHandle>& Handle, const FText& NameX, const FText& NameY)
	{
		return SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.Padding(0.0f, 0.0f, 0.0f, 2.0f)
			.VAlign(VAlign_Center)
			.MinHeight(20.0f)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.Padding(0.0f, 0.0f, 4.0f, 0.0f)
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Left)
				.FillWidth(0.5f)
				[
					SIMPLE_LABEL(NameX)
				]
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Fill)
				.FillWidth(1.0f)
				[
					Handle->GetChildHandle(0)->CreatePropertyValueWidget()
				]
			]
			+SVerticalBox::Slot()
			.VAlign(VAlign_Center)
			.MinHeight(20.0f)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.Padding(0.0f, 0.0f, 4.0f, 0.0f)
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Left)
				.FillWidth(0.5f)
				[
					SIMPLE_LABEL(NameY)
				]
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Fill)
				.FillWidth(1.0f)
				[
					Handle->GetChildHandle(1)->CreatePropertyValueWidget()
				]
			];
	}

	TOROEDITOR_API inline TSharedRef<SWidget> ConstructHorizontalVec2D(const TSharedRef<IPropertyHandle>& Handle, const FText& NameX, const FText& NameY)
	{
		return SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.Padding(0.0f, 0.0f, 10.0f, 0.0f)
			.VAlign(VAlign_Center)
			.MinWidth(20.0f)
			.AutoWidth()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.Padding(0.0f, 0.0f, 4.0f, 0.0f)
				.VAlign(VAlign_Center)
				.MinWidth(20.0f)
				.AutoWidth()
				[
					SIMPLE_LABEL(NameX)
				]
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.MinWidth(30.0f)
				.AutoWidth()
				[
					Handle->GetChildHandle(0)->CreatePropertyValueWidget()
				]
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.MinWidth(20.0f)
			.AutoWidth()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.Padding(0.0f, 0.0f, 4.0f, 0.0f)
				.VAlign(VAlign_Center)
				.MinWidth(20.0f)
				.AutoWidth()
				[
					SIMPLE_LABEL(NameY)
				]
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.MinWidth(30.0f)
				.AutoWidth()
				[
					Handle->GetChildHandle(1)->CreatePropertyValueWidget()
				]
			];
	}
}