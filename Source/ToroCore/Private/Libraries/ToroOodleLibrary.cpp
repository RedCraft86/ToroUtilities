// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Libraries/ToroOodleLibrary.h"
#include "Compression/OodleDataCompressionUtil.h"

bool UToroOodleLibrary::OodleCompress(const TArray<uint8>& InData, TArray<uint8>& OutData,
	const EOodleCompressor Compressor, const EOodleCompressionLevel Level)
{
	OutData.Empty();
	return !InData.IsEmpty() && FOodleCompressedArray::CompressTArray(OutData, InData, 
		OodleCompressorToNative(Compressor), OodleCompressionLevelToNative(Level));
}

bool UToroOodleLibrary::OodleDecompress(const TArray<uint8>& InData, TArray<uint8>& OutData)
{
	OutData.Empty();
	return !InData.IsEmpty() && FOodleCompressedArray::DecompressToTArray(OutData, InData);
}
