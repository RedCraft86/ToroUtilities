// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "UserSettingsProvider.h"
#include "ScalabilitySettingProviders.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Scalability", meta = (Hidden))
struct TORORUNTIME_API FToroUSP_ScalabilityBase : public FUserSettingsProvider_IntSwap
{
	GENERATED_BODY()

public:

	FToroUSP_ScalabilityBase();
};

USTRUCT(BlueprintType, DisplayName = "[Scalability] Overall")
struct TORORUNTIME_API FToroUSP_ScalabilityOverall : public FToroUSP_ScalabilityBase
{
	GENERATED_BODY()

public:

	FToroUSP_ScalabilityOverall();

	virtual uint8 GetValue() const override;
	virtual void SetValue(const uint8 InValue) override;
};

USTRUCT(BlueprintType, DisplayName = "[Scalability] View Distance")
struct TORORUNTIME_API FToroUSP_ScalabilityViewDist : public FToroUSP_ScalabilityBase
{
	GENERATED_BODY()

public:

	FToroUSP_ScalabilityViewDist();

	virtual uint8 GetValue() const override;
	virtual void SetValue(const uint8 InValue) override;
};

USTRUCT(BlueprintType, DisplayName = "[Scalability] Anti Aliasing")
struct TORORUNTIME_API FToroUSP_ScalabilityAntiAlias : public FToroUSP_ScalabilityBase
{
	GENERATED_BODY()

public:

	FToroUSP_ScalabilityAntiAlias();

	virtual uint8 GetValue() const override;
	virtual void SetValue(const uint8 InValue) override;
};

USTRUCT(BlueprintType, DisplayName = "[Scalability] Shadow")
struct TORORUNTIME_API FToroUSP_ScalabilityShadow : public FToroUSP_ScalabilityBase
{
	GENERATED_BODY()

public:

	FToroUSP_ScalabilityShadow();

	virtual uint8 GetValue() const override;
	virtual void SetValue(const uint8 InValue) override;
};

USTRUCT(BlueprintType, DisplayName = "[Scalability] Global Illumination")
struct TORORUNTIME_API FToroUSP_ScalabilityGI : public FToroUSP_ScalabilityBase
{
	GENERATED_BODY()

public:

	FToroUSP_ScalabilityGI();

	virtual uint8 GetValue() const override;
	virtual void SetValue(const uint8 InValue) override;
};

USTRUCT(BlueprintType, DisplayName = "[Scalability] Reflection")
struct TORORUNTIME_API FToroUSP_ScalabilityReflection : public FToroUSP_ScalabilityBase
{
	GENERATED_BODY()

public:

	FToroUSP_ScalabilityReflection();

	virtual uint8 GetValue() const override;
	virtual void SetValue(const uint8 InValue) override;
};

USTRUCT(BlueprintType, DisplayName = "[Scalability] Post Processing")
struct TORORUNTIME_API FToroUSP_ScalabilityPostProc : public FToroUSP_ScalabilityBase
{
	GENERATED_BODY()

public:

	FToroUSP_ScalabilityPostProc();

	virtual uint8 GetValue() const override;
	virtual void SetValue(const uint8 InValue) override;
};

USTRUCT(BlueprintType, DisplayName = "[Scalability] Texture")
struct TORORUNTIME_API FToroUSP_ScalabilityTexture : public FToroUSP_ScalabilityBase
{
	GENERATED_BODY()

public:

	FToroUSP_ScalabilityTexture();

	virtual uint8 GetValue() const override;
	virtual void SetValue(const uint8 InValue) override;
};

USTRUCT(BlueprintType, DisplayName = "[Scalability] Visual Effect")
struct TORORUNTIME_API FToroUSP_ScalabilityEffect : public FToroUSP_ScalabilityBase
{
	GENERATED_BODY()

public:

	FToroUSP_ScalabilityEffect();

	virtual uint8 GetValue() const override;
	virtual void SetValue(const uint8 InValue) override;
};

USTRUCT(BlueprintType, DisplayName = "[Scalability] Foliage")
struct TORORUNTIME_API FToroUSP_ScalabilityFoliage : public FToroUSP_ScalabilityBase
{
	GENERATED_BODY()

public:

	FToroUSP_ScalabilityFoliage();

	virtual uint8 GetValue() const override;
	virtual void SetValue(const uint8 InValue) override;
};

USTRUCT(BlueprintType, DisplayName = "[Scalability] Shading")
struct TORORUNTIME_API FToroUSP_ScalabilityShading : public FToroUSP_ScalabilityBase
{
	GENERATED_BODY()

public:

	FToroUSP_ScalabilityShading();

	virtual uint8 GetValue() const override;
	virtual void SetValue(const uint8 InValue) override;
};

USTRUCT(BlueprintType, DisplayName = "[Scalability] Landscape")
struct TORORUNTIME_API FToroUSP_ScalabilityLandscape : public FToroUSP_ScalabilityBase
{
	GENERATED_BODY()

public:

	FToroUSP_ScalabilityLandscape();

	virtual uint8 GetValue() const override;
	virtual void SetValue(const uint8 InValue) override;
};