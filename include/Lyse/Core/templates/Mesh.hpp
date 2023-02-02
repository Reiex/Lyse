///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Lyse/Core/CoreDecl.hpp>

namespace lys
{
	template<CVertex TVertex>
	Mesh<TVertex>::Mesh() :
		_vao(),
		_vbo(),
		_ebo()
	{
		for (uint32_t i = 0; i < TVertex::getAttributeCount(); ++i)
		{
			_vao.setAttributeFormat(i, TVertex::getAttributeType(i), TVertex::getAttributeOffsetInStructure(i));
			_vao.setAttributeEnabled(i, true);
			_vao.setAttributeBinding(i, 0);
		}

		_vao.setBindingDivisor(0, 0);
	}

	
	template<CVertex TVertex>
	template<uint32_t IndexPos, uint32_t IndexNormal, uint32_t IndexTexCoords>
	Mesh<TVertex>::Mesh(const std::filesystem::path& path, spl::BufferUsage vertexUsage, spl::BufferUsage indexUsage) : Mesh()
	{
		createNew<IndexPos, IndexNormal, IndexTexCoords>(path, vertexUsage, indexUsage);
	}
	
	template<CVertex TVertex>
	template<uint32_t IndexPos, uint32_t IndexNormal, uint32_t IndexTexCoords>
	Mesh<TVertex>::Mesh(const std::filesystem::path& path, spl::BufferUsage vertexUsage, spl::BufferStorageFlags::Flags indexFlags) : Mesh()
	{
		createNew<IndexPos, IndexNormal, IndexTexCoords>(path, vertexUsage, indexFlags);
	}
	
	template<CVertex TVertex>
	template<uint32_t IndexPos, uint32_t IndexNormal, uint32_t IndexTexCoords>
	Mesh<TVertex>::Mesh(const std::filesystem::path& path, spl::BufferStorageFlags::Flags vertexFlags, spl::BufferUsage indexUsage) : Mesh()
	{
		createNew<IndexPos, IndexNormal, IndexTexCoords>(path, vertexFlags, indexUsage);
	}
	
	template<CVertex TVertex>
	template<uint32_t IndexPos, uint32_t IndexNormal, uint32_t IndexTexCoords>
	Mesh<TVertex>::Mesh(const std::filesystem::path& path, spl::BufferStorageFlags::Flags vertexFlags, spl::BufferStorageFlags::Flags indexFlags) : Mesh()
	{
		createNew<IndexPos, IndexNormal, IndexTexCoords>(path, vertexFlags, indexFlags);
	}

	template<CVertex TVertex>
	Mesh<TVertex>::Mesh(const TVertex* vertices, uint32_t vertexCount, spl::BufferUsage vertexUsage, const uint32_t* indices, uint32_t indexCount, spl::BufferUsage indexUsage) : Mesh()
	{
		createNew(vertices, vertexCount, vertexUsage, indices, indexCount, indexUsage);
	}

	template<CVertex TVertex>
	Mesh<TVertex>::Mesh(const TVertex* vertices, uint32_t vertexCount, spl::BufferUsage vertexUsage, const uint32_t* indices, uint32_t indexCount, spl::BufferStorageFlags::Flags indexFlags) : Mesh()
	{
		createNew(vertices, vertexCount, vertexUsage, indices, indexCount, indexFlags);
	}

	template<CVertex TVertex>
	Mesh<TVertex>::Mesh(const TVertex* vertices, uint32_t vertexCount, spl::BufferStorageFlags::Flags vertexFlags, const uint32_t* indices, uint32_t indexCount, spl::BufferUsage indexUsage) : Mesh()
	{
		createNew(vertices, vertexCount, vertexFlags, indices, indexCount, indexUsage);
	}

	template<CVertex TVertex>
	Mesh<TVertex>::Mesh(const TVertex* vertices, uint32_t vertexCount, spl::BufferStorageFlags::Flags vertexFlags, const uint32_t* indices, uint32_t indexCount, spl::BufferStorageFlags::Flags indexFlags) : Mesh()
	{
		createNew(vertices, vertexCount, vertexFlags, indices, indexCount, indexFlags);
	}

	template<CVertex TVertex>
	Mesh<TVertex>::Mesh(const Mesh<TVertex>& mesh) : Mesh()
	{
		Mesh<TVertex>::operator=(mesh);
	}

	template<CVertex TVertex>
	Mesh<TVertex>::Mesh(Mesh<TVertex>&& mesh) : Mesh()
	{
		Mesh<TVertex>::operator=(std::move(mesh));
	}

