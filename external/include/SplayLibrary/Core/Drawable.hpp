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
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \relates Drawable
	/// \struct VertexAttribute
	/// \brief Struct representing a shader vertex attribute definition.
	/// 
	/// For more information, see class Drawable.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct VertexAttribute
	{
		enum class Type
		{
			Float = 0x10,
			Vec2 = 0x20,
			Vec3 = 0x30,
			Vec4 = 0x40,

			Int = 0x11,
			IVec2 = 0x21,
			IVec3 = 0x31,
			IVec4 = 0x41,

			UnsignedInt = 0x12,
			UVec2 = 0x22,
			UVec3 = 0x32,
			UVec4 = 0x42,
		};

		uint32_t index;				///< Index of the vertex attribute in the shader layout.
		Type type;					///< Type of the attribute.
		uint32_t offset;			///< Offset of the first instance of the attribute in the data. Usually the offset of the field in the underlying vertex struct.
		uint32_t stride;			///< Offset between two instance of the attribute in the data. Usually the size of the underlying vertex struct.
		bool normalized = false;	///< Whether the data must be normalized or not in the shader.
		uint32_t divisor = 0;		///< Divisor for instanced draw.
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \relates Drawable
	/// \enum DrawableStorage
	/// \brief Enum for giving insight on how often drawables GPU data is updated.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum class DrawableStorage
	{
		Immutable,	///< The data is never updated. Each update is very costly.
		Static,		///< The data is updated rarely. Equivalent to BufferUsage::StaticDraw.
		Dynamic,	///< The data is updated frequently but not at each frame. Equivalent to BufferUsage::DynamicDraw.
		Stream		///< The data is updated at each frame. Equivalent to BufferUsage::StreamDraw.
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \ingroup Core
	/// \class Drawable
	/// \brief Class representing an object that can be drawn on a SplayLibrary window.
	/// 
	/// A drawable is an object composed of a list of vertices and a list of indices that can be drawn an a
	/// framebuffer.
	/// 
	/// The vertices are defined using VertexAttribute: A vertex is simply a struct with different fields, the vertex
	/// attributes, that can each be described using an instance of VertexAttribute. To define the list of vertices you
	/// need to first define the vertices data using `createNewVertices` then to define how this data must be read
	/// using `setVertexAttributes`.
	/// You can then update the vertices data using `updateVertices`, this operation might require to redefine the
	/// vertex attributes - you can check it using needsAttributesUpdate - and if so, a new call to
	/// `setVertexAttributes` is required.
	/// 
	/// The indices refers to an order for the vertices to be transmitted to the shaders.
	/// 
	/// The vertices and indices are stored in GPU memory and need insights on how often they will be updated. To learn
	/// more about these insights, see DrawableStorage.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class Drawable
	{
		public:

			Drawable(const Drawable& mesh) = delete;
			Drawable(Drawable&& mesh) = delete;

			Drawable& operator=(const Drawable& mesh) = delete;
			Drawable& operator=(Drawable&& mesh) = delete;

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Draws the drawable on the current framebuffer.
			///
			/// \param indicesCount	It is possible to draw only a part of the indices without having to modify the
			///						index buffer. This param specifies how many indices the draw command must read. If
			///						-1, all the indices are read.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			virtual void draw(PrimitiveType primitiveType, uintptr_t indicesCount = -1) const;

			~Drawable();

		protected:

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Default constructor. Create an invalid drawable for which the vertices and indices must be set.
			///
			/// Drawables cannot be instanciated directly. A class must inherit from drawable and make sure vertices
			/// and indices are correctly set.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			Drawable();

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Set vertex attributes description. For more info, see documentation of class Drawable.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			void setVertexAttributes(const std::vector<VertexAttribute>& attributes);

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Create vertex buffer and fill it with `vertices` if not `nullptr`.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			void createNewVertices(const void* vertices, uintptr_t size, DrawableStorage storage);
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Updates the type of vertex data storage.
			///
			/// See `updateVertices(const void*, uintptr_t, DrawableStorage, uintptr_t)` for more information.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			void updateVertices(DrawableStorage storage);
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Updates vertex data.
			///
			/// See `updateVertices(const void*, uintptr_t, DrawableStorage, uintptr_t)` for more information.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			void updateVertices(const void* vertices, uintptr_t size, uintptr_t offset = 0);
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Updates the vertices of the drawable.
			///
			/// If the operation requires to resize the buffer or to change its storage type, a whole new buffer must
			/// be created and the content of the previous one is copied into it. This operation is quite costly and
			/// must thus be avoided if not necessary.
			/// Furthermore, if a new buffer is created, the vertex attributes needs to be redefined by calling
			/// `setVertexAttributes`. To know if this is necessary, call `needsAttributesUpdate`.
			/// 
			/// \param vertices	Vertices data.
			/// \param size		Number of bytes to update.
			/// \param storage	Type of storage used. Should be equal to the value returned by `getVerticesStorage`
			///					most of the time.
			/// \param offset	Offset for vertex data to be copied in.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			void updateVertices(const void* vertices, uintptr_t size, DrawableStorage storage, uintptr_t offset = 0);
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Destroyes the underlying OpenGL vertex buffer.
			/// 
			/// Makes the drawable invalid until new vertex data is set using `createNewVertices`.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			void destroyVertices();

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Create indices buffer and fill it with `indices` if not `nullptr`.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			void createNewIndices(const uint32_t* indices, uintptr_t count, DrawableStorage storage);
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Updates the type of index storage.
			///
			/// See `updateIndices(const uint32_t*, uint32_t, DrawableStorage, uintptr_t)` for more information.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			void updateIndices(DrawableStorage storage);
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Updates indices.
			///
			/// See `updateIndices(const uint32_t*, uint32_t, DrawableStorage, uint32_t)` for more information.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			void updateIndices(const uint32_t* indices, uintptr_t count, uintptr_t startIndex = 0);
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Updates the indices of the drawable.
			///
			/// If the operation requires to resize the buffer or to change its storage type, a whole new buffer must
			/// be created and the content of the previous one is copied into it. This operation is quite costly and
			/// must thus be avoided if not necessary.
			/// 
			/// \param indices		Indices.
			/// \param count		Number of indices to update. *WARNING* It is NOT the number of bytes like in
			///						`updateVertices`.
			/// \param storage		Type of storage used. Should be equal to the value returned by `getIndicesStorage`
			///						most of the time.
			/// \param startIndex	First index to be modified by the method.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			void updateIndices(const uint32_t* indices, uintptr_t count, DrawableStorage storage, uintptr_t startIndex = 0);
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Destroyes the underlying OpenGL index buffer.
			/// 
			/// Makes the drawable invalid until new indices are set using `createNewIndices`.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			void destroyIndices();

			DrawableStorage getVerticesStorage() const;	///< Returns the type of storage used for vertex buffer.
			DrawableStorage getIndicesStorage() const;	///< Returns the type of storage used for index buffer.
			bool needsAttributesUpdate() const;			///< Returns true if setVertexAttributes needs to be called.
			virtual bool isValid() const;				///< Returns true if the drawable is ready to be drawn.

		private:

			uint32_t _vao;					///< OpenGL Vertex Array Object name.
			Buffer _vbo;					///< Vertex Buffer Object.
			DrawableStorage _vboStorage;	///< Vertex buffer storage.
			Buffer _ebo;					///< Element (indices) Buffer Object.
			DrawableStorage _eboStorage;	///< Element (indices) buffer storage.
			uintptr_t _indicesCount;		///< Number of indices in the EBO. Always equal to `_ebo.getSize() / sizeof(uint32_t)`

			bool _needsAttributesUpdate;	///< If a new vertex buffer has been created and the VAO has not been updated.
	};
}
