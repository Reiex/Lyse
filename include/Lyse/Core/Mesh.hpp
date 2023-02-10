///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Lyse/Core/CoreTypes.hpp>

namespace lys
{
	enum class MeshFormat
	{
		Obj
	};

	template<CVertex TVertex = DefaultVertex>
	class Mesh : public Drawable
	{
		public:

			template<CBufferStorageSpecifier TVertexStorage, CBufferStorageSpecifier TIndexStorage> Mesh(const std::initializer_list<TVertex>& vertices, TVertexStorage vertexStorage, TIndexStorage indexStorage);
			template<CBufferStorageSpecifier TVertexStorage, CBufferStorageSpecifier TIndexStorage> Mesh(const TVertex* vertices, uint32_t vertexCount, TVertexStorage vertexStorage, TIndexStorage indexStorage);
			template<CBufferStorageSpecifier TVertexStorage, CBufferStorageSpecifier TIndexStorage> Mesh(const std::initializer_list<TVertex>& vertices, TVertexStorage vertexStorage, const std::initializer_list<uint32_t>& indices, TIndexStorage indexStorage);
			template<CBufferStorageSpecifier TVertexStorage, CBufferStorageSpecifier TIndexStorage> Mesh(const TVertex* vertices, uint32_t vertexCount, TVertexStorage vertexStorage, const uint32_t* indices, uint32_t indexCount, TIndexStorage indexStorage);
			template<CBufferStorageSpecifier TVertexStorage, CBufferStorageSpecifier TIndexStorage> Mesh(const std::filesystem::path& path, TVertexStorage vertexStorage, TIndexStorage indexStorage);
			Mesh(const Mesh<TVertex>& mesh);
			Mesh(Mesh<TVertex>&& mesh);

			Mesh<TVertex>& operator=(const Mesh<TVertex>& mesh);
			Mesh<TVertex>& operator=(Mesh<TVertex>&& mesh);

			
			template<CBufferStorageSpecifier TStorage> void createNewVertices(const TVertex* vertices, uint32_t count, TStorage storage);
			template<CBufferStorageSpecifier TStorage> void createNewIndices(const uint32_t* indices, uint32_t count, TStorage storage);
			template<CBufferStorageSpecifier TVertexStorage, CBufferStorageSpecifier TIndexStorage> void createFromFile(const std::filesystem::path& path, TVertexStorage vertexStorage, TIndexStorage indexStorage);

			void updateVertices(const TVertex* vertices, uint32_t count, uint32_t offset = 0);
			void updateIndices(const uint32_t* indices, uint32_t count, uint32_t offset = 0);

			void destroy();


			void setPrimitiveType(spl::PrimitiveType primitiveType);

			virtual void draw() const override final;


			const spl::VertexArray& getVertexArray() const;
			spl::VertexArray& getVertexArray();
			const spl::Buffer& getVertexBuffer() const;
			spl::Buffer& getVertexBuffer();
			const spl::Buffer& getElementBuffer() const;
			spl::Buffer& getElementBuffer();
			spl::PrimitiveType getPrimitiveType() const;
			bool isValid() const;


			virtual ~Mesh() override final = default;

		private:

			Mesh();

			virtual void draw(const spl::ShaderProgram& program, const scp::f32mat4x4& transform) const override final;

			void _createFromFile(const std::filesystem::path& path, std::vector<TVertex>& vertices, std::vector<uint32_t>& indices);
			void _createFromStream(std::istream& stream, MeshFormat format, std::vector<TVertex>& vertices, std::vector<uint32_t>& indices);

			void _createFromObj(std::istream& stream, std::vector<TVertex>& vertices, std::vector<uint32_t>& indices);

			static constexpr void (Mesh<TVertex>::* _meshFormatToLoadFunc[])(std::istream&, std::vector<TVertex>&, std::vector<uint32_t>&) = {
				&Mesh<TVertex>::_createFromObj
			};
			static bool _extensionToMeshFormat(const std::filesystem::path& extension, MeshFormat& format);

			spl::VertexArray _vao;
			spl::Buffer _vbo;
			spl::Buffer _ebo;

			spl::PrimitiveType _primitiveType;
	};
}
