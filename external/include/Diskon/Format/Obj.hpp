///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2022-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Diskon/Format/FormatTypes.hpp>

namespace dsk
{
	namespace fmt
	{
		namespace obj
		{
			struct VertexPosition
			{
				double x, y, z, w;
			};

			struct VertexTextureCoordinate
			{
				double u, v, w;
			};

			struct VertexNormal
			{
				double i, j, k;
			};

			struct PointCloud
			{
				std::vector<uint64_t> vertices;
			};

			struct LineVertex
			{
				uint64_t position;
				uint64_t texCoord;
			};

			struct Line
			{
				std::vector<LineVertex> vertices;
			};

			struct FaceVertex
			{
				uint64_t position;
				uint64_t texCoord;
				uint64_t normal;
			};

			struct Face
			{
				std::vector<FaceVertex> vertices;
			};

			struct File
			{
				std::vector<VertexPosition> positions;
				std::vector<VertexTextureCoordinate> texCoords;
				std::vector<VertexNormal> normals;
				// TODO: Free form attributes

				std::vector<PointCloud> pointClouds;
				std::vector<Line> lines;
				std::vector<Face> faces;
				// TODO: curv, curv2, surf...

				// TODO: Others...
			};
		}

		class ObjIStream : public FormatIStream
		{
			public:

				ObjIStream();
				ObjIStream(const ObjIStream& stream) = delete;
				ObjIStream(ObjIStream&& stream) = delete;

				ObjIStream& operator=(const ObjIStream& stream) = delete;
				ObjIStream& operator=(ObjIStream&& stream) = delete;

				const FormatError& readFile(obj::File& file);

				~ObjIStream() = default;

			private:

				void onSourceRemoved() override final;

				const FormatError& _readVertexPosition(obj::VertexPosition& position);
				const FormatError& _readVertexTextureCoordinate(obj::VertexTextureCoordinate& texCoord);
				const FormatError& _readVertexNormal(obj::VertexNormal& normal);

				const FormatError& _readPointCloud(const obj::File& file, obj::PointCloud& pointCloud);
				const FormatError& _readLine(const obj::File& file, obj::Line& line);
				const FormatError& _readFace(const obj::File& file, obj::Face& face);

				const FormatError& _readSlashSeparatedIndices(const uint64_t* sizes, uint64_t* indices, uint64_t count, bool& parsedSomething);
				const FormatError& _readInteger(uint64_t& value, bool& parsedSomething);
				const FormatError& _readSignedInteger(int64_t& value, bool& parsedSomething);
				const FormatError& _readNumber(double& value, bool& parsedSomething);
				const FormatError& _readSpaces(bool& parsedSomething);
				const FormatError& _readEndLine();
				const FormatError& _ignoreEndLine();	// TODO: Remove this when format is fully supported
		};

		class ObjOStream : public FormatOStream
		{
			public:

				ObjOStream();
				ObjOStream(const ObjOStream& stream) = delete;
				ObjOStream(ObjOStream&& streal) = delete;

				ObjOStream& operator=(const ObjOStream& stream) = delete;
				ObjOStream& operator=(ObjOStream&& stream) = delete;

				const FormatError& writeFile(const obj::File& file);

				~ObjOStream() = default;

			private:

				void onDestinationRemoved() override final;

				const FormatError& _writeVertexPosition(const obj::VertexPosition& position);
				const FormatError& _writeVertexTextureCoordinate(const obj::VertexTextureCoordinate& texCoord);
				const FormatError& _writeVertexNormal(const obj::VertexNormal& normal);

				const FormatError& _writePointCloud(const obj::File& file, const obj::PointCloud& pointCloud);
				const FormatError& _writeLine(const obj::File& file, const obj::Line& line);
				const FormatError& _writeFace(const obj::File& file, const obj::Face& face);
		};
	}
}
