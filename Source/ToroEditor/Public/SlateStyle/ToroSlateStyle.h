// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "ToroEditor.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleMacros.h"
#include "Styling/SlateStyleRegistry.h"

#define TORO_SLATE_STYLE(ModuleClass, StyleClass, StyleName) \
		friend class ModuleClass; \
	public: \
		StyleClass(): FToroSlateStyle(#StyleName) {} \
		static const ISlateStyle& Get() { return *INSTANCE; } \
		static FName GetName() { return INSTANCE.IsValid() ? INSTANCE->GetStyleSetName() : NAME_None; } \
    private: \
        static inline TSharedPtr<StyleClass> INSTANCE = nullptr; \
		static void Register() \
		{ \
			if (FSlateStyleRegistry::FindSlateStyle(GetName())) \
			{ \
				UE_LOG(LogToroEditor, Warning, \
					TEXT("Failed to register style '%s.' A style with that name already exist."), TEXT(#StyleName) \
				); \
				return; \
			} \
			INSTANCE = MakeShared<StyleClass>(); \
			if (!INSTANCE.IsValid()) \
			{ \
				UE_LOG(LogToroEditor, Warning, \
					TEXT("Failed to register style '%s.' Class could not be instantiated."), TEXT(#StyleName) \
				); \
				return; \
			} \
			INSTANCE->AddResources(); \
			FSlateStyleRegistry::RegisterSlateStyle(*INSTANCE); \
		} \
		static void Unregister() \
		{ \
			if (INSTANCE.IsValid()) \
			{ \
				FSlateStyleRegistry::UnRegisterSlateStyle(*INSTANCE); \
				INSTANCE.Reset(); \
			} \
		}

/**
 * A base class to easily create custom slate style sets.
 *
 * <b>Usage:</b>
 * <pre>
 * class FYourSlateStyle final : public FToroSlateStyle
 * {
 *	TORO_SLATE_STYLE(FYourModule, FYourSlateStyle, YourStyleName)
 *
 * public:
 *
 *	virtual void AddResources() override
 *	{
 *		SetContentRoot(...);
 *
 *		AddSVG(...);
 *		AddPNG(...);
 *	}
 * };
 * </pre>
 * 
 * <b>Then in your module Startup/Shutdown:</b>
 * <pre>
 * FYourSlateStyle::Register();
 * 
 * FYourSlateStyle::Unregister();
 * </pre>
 */
class TOROEDITOR_API FToroSlateStyle : protected FSlateStyleSet, public TSharedFromThis<FToroSlateStyle>
{
public:

	virtual void AddResources() = 0;

protected:

	static inline const FVector2D Icon20x20 = FVector2D(20.0f, 20.0f);
	static inline const FVector2D Icon32x32 = FVector2D(32.0f, 32.0f);
	static inline const FVector2D Icon64x64 = FVector2D(64.0f, 64.0f);

	FToroSlateStyle(const FName& InStyleSetName)
		: FSlateStyleSet(InStyleSetName)
	{}

	void AddPNG(const FName& Name, const FString& Path, const FVector2D& Size)
	{
		Set(Name, new IMAGE_BRUSH(Path, Size));
	}

	void AddSVG(const FName& Name, const FString& Path, const FVector2D& Size)
	{
		Set(Name, new IMAGE_BRUSH_SVG(Path, Size));
	}
};