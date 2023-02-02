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
	// TODO: template<typename T> concept CBufferStorageSpecification = std::same_as<T, spl::BufferUsage> || std::same_as<T, spl::BufferStorageFlags::Flags>;

	template<CVertex TVertex = DefaultVertex>
	class Mesh : public Drawable
	{
		public:

			Mesh();
			template<uint32_t IndexPos = 0, uint32_t IndexNormal = 1, uint32_t IndexTexCoords = 2> Mesh(const std::filesystem::path& path, spl::BufferUsage vertexUsage, spl::BufferUsage indexUsage);
			template<uint32_t IndexPos = 0, uint32_t IndexNormal = 1, uint32_t IndexTexCoords = 2> Mesh(const std::filesystem::path& path, spl::BufferUsage vertexUsage, spl::BufferStorageFlags::Flags indexFlags);
			template<uint32_t IndexPos = 0, uint32_t IndexNormal = 1, uint32_t IndexTexCoords = 2> Mesh(const std::filesystem::path& path, spl::BufferStorageFlags::Flags vertexFlags, spl::BufferUsage indexUsage);
			template<uint32_t IndexPos = 0, uint32_t IndexNormal = 1, uint32_t IndexTexCoords = 2> Mesh(const std::filesystem::path& path, spl::BufferStorageFlags::Flags vertexFlags, spl::BufferStorageFlags::Flags indexFlags);
			Mesh(const TVertex* vertices, uint32_t vertexCount, spl::BufferUsage vertexUsage, const uint32_t* indices, uint32_t indexCount, spl::BufferUsage indexUsage);
			Mesh(const TVertex* vertices, uint32_t vertexCount, spl::BufferUsage vertexUsage, const uint32_t* indices, uint32_t indexCount, spl::BufferStorageFlags::Flags indexFlags);
			Mesh(const TVertex* vertices, uint32_t vertexCount, spl::BufferStorageFlags::Flags vertexFlags, const uint32_t* indices, uint32_t indexCount, spl::BufferUsage indexUsage);
			Mesh(const TVertex* vertices, uint32_t vertexCount, spl::BufferStorageFlags::Flags vertexFlags, const uint32_t* indices, uint32_t indexCount, spl::BufferStorageFlags::Flags indexFlags);
			Mesh(const Mesh<TVertex>& mesh);
			Mesh(Mesh<TVertex>&& mesh);

			Mesh<TVertex>& operator=(const Mesh<TVertex>& mesh);
			Mesh<TVertex>& operator=(Mesh<TVertex>&& mesh);


			template<uint32_t IndexPos = 0, uint32_t IndexNormal = 1, uint32_t IndexTexCoords = 2> void createNew(const std::filesystem::path& path, spl::BufferUsage vertexUsage, spl::BufferUsage indexUsage);
			template<uint32_t IndexPos = 0, uint32_t IndexNormal = 1, uint32_t IndexTexCoords = 2> void createNew(const std::filesystem::path& path, spl::BufferUsage vertexUsage, spl::BufferStorageFlags::Flags indexFlags);
			template<uint32_t IndexPos = 0, uint32_t IndexNormal = 1, uint32_t IndexTexCoords = 2> void createNew(const std::filesystem::path& path, spl::BufferStorageFlags::Flags vertexFlags, spl::BufferUsage indexUsage);
			template<uint32_t IndexPos = 0, uint32_t IndexNormal = 1, uint32_t IndexTexCoords = 2> void createNew(const std::filesystem::path& path, spl::BufferStorageFlags::Flags vertexFlags, spl::BufferStorageFlags::Flags indexFlags);
			void createNew(const TVertex* vertices, uint32_t vertexCount, spl::BufferUsage vertexUsage, const uint32_t* indices, uint32_t indexCount, spl::BufferUsage indexUsage);
			void createNew(const TVertex* vertices, uint32_t vertexCount, spl::BufferUsage vertexUsage, const uint32_t* indices, uint32_t indexCount, spl::BufferStorageFlags::Flags indexFlags);
			void createNew(const TVertex* vertices, uint32_t vertexCount, spl::BufferStorageFlags::Flags vertexFlags, const uint32_t* indices, uint32_t indexCount, spl::BufferUsage indexUsage);
			void createNew(const TVertex* vertices, uint32_t vertexCount, spl::BufferStorageFlags::Flags vertexFlags, const uint32_t* indices, uint32_t indexCount, spl::BufferStorageFlags::Flags indexFlags);
			
			// TODO: Update

			void destroy();


			virtual void draw() const override final;


			// TODO: getters VAO, EBO, VBO...
			bool isValid() const;


			virtual ~Mesh() override final = default;

		private:

			virtual void draw(const spl::ShaderProgram& program, const scp::f32mat4x4& transform) const override final;

			template<uint32_t IndexPos, uint32_t IndexNormal, uint32_t IndexTexCoords> void _loadFromFile(const std::filesystem::path& path, std::vector<TVertex>& vertices, std::vector<uint32_t>& indices);
			template<uint32_t IndexPos, uint32_t IndexNormal, uint32_t IndexTexCoords> bool _loadObj(const std::filesystem::path& path, std::vector<TVertex>& vertices, std::vector<uint32_t>& indices);

			spl::VertexArray _vao;
			spl::Buffer _vbo;
			spl::Buffer _ebo;
	};
}
