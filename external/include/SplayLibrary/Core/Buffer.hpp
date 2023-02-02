///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2021-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <SplayLibrary/Core/CoreTypes.hpp>

namespace spl
{
	enum class BufferTarget
	{
		Array,
		AtomicCounter,
		CopyRead,
		CopyWrite,
		DispatchIndirect,
		DrawIndirect,
		ElementArray,
		Parameter,
		PixelPack,
		PixelUnpack,
		Query,
		ShaderStorage,
		Texture,
		TransformFeedback,
		Uniform
	};


	/*
	
	Stream : Updated every frame
	Static : Updated very rarely - once every few seconds at most
	Dynamic : Other update frequencies (not at all frame, more often than once every few seconds...)

	Draw : Written by CPU, read by GPU
	Read : Written by GPU, read by CPU
	Copy : Written by GPU, read by GPU

	*/
	enum class BufferUsage
	{
		Undefined,
		StreamDraw,
		StreamRead,
		StreamCopy,
		StaticDraw,
		StaticRead,
		StaticCopy,
		DynamicDraw,
		DynamicRead,
		DynamicCopy
	};


	namespace BufferStorageFlags
	{
		enum Flags
		{
			None			= 0,
			DynamicStorage	= 1 << 0,
			MapRead			= 1 << 1,
			MapWrite		= 1 << 2,
			MapPersistent	= 1 << 3,
			MapCoherent		= 1 << 4,
			ClientStorage	= 1 << 5
		};
	}


	namespace BufferMapAccessFlags
	{
		enum Flags
		{
			None				= 0,
			Read				= 1 << 0,
			Write				= 1 << 1,
			Persistent			= 1 << 2,
			Coherent			= 1 << 3,
			InvalidateRange		= 1 << 4,
			InvalidateBuffer	= 1 << 5,
			FlushExplicit		= 1 << 6,
			Unsynchronized		= 1 << 7
		};
	}


	class Buffer
	{
		public:

			Buffer();
			Buffer(uintptr_t size, BufferUsage usage, const void* data = nullptr);
			Buffer(uintptr_t size, BufferStorageFlags::Flags flags, const void* data = nullptr);
			Buffer(const Buffer& buffer);
			Buffer(Buffer&& buffer);

			Buffer& operator=(const Buffer& buffer);
			Buffer& operator=(Buffer&& buffer);


			void createNew(uintptr_t size, BufferUsage usage, const void* data = nullptr);
			void createNew(uintptr_t size, BufferStorageFlags::Flags flags, const void* data = nullptr);
			void copyFrom(const Buffer& buffer);
			void moveFrom(Buffer&& buffer);

			void update(const void* data, uintptr_t size = -1, uintptr_t dstOffset = 0);
			void update(const Buffer& data, uintptr_t size = -1, uintptr_t dstOffset = 0, uintptr_t srcOffset = 0);
			template<CTextureInternalFormatType TClearValue> void clear(const TClearValue& clearValue, uintptr_t size = -1, uintptr_t offset = 0);

			void map(BufferMapAccessFlags::Flags flags, uintptr_t size = -1, uintptr_t offset = 0);
			void flush(uintptr_t size = -1, uintptr_t offset = 0);
			bool unmap();

			// TODO: glGetNamedBufferSubData...

			void invalidate(uintptr_t size = -1, uintptr_t offset = 0);

			void destroy();


			uint32_t getHandle() const;
			uintptr_t getSize() const;
			BufferUsage getUsage() const;
			BufferStorageFlags::Flags getStorageFlags() const;
			const void* getMapPointer() const;
			void* getMapPointer();
			BufferMapAccessFlags::Flags getMapAccessFlags() const;
			uintptr_t getMapOffset() const;
			uintptr_t getMapSize() const;

			bool isValid() const;
			bool hasImmutableStorage() const;
			bool isMapped() const;


			~Buffer();


			static void bind(const Buffer& buffer, BufferTarget target, uint32_t index = -1, uintptr_t size = -1, uintptr_t offset = 0);
			static void bind(const Buffer* const* buffers, uint32_t count, BufferTarget target, uint32_t firstIndex, const uintptr_t* sizes = nullptr, const uintptr_t* offsets = nullptr);
			static void unbind(BufferTarget target, uint32_t index = -1, uint32_t count = 1);

		private:

			void _clear(TextureInternalFormat internalFormat, uintptr_t offset, uintptr_t size, TextureFormat format, TextureDataType type, const void* data, uint32_t granularity);

			uint32_t _buffer;
			uintptr_t _size;
			BufferUsage _usage;
			BufferStorageFlags::Flags _storageFlags;

			void* _mapPtr;
			BufferMapAccessFlags::Flags _mapAccess;
			uintptr_t _mapSize;
			uintptr_t _mapOffset;
	};
}
