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
	Mesh<TVertex>::Mesh() : Drawable(),
		_vao(),
		_vbo(),
		_ebo(),
		_normalMap(nullptr)
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
	Mesh<TVertex>::Mesh(const std::initializer_list<TVertex>& vertices, TVertexStorage vertexStorage, TIndexStorage indexStorage) : Mesh(vertices.begin(), vertices.size(), vertexStorage, indexStorage)
	{
	}

	template<CVertex TVertex>
	template<CBufferStorageSpecifier TVertexStorage, CBufferStorageSpecifier TIndexStorage>
	Mesh<TVertex>::Mesh(const TVertex* vertices, uint32_t vertexCount, TVertexStorage vertexStorage, TIndexStorage indexStorage) : Mesh()
	{
		std::vector<uint32_t> indices(vertexCount);
		for (uint32_t i = 0; i < vertexCount; ++i)
		{
			indices[i] = i;
		}

		createNewVertices(vertices, vertexCount, vertexStorage);
		createNewIndices(indices.data(), indices.size(), indexStorage);
	}

	template<CVertex TVertex>
	template<CBufferStorageSpecifier TVertexStorage, CBufferStorageSpecifier TIndexStorage>
	Mesh<TVertex>::Mesh(const std::initializer_list<TVertex>& vertices, TVertexStorage vertexStorage, const std::initializer_list<uint32_t>& indices, TIndexStorage indexStorage) : Mesh(vertices.begin(), vertices.size(), vertexStorage, indices.begin(), indices.size(), indexStorage)
	{
	}

	template<CVertex TVertex>
	template<CBufferStorageSpecifier TVertexStorage, CBufferStorageSpecifier TIndexStorage>
	Mesh<TVertex>::Mesh(const TVertex* vertices, uint32_t vertexCount, TVertexStorage vertexStorage, const uint32_t* indices, uint32_t indexCount, TIndexStorage indexStorage) : Mesh()
	{
		createNewVertices(vertices, vertexCount, vertexStorage);
		createNewIndices(indices, indexCount, indexStorage);
	}
	
	template<CVertex TVertex>
	template<CBufferStorageSpecifier TVertexStorage, CBufferStorageSpecifier TIndexStorage>
	Mesh<TVertex>::Mesh(const std::filesystem::path& path, TVertexStorage vertexStorage, TIndexStorage indexStorage) : Mesh()
	{
		createFromFile(path, vertexStorage, indexStorage);
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
		Drawable::operator=(mesh);

		_vbo.copyFrom(mesh._vbo);
		_ebo.copyFrom(mesh._ebo);

		_vao.bindArrayBuffer(&_vbo, 0, sizeof(TVertex));
		_vao.bindElementBuffer(&_ebo);

		_normalMap = mesh._normalMap;

		return *this;
	}

	template<CVertex TVertex>
	Mesh<TVertex>& Mesh<TVertex>::operator=(Mesh<TVertex>&& mesh)
	{
		Drawable::operator=(std::move(mesh));

		_vbo.moveFrom(mesh._vbo);
		_ebo.moveFrom(mesh._ebo);

		_vao.bindArrayBuffer(&_vbo, 0, sizeof(TVertex));
		_vao.bindElementBuffer(&_ebo);

		_normalMap = mesh._normalMap;

		return *this;
	}

	template<CVertex TVertex>
	template<CBufferStorageSpecifier TStorage>
	void Mesh<TVertex>::createNewVertices(const TVertex* vertices, uint32_t count, TStorage storage)
	{
		_vbo.createNew(sizeof(TVertex) * count, storage, vertices);
		_vao.bindArrayBuffer(&_vbo, 0, sizeof(TVertex));
	}

	template<CVertex TVertex>
	template<CBufferStorageSpecifier TStorage>
	void Mesh<TVertex>::createNewIndices(const uint32_t* indices, uint32_t count, TStorage storage)
	{
		_ebo.createNew(sizeof(uint32_t) * count, storage, indices);
		_vao.bindElementBuffer(&_ebo);
	}

	template<CVertex TVertex>
	template<CBufferStorageSpecifier TVertexStorage, CBufferStorageSpecifier TIndexStorage>
	void Mesh<TVertex>::createFromFile(const std::filesystem::path& path, TVertexStorage vertexStorage, TIndexStorage indexStorage)
	{
		std::vector<TVertex> vertices;
		std::vector<uint32_t> indices;

		_createFromFile(path, vertices, indices);

		if (!vertices.empty() && !indices.empty())
		{
			createNewVertices(vertices.data(), vertices.size(), vertexStorage);
			createNewIndices(indices.data(), indices.size(), indexStorage);
		}
	}

	template<CVertex TVertex>
	void Mesh<TVertex>::updateVertices(const TVertex* vertices, uint32_t count, uint32_t offset)
	{
		_vbo.update(vertices, sizeof(TVertex) * count, sizeof(TVertex) * offset);
	}

	template<CVertex TVertex>
	void Mesh<TVertex>::updateIndices(const uint32_t* indices, uint32_t count, uint32_t offset)
	{
		_ebo.update(indices, sizeof(uint32_t) * count, sizeof(uint32_t) * offset);
	}

	template<CVertex TVertex>
	void Mesh<TVertex>::destroy()
	{
		_vbo.destroy();
		_ebo.destroy();
	}

	template<CVertex TVertex>
	void Mesh<TVertex>::setNormalMap(const spl::Texture2D* texture)
	{
		_normalMap = texture;
	}

	template<CVertex TVertex>
	void Mesh<TVertex>::draw(spl::PrimitiveType primitiveType) const
	{
		assert(isValid());

		_vao.drawElements(primitiveType, spl::IndexType::UnsignedInt, 0, _ebo.getSize() / sizeof(uint32_t));
	}

	template<CVertex TVertex>
	DrawableType Mesh<TVertex>::getType() const
	{
		return DrawableType::Mesh;
	}

	template<CVertex TVertex>
	const spl::VertexArray& Mesh<TVertex>::getVertexArray() const
	{
		return _vao;
	}

	template<CVertex TVertex>
	spl::VertexArray& Mesh<TVertex>::getVertexArray()
	{
		return _vao;
	}

	template<CVertex TVertex>
	const spl::Buffer& Mesh<TVertex>::getVertexBuffer() const
	{
		return _vbo;
	}

	template<CVertex TVertex>
	spl::Buffer& Mesh<TVertex>::getVertexBuffer()
	{
		return _vbo;
	}

	template<CVertex TVertex>
	const spl::Buffer& Mesh<TVertex>::getElementBuffer() const
	{
		return _ebo;
	}

	template<CVertex TVertex>
	spl::Buffer& Mesh<TVertex>::getElementBuffer()
	{
		return _ebo;
	}

	template<CVertex TVertex>
	const spl::Texture2D* Mesh<TVertex>::getNormalMap() const
	{
		return _normalMap;
	}

	template<CVertex TVertex>
	bool Mesh<TVertex>::isValid() const
	{
		return _vbo.isValid() && _ebo.isValid();
	}

	template<CVertex TVertex>
	void Mesh<TVertex>::_draw() const
	{
		draw();
	}
	
	namespace _lys
	{
		inline uint64_t read(void* handle, uint8_t* data, uint64_t size)
		{
			return std::fread(data, 1, size, reinterpret_cast<std::FILE*>(handle));
		}

		inline bool eof(void* handle)
		{
			return std::feof(reinterpret_cast<std::FILE*>(handle));
		}
	}

	template<CVertex TVertex>
	void Mesh<TVertex>::_createFromFile(const std::filesystem::path& path, std::vector<TVertex>& vertices, std::vector<uint32_t>& indices)
	{
		if (!std::filesystem::exists(path))
		{
			return;
		}

		std::FILE* file = std::fopen(path.string().c_str(), "rb");
		if (!file)
		{
			return;
		}

		MeshFormat format;
		if (_extensionToMeshFormat(path.extension(), format))
		{
			dsk::IStream* stream = new dsk::IStream(file, _lys::read, _lys::eof);
			_createFromStream(stream, format, vertices, indices);
			delete stream;
		}
	}

	template<CVertex TVertex>
	void Mesh<TVertex>::_createFromStream(dsk::IStream* stream, MeshFormat format, std::vector<TVertex>& vertices, std::vector<uint32_t>& indices)
	{
		assert(stream);

		(this->*(_meshFormatToLoadFunc[static_cast<size_t>(format)]))(stream, vertices, indices);
	}

	namespace _lys
	{
		template<CVertex TVertex>
		void fillVertex(TVertex& v0, TVertex& v1, TVertex& v2)
		{
			float dummy;

			// Extract "tex coords vectors" and check they're not colinear

			scp::f32vec2 tc0, tc1, tc2, d0, d1;
			v0.getTexCoords(tc0.x, tc0.y, dummy, dummy);
			v1.getTexCoords(tc1.x, tc1.y, dummy, dummy);
			v2.getTexCoords(tc2.x, tc2.y, dummy, dummy);
			d0 = tc1 - tc0;
			d1 = tc2 - tc1;
			const float det = d0.x * d1.y - d1.x * d0.y;
			if (det == 0.f)
			{
				return;
			}

			// Extract "position vectors" and check they're not colinear

			scp::f32vec3 p0, p1, p2, e0, e1;
			v0.getPosition(p0.x, p0.y, p0.z, dummy);
			v1.getPosition(p1.x, p1.y, p1.z, dummy);
			v2.getPosition(p2.x, p2.y, p2.z, dummy);
			e0 = p1 - p0;
			e1 = p2 - p1;

			scp::f32vec3 t;
			t = (d1.y * e0 - d0.y * e1) / det;
			if (t.x == 0.f && t.y == 0.f && t.z == 0.f)
			{
				return;
			}
			t = scp::normalize(t);

			// Compute/Extract normals

			scp::f32vec3 n0, n1, n2;

			v0.getNormal(n0.x, n0.y, n0.z, dummy);
			if (n0.x == 0.f && n0.y == 0.f && n0.z == 0.f)
			{
				n0 = scp::normalize(scp::cross(e0, e1));
			}

			v1.getNormal(n1.x, n1.y, n1.z, dummy);
			if (n1.x == 0.f && n1.y == 0.f && n1.z == 0.f)
			{
				n1 = scp::normalize(scp::cross(e0, e1));
			}

			v2.getNormal(n2.x, n2.y, n2.z, dummy);
			if (n2.x == 0.f && n2.y == 0.f && n2.z == 0.f)
			{
				n2 = scp::normalize(scp::cross(e0, e1));
			}

			// Compute tangents

			scp::f32vec3 tn;

			tn = scp::normalize(t - scp::dot(t, n0) * n0);
			v0.setTangent(tn.x, tn.y, tn.z, 0.f);
			tn = scp::normalize(t - scp::dot(t, n1) * n1);
			v1.setTangent(tn.x, tn.y, tn.z, 0.f);
			tn = scp::normalize(t - scp::dot(t, n2) * n2);
			v2.setTangent(tn.x, tn.y, tn.z, 0.f);
		}
	}

	template<CVertex TVertex>
	void Mesh<TVertex>::_createFromObj(dsk::IStream* stream, std::vector<TVertex>& vertices, std::vector<uint32_t>& indices)
	{
		dsk::fmt::ObjIStream objIStream(stream);

		dsk::fmt::obj::File objFile;
		objIStream.readFile(objFile);

		for (const dsk::fmt::obj::Face& face : objFile.faces)
		{
			if (face.vertices.size() != 3)
			{
				continue;
			}

			for (const dsk::fmt::obj::FaceVertex& vertex : face.vertices)
			{
				TVertex outVertex;

				assert(vertex.position != UINT64_MAX);
				outVertex.setPosition(objFile.positions[vertex.position].x, objFile.positions[vertex.position].y, objFile.positions[vertex.position].z, objFile.positions[vertex.position].w);

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

			_lys::fillVertex(vertices[vertices.size() - 3], vertices[vertices.size() - 2], vertices[vertices.size() - 1]);
		}
	}

	template<CVertex TVertex>
	bool Mesh<TVertex>::_extensionToMeshFormat(const std::filesystem::path& extension, MeshFormat& format)
	{
		static const std::unordered_map<std::filesystem::path, MeshFormat> extensionToMeshFormat = {
			{ ".obj", MeshFormat::Obj }
		};

		auto it = extensionToMeshFormat.find(extension);

		if (it == extensionToMeshFormat.end())
		{
			return false;
		}
		else
		{
			format = it->second;
			return true;
		}
	}
}
