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
	template<CBufferStorageSpecifier TVertexStorage, CBufferStorageSpecifier TIndexStorage>
	Mesh<TVertex>::Mesh(const TVertex* vertices, uint32_t vertexCount, TVertexStorage vertexStorage, const uint32_t* indices, uint32_t indexCount, TIndexStorage indexStorage) : Mesh()
	{
		createNew(vertices, vertexCount, vertexStorage, indices, indexCount, indexStorage);
	}
	
	template<CVertex TVertex>
	template<CBufferStorageSpecifier TVertexStorage, CBufferStorageSpecifier TIndexStorage>
	Mesh<TVertex>::Mesh(const std::filesystem::path& path, TVertexStorage vertexStorage, TIndexStorage indexStorage) : Mesh()
	{
		createNew(path, vertexStorage, indexStorage);
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
	template<CBufferStorageSpecifier TVertexStorage, CBufferStorageSpecifier TIndexStorage>
	void Mesh<TVertex>::createNew(const TVertex* vertices, uint32_t vertexCount, TVertexStorage vertexStorage, const uint32_t* indices, uint32_t indexCount, TIndexStorage indexStorage)
	{
		_vbo.createNew(sizeof(TVertex) * vertexCount, vertexStorage, vertices);
		_ebo.createNew(sizeof(uint32_t) * indexCount, indexStorage, indices);

		_vao.bindArrayBuffer(_vbo, 0, 0, sizeof(TVertex));
		_vao.bindElementBuffer(_ebo);
	}

	template<CVertex TVertex>
	template<CBufferStorageSpecifier TVertexStorage, CBufferStorageSpecifier TIndexStorage>
	void Mesh<TVertex>::createNew(const std::filesystem::path& path, TVertexStorage vertexStorage, TIndexStorage indexStorage)
	{
		std::vector<TVertex> vertices;
		std::vector<uint32_t> indices;

		_loadFromFile(path, vertices, indices);

		if (!vertices.empty() && !indices.empty())
		{
			createNew(vertices.data(), vertices.size(), vertexStorage, indices.data(), indices.size(), indexStorage);
		}
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
		// TODO: assert on TVertex !
		program.setUniform("model", transform * getTransformMatrix());
		draw();
	}

	template<CVertex TVertex>
	void Mesh<TVertex>::_loadFromFile(const std::filesystem::path& path, std::vector<TVertex>& vertices, std::vector<uint32_t>& indices)
	{
		if (!std::filesystem::exists(path))
		{
			return;
		}

		if (path.extension() == ".obj")
		{
			_loadObj(path, vertices, indices);
		}
	}

	template<CVertex TVertex>
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
				TVertex outVertex = {};

				if (vertex.position != UINT64_MAX)
				{
					outVertex.setPosition(objFile.positions[vertex.position].x, objFile.positions[vertex.position].y, objFile.positions[vertex.position].z, objFile.positions[vertex.position].w);
				}

				if (vertex.normal != UINT64_MAX)
				{
					outVertex.setNormal(objFile.normals[vertex.normal].i, objFile.normals[vertex.normal].j, objFile.normals[vertex.normal].k, 0.f);
				}

				if (vertex.texCoord != UINT64_MAX)
				{
					outVertex.setTexCoords(objFile.texCoords[vertex.texCoord].u, objFile.texCoords[vertex.texCoord].v, objFile.texCoords[vertex.texCoord].w, 0.f);
				}

				vertices.push_back(outVertex);
				indices.push_back(indices.size());
			}
		}

		return true;
	}
}
