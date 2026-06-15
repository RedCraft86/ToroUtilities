// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Engine/Engine.h"
#include "Sound/SoundClass.h"
#include "UserSettingTypes.h"
#include "GameFramework/GameUserSettings.h"
#include "ToroGameUserSettings.generated.h"

extern ENGINE_API float GAverageMS;
extern ENGINE_API float GAverageFPS;

/**
 * Defines the context in which settings are being applied.
 */
UENUM(BlueprintInternalUseOnly)
enum class EUserSettingApplyType : uint8
{
	/** Settings applied instantly without waiting for apply (e.g., Show FPS). */
	Seamless,

	/** Settings requiring a manual trigger (e.g., Scalability changes). */
	Manual,

	/** Settings that require the UI to refresh its state to reflect changes (e.g., Image Fidelity). */
	UIRefresh
};

/**
 * Custom Game User Settings class for ToroUtilities. Handles persistence (via GConfig),
 * audio volumes, rendering fidelity (DLSS/FSR/TSR), and gameplay preferences.
 */
UCLASS(Blueprintable, BlueprintType)
class TORORUNTIME_API UToroGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:

	UToroGameUserSettings()
		: bInitialized(false)
	{}

	template<typename T = UToroGameUserSettings>
	[[nodiscard]] static T* Get()
	{
		return GEngine ? Cast<T>(GEngine->GetGameUserSettings()) : nullptr;
	}

	/** Returns the current average frame time in milliseconds. */
	UFUNCTION(BlueprintPure, Category = Settings)
		static float GetAverageMS() { return GAverageMS; }

	/** Returns the current average frames per second. */
	UFUNCTION(BlueprintPure, Category = Settings)
		static float GetAverageFPS() { return GAverageFPS; }

	/** 
	 * Returns a list of supported window resolutions for common aspect ratios with a minimum limit. 
	 * Common Ratios = 16:9, 16:10, 4:3, 21:9, 32:9
	 * Minimum Resolution = 1280x720
	 */
	UFUNCTION(BlueprintPure, Category = Settings)
		static const TArray<FIntPoint>& GetSupportedResolutions();

	/** Runs the built-in Unreal benchmark to auto-set scalability settings. */
	UFUNCTION(BlueprintCallable, Category = Settings)
		void AutoAdjustScalability();

	/** 
	 * Sets and applies the Resolution and Fullscreen Mode.
	 * Additionally, it also shows a confirmation dialog when possible.
	 */
	UFUNCTION(BlueprintCallable, Category = Settings)
		void SetResolutionAndFullscreen(const FIntPoint& Resolution, const EWindowMode::Type WindowMode);

	/** Toggles the visibility of the on-screen FPS counter. */
	UFUNCTION(BlueprintCallable, Category = Settings)
		void SetShowFPS(const bool bShow);

	UFUNCTION(BlueprintPure, Category = Settings)
		bool GetShowFPS() const;

	/** Toggles camera smoothing/interpolation for cinematic feel. */
	UFUNCTION(BlueprintCallable, Category = Settings)
		void SetSmoothCamera(const bool bSmooth);

	UFUNCTION(BlueprintPure, Category = Settings)
		bool GetSmoothCamera() const;

	/** Sets the horizontal mouse/look sensitivity. */
	UFUNCTION(BlueprintCallable, Category = Settings)
		void SetSensitivityX(const float Value);

	UFUNCTION(BlueprintPure, Category = Settings)
		float GetSensitivityX() const;

	/** Sets the vertical mouse/look sensitivity. */
	UFUNCTION(BlueprintCallable, Category = Settings)
		void SetSensitivityY(const float Value);

	UFUNCTION(BlueprintPure, Category = Settings)
		float GetSensitivityY() const;

	/** Sets the global brightness/gamma offset (20-80). Default: 50 */
	UFUNCTION(BlueprintCallable, Category = Settings)
		void SetBrightness(const uint8 Value);

	UFUNCTION(BlueprintPure, Category = Settings)
		uint8 GetBrightness() const;

	/** 
	 * Sets the motion blur quality level. (Gets clamped if needed)
	 * 0: Off | 1: Low | 2: Medium | 3: High | 4: Very High
	 */
	UFUNCTION(BlueprintCallable, Category = Settings)
		void SetMotionBlurQuality(const uint8 Value);

	UFUNCTION(BlueprintPure, Category = Settings)
		uint8 GetMotionBlurQuality() const;

	/** Configures how Lumen GI and Reflections are utilized. */
	UFUNCTION(BlueprintCallable, Category = Settings)
		void SetLumenMode(const ELumenUsageMode Mode);

	UFUNCTION(BlueprintPure, Category = Settings)
		ELumenUsageMode GetLumenMode() const;

	/** Sets the AA/Upscaling method (DLSS, FSR, TSR, etc.). */
	UFUNCTION(BlueprintCallable, Category = Settings)
		void SetImageFidelityMode(const EImageFidelityMode Mode);

	UFUNCTION(BlueprintPure, Category = Settings)
		EImageFidelityMode GetImageFidelityMode() const;

	/** Resolution (10% ~ 100%) that should be rendered to let TSR upscale. Defaulted to 100% if TSR is not used. */
	UFUNCTION(BlueprintCallable, Category = Settings)
		void SetTSRScreenPercentage(const float Percentage);

	UFUNCTION(BlueprintPure, Category = Settings)
		float GetTSRScreenPercentage() const;

	/** 
	 * Updates the volume for a specific SoundClass. 
	 * Maps internally to the AudioVolumes TSet.
	 */
	UFUNCTION(BlueprintCallable, Category = Settings)
		void SetSoundVolume(const USoundClass* InClass, const uint8 Value);

	/** Retrieves the stored volume for a specific SoundClass. Adds and returns default 100 if not found. */
	UFUNCTION(BlueprintPure, Category = Settings)
		uint8 GetSoundVolume(const USoundClass* InClass);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSettingsAppliedDelegateBP, const UToroGameUserSettings*, Settings, const EUserSettingApplyType, Type);
	UPROPERTY(BlueprintAssignable, DisplayName = "Settings Applied") 
		FOnSettingsAppliedDelegateBP OnSettingsAppliedBP;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSettingsAppliedDelegate, const UToroGameUserSettings*, const EUserSettingApplyType);
	FOnSettingsAppliedDelegate OnSettingsApplied;

	void InitializeSettings();
	void BroadcastUpdate(EUserSettingApplyType Type) const;

	virtual void SetOverallScalabilityLevel(int32 Value) override;
	virtual void ApplyNonResolutionSettings() override;

protected:

	void ApplyBrightness() const;
	void ApplyMotionBlur() const;

	void ApplyImageFidelity();
	void ApplyTSRSettings() const;
	void ApplyFSRSettings() const;
	void ApplyXeSSSettings() const;
	void ApplyDLSSSettings() const;

	uint8& FindOrAddSoundVolume(const USoundClass* InClass);
	void ApplySoundAdjustments();

	virtual void SetToDefaults() override;
	virtual UWorld* GetWorld() const override;
	virtual void PostInitProperties() override;

	UPROPERTY(Config) bool bInitialized;

	UPROPERTY(Config) bool bShowFPS;
	UPROPERTY(Config) bool bSmoothCamera;
	UPROPERTY(Config) float SensitivityX;
	UPROPERTY(Config) float SensitivityY;

	UPROPERTY(Config) uint8 Brightness;
	UPROPERTY(Config) uint8 MotionBlur;
	UPROPERTY(Config) ELumenUsageMode LumenMode;

	UPROPERTY(Config) EImageFidelityMode ImageFidelity;
	UPROPERTY(Config) float TSRScreenPercentage;

	UPROPERTY(Config) TSet<FAudioChannelVolume> AudioVolumes;
};
