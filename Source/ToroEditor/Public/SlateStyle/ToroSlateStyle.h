// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "ToroEditor.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleMacros.h"
#include "Styling/SlateStyleRegistry.h"

/**
 * Declares the registration API and singleton storage for a Slate style class.
 * @param ModuleClass Module class allowed to register and unregister the style.
 * @param StyleClass Slate style class receiving the generated members.
 * @param StyleName Unique name used to register the style.
 */
#define TORO_SLATE_STYLE(ModuleClass, StyleClass, StyleName) \
		friend class ModuleClass; \
	public: \
		StyleClass() : FToroSlateStyle(#StyleName) {} \
		static const ISlateStyle& Get() \
		{ \
			checkf(INSTANCE.IsValid(), TEXT("Slate style '%s' is not registered."), TEXT(#StyleName)); \
			return *INSTANCE; \
		} \
		static FName GetName() { return TEXT(#StyleName); } \
	private: \
		static inline TSharedPtr<StyleClass> INSTANCE = nullptr; \
		static void Register() \
		{ \
			if (INSTANCE.IsValid()) \
			{ \
				return; \
			} \
			if (FSlateStyleRegistry::FindSlateStyle(TEXT(#StyleName))) \
			{ \
				UE_LOG(LogToroEditor, Warning, \
					TEXT("Failed to register style '%s'. A style with that name already exists."), TEXT(#StyleName) \
				); \
				return; \
			} \
			INSTANCE = MakeShared<StyleClass>(); \
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
 * Base class for self-registering Slate style sets owned by an editor module.
 * Derived classes use TORO_SLATE_STYLE and provide their resources through AddResources().
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
 * ...
 * FYourSlateStyle::Unregister();
 * </pre>
 */
class TOROEDITOR_API FToroSlateStyle : protected FSlateStyleSet, public TSharedFromThis<FToroSlateStyle>
{
public:

	/**
	 * Adds brushes and other resources before the style is registered.
	 */
	virtual void AddResources() = 0;

protected:

	static inline const FVector2D Icon20x20 = FVector2D(20.0f, 20.0f);
	static inline const FVector2D Icon32x32 = FVector2D(32.0f, 32.0f);
	static inline const FVector2D Icon64x64 = FVector2D(64.0f, 64.0f);

	FToroSlateStyle(const FName& InStyleSetName)
		: FSlateStyleSet(InStyleSetName)
	{}

	/**
	 * Adds a PNG image brush to the style set.
	 * @param Name Key used to retrieve the brush.
	 * @param Path Resource path relative to the style's content root.
	 * @param Size Brush size in Slate units.
	 */
	void AddPNG(const FName& Name, const FString& Path, const FVector2D& Size)
	{
		Set(Name, new IMAGE_BRUSH(Path, Size));
	}

	/**
	 * Adds an SVG image brush to the style set.
	 * @param Name Key used to retrieve the brush.
	 * @param Path Resource path relative to the style's content root.
	 * @param Size Brush size in Slate units.
	 */
	void AddSVG(const FName& Name, const FString& Path, const FVector2D& Size)
	{
		Set(Name, new IMAGE_BRUSH_SVG(Path, Size));
	}
};