// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Compression/OodleDataCompressionUtil.h"

namespace DataCompression
{
	inline bool OodleCompress(const TArray<uint8>& InData, TArray<uint8>& OutData)
	{
		if (InData.IsEmpty())
		{
			OutData.Reset();
			return true;
		}
		return FOodleCompressedArray::CompressTArray(OutData, InData,
			FOodleDataCompression::ECompressor::Kraken, FOodleDataCompression::ECompressionLevel::SuperFast);
	}

	inline bool OodleDecompress(const TArray<uint8>& InData, TArray<uint8>& OutData)
	{
		if (InData.IsEmpty())
		{
			OutData.Reset();
			return true;
		}
		return FOodleCompressedArray::DecompressToTArray(OutData, InData);
	}
}