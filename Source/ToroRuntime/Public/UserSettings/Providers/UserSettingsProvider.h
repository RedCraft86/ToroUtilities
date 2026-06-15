// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "DataTypes/CachedGetter.h"
#include "StructUtils/InstancedStruct.h"
#include "UserSettings/ToroGameUserSettings.h"
#include "UserSettingsProvider.generated.h"

/**
 * Categorizes the performance cost or system impact of a specific setting.
 * Used by the UI to display warnings or icons (e.g., a "high" impact setting might show a warning).
 */
UENUM(BlueprintType)
enum class EUserSettingImpact : uint8
{
	/** No measurable impact on performance. */
	None,

	/** Negligible impact (e.g., UI toggles). */
	Low,

	/** Moderate impact on GPU or CPU (e.g., Anti-Aliasing Quality). */
	Medium,

	/** Significant impact on frame rates (e.g., Lumen or Ray Tracing). */
	High,

	/** Impact depends on other hardware or software factors. */
	Varies
};

/**
 * The foundational accessor layer between the Settings UI and ToroGameUserSettings.
 * This base struct provides common metadata like naming and performance tooltips.
 */
USTRUCT(BlueprintInternalUseOnly, DisplayName = "Settings Provider", meta = (Hidden))
struct TORORUNTIME_API FUserSettingsProviderBase
{
	GENERATED_BODY()

public:

	FUserSettingsProviderBase();
	virtual ~FUserSettingsProviderBase() = default;

	/** The localized name displayed in the settings menu. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Display)
		FText DisplayName;

	/** A detailed explanation of what this setting changes. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Display, meta = (MultiLine = true))
		FText Description;

	/** Visual indicator of how much this setting affects game performance. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Display)
		EUserSettingImpact Performance;

	/** If true, the UI should re-query this provider whenever any setting is applied. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Display)
		bool bUpdateDynamically;

	FText GetFormattedTooltip() const;
	const FText& GetPerformanceLabel() const;
	virtual bool ShouldBeEnabled() const { return true; }

protected:
#if WITH_EDITOR
	virtual void UpdateProvider() {}
#endif

	static inline TCachedGetter<UToroGameUserSettings> UserSettings {[](){
		return UToroGameUserSettings::Get();
	}};
};

/**
 * Provides access to a boolean-based game setting (e.g., VSync, Show FPS Toggle).
 */
USTRUCT(BlueprintInternalUseOnly, DisplayName = "Boolean Provider", meta = (Hidden))
struct TORORUNTIME_API FUserSettingsProvider_Bool : public FUserSettingsProviderBase
{
	GENERATED_BODY()

public:

	FUserSettingsProvider_Bool()
		: DefaultOption(false)
	{}

	/** The value to revert to when 'Reset to Defaults' is triggered. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Option)
		bool DefaultOption;

	virtual bool GetValue() const { return false; }
	virtual void SetValue(const bool bInValue) {}
};

/**
 * Provides access to a numeric range setting (e.g., Sensitivity, Brightness).
 */
USTRUCT(BlueprintInternalUseOnly, DisplayName = "Float Provider", meta = (Hidden))
struct TORORUNTIME_API FUserSettingsProvider_Float : public FUserSettingsProviderBase
{
	GENERATED_BODY()

public:

	FUserSettingsProvider_Float()
		: DefaultOption(0.0f), MinValue(0.0f), MaxValue(1.0f), NumDecimals(1)
	{}

	/** The value to revert to when 'Reset to Defaults' is triggered. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Option)
		float DefaultOption;

	/** The lowest valid value allowed for this setting. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Option)
		float MinValue;

	/** The highest valid value allowed for this setting. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Option)
		float MaxValue;

	/** Controls the precision of the UI slider (0 for integers, up to 2 for precise floats). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Option, meta = (ClampMax = 2, UIMax = 2))
		uint8 NumDecimals;

	float GetValueDelta() const;
	virtual float GetValue() const { return 0.0f; }
	virtual void SetValue(const float InValue) {}

#if WITH_EDITOR
protected:
	virtual void UpdateProvider() override;
#endif
};

/**
 * Provides access to a discrete index-based setting (e.g., Quality Levels: Low, Med, High).
 */
USTRUCT(BlueprintInternalUseOnly, DisplayName = "IntSwap Provider", meta = (Hidden))
struct TORORUNTIME_API FUserSettingsProvider_IntSwap : public FUserSettingsProviderBase
{
	GENERATED_BODY()

public:

	FUserSettingsProvider_IntSwap()
		: DefaultOption(0), OptionNames({TEXT("None")})
	{}

	/** The default index to revert to. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Option)
		uint8 DefaultOption;

	/** The list of human-readable labels for each index (e.g., ["Low", "Medium", "High"]). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Option)
		TArray<FString> OptionNames;

	virtual uint8 GetValue() const { return 0; }
	virtual void SetValue(const uint8 InValue) {}

#if WITH_EDITOR
protected:
	virtual void UpdateProvider() override;
#endif
};

/**
 * Provides access to string-based or specialized named settings (e.g., Language selection, Resolution strings).
 */
USTRUCT(BlueprintInternalUseOnly, DisplayName = "String Provider", meta = (Hidden))
struct TORORUNTIME_API FUserSettingsProvider_String : public FUserSettingsProviderBase
{
	GENERATED_BODY()

public:

	FUserSettingsProvider_String()
		: DefaultOption(FString())
	{}

	/** The default string to revert to. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Option)
		FString DefaultOption;

	/** The collection of valid strings this setting can accept. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Option)
		TArray<FString> DefaultChoices;

	/** Tooltips associated with specific choices, mapped by the choice string. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Option)
		TMap<FString, FText> OptionTooltips;

	virtual const TArray<FString>& GetOptions() { return DefaultChoices; }

	virtual FString GetValue() const { return FString(); }
	virtual void SetValue(const FString& InValue) {}
};