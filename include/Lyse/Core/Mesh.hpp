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
	template<CVertex TVertex = DefaultVertex>
	class Mesh : public Drawable
	{
		public:

			Mesh();
			template<CBufferStorageSpecifier TVertexStorage, CBufferStorageSpecifier TIndexStorage> Mesh(const TVertex* vertices, uint32_t vertexCount, TVertexStorage vertexStorage, const uint32_t* indices, uint32_t indexCount, TIndexStorage indexStorage);
			template<CBufferStorageSpecifier TVertexStorage, CBufferStorageSpecifier TIndexStorage> Mesh(const std::filesystem::path& path, TVertexStorage vertexStorage, TIndexStorage indexStorage);
			Mesh(const Mesh<TVertex>& mesh);
			Mesh(Mesh<TVertex>&& mesh);

			Mesh<TVertex>& operator=(const Mesh<TVertex>& mesh);
			Mesh<TVertex>& operator=(Mesh<TVertex>&& mesh);


			template<CBufferStorageSpecifier TVertexStorage, CBufferStorageSpecifier TIndexStorage> void createNew(const TVertex* vertices, uint32_t vertexCount, TVertexStorage vertexStorage, const uint32_t* indices, uint32_t indexCount, TIndexStorage indexStorage);
			template<CBufferStorageSpecifier TVertexStorage, CBufferStorageSpecifier TIndexStorage> void createNew(const std::filesystem::path& path, TVertexStorage vertexStorage, TIndexStorage indexStorage);

			// TODO: Update

			void destroy();


			virtual void draw() const override final;


			// TODO: getters VAO, EBO, VBO...
			bool isValid() const;


			virtual ~Mesh() override final = default;

		private:

			virtual void draw(const spl::ShaderProgram& program, const scp::f32mat4x4& transform) const override final;

			// TODO: Proper loading from file (same system as for DejaVu::Image)
			void _loadFromFile(const std::filesystem::path& path, std::vector<TVertex>& vertices, std::vector<uint32_t>& indices);
			bool _loadObj(const std::filesystem::path& path, std::vector<TVertex>& vertices, std::vector<uint32_t>& indices);

			spl::VertexArray _vao;
			spl::Buffer _vbo;
			spl::Buffer _ebo;
	};
}
