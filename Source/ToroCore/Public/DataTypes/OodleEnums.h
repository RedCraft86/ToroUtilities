// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Compression/OodleDataCompression.h"
#include "OodleEnums.generated.h"

// FOodleDataCompression::ECompressor has NotSet = 0 but this is omitted so offset by -1 (this + 1 = orig)
UENUM(BlueprintType)
enum class EOodleCompressor : uint8
{
	Selkie		= 0, // 1
	Mermaid		= 1, // 2
	Kraken		= 2, // 3
	Leviathan	= 3  // 4
};
inline FOodleDataCompression::ECompressor OodleCompressorToNative(const EOodleCompressor InCompressor)
{
	return static_cast<FOodleDataCompression::ECompressor>(static_cast<uint8>(InCompressor) + 1);
}
inline EOodleCompressor NativeToOodleCompressor(const FOodleDataCompression::ECompressor InCompressor)
{
	return static_cast<EOodleCompressor>(FMath::Max(static_cast<int8>(InCompressor) - 1, 0));
}

// FOodleDataCompression::ECompressionLevel starts at -4 but since 0 is min, this is offset by +4 (this - 4 = orig)
UENUM(BlueprintType)
enum class EOodleCompressionLevel : uint8
{
	HyperFast4	= 0,  // -4
	HyperFast3	= 1,  // -3
	HyperFast2	= 2,  // -2
	HyperFast1	= 3,  // -1
	None		= 4,  // 0
	SuperFast	= 5,  // 1
	VeryFast	= 6,  // 2
	Fast		= 7,  // 3
	Normal		= 8,  // 4
	Optimal1	= 9,  // 5
	Optimal2	= 10, // 6
	Optimal3	= 11, // 7
	Optimal4	= 12, // 8
	Optimal5	= 13  // 9
};
inline FOodleDataCompression::ECompressionLevel OodleCompressionLevelToNative(const EOodleCompressionLevel InLevel)
{
	return static_cast<FOodleDataCompression::ECompressionLevel>(static_cast<int8>(InLevel) - 4);
}
inline EOodleCompressionLevel NativeToOodleCompressionLevel(const FOodleDataCompression::ECompressionLevel InLevel)
{
	return static_cast<EOodleCompressionLevel>(static_cast<int8>(InLevel) + 4);
}