	template<CVertex TVertex>
	Mesh<TVertex>& Mesh<TVertex>::operator=(const Mesh<TVertex>& mesh)
	{
		_vbo.copyFrom(mesh._vbo);
		_ebo.copyFrom(mesh._ebo);

		_vao.bindArrayBuffer(_vbo, 0, 0, sizeof(TVertex));
		_vao.bindElementBuffer(_ebo);

		return *this;
	}

	template<CVertex TVertex>
	Mesh<TVertex>& Mesh<TVertex>::operator=(Mesh<TVertex>&& mesh)
	{
		_vbo.moveFrom(mesh._vbo);
		_ebo.moveFrom(mesh._ebo);

		_vao.bindArrayBuffer(_vbo, 0, 0, sizeof(TVertex));
		_vao.bindElementBuffer(_ebo);

		return *this;
	}

	template<CVertex TVertex>
	template<uint32_t IndexPos, uint32_t IndexNormal, uint32_t IndexTexCoords>
	void Mesh<TVertex>::createNew(const std::filesystem::path& path, spl::BufferUsage vertexUsage, spl::BufferUsage indexUsage)
	{
		std::vector<TVertex> vertices;
		std::vector<uint32_t> indices;

		_loadFromFile<IndexPos, IndexNormal, IndexTexCoords>(path, vertices, indices);

		if (!vertices.empty() && !indices.empty())
		{
			createNew(vertices.data(), vertices.size(), vertexUsage, indices.data(), indices.size(), indexUsage);
		}
	}

	template<CVertex TVertex>
	template<uint32_t IndexPos, uint32_t IndexNormal, uint32_t IndexTexCoords>
	void Mesh<TVertex>::createNew(const std::filesystem::path& path, spl::BufferUsage vertexUsage, spl::BufferStorageFlags::Flags indexFlags)
	{
		std::vector<TVertex> vertices;
		std::vector<uint32_t> indices;

		_loadFromFile<IndexPos, IndexNormal, IndexTexCoords>(path, vertices, indices);

		if (!vertices.empty() && !indices.empty())
		{
			createNew(vertices.data(), vertices.size(), vertexUsage, indices.data(), indices.size(), indexFlags);
		}
	}

	template<CVertex TVertex>
	template<uint32_t IndexPos, uint32_t IndexNormal, uint32_t IndexTexCoords>
	void Mesh<TVertex>::createNew(const std::filesystem::path& path, spl::BufferStorageFlags::Flags vertexFlags, spl::BufferUsage indexUsage)
	{
		std::vector<TVertex> vertices;
		std::vector<uint32_t> indices;

		_loadFromFile<IndexPos, IndexNormal, IndexTexCoords>(path, vertices, indices);

		if (!vertices.empty() && !indices.empty())
		{
			createNew(vertices.data(), vertices.size(), vertexFlags, indices.data(), indices.size(), indexUsage);
		}
	}

	template<CVertex TVertex>
	template<uint32_t IndexPos, uint32_t IndexNormal, uint32_t IndexTexCoords>
	void Mesh<TVertex>::createNew(const std::filesystem::path& path, spl::BufferStorageFlags::Flags vertexFlags, spl::BufferStorageFlags::Flags indexFlags)
	{
		std::vector<TVertex> vertices;
		std::vector<uint32_t> indices;

		_loadFromFile<IndexPos, IndexNormal, IndexTexCoords>(path, vertices, indices);

		if (!vertices.empty() && !indices.empty())
		{
			createNew(vertices.data(), vertices.size(), vertexFlags, indices.data(), indices.size(), indexFlags);
		}
	}

	template<CVertex TVertex>
	void Mesh<TVertex>::createNew(const TVertex* vertices, uint32_t vertexCount, spl::BufferUsage vertexUsage, const uint32_t* indices, uint32_t indexCount, spl::BufferUsage indexUsage)
	{
		_vbo.createNew(sizeof(TVertex) * vertexCount, vertexUsage, vertices);
		_ebo.createNew(sizeof(uint32_t) * indexCount, indexUsage, indices);

		_vao.bindArrayBuffer(_vbo, 0, 0, sizeof(TVertex));
		_vao.bindElementBuffer(_ebo);
	}

	template<CVertex TVertex>
	void Mesh<TVertex>::createNew(const TVertex* vertices, uint32_t vertexCount, spl::BufferUsage vertexUsage, const uint32_t* indices, uint32_t indexCount, spl::BufferStorageFlags::Flags indexFlags)
	{
		_vbo.createNew(sizeof(TVertex) * vertexCount, vertexUsage, vertices);
		_ebo.createNew(sizeof(uint32_t) * indexCount, indexFlags, indices);

		_vao.bindArrayBuffer(_vbo, 0, 0, sizeof(TVertex));
		_vao.bindElementBuffer(_ebo);
	}

