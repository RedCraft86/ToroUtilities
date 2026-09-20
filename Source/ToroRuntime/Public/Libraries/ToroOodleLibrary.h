// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroOodleLibrary.generated.h"

/**
 * Oodle compression algorithm.
 */
UENUM(BlueprintType)
enum class EOodleCompressor : uint8
{
	Selkie		= 0,  // Native: 1
	Mermaid		= 1,  // Native: 2
	Kraken		= 2,  // Native: 3
	Leviathan	= 3   // Native: 4
};
inline FOodleDataCompression::ECompressor OodleCompressorToNative(const EOodleCompressor InCompressor)
{
	return static_cast<FOodleDataCompression::ECompressor>(static_cast<uint8>(InCompressor) + 1);
}
inline EOodleCompressor NativeToOodleCompressor(const FOodleDataCompression::ECompressor InCompressor)
{
	return static_cast<EOodleCompressor>(FMath::Max(static_cast<int8>(InCompressor) - 1, 0));
}

/**
 * Oodle compression effort level.
 */
UENUM(BlueprintType)
enum class EOodleCompressionLevel : uint8
{
	HyperFast4	= 0,  // Native: -4
	HyperFast3	= 1,  // Native: -3
	HyperFast2	= 2,  // Native: -2
	HyperFast1	= 3,  // Native: -1
	None		= 4,  // Native:  0
	SuperFast	= 5,  // Native:  1
	VeryFast	= 6,  // Native:  2
	Fast		= 7,  // Native:  3
	Normal		= 8,  // Native:  4
	Optimal1	= 9,  // Native:  5
	Optimal2	= 10, // Native:  6
	Optimal3	= 11, // Native:  7
	Optimal4	= 12, // Native:  8
	Optimal5	= 13  // Native:  9
};
inline FOodleDataCompression::ECompressionLevel OodleCompressionLevelToNative(const EOodleCompressionLevel InLevel)
{
	return static_cast<FOodleDataCompression::ECompressionLevel>(static_cast<int8>(InLevel) - 4);
}
inline EOodleCompressionLevel NativeToOodleCompressionLevel(const FOodleDataCompression::ECompressionLevel InLevel)
{
	return static_cast<EOodleCompressionLevel>(static_cast<int8>(InLevel) + 4);
}

/**
 * Utility functions for Oodle data compression.
 */
UCLASS()
class TORORUNTIME_API UToroOodleLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Compresses a byte array with Oodle.
	 * @param InData Uncompressed input bytes.
	 * @param OutData Receives compressed bytes; cleared before compression.
	 * @param Compressor Oodle compressor to use.
	 * @param Level Compression effort level.
	 * @return Whether compression succeeded.
	 */
	UFUNCTION(BlueprintCallable, Category = Compression)
		static bool OodleCompress(const TArray<uint8>& InData, TArray<uint8>& OutData,
			const EOodleCompressor Compressor = EOodleCompressor::Kraken,
			const EOodleCompressionLevel Level = EOodleCompressionLevel::SuperFast);

	/**
	 * Decompresses an Oodle-compressed byte array.
	 * @param InData Compressed input bytes.
	 * @param OutData Receives decompressed bytes; cleared before decompression.
	 * @return Whether decompression succeeded.
	 */
	UFUNCTION(BlueprintCallable, Category = Compression)
		static bool OodleDecompress(const TArray<uint8>& InData, TArray<uint8>& OutData);
};
