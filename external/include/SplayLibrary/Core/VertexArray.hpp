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
	enum class PrimitiveType
	{
		Points,

		Lines,
		LineStrips,
		LineLoops,
		LinesAdjacency,
		LineStripsAdjacency,

		Triangles,
		TriangleStrips,
		TriangleFans,
		TrianglesAdjacency,
		TriangleStripsAdjacency,

		Patches
	};

	enum class IndexType
	{
		UnsignedByte,
		UnsignedShort,
		UnsignedInt
	};

	class VertexArray
	{
		public:

			VertexArray();
			VertexArray(const VertexArray& vertexArray) = delete;
			VertexArray(VertexArray&& vertexArray) = delete;

			VertexArray& operator=(const VertexArray& vertexArray) = delete;
			VertexArray& operator=(VertexArray&& vertexArray) = delete;

			// TODO: Current vertex attribute values

			void bindElementBuffer(const Buffer* buffer);
			void bindArrayBuffer(const Buffer* buffer, uint32_t bindingIndex, uint32_t stride, uintptr_t offset = 0);
			void bindArrayBuffer(const Buffer* const* buffers, uint32_t firstIndex, uint32_t count, const uint32_t* strides, const uintptr_t* offsets = nullptr);

			void setAttributeFormat(uint32_t attributeIndex, GlslType type, uint32_t offsetInVertexStruct);
			void setAttributeEnabled(uint32_t attributeIndex, bool enabled);
			void setAttributeBinding(uint32_t attributeIndex, uint32_t bindingIndex);
			void setBindingDivisor(uint32_t bindingIndex, uint32_t divisor);

			void drawArrays(PrimitiveType type, uint32_t first, uint32_t count, uint32_t instanceCount = 1, uint32_t baseInstance = 0) const;
			void drawElements(PrimitiveType primitiveType, IndexType indexType, uintptr_t first, uint32_t count, uint32_t instanceCount = 1, uint32_t baseInstance = 0, uint32_t baseVertex = 0) const;
			void multiDrawArrays(PrimitiveType type, const uint32_t* firsts, const uint32_t* counts, uint32_t drawCount) const;
			void multiDrawElements(PrimitiveType primitiveType, IndexType indexType, const uintptr_t* firsts, const uint32_t* counts, uint32_t drawCount, const uint32_t* baseVertex = nullptr) const;
			// TODO: indirect

			uint32_t getHandle() const;

			~VertexArray();

		private:

			uint32_t _vao;
	};
}