	template<CVertex TVertex>
	void Mesh<TVertex>::createNew(const TVertex* vertices, uint32_t vertexCount, spl::BufferStorageFlags::Flags vertexFlags, const uint32_t* indices, uint32_t indexCount, spl::BufferUsage indexUsage)
	{
		_vbo.createNew(sizeof(TVertex) * vertexCount, vertexFlags, vertices);
		_ebo.createNew(sizeof(uint32_t) * indexCount, indexUsage, indices);

		_vao.bindArrayBuffer(_vbo, 0, 0, sizeof(TVertex));
		_vao.bindElementBuffer(_ebo);
	}

	template<CVertex TVertex>
	void Mesh<TVertex>::createNew(const TVertex* vertices, uint32_t vertexCount, spl::BufferStorageFlags::Flags vertexFlags, const uint32_t* indices, uint32_t indexCount, spl::BufferStorageFlags::Flags indexFlags)
	{
		_vbo.createNew(sizeof(TVertex) * vertexCount, vertexFlags, vertices);
		_ebo.createNew(sizeof(uint32_t) * indexCount, indexFlags, indices);

		_vao.bindArrayBuffer(_vbo, 0, 0, sizeof(TVertex));
		_vao.bindElementBuffer(_ebo);
	}

	template<CVertex TVertex>
	void Mesh<TVertex>::destroy()
	{
		_vbo.destroy();
		_ebo.destroy();
	}

	template<CVertex TVertex>
	void Mesh<TVertex>::draw() const
	{
		assert(isValid());

		_vao.drawElements(spl::PrimitiveType::Triangles, spl::IndexType::UnsignedInt, 0, _ebo.getSize() / sizeof(uint32_t));
	}

	template<CVertex TVertex>
	bool Mesh<TVertex>::isValid() const
	{
		return _vbo.isValid() && _ebo.isValid();
	}

	template<CVertex TVertex>
	void Mesh<TVertex>::draw(const spl::ShaderProgram& program, const scp::f32mat4x4& transform) const
	{
		program.setUniform("model", transform * getTransformMatrix());
		draw();
	}

	template<CVertex TVertex>
	template<uint32_t IndexPos, uint32_t IndexNormal, uint32_t IndexTexCoords>
	void Mesh<TVertex>::_loadFromFile(const std::filesystem::path& path, std::vector<TVertex>& vertices, std::vector<uint32_t>& indices)
	{
		// TODO: Check file exists...
		_loadObj<IndexPos, IndexNormal, IndexTexCoords>(path, vertices, indices);
	}

	template<CVertex TVertex>
	template<uint32_t IndexPos, uint32_t IndexNormal, uint32_t IndexTexCoords>
	bool Mesh<TVertex>::_loadObj(const std::filesystem::path& path, std::vector<TVertex>& vertices, std::vector<uint32_t>& indices)
	{
		dsk::fmt::ObjIStream objIStream;
		objIStream.setSource(path);

		dsk::fmt::obj::File objFile;
		objIStream.readFile(objFile);

		for (const dsk::fmt::obj::Face& face : objFile.faces)
		{
			if (face.vertices.size() != 3)
			{
				return false;
			}

			for (const dsk::fmt::obj::FaceVertex& vertex : face.vertices)
			{
				TVertex outVertex;

				if (vertex.position != UINT64_MAX)
				{
					outVertex.getAttribute<IndexPos>().x = objFile.positions[vertex.position].x;
					outVertex.getAttribute<IndexPos>().y = objFile.positions[vertex.position].y;
					outVertex.getAttribute<IndexPos>().z = objFile.positions[vertex.position].z;
				}

				if (vertex.normal != UINT64_MAX)
				{
					outVertex.getAttribute<IndexNormal>().x = objFile.normals[vertex.normal].i;
					outVertex.getAttribute<IndexNormal>().y = objFile.normals[vertex.normal].j;
					outVertex.getAttribute<IndexNormal>().z = objFile.normals[vertex.normal].k;
				}

				if (vertex.texCoord != UINT64_MAX)
				{
					outVertex.getAttribute<IndexTexCoords>().x = objFile.texCoords[vertex.texCoord].u;
					outVertex.getAttribute<IndexTexCoords>().y = objFile.texCoords[vertex.texCoord].v;
				}

				vertices.push_back(outVertex);
				indices.push_back(indices.size());
			}
		}

		return true;
	}
}
