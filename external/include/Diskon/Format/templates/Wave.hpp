///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2022-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Diskon/Format/FormatDecl.hpp>

namespace dsk
{
	namespace fmt
	{
		template<typename TSample>
		const FormatError& WaveIStream::readFile(wave::File<TSample>& file)
		{
			FMT_BEGIN("WaveIStream::readFile(wave::File<TSample>& file)");

			FMT_CALL(readHeader, _header);
			file.metadata = _header.metadata;
			file.samples.resize(_header.blockCount * _header.metadata.channelCount);

			FMT_CALL(readSampleBlocks, file.samples.data(), _header.blockCount);

			FMT_CALL(readEndFile);

			return *_error;
		}

		template<typename TSample>
		const FormatError& WaveIStream::readSampleBlocks(TSample* samples, uint32_t blockCount)
		{
			FMT_BEGIN("WaveIStream::readSampleBlock(TSample* samples, uint32_t blockCount)");

			assert(_headerRead);
			assert(_remainingBlocks >= blockCount);

			switch (_header.metadata.format)
			{
				case wave::Format::PCM:
				{
					const FormatError& (WaveIStream::*reader)(TSample*, uint32_t) = nullptr;

					switch (_header.metadata.bitsPerSample)
					{
						case 8:
						{
							reader = &WaveIStream::_readRawSampleBlocks<uint8_t, TSample>;
							break;
						}
						case 16:
						{
							reader = &WaveIStream::_readRawSampleBlocks<int16_t, TSample>;
							break;
						}
						// case 24:	// TODO: Handle 24 bit integers
						// {
						// 	reader = &WaveIStream::_readRawSampleBlock<SignedInt<24>, TSample>;
						// 	break;
						// }
						case 32:
						{
							reader = &WaveIStream::_readRawSampleBlocks<int32_t, TSample>;
							break;
						}
					}

					FMT_CALL((this->*reader), samples, blockCount);

					break;
				}
				case wave::Format::Float:
				{
					const FormatError& (WaveIStream::*reader)(TSample*, uint32_t) = nullptr;

					switch (_header.metadata.bitsPerSample)
					{
						case 32:
						{
							reader = &WaveIStream::_readRawSampleBlocks<float, TSample>;
							break;
						}
						case 64:
						{
							reader = &WaveIStream::_readRawSampleBlocks<double, TSample>;
							break;
						}
					}

					FMT_CALL((this->*reader), samples, blockCount);

					break;
				}
			}

			_remainingBlocks -= blockCount;

			return *_error;
		}

		template<typename TRaw, typename TSample>
		const FormatError& WaveIStream::_readRawSampleBlocks(TSample* samples, uint32_t blockCount)
		{
			FMT_BEGIN("WaveIStream::_readRawSampleBlock(TSample* samples, uint32_t blockCount)");

			const uint32_t sampleCount = _header.metadata.channelCount * blockCount;
			std::vector<TRaw> buffer(sampleCount);

			FMT_SUB_CALL(_riffStream, readChunkData, buffer.data(), sampleCount);
			const TRaw* it = buffer.data();
			const TRaw* const itEnd = it + sampleCount;
			for (; it != itEnd; ++it, ++samples)
			{
				convertNum<TRaw, TSample>(it, samples);
			}

			return *_error;
		}


		template<typename TSample>
		const FormatError& WaveOStream::writeFile(const wave::File<TSample>& file)
		{
			FMT_BEGIN("WaveOStream::writeFile(const wave::File<TSample>& file)");

			assert(_remainingBlocks == 0);

			_header.metadata = file.metadata;
			_header.blockCount = file.samples.size() / file.metadata.channelCount;

			FMT_CALL(writeHeader, _header);
			FMT_CALL(writeSampleBlocks, file.samples.data(), _header.blockCount);

			return *_error;
		}

		template<typename TSample>
		const FormatError& WaveOStream::writeSampleBlocks(const TSample* samples, uint32_t blockCount)
		{
			FMT_BEGIN("WaveOStream::writeSampleBlocks(const TSample* samples, uint32_t blockCount)");

			assert(_remainingBlocks >= blockCount);

			switch (_header.metadata.format)
			{
				case wave::Format::PCM:
				{
					const FormatError& (WaveOStream::*writer)(const TSample*, uint32_t) = nullptr;

					switch (_header.metadata.bitsPerSample)
					{
						case 8:
						{
							writer = &WaveOStream::_writeRawSampleBlocks<uint8_t, TSample>;
							break;
						}
						case 16:
						{
							writer = &WaveOStream::_writeRawSampleBlocks<int16_t, TSample>;
							break;
						}
						// case 24:
						// {
						// 	writer = &WaveOStream::_writeRawSampleBlock<SignedInt<24>, TSample>;
						// 	break;
						// }
						case 32:
						{
							writer = &WaveOStream::_writeRawSampleBlocks<int32_t, TSample>;
							break;
						}
					}

					FMT_CALL((this->*writer), samples, blockCount);

					break;
				}
				case wave::Format::Float:
				{
					const FormatError& (WaveOStream::*writer)(const TSample*, uint32_t) = nullptr;

					switch (_header.metadata.bitsPerSample)
					{
						case 32:
						{
							writer = &WaveOStream::_writeRawSampleBlocks<float, TSample>;
							break;
						}
						case 64:
						{
							writer = &WaveOStream::_writeRawSampleBlocks<double, TSample>;
							break;
						}
					}

					FMT_CALL((this->*writer), samples, blockCount);

					break;
				}
			}

			_remainingBlocks -= blockCount;

			return *_error;
		}

		template<typename TRaw, typename TSample>
		const FormatError& WaveOStream::_writeRawSampleBlocks(const TSample* samples, uint32_t blockCount)
		{
			FMT_BEGIN("WaveOStream::_writeRawSampleBlocks(const TSample* samples, uint32_t blockCount)");

			const uint32_t sampleCount = _header.metadata.channelCount * blockCount;
			std::vector<TRaw> buffer(sampleCount);

			TRaw* it = buffer.data();
			const TRaw* const itEnd = it + sampleCount;
			for (; it != itEnd; ++it, ++samples)
			{
				convertNum<TSample, TRaw>(samples, it);
			}

			FMT_SUB_CALL(_riffStream, writeChunkData, buffer.data(), sampleCount);

			return *_error;
		}
	}
}
