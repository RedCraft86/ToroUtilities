// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "UserSettingTypes.h"
#include "Libraries/ToroConsoleLibrary.h"

// TODO: Add third party

// #include "XeFGRHI.h"
// #include "XeSSBlueprintLibrary.h"
// #include "XeFGBlueprintLibrary.h"
// #include "XeLLBlueprintLibrary.h"
//
// #include "DLSSLibrary.h"
// #include "StreamlineLibraryDLSSG.h"
// #include "StreamlineLibraryReflex.h"

namespace ImageFidelityAPI
{
	inline const TMap<EImageFidelityMode, FName> ModeToNameMap = {
		{ EImageFidelityMode::None,	TEXT("None")	},
		{ EImageFidelityMode::FXAA,	TEXT("FXAA")	},
		{ EImageFidelityMode::SMAA,	TEXT("SMAA")	},
		{ EImageFidelityMode::CMAA2,	TEXT("CMAA2")	},
		{ EImageFidelityMode::TAA,	TEXT("TAA")		},
		{ EImageFidelityMode::TSR,	TEXT("TSR")		},
		{ EImageFidelityMode::FSR,	TEXT("FSR")		},
		{ EImageFidelityMode::XeSS,	TEXT("XeSS")	},
		{ EImageFidelityMode::DLSS,	TEXT("DLSS")	}
	};

	inline bool IsSupportedMode(const EImageFidelityMode InMode)
	{
		return InMode == EImageFidelityMode::None
			|| InMode == EImageFidelityMode::FXAA
			|| InMode == EImageFidelityMode::SMAA
			|| InMode == EImageFidelityMode::TAA
			|| InMode == EImageFidelityMode::TSR;
		// TODO: Add upscalers
	}

	inline bool SupportsVSync()
	{
		// Check for: Any kind of Frame Gen + DLSS being on at all
		return true;
	}

	//
	// namespace FSR
	// {
	// 	// inline const TMap<EFSRQualityMode, FName> ModeNameMap = {
	// 	// 	{EFSRQualityMode::Off,				TEXT("Off")						},
	// 	// 	{EFSRQualityMode::NativeAA,			TEXT("Native AA (1.0x)")		},
	// 	// 	{EFSRQualityMode::Quality,			TEXT("Quality (1.5x)")			},
	// 	// 	{EFSRQualityMode::Balanced,			TEXT("Balanced (1.7x)")			},
	// 	// 	{EFSRQualityMode::Performance,		TEXT("Performance (2.0x)")		},
	// 	// 	{EFSRQualityMode::UltraPerformance,	TEXT("Ultra Performance (3.0x)")}
	// 	// };
	// 	//
	// 	// inline void SetSRMode(const bool bEnabled, const EFSRQualityMode Quality)
	// 	// {
	// 	// 	const uint8 QualityNum = FMath::Min<uint8>(static_cast<uint8>(Quality), 5);
	// 	// 	UToroConsoleUtils::SetCVarBool(TEXT("r.FidelityFX.FSR3.Enabled"), bEnabled && QualityNum > 0);
	// 	// 	if (QualityNum > 0)
	// 	// 	{
	// 	// 		// Offset by 1 since OFF is not a quality mode
	// 	// 		UToroConsoleUtils::SetCVarInt(TEXT("r.FidelityFX.FSR.QualityMode"), QualityNum - 1);
	// 	// 	}
	// 	// }
	// 	//
	// 	// inline void SetFGMode(const bool bEnabled)
	// 	// {
	// 	// 	UToroConsoleUtils::SetCVarBool(TEXT("r.FidelityFX.FI.Enabled"), bEnabled);
	// 	// }
	// 	//
	// 	// inline void ForEachMode(const TFunction<void(const EFSRQualityMode /*Mode*/)>& Func)
	// 	// {
	// 	// 	static constexpr EFSRQualityMode VALID_MODES[] = {
	// 	// 		EFSRQualityMode::NativeAA,
	// 	// 		EFSRQualityMode::Quality,
	// 	// 		EFSRQualityMode::Balanced,
	// 	// 		EFSRQualityMode::Performance,
	// 	// 		EFSRQualityMode::UltraPerformance
	// 	// 	};
	// 	//
	// 	// 	if (Func)
	// 	// 	{
	// 	// 		for (const EFSRQualityMode Mode : VALID_MODES)
	// 	// 		{
	// 	// 			Func(Mode);
	// 	// 		}
	// 	// 	}
	// 	// }
	// }
	//
	// namespace XeSS
	// {
	// 	// inline bool UseXeFGFrameRate()
	// 	// {
	// 	// 	return UXeSSBlueprintLibrary::GetXeSSQualityMode() != EXeSSQualityMode::Off
	// 	// 		&& UXeFGBlueprintLibrary::GetXeFGMode() != EXeFGMode::Off;
	// 	// }
	// 	//
	// 	// namespace SR
	// 	// {
	// 	// 	inline const TMap<EXeSSQualityMode, FName> ModeNameMap = {
	// 	// 		{EXeSSQualityMode::Off,				TEXT("Off")							},
	// 	// 		{EXeSSQualityMode::AntiAliasing,		TEXT("Anti Aliasing (1.0x)")		},
	// 	// 		{EXeSSQualityMode::UltraQualityPlus, TEXT("Ultra Quality Plus (1.3x)")	},
	// 	// 		{EXeSSQualityMode::UltraQuality,		TEXT("Ultra Quality (1.5x)")		},
	// 	// 		{EXeSSQualityMode::Quality,			TEXT("Quality (1.7x)")				},
	// 	// 		{EXeSSQualityMode::Balanced,			TEXT("Balanced (2.0x)")				},
	// 	// 		{EXeSSQualityMode::Performance,		TEXT("Performance (2.3x)")			},
	// 	// 		{EXeSSQualityMode::UltraPerformance, TEXT("Ultra Performance (3.0x)")	}
	// 	// 	};
	// 	//
	// 	// 	inline bool IsSupported()
	// 	// 	{
	// 	// 		return UXeSSBlueprintLibrary::IsXeSSSupported();
	// 	// 	}
	// 	//
	// 	// 	inline void SetMode(const bool bEnabled, const EXeSSQualityMode Quality)
	// 	// 	{
	// 	// 		if (!bEnabled || Quality == EXeSSQualityMode::Off || static_cast<uint8>(Quality) > 7 || !IsSupported())
	// 	// 		{
	// 	// 			UXeSSBlueprintLibrary::SetXeSSQualityMode(EXeSSQualityMode::Off);
	// 	// 		}
	// 	// 		else
	// 	// 		{
	// 	// 			UXeSSBlueprintLibrary::SetXeSSQualityMode(Quality);
	// 	// 		}
	// 	// 	}
	// 	//
	// 	// 	inline void ForEachMode(const TFunction<void(const EXeSSQualityMode /*Mode*/)>& Func)
	// 	// 	{
	// 	// 		static constexpr EXeSSQualityMode VALID_MODES[] = {
	// 	// 			EXeSSQualityMode::AntiAliasing,
	// 	// 			EXeSSQualityMode::UltraQualityPlus,
	// 	// 			EXeSSQualityMode::UltraQuality,
	// 	// 			EXeSSQualityMode::Quality,
	// 	// 			EXeSSQualityMode::Balanced,
	// 	// 			EXeSSQualityMode::Performance,
	// 	// 			EXeSSQualityMode::UltraPerformance
	// 	// 		};
	// 	//
	// 	// 		if (Func)
	// 	// 		{
	// 	// 			for (const EXeSSQualityMode Mode : VALID_MODES)
	// 	// 			{
	// 	// 				Func(Mode);
	// 	// 			}
	// 	// 		}
	// 	// 	}
	// 	// }
	//
	// 	namespace FG
	// 	{
	// 		// inline bool IsSupported()
	// 		// {
	// 		// 	return UXeFGBlueprintLibrary::IsXeFGSupported();
	// 		// }
	// 		//
	// 		// inline void SetMode(const bool bEnabled)
	// 		// {
	// 		// 	UXeFGBlueprintLibrary::SetXeFGMode(bEnabled && IsSupported() ? EXeFGMode::On : EXeFGMode::Off);
	// 		// }
	// 	}
	//
	// 	namespace LL
	// 	{
	// 		// inline bool IsSupported()
	// 		// {
	// 		// 	return UXeLLBlueprintLibrary::IsXeLLSupported();
	// 		// }
	// 		//
	// 		// inline void SetMode(const bool bEnabled)
	// 		// {
	// 		// 	UXeLLBlueprintLibrary::SetXeLLMode(bEnabled && IsSupported() ? EXeLLMode::On : EXeLLMode::Off);
	// 		// }
	// 	}
	// }
	//
	// namespace DLSS
	// {
	// 	namespace SR
	// 	{
	// 		// inline const TMap<UDLSSMode, FName> ModeNameMap = {
	// 		// 	{UDLSSMode::Off,				TEXT("Off")						},
	// 		// 	{UDLSSMode::Auto,				TEXT("Auto")					},
	// 		// 	{UDLSSMode::DLAA,				TEXT("DLAA (1.0x)")				},
	// 		// 	{UDLSSMode::UltraQuality,		TEXT("Ultra Quality (1.3x)")	},
	// 		// 	{UDLSSMode::Quality,			TEXT("Quality (1.5x)")			},
	// 		// 	{UDLSSMode::Balanced,			TEXT("Balanced (1.7x)")			},
	// 		// 	{UDLSSMode::Performance,		TEXT("Performance (2.0x)")		},
	// 		// 	{UDLSSMode::UltraPerformance,	TEXT("Ultra Performance (3.0x)")}
	// 		// };
	// 		//
	// 		// inline bool IsSupported()
	// 		// {
	// 		// 	return UDLSSLibrary::IsDLSSSupported();
	// 		// }
	// 		//
	// 		// inline void SetMode(const bool bEnabled, const UDLSSMode Quality)
	// 		// {
	// 		// 	if (!bEnabled || Quality == UDLSSMode::Off || Quality == UDLSSMode::Auto || !IsSupported())
	// 		// 	{
	// 		// 		UDLSSLibrary::EnableDLSS(false);
	// 		// 		UToroConsoleUtils::SetCVarFloat(TEXT("r.ScreenPercentage"), 100.0f);
	// 		// 	}
	// 		// 	else
	// 		// 	{
	// 		// 		UDLSSLibrary::EnableDLSS(true);
	// 		//
	// 		// 		bool bSupported, bFixedScreenPercent;
	// 		// 		float OptimalScreenPercent, MinPercent, MaxPercent, Sharpness;
	// 		// 		UDLSSLibrary::GetDLSSModeInformation(Quality, FVector2D::ZeroVector, bSupported,
	// 		// 			OptimalScreenPercent, bFixedScreenPercent, MinPercent, MaxPercent, Sharpness
	// 		// 		);
	// 		//
	// 		// 		UToroConsoleUtils::SetCVarFloat(TEXT("r.ScreenPercentage"), OptimalScreenPercent);
	// 		// 	}
	// 		// }
	// 		//
	// 		// inline void ForEachMode(const TFunction<void(const UDLSSMode /*Mode*/, const bool /*bSupported*/)>& Func, bool bCheckSupported = true)
	// 		// {
	// 		// 	static constexpr UDLSSMode VALID_MODES[] = {
	// 		// 		UDLSSMode::DLAA,
	// 		// 		UDLSSMode::UltraQuality,
	// 		// 		UDLSSMode::Quality,
	// 		// 		UDLSSMode::Balanced,
	// 		// 		UDLSSMode::Performance,
	// 		// 		UDLSSMode::UltraPerformance
	// 		// 	};
	// 		//
	// 		// 	if (Func)
	// 		// 	{
	// 		// 		for (const UDLSSMode Mode : VALID_MODES)
	// 		// 		{
	// 		// 			Func(Mode, !bCheckSupported || UDLSSLibrary::IsDLSSModeSupported(Mode));
	// 		// 		}
	// 		// 	}
	// 		// }
	// 	}
	//
	// 	namespace RR
	// 	{
	// 		// inline bool IsSupported()
	// 		// {
	// 		// 	return UDLSSLibrary::IsDLSSRRSupported();
	// 		// }
	// 		//
	// 		// inline void SetMode(const bool bEnabled)
	// 		// {
	// 		// 	UDLSSLibrary::EnableDLSSRR(bEnabled);
	// 		// }
	// 	}
	//
	// 	namespace FG
	// 	{
	// 		// inline const TMap<EStreamlineDLSSGMode, FName> ModeNameMap = {
	// 		// 	{EStreamlineDLSSGMode::Off,  TEXT("Off")		},
	// 		// 	{EStreamlineDLSSGMode::Auto, TEXT("Auto")	},
	// 		// 	{EStreamlineDLSSGMode::On2X, TEXT("2x")		},
	// 		// 	{EStreamlineDLSSGMode::On3X, TEXT("3x")		},
	// 		// 	{EStreamlineDLSSGMode::On4X, TEXT("4x")		}
	// 		// };
	// 		//
	// 		// inline bool IsSupported()
	// 		// {
	// 		// 	return UStreamlineLibraryDLSSG::IsDLSSGSupported();
	// 		// }
	// 		//
	// 		// inline void SetMode(const EStreamlineDLSSGMode Mode)
	// 		// {
	// 		// 	if (Mode == EStreamlineDLSSGMode::Off || !IsSupported() 
	// 		// 		|| !UStreamlineLibraryDLSSG::IsDLSSGModeSupported(Mode))
	// 		// 	{
	// 		// 		UStreamlineLibraryDLSSG::SetDLSSGMode(EStreamlineDLSSGMode::Off);
	// 		// 	}
	// 		// 	else
	// 		// 	{
	// 		// 		UStreamlineLibraryDLSSG::SetDLSSGMode(Mode);
	// 		// 	}
	// 		// }
	// 		//
	// 		// inline void ForEachMode(const TFunction<void(const EStreamlineDLSSGMode /*Mode*/, const bool /*bSupported*/)>& Func, bool bCheckSupported = true)
	// 		// {
	// 		// 	static constexpr EStreamlineDLSSGMode VALID_MODES[] = {
	// 		// 		EStreamlineDLSSGMode::Off,
	// 		// 		EStreamlineDLSSGMode::Auto,
	// 		// 		EStreamlineDLSSGMode::On2X,
	// 		// 		EStreamlineDLSSGMode::On3X,
	// 		// 		EStreamlineDLSSGMode::On4X
	// 		// 	};
	// 		//
	// 		// 	if (Func)
	// 		// 	{
	// 		// 		for (const EStreamlineDLSSGMode Mode : VALID_MODES)
	// 		// 		{
	// 		// 			Func(Mode, !bCheckSupported || UStreamlineLibraryDLSSG::IsDLSSGModeSupported(Mode));
	// 		// 		}
	// 		// 	}
	// 		// }
	// 	}
	//
	// 	namespace Reflex
	// 	{
	// 		// inline const TMap<EStreamlineReflexMode, FName> ModeNameMap = {
	// 		// 	{EStreamlineReflexMode::Off,		TEXT("Off")		},
	// 		// 	{EStreamlineReflexMode::Enabled, TEXT("Enabled")	},
	// 		// 	{EStreamlineReflexMode::Boost,	TEXT("Boost")	}
	// 		// };
	// 		//
	// 		// inline bool IsSupported()
	// 		// {
	// 		// 	return UStreamlineLibraryReflex::IsReflexSupported();
	// 		// }
	// 		//
	// 		// inline void SetMode(const EStreamlineReflexMode Mode)
	// 		// {
	// 		// 	if (Mode == EStreamlineReflexMode::Off || !IsSupported() 
	// 		// 		|| !UStreamlineLibraryReflex::IsReflexModeSupported(Mode))
	// 		// 	{
	// 		// 		UStreamlineLibraryReflex::SetReflexMode(EStreamlineReflexMode::Off);
	// 		// 	}
	// 		// 	else
	// 		// 	{
	// 		// 		UStreamlineLibraryReflex::SetReflexMode(Mode);
	// 		// 	}
	// 		// }
	// 		//
	// 		// inline void ForEachMode(const TFunction<void(const EStreamlineReflexMode /*Mode*/, const bool /*bSupported*/)>& Func, bool bCheckSupported = true)
	// 		// {
	// 		// 	static constexpr EStreamlineReflexMode VALID_MODES[] = {
	// 		// 		EStreamlineReflexMode::Off,
	// 		// 		EStreamlineReflexMode::Enabled,
	// 		// 		EStreamlineReflexMode::Boost
	// 		// 	};
	// 		//
	// 		// 	if (Func)
	// 		// 	{
	// 		// 		for (const EStreamlineReflexMode Mode : VALID_MODES)
	// 		// 		{
	// 		// 			Func(Mode, !bCheckSupported || UStreamlineLibraryReflex::IsReflexModeSupported(Mode));
	// 		// 		}
	// 		// 	}
	// 		// }
	// 	}
	// }
}