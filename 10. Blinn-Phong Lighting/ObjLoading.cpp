#include "ObjLoading.h"
#include <assert.h>
#include <math.h> //pow(), fabs(), sqrtf()
#include <stdio.h>
#include <stdlib.h>
#include <utility>

#pragma warning(push)
#pragma warning(disable:4996) // disable warning that fopen() is unsafe


namespace geo {
	int CreateBox(
		std::string Name,
		float width, 
		float height, 
		float depth
	)
	{
		ObjectData* newData = new ObjectData;
		newData->objType = ObjectType::OBJ_TYPE;

		newData->objectName = Name;
		
		newData->SubmeshCount = 1;
		newData->meshDatas.resize(1);
		newData->VertexBuffer.resize(1);
		newData->IndexBuffer.resize(1);

		//
		// Create the vertices.
		//

		Vertex v[24];

		float w2 = 0.5f * width;
		float h2 = 0.5f * height;
		float d2 = 0.5f * depth;

		// Fill in the front face vertex data.
		v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		v[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		v[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

		// Fill in the back face vertex data.
		v[4] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
		v[5] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		v[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[7] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		// Fill in the top face vertex data.
		v[8] = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		v[9] = Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[10] = Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		v[11] = Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

		// Fill in the bottom face vertex data.
		v[12] = Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
		v[13] = Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		v[14] = Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[15] = Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		// Fill in the left face vertex data.
		v[16] = Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
		v[17] = Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		v[18] = Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
		v[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

		// Fill in the right face vertex data.
		v[20] = Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
		v[21] = Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		v[22] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
		v[23] = Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

		newData->meshDatas[0].Vertices.assign(&v[0], &v[24]);
		newData->meshBoundBoxData.Vertices.assign(&v[0], &v[24]);

		//
		// Create the indices.
		//

		uint32 i[36];

		// Fill in the front face index data
		i[0] = 0; i[1] = 1; i[2] = 2;
		i[3] = 0; i[4] = 2; i[5] = 3;

		// Fill in the back face index data
		i[6] = 4; i[7] = 5; i[8] = 6;
		i[9] = 4; i[10] = 6; i[11] = 7;

		// Fill in the top face index data
		i[12] = 8; i[13] = 9; i[14] = 10;
		i[15] = 8; i[16] = 10; i[17] = 11;

		// Fill in the bottom face index data
		i[18] = 12; i[19] = 13; i[20] = 14;
		i[21] = 12; i[22] = 14; i[23] = 15;

		// Fill in the left face index data
		i[24] = 16; i[25] = 17; i[26] = 18;
		i[27] = 16; i[28] = 18; i[29] = 19;

		// Fill in the right face index data
		i[30] = 20; i[31] = 21; i[32] = 22;
		i[33] = 20; i[34] = 22; i[35] = 23;

		newData->meshDatas[0].Indices32.assign(&i[0], &i[36]);
		newData->meshBoundBoxData.Indices32.assign(&i[0], &i[36]);

		MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_.insert(
			std::pair<std::string, ObjectData*>(Name, newData));

		return 0;
	}

	int CreateSphere (
		std::string Name,
		float radius, 
		uint32 sliceCount,
		uint32 stackCount
	)
	{
		ObjectData* newData = new ObjectData;
		newData->objType = ObjectType::OBJ_TYPE;

		newData->objectName = Name;

		newData->SubmeshCount = 1;
		newData->meshDatas.resize(1);
		newData->VertexBuffer.resize(1);
		newData->IndexBuffer.resize(1);

		//
		// Compute the vertices stating at the top pole and moving down the stacks.
		//

		// Poles: note that there will be texture coordinate distortion as there is
		// not a unique point on the texture map to assign to the pole when mapping
		// a rectangular texture onto a sphere.
		Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

		newData->meshDatas[0].Vertices.push_back(topVertex);

		float phiStep = DirectX::XM_PI / stackCount;
		float thetaStep = 2.0f * DirectX::XM_PI / sliceCount;

		// Compute vertices for each stack ring (do not count the poles as rings).
		for (uint32 i = 1; i <= stackCount - 1; ++i)
		{
			float phi = i * phiStep;

			// Vertices of ring.
			for (uint32 j = 0; j <= sliceCount; ++j)
			{
				float theta = j * thetaStep;

				Vertex v;

				// spherical to cartesian
				v.Position[0] = radius * sinf(phi) * cosf(theta);
				v.Position[1] = radius * cosf(phi);
				v.Position[2] = radius * sinf(phi) * sinf(theta);

				// Partial derivative of P with respect to theta
				v.TangentU[0] = -radius * sinf(phi) * sinf(theta);
				v.TangentU[1] = 0.0f;
				v.TangentU[2] = +radius * sinf(phi) * cosf(theta);

				DirectX::XMVECTOR TangentVec = { v.TangentU[0] , v.TangentU[1] , v.TangentU[2], 1.0f };
				TangentVec = DirectX::XMVector3Normalize(TangentVec);
				v.TangentU[0] = TangentVec.m128_f32[0];
				v.TangentU[1] = TangentVec.m128_f32[1];
				v.TangentU[2] = TangentVec.m128_f32[2];

				DirectX::XMVECTOR NormalVec = { v.Position[0] , v.Position[1] , v.Position[2], 1.0f };
				NormalVec = DirectX::XMVector3Normalize(NormalVec);
				v.Normal[0] = NormalVec.m128_f32[0];
				v.Normal[1] = NormalVec.m128_f32[1];
				v.Normal[2] = NormalVec.m128_f32[2];

				v.TexCoord0[0] = theta / DirectX::XM_2PI;
				v.TexCoord0[1] = phi / DirectX::XM_PI;

				newData->meshDatas[0].Vertices.push_back(v);
			}
		}

		newData->meshDatas[0].Vertices.push_back(bottomVertex);

		//
		// Compute indices for top stack.  The top stack was written first to the vertex buffer
		// and connects the top pole to the first ring.
		//

		for (uint32 i = 1; i <= sliceCount; ++i)
		{
			newData->meshDatas[0].Indices32.push_back(0);
			newData->meshDatas[0].Indices32.push_back(i + 1);
			newData->meshDatas[0].Indices32.push_back(i);
		}

		//
		// Compute indices for inner stacks (not connected to poles).
		//

		// Offset the indices to the index of the first vertex in the first ring.
		// This is just skipping the top pole vertex.
		uint32 baseIndex = 1;
		uint32 ringVertexCount = sliceCount + 1;
		for (uint32 i = 0; i < stackCount - 2; ++i)
		{
			for (uint32 j = 0; j < sliceCount; ++j)
			{
				newData->meshDatas[0].Indices32.push_back(baseIndex + i * ringVertexCount + j);
				newData->meshDatas[0].Indices32.push_back(baseIndex + i * ringVertexCount + j + 1);
				newData->meshDatas[0].Indices32.push_back(baseIndex + (i + 1) * ringVertexCount + j);

				newData->meshDatas[0].Indices32.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				newData->meshDatas[0].Indices32.push_back(baseIndex + i * ringVertexCount + j + 1);
				newData->meshDatas[0].Indices32.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
			}
		}

		//
		// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
		// and connects the bottom pole to the bottom ring.
		//

		// South pole vertex was added last.
		uint32 southPoleIndex = (uint32)newData->meshDatas[0].Vertices.size() - 1;

		// Offset the indices to the index of the first vertex in the last ring.
		baseIndex = southPoleIndex - ringVertexCount;

		for (uint32 i = 0; i < sliceCount; ++i)
		{
			newData->meshDatas[0].Indices32.push_back(southPoleIndex);
			newData->meshDatas[0].Indices32.push_back(baseIndex + i);
			newData->meshDatas[0].Indices32.push_back(baseIndex + i + 1);
		}

		MemoryPool::getInstance().AddObject(Name, newData);
		MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_.insert(
			std::pair<std::string, ObjectData*>(Name, newData));

		return 0;
	}
}

namespace obj
{
	// Based on the .obj loading code by Arseny Kapoulkine
	// in the meshoptimizer project

	static int parseInt(const char* s, const char** end)
	{
		// skip whitespace
		while (*s == ' ' || *s == '\t')
			++s;

		// read sign bit
		int sign = (*s == '-');
		if (*s == '-' || *s == '+')
			++s;

		unsigned int result = 0;
		while ((unsigned(*s - '0') < 10))
		{
			result = result * 10 + (*s - '0');
			++s;
		}

		// return end-of-string
		*end = s;

		return sign ? -int(result) : int(result);
	}

	static float parseFloat(const char* s, const char** end)
	{
		static const double powers[] = { 1e0, 1e+1, 1e+2, 1e+3, 1e+4, 1e+5, 1e+6, 1e+7, 1e+8, 1e+9, 1e+10, 1e+11, 1e+12, 1e+13, 1e+14, 1e+15, 1e+16, 1e+17, 1e+18, 1e+19, 1e+20, 1e+21, 1e+22 };

		// skip whitespace
		while (*s == ' ' || *s == '\t')
			++s;

		// read sign
		double sign = (*s == '-') ? -1 : 1;
		if (*s == '-' || *s == '+')
			++s;

		// read integer part
		double result = 0;
		int power = 0;

		while (unsigned(*s - '0') < 10)
		{
			result = result * 10 + (double)(*s - '0');
			++s;
		}

		// read fractional part
		if (*s == '.')
		{
			++s;

			while (unsigned(*s - '0') < 10)
			{
				result = result * 10 + (double)(*s - '0');
				++s;
				--power;
			}
		}

		// read exponent part
		// NOTE: bitwise OR with ' ' will transform an uppercase char 
		// to lowercase while leaving lowercase chars unchanged
		if ((*s | ' ') == 'e')
		{
			++s;

			// read exponent sign
			int expSign = (*s == '-') ? -1 : 1;
			if (*s == '-' || *s == '+')
				++s;

			// read exponent
			int expPower = 0;
			while (unsigned(*s - '0') < 10)
			{
				expPower = expPower * 10 + (*s - '0');
				++s;
			}

			power += expSign * expPower;
		}

		// return end-of-string
		*end = s;

		// note: this is precise if result < 9e15
		// for longer inputs we lose a bit of precision here
		if (unsigned(-power) < sizeof(powers) / sizeof(powers[0]))
			return float(sign * result / powers[-power]);
		else if (unsigned(power) < sizeof(powers) / sizeof(powers[0]))
			return float(sign * result * powers[power]);
		else
			return float(sign * result * pow(10.0, power));
	}

	static const char* parseFaceElement(const char* s, int& vi, int& vti, int& vni)
	{
		while (*s == ' ' || *s == '\t')
			++s;

		vi = parseInt(s, &s);

		if (*s != '/')
			return s;
		++s;

		// handle vi//vni indices
		if (*s != '/')
			vti = parseInt(s, &s);

		if (*s != '/')
			return s;
		++s;

		vni = parseInt(s, &s);

		return s;
	}

	static int fixupIndex(int index, size_t size)
	{
		return (index >= 0) ? index - 1 : int(size) + index;
	}

	static bool areAlmostEqual(float a, float b)
	{
		return (fabs(a - b) < 0.00001f);
	}

	static void growArray(void** array, size_t* capacity, size_t itemSize)
	{
		*capacity = (*capacity == 0) ? 32 : (*capacity + *capacity / 2);
		*array = realloc(*array, *capacity * itemSize);
		assert(*array);
	}

	LoadedObj loadObj(const char* filename)
	{
		LoadedObj result = {};

		// Read entire file into string
		char* fileBytes;
		{
			FILE* file = fopen(filename, "rb");
			assert(file);
			fseek(file, 0, SEEK_END);
			size_t fileNumBytes = ftell(file);
			fseek(file, 0, SEEK_SET);

			fileBytes = (char*)malloc(fileNumBytes + 1);
			assert(fileBytes);
			fread(fileBytes, 1, fileNumBytes, file);
			fileBytes[fileNumBytes] = '\0';
			fclose(file);
		}

		// Count number of elements in obj file
		uint32_t numVertexPositions = 0;
		uint32_t numVertexTexCoords = 0;
		uint32_t numVertexNormals = 0;
		uint32_t numFaces = 0;
		{
			const char* s = fileBytes;
			while (*s)
			{
				if (*s == 'v') {
					++s;
					if (*s == ' ') ++numVertexPositions;
					else if (*s == 't') ++numVertexTexCoords;
					else if (*s == 'n') ++numVertexNormals;
				}
				else if (*s == 'f') ++numFaces;

				while (*s != 0 && *s++ != '\n');
			}
		}

		float* vpBuffer = (float*)malloc(numVertexPositions * 3 * sizeof(float));
		float* vtBuffer = (float*)malloc(numVertexTexCoords * 2 * sizeof(float));
		float* vnBuffer = (float*)malloc(numVertexNormals * 3 * sizeof(float));
		float* vpIt = vpBuffer;
		float* vtIt = vtBuffer;
		float* vnIt = vnBuffer;

		size_t vertexBufferCapacity = 0;
		size_t indexBufferCapacity = 0;
		size_t vertexBufferSize = 0;
		size_t indexBufferSize = 0;
		Vertex* outVertexBuffer = NULL;
		uint16_t* outIndexBuffer = NULL;

		bool smoothNormals = false;

		const char* s = fileBytes;
		while (*s)
		{
			char currChar = *s;
			if (currChar == 'v') {
				++s;
				currChar = *s++;
				if (currChar == ' ') {
					*vpIt++ = parseFloat(s, &s);
					*vpIt++ = parseFloat(s, &s);
					*vpIt++ = parseFloat(s, &s);
				}
				else if (currChar == 't') {
					*vtIt++ = parseFloat(s, &s);
					*vtIt++ = parseFloat(s, &s);
				}
				else if (currChar == 'n') {
					*vnIt++ = parseFloat(s, &s);
					*vnIt++ = parseFloat(s, &s);
					*vnIt++ = parseFloat(s, &s);
				}
			}
			else if (currChar == 'f')
			{
				++s;
				while (*s != '\r' && *s != '\n' && *s != '\0')
				{
					int vpIdx = 0, vtIdx = 0, vnIdx = 0;
					s = parseFaceElement(s, vpIdx, vtIdx, vnIdx);
					if (!vpIdx)
						assert(vpIdx != 0);

					vpIdx = fixupIndex(vpIdx, numVertexPositions);
					vtIdx = fixupIndex(vtIdx, numVertexTexCoords);
					vnIdx = fixupIndex(vnIdx, numVertexNormals);

					Vertex newVert = {
						vpBuffer[3 * vpIdx], vpBuffer[3 * vpIdx + 1], vpBuffer[3 * vpIdx + 2],
						vnBuffer[3 * vnIdx], vnBuffer[3 * vnIdx + 1], vnBuffer[3 * vnIdx + 2],
						0.0f, 0.0f, 0.0f,
						vtBuffer[2 * vtIdx], vtBuffer[2 * vtIdx + 1],
					};

					// Search vertexBuffer for matching vertex
					uint32_t index;
					for (index = 0; index < vertexBufferSize; ++index)
					{
						Vertex* v = outVertexBuffer + index;
						bool posMatch = 
							areAlmostEqual(v->Position[0], newVert.Position[0])
							&& areAlmostEqual(v->Position[1], newVert.Position[1])
							&& areAlmostEqual(v->Position[2], newVert.Position[2]);
						bool uvMatch = 
							areAlmostEqual(v->TexCoord0[0], newVert.TexCoord0[0])
							&& areAlmostEqual(v->TexCoord0[1], newVert.TexCoord0[1]);
						bool normMatch = 
							areAlmostEqual(v->Normal[0], newVert.Normal[0])
							&& areAlmostEqual(v->Normal[1], newVert.Normal[1])
							&& areAlmostEqual(v->Normal[2], newVert.Normal[2]);
						if (posMatch && uvMatch)
						{
							if (normMatch || smoothNormals) {
								v->Normal[0] += newVert.Normal[0];
								v->Normal[1] += newVert.Normal[1];
								v->Normal[2] += newVert.Normal[2];
								break;
							}
						}
					}
					if (index == vertexBufferSize) {
						if (vertexBufferSize + 1 > vertexBufferCapacity) {
							growArray((void**)(&outVertexBuffer), &vertexBufferCapacity, sizeof(Vertex));
						}
						outVertexBuffer[vertexBufferSize++] = newVert;
					}
					if (indexBufferSize + 1 > indexBufferCapacity) {
						growArray((void**)(&outIndexBuffer), &indexBufferCapacity, sizeof(uint16_t));
					}
					outIndexBuffer[indexBufferSize++] = (uint16_t)index;
				}
			}
			else if (currChar == 's' && *(++s) == ' ')
			{
				++s;
				if ((*s == 'o' && *(s + 1) == 'f' && *(s + 2) == 'f') || *s == '0')
					smoothNormals = false;
				else {
					assert((*s == 'o' && *(s + 1) == 'n') || (*s >= '1' && *s <= '9'));
					smoothNormals = true;
				}
			}

			while (*s != 0 && *s++ != '\n');
		}

		// Normalise the normals
		for (uint32_t i = 0; i < vertexBufferSize; ++i) {
			Vertex* v = outVertexBuffer + i;
			float normLength = sqrtf(
				v->Normal[0] * v->Normal[0]
				+ v->Normal[1] * v->Normal[1]
				+ v->Normal[2] * v->Normal[2]);
			float invNormLength = 1.f / normLength;
			v->Normal[0] *= invNormLength;
			v->Normal[1] *= invNormLength;
			v->Normal[2] *= invNormLength;
		}

		free(vpBuffer);
		free(vtBuffer);
		free(vnBuffer);
		free(fileBytes);

		result.numVertices = vertexBufferSize;
		result.numIndices = indexBufferSize;
		result.vertexBuffer = outVertexBuffer;
		result.indexBuffer = outIndexBuffer;

		return result;
	}

	void freeLoadedObj(LoadedObj loadedObj)
	{
		free(loadedObj.vertexBuffer);
		free(loadedObj.indexBuffer);
	}

#pragma warning(pop)
}

namespace pmx
{
	// PMX Loader
	ObjectData* CreatePMXModel(
		ObjectData* newData,
		std::string Name,
		std::string Path
	) {
		if (newData == nullptr)
			throw std::runtime_error("newData is NULL");

		newData->objType = ObjectType::PMX_TYPE;

		newData->objectName = Name;

		MemoryPool::getInstance().ObjectNames.push_back(Name);

		// Read PMX
		pmx::PmxLoader model;
		model.Init();

		std::ifstream stream(Path.c_str(), std::ifstream::binary);
		model.Read(&stream);
		// Load Texture Path
		for (int i = 0; i < model.texture_count; i++) {
			MemoryPool::getInstance().TextureWildPaths.insert(model.textures[i].c_str());
		}

		// Vertex 정보가 담긴 Mesh를 저장하는 공간
		MeshData mesh, mesh1;
		pmx::PmxVertex* cv = NULL;

		mesh.Vertices.resize(model.vertex_count);
		for (int i = 0; i < model.vertex_count; i++) 
		{
			cv = &model.vertices[i];

			mesh.Vertices[i].Position[0] = cv->position[0];
			mesh.Vertices[i].Position[1] = cv->position[1];
			mesh.Vertices[i].Position[2] = cv->position[2];

			mesh.Vertices[i].Normal[0] = cv->normal[0];
			mesh.Vertices[i].Normal[1] = cv->normal[1];
			mesh.Vertices[i].Normal[2] = cv->normal[2];

			mesh.Vertices[i].TangentU[0] = 0.0f;
			mesh.Vertices[i].TangentU[1] = 0.0f;
			mesh.Vertices[i].TangentU[2] = 0.0f;

			mesh.Vertices[i].TexCoord0[0] = cv->uv[0];
			mesh.Vertices[i].TexCoord0[1] = cv->uv[1];
		}

		newData->meshDatas.push_back(mesh);

		int stack = 0;
		for (int i = 0; i < model.material_count; i++) {
			//model.materials[i].diffuse_texture_index;
			//model.materials[i].sphere_texture_index;
			//model.materials[i].toon_texture_index;

			mesh1.Indices32.resize(model.materials[i].index_count);
			for (int j = 0; j < model.materials[i].index_count; j++) 
			{
				mesh1.Indices32[j] = model.indices[stack++];
			}

			newData->meshDatas.push_back(mesh1);
		}

		// Set Submesh Info
		newData->SubmeshCount = model.material_count;

		newData->VertexBuffer.resize(newData->SubmeshCount);
		newData->IndexBuffer.resize(newData->SubmeshCount);

		MemoryPool::getInstance().AddObject(Name, newData);

		return newData;
	}

	int ReadIndex(std::istream* stream, int size)
	{
		switch (size)
		{
		case 1:
			uint8_t tmp8;
			stream->read((char*)&tmp8, sizeof(uint8_t));
			if (255 == tmp8)
			{
				return -1;
			}
			else {
				return (int)tmp8;
			}
		case 2:
			uint16_t tmp16;
			stream->read((char*)&tmp16, sizeof(uint16_t));
			if (65535 == tmp16)
			{
				return -1;
			}
			else {
				return (int)tmp16;
			}
		case 4:
			int tmp32;
			stream->read((char*)&tmp32, sizeof(int));
			return tmp32;
		default:
			return -1;
		}
	}

	utfstring ReadString(std::istream* stream, uint8_t encoding)
	{
#ifndef __unix__
		oguna::EncodingConverter converter = oguna::EncodingConverter();
#endif
		int size;
		stream->read((char*)&size, sizeof(int));
		std::vector<char> buffer;
		if (size == 0)
		{
#ifndef __unix__
			return utfstring(L"");
#else
			return utfstring("");
#endif
		}
		buffer.reserve(size);
		stream->read((char*)buffer.data(), size);
		if (encoding == 0)
		{
			// UTF16
#ifndef __unix__
			return utfstring((wchar_t*)buffer.data(), size / 2);
#else
			utfstring result;
			std::vector<char> outbuf;
			outbuf.reserve(size * 2);

			// Always remember to set U_ZERO_ERROR before calling ucnv_convert(),
			// otherwise the function will fail.
			UErrorCode err = U_ZERO_ERROR;
			size = ucnv_convert("UTF-8", "UTF-16LE", (char*)outbuf.data(), outbuf.capacity(), buffer.data(), size, &err);
			if (!U_SUCCESS(err)) {
				std::cout << "oops, something wrong?" << std::endl;
				std::cout << u_errorName(err) << std::endl;
				exit(-1);
			}

			result.assign((const char*)outbuf.data(), size);
			return result;
#endif
		}
		else
		{
			// UTF8
#ifndef __unix__
			utfstring result;
			converter.Utf8ToUtf16(buffer.data(), size, &result);
			return result;
#else
			return utfstring((const char*)buffer.data(), size);
#endif
		}
	}

	void PmxSetting::Read(std::istream* stream)
	{
		uint8_t count;
		stream->read((char*)&count, sizeof(uint8_t));
		if (count < 8)
		{
			throw;
		}
		stream->read((char*)&encoding, sizeof(uint8_t));
		stream->read((char*)&uv, sizeof(uint8_t));
		stream->read((char*)&vertex_index_size, sizeof(uint8_t));
		stream->read((char*)&texture_index_size, sizeof(uint8_t));
		stream->read((char*)&material_index_size, sizeof(uint8_t));
		stream->read((char*)&bone_index_size, sizeof(uint8_t));
		stream->read((char*)&morph_index_size, sizeof(uint8_t));
		stream->read((char*)&rigidbody_index_size, sizeof(uint8_t));
		uint8_t temp;
		for (int i = 8; i < count; i++)
		{
			stream->read((char*)&temp, sizeof(uint8_t));
		}
	}

	void PmxVertexSkinningBDEF1::Read(std::istream* stream, PmxSetting* setting)
	{
		this->bone_index = ReadIndex(stream, setting->bone_index_size);
	}

	void PmxVertexSkinningBDEF2::Read(std::istream* stream, PmxSetting* setting)
	{
		this->bone_index1 = ReadIndex(stream, setting->bone_index_size);
		this->bone_index2 = ReadIndex(stream, setting->bone_index_size);
		stream->read((char*)&this->bone_weight, sizeof(float));
	}

	void PmxVertexSkinningBDEF4::Read(std::istream* stream, PmxSetting* setting)
	{
		this->bone_index1 = ReadIndex(stream, setting->bone_index_size);
		this->bone_index2 = ReadIndex(stream, setting->bone_index_size);
		this->bone_index3 = ReadIndex(stream, setting->bone_index_size);
		this->bone_index4 = ReadIndex(stream, setting->bone_index_size);
		stream->read((char*)&this->bone_weight1, sizeof(float));
		stream->read((char*)&this->bone_weight2, sizeof(float));
		stream->read((char*)&this->bone_weight3, sizeof(float));
		stream->read((char*)&this->bone_weight4, sizeof(float));
	}

	void PmxVertexSkinningSDEF::Read(std::istream* stream, PmxSetting* setting)
	{
		this->bone_index1 = ReadIndex(stream, setting->bone_index_size);
		this->bone_index2 = ReadIndex(stream, setting->bone_index_size);
		stream->read((char*)&this->bone_weight, sizeof(float));
		stream->read((char*)this->sdef_c, sizeof(float) * 3);
		stream->read((char*)this->sdef_r0, sizeof(float) * 3);
		stream->read((char*)this->sdef_r1, sizeof(float) * 3);
	}

	void PmxVertexSkinningQDEF::Read(std::istream* stream, PmxSetting* setting)
	{
		this->bone_index1 = ReadIndex(stream, setting->bone_index_size);
		this->bone_index2 = ReadIndex(stream, setting->bone_index_size);
		this->bone_index3 = ReadIndex(stream, setting->bone_index_size);
		this->bone_index4 = ReadIndex(stream, setting->bone_index_size);
		stream->read((char*)&this->bone_weight1, sizeof(float));
		stream->read((char*)&this->bone_weight2, sizeof(float));
		stream->read((char*)&this->bone_weight3, sizeof(float));
		stream->read((char*)&this->bone_weight4, sizeof(float));
	}

	void PmxVertex::Read(std::istream* stream, PmxSetting* setting)
	{
		stream->read((char*)this->position, sizeof(float) * 3);
		stream->read((char*)this->normal, sizeof(float) * 3);
		stream->read((char*)this->uv, sizeof(float) * 2);
		for (int i = 0; i < setting->uv; ++i)
		{
			stream->read((char*)this->uva[i], sizeof(float) * 4);
		}
		stream->read((char*)&this->skinning_type, sizeof(PmxVertexSkinningType));
		switch (this->skinning_type)
		{
		case PmxVertexSkinningType::BDEF1:
			this->skinning = std::make_unique<PmxVertexSkinningBDEF1>();
			break;
		case PmxVertexSkinningType::BDEF2:
			this->skinning = std::make_unique<PmxVertexSkinningBDEF2>();
			break;
		case PmxVertexSkinningType::BDEF4:
			this->skinning = std::make_unique<PmxVertexSkinningBDEF4>();
			break;
		case PmxVertexSkinningType::SDEF:
			this->skinning = std::make_unique<PmxVertexSkinningSDEF>();
			break;
		case PmxVertexSkinningType::QDEF:
			this->skinning = std::make_unique<PmxVertexSkinningQDEF>();
			break;
		default:
			throw "invalid skinning type";
		}
		this->skinning->Read(stream, setting);
		stream->read((char*)&this->edge, sizeof(float));
	}

	void PmxMaterial::Read(std::istream* stream, PmxSetting* setting)
	{
		this->material_name = std::move(ReadString(stream, setting->encoding));
		this->material_english_name = std::move(ReadString(stream, setting->encoding));
		stream->read((char*)this->diffuse, sizeof(float) * 4);
		stream->read((char*)this->specular, sizeof(float) * 3);
		stream->read((char*)&this->specularlity, sizeof(float));
		stream->read((char*)this->ambient, sizeof(float) * 3);
		stream->read((char*)&this->flag, sizeof(uint8_t));
		stream->read((char*)this->edge_color, sizeof(float) * 4);
		stream->read((char*)&this->edge_size, sizeof(float));
		this->diffuse_texture_index = ReadIndex(stream, setting->texture_index_size);
		this->sphere_texture_index = ReadIndex(stream, setting->texture_index_size);
		stream->read((char*)&this->sphere_op_mode, sizeof(uint8_t));
		stream->read((char*)&this->common_toon_flag, sizeof(uint8_t));
		if (this->common_toon_flag)
		{
			stream->read((char*)&this->toon_texture_index, sizeof(uint8_t));
		}
		else {
			this->toon_texture_index = ReadIndex(stream, setting->texture_index_size);
		}
		this->memo = std::move(ReadString(stream, setting->encoding));
		stream->read((char*)&this->index_count, sizeof(int));
	}

	void PmxIkLink::Read(std::istream* stream, PmxSetting* setting)
	{
		this->link_target = ReadIndex(stream, setting->bone_index_size);
		stream->read((char*)&this->angle_lock, sizeof(uint8_t));
		if (angle_lock == 1)
		{
			stream->read((char*)this->max_radian, sizeof(float) * 3);
			stream->read((char*)this->min_radian, sizeof(float) * 3);
		}
	}

	void PmxBone::Read(std::istream* stream, PmxSetting* setting)
	{
		this->bone_name = std::move(ReadString(stream, setting->encoding));
		this->bone_english_name = std::move(ReadString(stream, setting->encoding));
		stream->read((char*)this->position, sizeof(float) * 3);
		this->parent_index = ReadIndex(stream, setting->bone_index_size);
		stream->read((char*)&this->level, sizeof(int));
		stream->read((char*)&this->bone_flag, sizeof(uint16_t));
		if (this->bone_flag & 0x0001) {
			this->target_index = ReadIndex(stream, setting->bone_index_size);
		}
		else {
			stream->read((char*)this->offset, sizeof(float) * 3);
		}
		if (this->bone_flag & (0x0100 | 0x0200)) {
			this->grant_parent_index = ReadIndex(stream, setting->bone_index_size);
			stream->read((char*)&this->grant_weight, sizeof(float));
		}
		if (this->bone_flag & 0x0400) {
			stream->read((char*)this->lock_axis_orientation, sizeof(float) * 3);
		}
		if (this->bone_flag & 0x0800) {
			stream->read((char*)this->local_axis_x_orientation, sizeof(float) * 3);
			stream->read((char*)this->local_axis_y_orientation, sizeof(float) * 3);

			// Cross Production of x and y axis.
			this->local_axis_z_orientation[0] =
				local_axis_x_orientation[1] * local_axis_y_orientation[2] -
				local_axis_x_orientation[2] * local_axis_y_orientation[1];
			this->local_axis_z_orientation[1] =
				local_axis_x_orientation[2] * local_axis_y_orientation[0] -
				local_axis_x_orientation[0] * local_axis_y_orientation[2];
			this->local_axis_z_orientation[2] =
				local_axis_x_orientation[1] * local_axis_y_orientation[2] -
				local_axis_x_orientation[2] * local_axis_y_orientation[1];

			// make Local Matrix
			this->localMatrix[0][0] = this->local_axis_x_orientation[0];
			this->localMatrix[0][1] = this->local_axis_x_orientation[1];
			this->localMatrix[0][2] = this->local_axis_x_orientation[2];
			this->localMatrix[0][3] = 0.0f;

			this->localMatrix[1][0] = this->local_axis_y_orientation[0];
			this->localMatrix[1][1] = this->local_axis_y_orientation[1];
			this->localMatrix[1][2] = this->local_axis_y_orientation[2];
			this->localMatrix[1][3] = 0.0f;

			this->localMatrix[2][0] = this->local_axis_z_orientation[0];
			this->localMatrix[2][1] = this->local_axis_z_orientation[1];
			this->localMatrix[2][2] = this->local_axis_z_orientation[2];
			this->localMatrix[2][3] = 0.0f;

			this->localMatrix[3][0] = this->position[0];
			this->localMatrix[3][1] = this->position[1];
			this->localMatrix[3][2] = this->position[2];
			this->localMatrix[3][3] = 1.0f;
		}
		if (this->bone_flag & 0x2000) {
			stream->read((char*)&this->key, sizeof(int));
		}
		if (this->bone_flag & 0x0020) {
			this->ik_target_bone_index = ReadIndex(stream, setting->bone_index_size);
			stream->read((char*)&ik_loop, sizeof(int));
			stream->read((char*)&ik_loop_angle_limit, sizeof(float));
			stream->read((char*)&ik_link_count, sizeof(int));
			this->ik_links = std::make_unique<PmxIkLink[]>(ik_link_count);
			for (int i = 0; i < ik_link_count; i++) {
				ik_links[i].Read(stream, setting);
			}
		}
	}

	void PmxMorphVertexOffset::Read(std::istream* stream, PmxSetting* setting)
	{
		this->vertex_index = ReadIndex(stream, setting->vertex_index_size);
		stream->read((char*)this->position_offset, sizeof(float) * 3);
	}

	void PmxMorphUVOffset::Read(std::istream* stream, PmxSetting* setting)
	{
		this->vertex_index = ReadIndex(stream, setting->vertex_index_size);
		stream->read((char*)this->uv_offset, sizeof(float) * 4);
	}

	void PmxMorphBoneOffset::Read(std::istream* stream, PmxSetting* setting)
	{
		this->bone_index = ReadIndex(stream, setting->bone_index_size);
		stream->read((char*)this->translation, sizeof(float) * 3);
		stream->read((char*)this->rotation, sizeof(float) * 4);
	}

	void PmxMorphMaterialOffset::Read(std::istream* stream, PmxSetting* setting)
	{
		this->material_index = ReadIndex(stream, setting->material_index_size);
		stream->read((char*)&this->offset_operation, sizeof(uint8_t));
		stream->read((char*)this->diffuse, sizeof(float) * 4);
		stream->read((char*)this->specular, sizeof(float) * 3);
		stream->read((char*)&this->specularity, sizeof(float));
		stream->read((char*)this->ambient, sizeof(float) * 3);
		stream->read((char*)this->edge_color, sizeof(float) * 4);
		stream->read((char*)&this->edge_size, sizeof(float));
		stream->read((char*)this->texture_argb, sizeof(float) * 4);
		stream->read((char*)this->sphere_texture_argb, sizeof(float) * 4);
		stream->read((char*)this->toon_texture_argb, sizeof(float) * 4);
	}

	void PmxMorphGroupOffset::Read(std::istream* stream, PmxSetting* setting)
	{
		this->morph_index = ReadIndex(stream, setting->morph_index_size);
		stream->read((char*)&this->morph_weight, sizeof(float));
	}

	void PmxMorphFlipOffset::Read(std::istream* stream, PmxSetting* setting)
	{
		this->morph_index = ReadIndex(stream, setting->morph_index_size);
		stream->read((char*)&this->morph_value, sizeof(float));
	}

	void PmxMorphImpulseOffset::Read(std::istream* stream, PmxSetting* setting)
	{
		this->rigid_body_index = ReadIndex(stream, setting->rigidbody_index_size);
		stream->read((char*)&this->is_local, sizeof(uint8_t));
		stream->read((char*)this->velocity, sizeof(float) * 3);
		stream->read((char*)this->angular_torque, sizeof(float) * 3);
	}

	void PmxMorph::Read(std::istream* stream, PmxSetting* setting)
	{
		this->morph_name = ReadString(stream, setting->encoding);
		this->morph_english_name = ReadString(stream, setting->encoding);
		stream->read((char*)&category, sizeof(MorphCategory));
		stream->read((char*)&morph_type, sizeof(MorphType));
		stream->read((char*)&this->offset_count, sizeof(int));
		switch (this->morph_type)
		{
		case MorphType::Group:
			group_offsets = std::make_unique<PmxMorphGroupOffset[]>(this->offset_count);
			for (int i = 0; i < offset_count; i++)
			{
				group_offsets[i].Read(stream, setting);
			}
			break;
		case MorphType::Vertex:
			vertex_offsets = std::make_unique<PmxMorphVertexOffset[]>(this->offset_count);
			for (int i = 0; i < offset_count; i++)
			{
				vertex_offsets[i].Read(stream, setting);
			}
			break;
		case MorphType::Bone:
			bone_offsets = std::make_unique<PmxMorphBoneOffset[]>(this->offset_count);
			for (int i = 0; i < offset_count; i++)
			{
				bone_offsets[i].Read(stream, setting);
			}
			break;
		case MorphType::Matrial:
			material_offsets = std::make_unique<PmxMorphMaterialOffset[]>(this->offset_count);
			for (int i = 0; i < offset_count; i++)
			{
				material_offsets[i].Read(stream, setting);
			}
			break;
		case MorphType::UV:
		case MorphType::AdditionalUV1:
		case MorphType::AdditionalUV2:
		case MorphType::AdditionalUV3:
		case MorphType::AdditionalUV4:
			uv_offsets = std::make_unique<PmxMorphUVOffset[]>(this->offset_count);
			for (int i = 0; i < offset_count; i++)
			{
				uv_offsets[i].Read(stream, setting);
			}
			break;
		default:
			throw;
		}
	}

	void PmxFrameElement::Read(std::istream* stream, PmxSetting* setting)
	{
		stream->read((char*)&this->element_target, sizeof(uint8_t));
		if (this->element_target == 0x00)
		{
			this->index = ReadIndex(stream, setting->bone_index_size);
		}
		else {
			this->index = ReadIndex(stream, setting->morph_index_size);
		}
	}

	void PmxFrame::Read(std::istream* stream, PmxSetting* setting)
	{
		this->frame_name = ReadString(stream, setting->encoding);
		this->frame_english_name = ReadString(stream, setting->encoding);
		stream->read((char*)&this->frame_flag, sizeof(uint8_t));
		stream->read((char*)&this->element_count, sizeof(int));
		this->elements = std::make_unique<PmxFrameElement[]>(this->element_count);
		for (int i = 0; i < this->element_count; i++)
		{
			this->elements[i].Read(stream, setting);
		}
	}

	void PmxRigidBody::Read(std::istream* stream, PmxSetting* setting)
	{
		this->rigid_body_name = ReadString(stream, setting->encoding);
		this->rigid_body_english_name = ReadString(stream, setting->encoding);
		this->target_bone = ReadIndex(stream, setting->bone_index_size);
		stream->read((char*)&this->group, sizeof(uint8_t));
		stream->read((char*)&this->mask, sizeof(uint16_t));
		stream->read((char*)&this->shape, sizeof(uint8_t));
		stream->read((char*)this->size, sizeof(float) * 3);
		stream->read((char*)this->position, sizeof(float) * 3);
		stream->read((char*)this->orientation, sizeof(float) * 3);
		stream->read((char*)&this->mass, sizeof(float));
		stream->read((char*)&this->move_attenuation, sizeof(float));
		stream->read((char*)&this->rotation_attenuation, sizeof(float));
		stream->read((char*)&this->repulsion, sizeof(float));
		stream->read((char*)&this->friction, sizeof(float));
		stream->read((char*)&this->physics_calc_type, sizeof(uint8_t));
	}

	void PmxJointParam::Read(std::istream* stream, PmxSetting* setting)
	{
		this->rigid_body1 = ReadIndex(stream, setting->rigidbody_index_size);
		this->rigid_body2 = ReadIndex(stream, setting->rigidbody_index_size);
		stream->read((char*)this->position, sizeof(float) * 3);
		stream->read((char*)this->orientaiton, sizeof(float) * 3);
		stream->read((char*)this->move_limitation_min, sizeof(float) * 3);
		stream->read((char*)this->move_limitation_max, sizeof(float) * 3);
		stream->read((char*)this->rotation_limitation_min, sizeof(float) * 3);
		stream->read((char*)this->rotation_limitation_max, sizeof(float) * 3);
		stream->read((char*)this->spring_move_coefficient, sizeof(float) * 3);
		stream->read((char*)this->spring_rotation_coefficient, sizeof(float) * 3);
	}

	void PmxJoint::Read(std::istream* stream, PmxSetting* setting)
	{
		this->joint_name = ReadString(stream, setting->encoding);
		this->joint_english_name = ReadString(stream, setting->encoding);
		stream->read((char*)&this->joint_type, sizeof(uint8_t));
		this->param.Read(stream, setting);
	}

	void PmxAnchorRigidBody::Read(std::istream* stream, PmxSetting* setting)
	{
		this->related_rigid_body = ReadIndex(stream, setting->rigidbody_index_size);
		this->related_vertex = ReadIndex(stream, setting->vertex_index_size);
		stream->read((char*)&this->is_near, sizeof(uint8_t));
	}

	void PmxSoftBody::Read(std::istream* stream, PmxSetting* setting)
	{
		throw std::runtime_error("Not Implemented Exception");
	}

	void PmxLoader::Init()
	{
		this->version = 0.0f;
		this->model_name.clear();
		this->model_english_name.clear();
		this->model_comment.clear();
		this->model_english_comment.clear();
		this->vertex_count = 0;
		this->vertices = nullptr;
		this->index_count = 0;
		this->indices = nullptr;
		this->texture_count = 0;
		this->textures = nullptr;
		this->material_count = 0;
		this->materials = nullptr;
		this->bone_count = 0;
		this->bones = nullptr;
		this->morph_count = 0;
		this->morphs = nullptr;
		this->frame_count = 0;
		this->frames = nullptr;
		this->rigid_body_count = 0;
		this->rigid_bodies = nullptr;
		this->joint_count = 0;
		this->joints = nullptr;
		this->soft_body_count = 0;
		this->soft_bodies = nullptr;
	}

	void PmxLoader::Read(std::istream* stream)
	{
		char magic[4];
		stream->read((char*)magic, sizeof(char) * 4);
		if (magic[0] != 0x50 || magic[1] != 0x4d || magic[2] != 0x58 || magic[3] != 0x20)
		{
			throw std::runtime_error("invalid magic number.");
		}	

		stream->read((char*)&version, sizeof(float));
		if (version != 2.0f && version != 2.1f)
		{
			throw std::runtime_error("this is not ver2.0 or ver2.1");
		}

		this->setting.Read(stream);

		this->model_name = std::move(ReadString(stream, setting.encoding));
		this->model_english_name = std::move(ReadString(stream, setting.encoding));
		this->model_comment = std::move(ReadString(stream, setting.encoding));
		this->model_english_comment = std::move(ReadString(stream, setting.encoding));

		stream->read((char*)&vertex_count, sizeof(int));
		this->vertices = std::make_unique<PmxVertex[]>(vertex_count);
		for (int i = 0; i < vertex_count; i++)
		{
			vertices[i].Read(stream, &setting);
		}
	
		stream->read((char*)&index_count, sizeof(int));
		this->indices = std::make_unique<int[]>(index_count);
		for (int i = 0; i < index_count; i++)
		{
			this->indices[i] = ReadIndex(stream, setting.vertex_index_size);
		}
	
		stream->read((char*)&texture_count, sizeof(int));
		this->textures = std::make_unique<utfstring[]>(texture_count);
		for (int i = 0; i < texture_count; i++)
		{
			this->textures[i] = ReadString(stream, setting.encoding);
		}
	
		stream->read((char*)&material_count, sizeof(int));
		this->materials = std::make_unique<PmxMaterial[]>(material_count);
		for (int i = 0; i < material_count; i++)
		{
			this->materials[i].Read(stream, &setting);
		}

		stream->read((char*)&this->bone_count, sizeof(int));
		this->bones = std::make_unique<PmxBone[]>(this->bone_count);
		for (int i = 0; i < this->bone_count; i++)
		{
			this->bones[i].Read(stream, &setting);
		}

		stream->read((char*)&this->morph_count, sizeof(int));
		this->morphs = std::make_unique<PmxMorph[]>(this->morph_count);
		for (int i = 0; i < this->morph_count; i++)
		{
			this->morphs[i].Read(stream, &setting);
		}

		stream->read((char*)&this->frame_count, sizeof(int));
		this->frames = std::make_unique<PmxFrame[]>(this->frame_count);
		for (int i = 0; i < this->frame_count; i++)
		{
			this->frames[i].Read(stream, &setting);
		}

		stream->read((char*)&this->rigid_body_count, sizeof(int));
		this->rigid_bodies = std::make_unique<PmxRigidBody[]>(this->rigid_body_count);
		for (int i = 0; i < this->rigid_body_count; i++)
		{
			this->rigid_bodies[i].Read(stream, &setting);
		}

		stream->read((char*)&this->joint_count, sizeof(int));
		this->joints = std::make_unique<PmxJoint[]>(this->joint_count);
		for (int i = 0; i < this->joint_count; i++)
		{
			this->joints[i].Read(stream, &setting);
		}

		//if (this->version == 2.1f)
		//{
		//	stream->read((char*) &this->soft_body_count, sizeof(int));
		//	this->soft_bodies = std::make_unique<PmxSoftBody []>(this->soft_body_count);
		//	for (int i = 0; i < this->soft_body_count; i++)
		//	{
		//		this->soft_bodies[i].Read(stream, &setting);
		//	}
		//}
	}

	//std::unique_ptr<PmxLoader> ReadFromFile(const char *filename)
	//{
	//	auto stream = std::ifstream(filename, std::ios_base::binary);
	//	auto pmx = PmxLoader::ReadFromStream(&stream);
	//	if (!stream.eof())
	//	{
	//		std::cerr << "don't reach the end of file." << std::endl;
	//	}
	//	stream.close();
	//	return pmx;
	//}

	//std::unique_ptr<PmxLoader> ReadFromStream(std::istream *stream)
	//{
	//	auto pmx = std::make_unique<PmxLoader>();
	//	pmx->Read(stream);
	//	return pmx;
	//}
}

namespace fbx
{
	static std::unordered_map<std::string, std::vector<std::string>> transSkeletonPairs;

	std::string mStaticName;
	std::string mStaticNumber;

	FbxArray<FbxString*> animNameLists;
	FbxAMatrix pRootGlobalPosition;
	std::vector<FbxAMatrix> pParentGlobalPosition;
	FbxAnimLayer* currAnimLayer;
	std::vector<FbxTime> mStart, mStop;
	FbxTime mCurrentTime;

	std::vector<FbxNode*> meshNodes;
	std::vector<std::pair<std::string, std::string>> parentSkeletonNames;
	std::vector<FbxAMatrix> meshParentGlobalPositions;

	ObjectData* FBXLoader::CreateFBXModel(
		ObjectData* newData,
		std::string Name,
		std::string Path
	)
	{
		if (newData == nullptr)
			throw std::runtime_error("newData is NULL");

		newData->objType = ObjectType::FBX_TYPE;

		newData->objectName = Name;

		MemoryPool::getInstance().ObjectNames.push_back(Name);

		// Fbx Kit
		FbxManager* manager = NULL;
		FbxScene* scene = NULL;
		FbxIOSettings* ios = NULL;
		FbxNode* node = NULL;

		manager = FbxManager::Create();
		scene = FbxScene::Create(manager, "scene");

		ios = FbxIOSettings::Create(manager, "");
		manager->SetIOSettings(ios);

		FbxImporter* importer = FbxImporter::Create(manager, "");

		bool loadRes = false;
		loadRes = importer->Initialize(Path.c_str(), -1, manager->GetIOSettings());

		if (!loadRes)
			throw std::runtime_error("FBX 파일을 찾지 못하였습니다.");

		importer->Import(scene);
		importer->Destroy();
		ios->Destroy();

		node = scene->GetRootNode();

		DrawNodeRecursive(node);

		MeshData meshData;

		for (int i = 0; i < meshNodes.size(); i++)
		{
			int res = DrawMesh(
				meshNodes[i],
				meshData,
				true
			);

			if (!res)
			{
				meshData.mName = std::string(node->GetName());
				newData->meshDatas.push_back(meshData);
			}
		}

		// Set Submesh Info
		newData->SubmeshCount = newData->meshDatas.size();

		newData->VertexBuffer.resize(newData->SubmeshCount);
		newData->IndexBuffer.resize(newData->SubmeshCount);

		meshNodes.clear();
		meshNodes.resize(0);

		// Set BoundBoxSize
		{
			//
			// Create the vertices.
			//

			Vertex v[24];
			uint32 i[36];

			float w2 = 0.0f;
			float h2 = 0.0f;
			float d2 = 0.0f;

			UINT	vsize;
			Vertex* vdata;

			for (int i = 0; i < newData->SubmeshCount; i++)
			{
				vsize = newData->meshDatas[i].Vertices.size();
				vdata = newData->meshDatas[i].Vertices.data();

				for (int vidx = 0; vidx < vsize; vidx++)
				{
					w2 = w2 < abs(vdata[vidx].Position[0]) ? abs(vdata[vidx].Position[0]) : w2;
					h2 = h2 < abs(vdata[vidx].Position[1]) ? abs(vdata[vidx].Position[1]) : h2;
					d2 = d2 < abs(vdata[vidx].Position[2]) ? abs(vdata[vidx].Position[2]) : d2;
				}
			}

			// Fill in the front face vertex data.
			v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			v[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			v[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
			v[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

			// Fill in the back face vertex data.
			v[4] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
			v[5] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			v[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			v[7] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

			// Fill in the top face vertex data.
			v[8] = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			v[9] = Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			v[10] = Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
			v[11] = Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

			// Fill in the bottom face vertex data.
			v[12] = Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
			v[13] = Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			v[14] = Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			v[15] = Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

			// Fill in the left face vertex data.
			v[16] = Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
			v[17] = Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
			v[18] = Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
			v[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

			// Fill in the right face vertex data.
			v[20] = Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
			v[21] = Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
			v[22] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
			v[23] = Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

			newData->meshBoundBoxData.Vertices.assign(&v[0], &v[24]);

			//
			// Create the indices.
			//

			// Fill in the front face index data
			i[0] = 0; i[1] = 1; i[2] = 2;
			i[3] = 0; i[4] = 2; i[5] = 3;

			// Fill in the back face index data
			i[6] = 4; i[7] = 5; i[8] = 6;
			i[9] = 4; i[10] = 6; i[11] = 7;

			// Fill in the top face index data
			i[12] = 8; i[13] = 9; i[14] = 10;
			i[15] = 8; i[16] = 10; i[17] = 11;

			// Fill in the bottom face index data
			i[18] = 12; i[19] = 13; i[20] = 14;
			i[21] = 12; i[22] = 14; i[23] = 15;

			// Fill in the left face index data
			i[24] = 16; i[25] = 17; i[26] = 18;
			i[27] = 16; i[28] = 18; i[29] = 19;

			// Fill in the right face index data
			i[30] = 20; i[31] = 21; i[32] = 22;
			i[33] = 20; i[34] = 22; i[35] = 23;

			newData->meshBoundBoxData.Indices32.assign(&i[0], &i[36]);

		}

		MemoryPool::getInstance().AddObject(Name, newData);

		return newData;
	}

	ObjectData* FBXLoader::LoadFBXModel(
		ObjectData* newData,
		std::string Name,
		std::string Path
	)
	{
		if (newData == nullptr)
			throw std::runtime_error("newData is NULL");

		newData->objType = ObjectType::FBX_TYPE;

		newData->objectName = Name;

		MemoryPool::getInstance().ObjectNames.push_back(Name);

		// Fbx Kit
		FbxManager* manager = NULL;
		FbxScene* scene = NULL;
		FbxIOSettings* ios = NULL;
		FbxNode* node = NULL;

		manager = FbxManager::Create();
		scene = FbxScene::Create(manager, "scene");

		ios = FbxIOSettings::Create(manager, "");
		manager->SetIOSettings(ios);

		FbxImporter* importer = FbxImporter::Create(manager, "");

		bool loadRes = false;
		loadRes = importer->Initialize(Path.c_str(), -1, manager->GetIOSettings());

		if (!loadRes)
			throw std::runtime_error("FBX 파일을 찾지 못하였습니다.");

		importer->Import(scene);
		importer->Destroy();
		ios->Destroy();

		node = scene->GetRootNode();

		DrawNodeRecursive(node);

		MeshData meshData;

		for (int i = 0; i < meshNodes.size(); i++)
		{
			int res = DrawMesh(
				meshNodes[i],
				meshData,
				true
			);

			if (!res)
			{
				meshData.mName = std::string(node->GetName());
				newData->meshDatas.push_back(meshData);
			}
		}

		// Set Submesh Info
		newData->SubmeshCount = newData->meshDatas.size();

		newData->VertexBuffer.resize(newData->SubmeshCount);
		newData->IndexBuffer.resize(newData->SubmeshCount);

		meshNodes.clear();
		meshNodes.resize(0);

		// Set BoundBoxSize
		{
			//
			// Create the vertices.
			//

			Vertex v[24];
			uint32 i[36];

			float w2 = 0.0f;
			float h2 = 0.0f;
			float d2 = 0.0f;

			UINT	vsize;
			Vertex* vdata;

			for (int i = 0; i < newData->SubmeshCount; i++)
			{
				vsize = newData->meshDatas[i].Vertices.size();
				vdata = newData->meshDatas[i].Vertices.data();

				for (int vidx = 0; vidx < vsize; vidx++)
				{
					w2 = w2 < abs(vdata[vidx].Position[0]) ? abs(vdata[vidx].Position[0]) : w2;
					h2 = h2 < abs(vdata[vidx].Position[1]) ? abs(vdata[vidx].Position[1]) : h2;
					d2 = d2 < abs(vdata[vidx].Position[2]) ? abs(vdata[vidx].Position[2]) : d2;
				}
			}

			// Fill in the front face vertex data.
			v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			v[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			v[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
			v[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

			// Fill in the back face vertex data.
			v[4] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
			v[5] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			v[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			v[7] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

			// Fill in the top face vertex data.
			v[8] = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			v[9] = Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			v[10] = Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
			v[11] = Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

			// Fill in the bottom face vertex data.
			v[12] = Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
			v[13] = Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			v[14] = Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			v[15] = Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

			// Fill in the left face vertex data.
			v[16] = Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
			v[17] = Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
			v[18] = Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
			v[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

			// Fill in the right face vertex data.
			v[20] = Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
			v[21] = Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
			v[22] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
			v[23] = Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

			newData->meshBoundBoxData.Vertices.assign(&v[0], &v[24]);

			//
			// Create the indices.
			//

			// Fill in the front face index data
			i[0] = 0; i[1] = 1; i[2] = 2;
			i[3] = 0; i[4] = 2; i[5] = 3;

			// Fill in the back face index data
			i[6] = 4; i[7] = 5; i[8] = 6;
			i[9] = 4; i[10] = 6; i[11] = 7;

			// Fill in the top face index data
			i[12] = 8; i[13] = 9; i[14] = 10;
			i[15] = 8; i[16] = 10; i[17] = 11;

			// Fill in the bottom face index data
			i[18] = 12; i[19] = 13; i[20] = 14;
			i[21] = 12; i[22] = 14; i[23] = 15;

			// Fill in the left face index data
			i[24] = 16; i[25] = 17; i[26] = 18;
			i[27] = 16; i[28] = 18; i[29] = 19;

			// Fill in the right face index data
			i[30] = 20; i[31] = 21; i[32] = 22;
			i[33] = 20; i[34] = 22; i[35] = 23;

			newData->meshBoundBoxData.Indices32.assign(&i[0], &i[36]);

		}

		return newData;
	}

	ObjectData* FBXLoader::CreateFBXSkinnedModel(
		ObjectData* newData,
		std::string Name,
		std::string Path
	)
	{
		if (newData == nullptr)
			throw std::runtime_error("newData is NULL");

		newData->objType = ObjectType::FBX_SKINNED_TYPE;

		newData->objectName = Name;

		MemoryPool::getInstance().ObjectNames.push_back(Name);

		// Fbx Kit
		FbxIOSettings* ios = NULL;
		FbxNode* node = NULL;

		// Animation Kit
		FbxArray<FbxPose*> pPoseArray;
		FbxTime time;

		FbxAnimStack* currAnimStack = NULL;
		FbxAnimLayer* currAnimLayer = NULL;

		FbxTakeInfo* currTakeInfo = NULL;

		int size = sizeof(fbxsdk::FbxVector4);

		fbxsdk::FbxManager* manager = FbxManager::Create();
		fbxsdk::FbxScene* scene = FbxScene::Create(manager, "scene");

		ios = FbxIOSettings::Create(manager, "");
		manager->SetIOSettings(ios);

		FbxImporter* importer = FbxImporter::Create(manager, "");

		bool loadRes = false;
		loadRes = importer->Initialize(Path.c_str(), -1, manager->GetIOSettings());

		if (!loadRes)
			throw std::runtime_error("Path를 찾을 수 없습니다.");

		importer->Import(scene);

		importer->Destroy();
		ios->Destroy();

		node = scene->GetRootNode();

		// LoadFile
		scene->FillAnimStackNameArray(animNameLists);

		const int poseCount = scene->GetPoseCount();
		for (int i = 0; i < poseCount; i++)
			pPoseArray.Add(scene->GetPose(i));

		time.SetTime(0, 0, 0, 1, 0, scene->GetGlobalSettings().GetTimeMode());

		if (animNameLists.Size() > 0)
			currAnimStack = scene->FindMember<FbxAnimStack>(animNameLists[0]->Buffer());

		if (currAnimStack == NULL)
			return nullptr;

		currAnimLayer = currAnimStack->GetMember<FbxAnimLayer>();

		scene->SetCurrentAnimationStack(currAnimStack);

		// Get Anim Time
		FbxTime mStart, mStop;
		newData->Sequences.push_back(new Sequence);

		for (int i = 0; i < animNameLists.Size(); i++)
		{
			currTakeInfo = scene->GetTakeInfo(*(animNameLists[i]));
			if (currTakeInfo)
			{
				mStart = currTakeInfo->mLocalTimeSpan.GetStart();
				mStop = currTakeInfo->mLocalTimeSpan.GetStop();
			}
			else
			{
				continue;
			}

			newData->Sequences[0]->mType = AnimationAsset::AnimationAssetType::SEQUENCE;
			newData->Sequences[0]->mName = "Sequence" + std::to_string(i);

			newData->Sequences[0]->mStarts.push_back(mStart);
			newData->Sequences[0]->mStops.push_back(mStop);

			newData->Sequences[0]->mFrameLength =
				newData->Sequences[0]->mStops[i].GetFrameCount();
		}

		long long halfFrame = newData->Sequences[0]->mStops[0].GetFrameCount();

		FbxTime pTime;

		DrawNodeRecursive(
			node, 
			pTime, 
			pPoseArray[0], 
			&newData->BoneHierarchy
		);

		newData->BoneCount = newData->BoneHierarchy.GetObjectCount();

		FbxAMatrix geomOffset;
		FbxVector4 IT, IR, IS;

		// 애니메이션 정보 데이터 공간을 서브메쉬 개수로 할당
		newData->Sequences[0]->mAnimVertexArrays = new float*** [meshNodes.size()];
		newData->Sequences[0]->mAnimVertexSizes = new fbxsdk::FbxUInt[meshNodes.size()];

		//for (threadIDX = 0; threadIDX < kMaxThreadCount; threadIDX++)
		//{
		//	hLoadAnimReadEvent[threadIDX] = CreateEvent(nullptr, false, false, nullptr);
		//	hLoadAnimWriteEvent[threadIDX] = CreateEvent(nullptr, false, false, nullptr);

		//	hLoadAnimThread[threadIDX] = CreateThread(
		//		NULL,
		//		0,
		//		LoadAnimationThread,
		//		(LPVOID*)threadIDX,
		//		0,
		//		&hLoadAnimThreadID[threadIDX]
		//	);
		//}

		// 최종적으로 DrawMesh를 병렬처리 할 수 있도록 구현
		for (int i = 0; i < meshNodes.size(); i++)
		{
			IT = meshNodes[i]->GetGeometricTranslation(FbxNode::eSourcePivot);
			IR = meshNodes[i]->GetGeometricRotation(FbxNode::eSourcePivot);
			IS = meshNodes[i]->GetGeometricScaling(FbxNode::eSourcePivot);

			geomOffset = FbxAMatrix(IT, IR, IS);

			newData->pGlobalPosition = meshParentGlobalPositions[i] * geomOffset;

			DrawMesh(
				i,
				meshNodes[i],
				newData
			);
		}

		// Set Submesh Info
		newData->SubmeshCount = newData->meshDatas.size();

		newData->VertexBuffer.resize(newData->SubmeshCount);
		newData->IndexBuffer.resize(newData->SubmeshCount);

		meshNodes.clear();
		meshNodes.resize(0);

		meshParentGlobalPositions.clear();
		meshParentGlobalPositions.resize(0);

		// Set BoundBoxSize
		{
			//
			// Create the vertices.
			//

			Vertex v[24];
			uint32 i[36];

			float w2 = 0.0f;
			float h2 = 0.0f;
			float d2 = 0.0f;

			UINT	vsize;
			Vertex*	vdata;

			for (int i = 0; i < newData->SubmeshCount; i++)
			{
				vsize = newData->meshDatas[i].Vertices.size();
				vdata = newData->meshDatas[i].Vertices.data();

				for (int vidx = 0; vidx < vsize; vidx++)
				{
					w2 = w2 < abs(vdata[vidx].Position[0]) ? abs(vdata[vidx].Position[0]) : w2;
					h2 = h2 < abs(vdata[vidx].Position[1]) ? abs(vdata[vidx].Position[1]) : h2;
					d2 = d2 < abs(vdata[vidx].Position[2]) ? abs(vdata[vidx].Position[2]) : d2;
				}
			}

			w2 *= 0.05f;
			h2 *= 0.05f;
			d2 *= 0.05f;

			newData->collider.Center	= {0.0f, 0.0f, 0.0f};
			newData->collider.Extents	= { w2, h2, d2 };

			// Fill in the front face vertex data.
			v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			v[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			v[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
			v[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

			// Fill in the back face vertex data.
			v[4] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
			v[5] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			v[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			v[7] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

			// Fill in the top face vertex data.
			v[8] = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			v[9] = Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			v[10] = Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
			v[11] = Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

			// Fill in the bottom face vertex data.
			v[12] = Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
			v[13] = Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			v[14] = Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			v[15] = Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

			// Fill in the left face vertex data.
			v[16] = Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
			v[17] = Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
			v[18] = Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
			v[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

			// Fill in the right face vertex data.
			v[20] = Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
			v[21] = Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
			v[22] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
			v[23] = Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

			newData->meshBoundBoxData.Vertices.assign(&v[0], &v[24]);

			//
			// Create the indices.
			//

			// Fill in the front face index data
			i[0] = 0; i[1] = 1; i[2] = 2;
			i[3] = 0; i[4] = 2; i[5] = 3;

			// Fill in the back face index data
			i[6] = 4; i[7] = 5; i[8] = 6;
			i[9] = 4; i[10] = 6; i[11] = 7;

			// Fill in the top face index data
			i[12] = 8; i[13] = 9; i[14] = 10;
			i[15] = 8; i[16] = 10; i[17] = 11;

			// Fill in the bottom face index data
			i[18] = 12; i[19] = 13; i[20] = 14;
			i[21] = 12; i[22] = 14; i[23] = 15;

			// Fill in the left face index data
			i[24] = 16; i[25] = 17; i[26] = 18;
			i[27] = 16; i[28] = 18; i[29] = 19;

			// Fill in the right face index data
			i[30] = 20; i[31] = 21; i[32] = 22;
			i[33] = 20; i[34] = 22; i[35] = 23;

			newData->meshBoundBoxData.Indices32.assign(&i[0], &i[36]);

		}

		newData->SetAsset((AnimationAsset*)newData->Sequences[0]);
		MemoryPool::getInstance().AddObject(Name, newData);

		return newData;
	}

	ObjectData* FBXLoader::LoadFBXSkinnedModel(
		std::string Name,
		std::string Path
	)
	{
		ObjectData* newData = new ObjectData;
		newData->Sequences.push_back(new Sequence);

		newData->objType = ObjectType::FBX_SKINNED_TYPE;
		newData->objectName = Name;

		// Fbx Kit
		FbxIOSettings* ios = NULL;
		FbxNode* node = NULL;

		// Animation Kit
		FbxArray<FbxPose*> pPoseArray;
		FbxTime time;

		FbxAnimStack* currAnimStack = NULL;
		FbxAnimLayer* currAnimLayer = NULL;

		FbxTakeInfo* currTakeInfo = NULL;

		int size = sizeof(fbxsdk::FbxVector4);

		fbxsdk::FbxManager* manager = FbxManager::Create();
		fbxsdk::FbxScene* scene = FbxScene::Create(manager, "scene");

		ios = FbxIOSettings::Create(manager, "");
		manager->SetIOSettings(ios);

		FbxImporter* importer = FbxImporter::Create(manager, "");

		bool loadRes = false;
		loadRes = importer->Initialize(Path.c_str(), -1, manager->GetIOSettings());

		if (!loadRes)
			throw std::runtime_error("Path를 찾을 수 없습니다.");

		importer->Import(scene);

		importer->Destroy();
		ios->Destroy();

		node = scene->GetRootNode();

		// LoadFile
		scene->FillAnimStackNameArray(animNameLists);

		const int poseCount = scene->GetPoseCount();
		for (int i = 0; i < poseCount; i++)
			pPoseArray.Add(scene->GetPose(i));

		time.SetTime(0, 0, 0, 1, 0, scene->GetGlobalSettings().GetTimeMode());

		if (animNameLists.Size() > 0)
			currAnimStack = scene->FindMember<FbxAnimStack>(animNameLists[0]->Buffer());

		if (currAnimStack == NULL)
			return nullptr;

		currAnimLayer = currAnimStack->GetMember<FbxAnimLayer>();

		scene->SetCurrentAnimationStack(currAnimStack);

		// Get Anim Time
		FbxTime mStart, mStop;

		for (int i = 0; i < animNameLists.Size(); i++)
		{
			currTakeInfo = scene->GetTakeInfo(*(animNameLists[i]));
			if (currTakeInfo)
			{
				mStart = currTakeInfo->mLocalTimeSpan.GetStart();
				mStop = currTakeInfo->mLocalTimeSpan.GetStop();
			}
			else
			{
				continue;
			}

			newData->Sequences[0]->mName = "Sequence" + std::to_string(i);

			newData->Sequences[0]->mStarts.push_back(mStart);
			newData->Sequences[0]->mStops.push_back(mStop);

			//break;
		}

		newData->Sequences[0]->mFrameLength =
			newData->Sequences[0]->mStops[0].GetFrameCount();

		long long halfFrame = newData->Sequences[0]->mStops[0].GetFrameCount();

		FbxTime pTime;

		DrawNodeRecursive(
			node,
			pTime,
			pPoseArray[0],
			&newData->BoneHierarchy
		);

		newData->BoneCount = newData->BoneHierarchy.GetObjectCount();

		FbxAMatrix geomOffset;
		FbxVector4 IT, IR, IS;

		// 애니메이션 정보 데이터 공간을 서브메쉬 개수로 할당
		newData->Sequences[0]->mAnimVertexArrays = new float*** [meshNodes.size()];
		newData->Sequences[0]->mAnimVertexSizes = new fbxsdk::FbxUInt[meshNodes.size()];

		// 최종적으로 DrawMesh를 병렬처리 할 수 있도록 구현
		for (int i = 0; i < meshNodes.size(); i++)
		{
			IT = meshNodes[i]->GetGeometricTranslation(FbxNode::eSourcePivot);
			IR = meshNodes[i]->GetGeometricRotation(FbxNode::eSourcePivot);
			IS = meshNodes[i]->GetGeometricScaling(FbxNode::eSourcePivot);

			geomOffset = FbxAMatrix(IT, IR, IS);

			newData->pGlobalPosition = meshParentGlobalPositions[i] * geomOffset;

			DrawMesh(
				i,
				meshNodes[i],
				newData
			);
		}

		// Set Submesh Info
		newData->SubmeshCount = newData->meshDatas.size();

		newData->VertexBuffer.resize(newData->SubmeshCount);
		newData->IndexBuffer.resize(newData->SubmeshCount);

		newData->SetAsset((AnimationAsset*)newData->Sequences[0]);

		meshNodes.clear();
		meshNodes.resize(0);

		meshParentGlobalPositions.clear();
		meshParentGlobalPositions.resize(0);

		return newData;
	}

	// Extract Animation Bone
	int FBXLoader::ExtractedAnimationBone(
		std::string Path,
		std::string targetPath,
		FbxArray<FbxString*>& animNameLists,
		std::vector<FbxTime>& mStarts,
		std::vector<FbxTime>& mStops,
		std::vector<long long>& countOfFrame
	)
	{
		// Init Bone List
		std::vector<std::string> mBoneStack;

		transSkeletonPairs[std::string("Center")].push_back(std::string("Center"));
		transSkeletonPairs[std::string("UpperBody")].push_back(std::string("LowerBody"));
		transSkeletonPairs[std::string("UpperBody")].push_back(std::string("groove"));
		transSkeletonPairs[std::string("UpperBody")].push_back(std::string("waist"));

		transSkeletonPairs[std::string("UpperBody")].push_back(std::string("UpperBody"));
		transSkeletonPairs[std::string("UpperBody2")].push_back(std::string("UpperBody2"));
		transSkeletonPairs[std::string("Breast.L")].push_back(std::string("Breast.L"));
		transSkeletonPairs[std::string("Breast.R")].push_back(std::string("Breast.R"));
		transSkeletonPairs[std::string("Neck")].push_back(std::string("Neck"));
		transSkeletonPairs[std::string("Head")].push_back(std::string("Head"));

		transSkeletonPairs[std::string("LegD.L")].push_back(std::string("LegD.L"));
		//transSkeletonPairs[std::string("LegD.L")].push_back(std::string("Leg.L"));
		transSkeletonPairs[std::string("KneeD.L")].push_back(std::string("KneeD.L"));
		//transSkeletonPairs[std::string("KneeD.L")].push_back(std::string("Knee.L"));
		//transSkeletonPairs[std::string("FootD.L")].push_back(std::string("FootD.L"));
		//transSkeletonPairs[std::string("FootD.L")].push_back(std::string("Foot.L"));
		//transSkeletonPairs[std::string("FootD.L")].push_back(std::string("FootIK.L"));
		//transSkeletonPairs[std::string("FootD.L")].push_back(std::string("leg IKP_L"));
		//transSkeletonPairs[std::string("FootD.L")].push_back(std::string("toe.L"));
		//transSkeletonPairs[std::string("FootD.L")].push_back(std::string("toeIK.L"));

		transSkeletonPairs[std::string("LegD.R")].push_back(std::string("LegD.R"));
		//transSkeletonPairs[std::string("LegD.R")].push_back(std::string("Leg.R"));
		transSkeletonPairs[std::string("KneeD.R")].push_back(std::string("KneeD.R"));
		//transSkeletonPairs[std::string("KneeD.R")].push_back(std::string("Knee.R"));
		//transSkeletonPairs[std::string("FootD.R")].push_back(std::string("FootD.R"));
		//transSkeletonPairs[std::string("FootD.R")].push_back(std::string("Foot.R"));
		//transSkeletonPairs[std::string("FootD.R")].push_back(std::string("FootIK.R"));
		//transSkeletonPairs[std::string("FootD.R")].push_back(std::string("leg IKP_R"));
		//transSkeletonPairs[std::string("FootD.R")].push_back(std::string("toe.R"));
		//transSkeletonPairs[std::string("FootD.R")].push_back(std::string("toeIK.R"));

		//transSkeletonPairs[std::string("Neck")].push_back(std::string("shoulderP.L"));
		transSkeletonPairs[std::string("Shoulder.L")].push_back(std::string("Shoulder.L"));
		transSkeletonPairs[std::string("Bip02 L UpperArm")].push_back(std::string("Arm.L"));
		transSkeletonPairs[std::string("Bip02 L UpperArm")].push_back(std::string("Bip02 L UpperArm"));
		//transSkeletonPairs[std::string("Bip02 LUpArmTwist")].push_back(std::string("arm twist_L"));
		transSkeletonPairs[std::string("Bip02 L Forearm")].push_back(std::string("Elbow.L"));
		//transSkeletonPairs[std::string("Bip02 L Forearm")].push_back(std::string("+Elbow.L"));
		//transSkeletonPairs[std::string("Bip02 L ForeTwist")].push_back(std::string("wrist twist_L"));
		transSkeletonPairs[std::string("Elbow.L")].push_back(std::string("Wrist.L"));
		//transSkeletonPairs[std::string("Elbow.L")].push_back(std::string("wrist twist_L"));

		//transSkeletonPairs[std::string("Neck")].push_back(std::string("shoulderP.R"));
		transSkeletonPairs[std::string("Shoulder.R")].push_back(std::string("Shoulder.R"));
		transSkeletonPairs[std::string("Bip02 R UpperArm")].push_back(std::string("Arm.R"));
		transSkeletonPairs[std::string("Bip02 R UpperArm")].push_back(std::string("Bip02 R UpperArm"));
		//transSkeletonPairs[std::string("Bip02 RUpArmTwist")].push_back(std::string("arm twist_R"));
		transSkeletonPairs[std::string("Elbow.R")].push_back(std::string("Elbow.R"));
		//transSkeletonPairs[std::string("Bip02 R Forearm")].push_back(std::string("+Elbow.R"));
		//transSkeletonPairs[std::string("Bip02 R ForeTwist")].push_back(std::string("wrist twist_R"));
		transSkeletonPairs[std::string("Elbow.R")].push_back(std::string("Wrist.R"));
		//transSkeletonPairs[std::string("Elbow.R")].push_back(std::string("wrist twist_R"));
		//transSkeletonPairs[std::string("Bip02 Rhand_Weapon")].push_back(std::string("Wrist.R"));


		// Fbx Kit
		FbxIOSettings* ios = NULL;
		FbxNode* node = NULL;

		// Animation Kit
		FbxArray<FbxPose*> pPoseArray;
		FbxTime time;

		FbxAnimStack* currAnimStack = NULL;

		FbxTakeInfo* currTakeInfo = NULL;

		int size = sizeof(fbxsdk::FbxVector4);


		fbxsdk::FbxManager* manager = FbxManager::Create();
		fbxsdk::FbxScene* scene = FbxScene::Create(manager, "scene");

		ios = FbxIOSettings::Create(manager, "");
		manager->SetIOSettings(ios);

		FbxImporter* importer = FbxImporter::Create(manager, "");

		bool loadRes = false;
		loadRes = importer->Initialize(Path.c_str(), -1, manager->GetIOSettings());

		if (!loadRes)
			throw std::runtime_error("Path를 찾지 못하였습니다.");

		importer->Import(scene);

		importer->Destroy();
		ios->Destroy();

		node = scene->GetRootNode();

		// Pmx Kit

		pmx::PmxLoader target;
		target.Init();

		std::string _FullFilePath = targetPath;
		std::ifstream stream(_FullFilePath.c_str(), std::ifstream::binary);
		target.Read(&stream);

		{
			// LoadFile
			scene->FillAnimStackNameArray(animNameLists);

			const int poseCount = scene->GetPoseCount();
			for (int i = 0; i < poseCount; i++)
				pPoseArray.Add(scene->GetPose(i));

			time.SetTime(0, 0, 0, 1, 0, scene->GetGlobalSettings().GetTimeMode());

			// Curr Anim Stack
			FbxAnimStack* currAnimStack = NULL;
			FbxAnimLayer* currAnimLayer = NULL;

			if (animNameLists.Size() > 0)
				currAnimStack = scene->FindMember<FbxAnimStack>(animNameLists[0]->Buffer());

			if (currAnimStack == NULL)
				return 1;

			currAnimLayer = currAnimStack->GetMember<FbxAnimLayer>();

			scene->SetCurrentAnimationStack(currAnimStack);

			// Get Anim Time
			FbxTime mStart, mStop;

			for (int i = 0; i < animNameLists.Size(); i++) {
				currTakeInfo = scene->GetTakeInfo(*(animNameLists[i]));
				if (currTakeInfo)
				{
					mStart = currTakeInfo->mLocalTimeSpan.GetStart();
					mStop = currTakeInfo->mLocalTimeSpan.GetStop();
				}
				else
				{
					continue;
				}

				mStarts.push_back(mStart);
				mStops.push_back(mStop);

				countOfFrame.push_back(mStops[i].GetFrameCount());
			}
			long long halfFrame = mStops[0].GetFrameCount();
			long long perFrame = mStops[0].Get() / mStops[0].GetFrameCount(mStops[0].eFrames30);

			std::ifstream checkExistsHello(std::string("Hello").c_str());
			if (checkExistsHello.good())
				std::remove("Hello");

			std::ofstream outFile(std::string("Hello"), std::ios::out | std::ios::binary);
			if (!outFile.is_open())
				throw std::runtime_error("");

			// GetRootTransform
			pRootGlobalPosition = GetGlobalPosition(node->GetChild(0), 0);

			DrawBoneRecursive
			(
				node,
				mStops,
				mCurrentTime,
				pRootGlobalPosition,
				pParentGlobalPosition,
				NULL,
				perFrame,
				outFile
			);

			const int FinSize = 1;

			outFile.write((const char*)&FinSize, sizeof(int));
			outFile.write("FIN\n", 4);
			outFile.close();
		}

		{
			std::ifstream inFile(std::string("Hello"), std::ios::in | std::ios::binary);
			if (!inFile.is_open())
				throw std::runtime_error("");

			long long mAnimFrameCount = mStops[0].GetFrameCount(fbxsdk::FbxTime::eFrames30);

			std::vector<std::vector<std::array<float4, 2>>> bones(target.bone_count);

			int targetCount = 0;
			int nameIDX = 0;
			char Name[10][30];
			char Buffer;

			int i = 0;

			std::vector<int> targetBoneIDX;

			std::string findBoneName;

			int bIDX = 0;
			int scaling = 0;

			// 카피하고자 하는 본만 추출하여 해당 본에만 카피
			while (true)
			{
				inFile.read((char*)&targetCount, sizeof(int));

				for (i = 0; i < targetCount; i++)
				{
					nameIDX = 0;

					do {
						if (nameIDX > 30)
							throw std::runtime_error("Failed to Read");

						inFile.read(&Buffer, 1);
						Name[i][nameIDX++] = Buffer;
					} while (Buffer != '\n');
				}

				if (
					Name[0][0] == 'F' &&
					Name[0][1] == 'I' &&
					Name[0][2] == 'N' &&
					Name[0][3] == '\n'
					) break;

				for (i = 0; i < targetCount; i++)
				{
					std::string findTargetBoneName(Name[i], nameIDX - 1);

					// find bone by name
					for (i = 0; i < target.bone_count; i++)
					{
						findBoneName.clear();

						findBoneName.assign(
							target.bones[i].bone_english_name.begin(),
							target.bones[i].bone_english_name.end()
						);

						if (findTargetBoneName == findBoneName)
						{
							targetBoneIDX.push_back(i);

							bones[i].resize(mAnimFrameCount);
						}
					}
				} // for (int i = 0; i < targetCount; i++)

				std::vector<float4> _CopyData(mAnimFrameCount * 2);
				inFile.read(
					(char*)_CopyData.data(),
					// 1 raw => Pos, Quat (* 2)
					(sizeof(float4) * 2) * mAnimFrameCount
				);

				if (targetBoneIDX.size() > 0)
				{
					i = targetBoneIDX[targetBoneIDX.size() - 1];

					for (scaling = 0; scaling < bones[i].size(); scaling++) {
						bones[i][scaling][0].x = _CopyData[scaling * 2].x;
						bones[i][scaling][0].y = _CopyData[scaling * 2].y;
						bones[i][scaling][0].z = _CopyData[scaling * 2].z;
						bones[i][scaling][0].w = 1.0f;

						bones[i][scaling][1].x = _CopyData[scaling * 2 + 1].x;
						bones[i][scaling][1].y = _CopyData[scaling * 2 + 1].y;
						bones[i][scaling][1].z = _CopyData[scaling * 2 + 1].z;
						bones[i][scaling][1].w = _CopyData[scaling * 2 + 1].w;
					}
				}
			}

			inFile.close();

			int rootIDX = 0;
			bool isUpdate = false;

			// Root index 설정
			// 카피 된 본 중 가장 조상인 본을 찾아 루트본으로 한다.
			for (int i = 0; i < bones.size(); i++)
			{
				if (bones[i].size() > 2)
				{
					rootIDX = i;
					break;
				}
			}

			// 우선 루트 본을 가져온다 (Frame = 0)
			bones[0].resize(mAnimFrameCount);
			for (int anim = 0; anim < mAnimFrameCount; anim++)
			{
				bones[0][anim][0].x =
					bones[rootIDX][anim][0].x;
				bones[0][anim][0].y =
					bones[rootIDX][anim][0].y;
				bones[0][anim][0].z =
					bones[rootIDX][anim][0].z;
				bones[0][anim][0].w = 1.0f;

				bones[0][anim][1].x =
					bones[rootIDX][anim][1].x;
				bones[0][anim][1].y =
					bones[rootIDX][anim][1].y;
				bones[0][anim][1].z =
					bones[rootIDX][anim][1].z;
				bones[0][anim][1].w =
					bones[rootIDX][anim][1].w;
			}

			i = 1;
			int j = 0;
			int parentIDX = 0;
			int anim = 0;
			// Frame 1 부터는 루트 본 -> 애니메이션 본의 델타를 구해서 저장.
			for (i = 1; i < target.bone_count; i++)
			{
				// 만일 현재 뼈대가 업데이트 되지 않은 뼈대라면 
				if (bones[i].size() < 2)
				{
					bones[i].resize(mAnimFrameCount);

					// 부모(바로 이전 뼈대)의 뼈대가 업데이트 되었다면
					if (isUpdate)
					{
						rootIDX = target.bones[i].parent_index;
						parentIDX = target.bones[i].parent_index;

						for (anim = 0; anim < bones[parentIDX].size(); anim++)
						{
							bones[i][anim][0].x =
								bones[parentIDX][anim][0].x;
							bones[i][anim][0].y =
								bones[parentIDX][anim][0].y;
							bones[i][anim][0].z =
								bones[parentIDX][anim][0].z;
							bones[i][anim][0].w = 1.0f;

							bones[i][anim][1].x =
								bones[parentIDX][anim][1].x;
							bones[i][anim][1].y =
								bones[parentIDX][anim][1].y;
							bones[i][anim][1].z =
								bones[parentIDX][anim][1].z;
							bones[i][anim][1].w =
								bones[parentIDX][anim][1].w;
						}
					}
					// 부모 뼈대가 업데이트 상태가 아니라면
					else
					{
						for (anim = 0; anim < mAnimFrameCount; anim++)
						{
							bones[i][anim][0].x =
								bones[rootIDX][anim][0].x;
							bones[i][anim][0].y =
								bones[rootIDX][anim][0].y;
							bones[i][anim][0].z =
								bones[rootIDX][anim][0].z;
							bones[i][anim][0].w = 1.0f;

							bones[i][anim][1].x =
								bones[rootIDX][anim][1].x;
							bones[i][anim][1].y =
								bones[rootIDX][anim][1].y;
							bones[i][anim][1].z =
								bones[rootIDX][anim][1].z;
							bones[i][anim][1].w =
								bones[rootIDX][anim][1].w;
						}
					}
				}
				else {
					isUpdate = true;
				}
			}

			// Transpose Bones
			std::vector<std::vector<std::array<float4, 2>>> transposeBones(mAnimFrameCount);
			for (i = 0; i < mAnimFrameCount; i++)
				transposeBones[i].resize(target.bone_count);

			float testy(0.0f);
			for (i = 0; i < target.bone_count; i++)
			{
				testy = 0.0f;
				//if (target.bones[i].bone_english_name == L"Shoulder.L" ||
				//	target.bones[i].bone_english_name == L"Arm.L")
				//{
				//	testy = -0.5f;
				//}
				//if (target.bones[i].bone_english_name == L"Shoulder.R" ||
				//	target.bones[i].bone_english_name == L"Arm.R")
				//{
				//	testy = 0.5f;
				//}

				for (j = 0; j < mAnimFrameCount; j++)
				{
					// Transpose Mat
					bones[i][j][0].y += testy;

					transposeBones[j][i][0] = bones[i][j][0];
					transposeBones[j][i][1] = bones[i][j][1];
				}
			}

			std::ifstream checkExistsResFile(std::string("resFile").c_str());
			if (checkExistsResFile.good())
				std::remove("resFile");

			std::ofstream outFile(std::string("resFile"), std::ios::out | std::ios::binary);

			outFile.write((char*)&mAnimFrameCount, sizeof(int));

			for (i = 0; i < mAnimFrameCount; i++)
			{
				for (j = 0; j < target.bone_count; j++)
				{
					outFile.write((char*)&transposeBones[i][j][0].x, sizeof(float));
					outFile.write((char*)&transposeBones[i][j][0].y, sizeof(float));
					outFile.write((char*)&transposeBones[i][j][0].z, sizeof(float));
					outFile.write((char*)&transposeBones[i][j][0].w, sizeof(float));

					outFile.write((char*)&transposeBones[i][j][1].x, sizeof(float));
					outFile.write((char*)&transposeBones[i][j][1].y, sizeof(float));
					outFile.write((char*)&transposeBones[i][j][1].z, sizeof(float));
					outFile.write((char*)&transposeBones[i][j][1].w, sizeof(float));
				}
			}

			bones.clear();
			transposeBones.clear();

			outFile.close();
			throw std::runtime_error("WELL DONE!!");
		}

		return 0;
	}

	/////////////////////////////////////////////////////

	// Get the matrix of the given pose
	FbxAMatrix FBXLoader::GetPoseMatrix(
		FbxPose* pPose,
		int pNodeIndex
	)
	{
		FbxAMatrix lPoseMatrix;
		FbxMatrix lMatrix = pPose->GetMatrix(pNodeIndex);

		memcpy((double*)lPoseMatrix, (double*)lMatrix, sizeof(lMatrix.mData));

		return lPoseMatrix;
	}

	FbxAMatrix FBXLoader::GetGlobalPosition(
		FbxNode* pNode,
		const FbxTime& pTime,
		FbxPose* pPose,
		FbxAMatrix* pParentGlobalPosition
	)
	{
		FbxAMatrix lGlobalPosition;
		bool        lPositionFound = false;

		if (pPose)
		{
			int lNodeIndex = pPose->Find(pNode);

			if (lNodeIndex > -1)
			{
				// The bind pose is always a global matrix.
				// If we have a rest pose, we need to check if it is
				// stored in global or local space.
				if (pPose->IsBindPose() || !pPose->IsLocalMatrix(lNodeIndex))
				{
					lGlobalPosition = GetPoseMatrix(pPose, lNodeIndex);
				}
				else
				{
					// We have a local matrix, we need to convert it to
					// a global space matrix.
					FbxAMatrix lParentGlobalPosition;

					if (pParentGlobalPosition)
					{
						lParentGlobalPosition = *pParentGlobalPosition;
					}
					else
					{
						if (pNode->GetParent())
						{
							lParentGlobalPosition = GetGlobalPosition(pNode->GetParent(), pTime, pPose);
						}
					}

					FbxAMatrix lLocalPosition = GetPoseMatrix(pPose, lNodeIndex);
					lGlobalPosition = lParentGlobalPosition * lLocalPosition;
				}

				lPositionFound = true;
			}
		}

		if (!lPositionFound)
		{
			// There is no pose entry for that node, get the current global position instead.

			// Ideally this would use parent global position and local position to compute the global position.
			// Unfortunately the equation 
			//    lGlobalPosition = pParentGlobalPosition * lLocalPosition
			// does not hold when inheritance type is other than "Parent" (RSrs).
			// To compute the parent rotation and scaling is tricky in the RrSs and Rrs cases.
			lGlobalPosition = pNode->EvaluateGlobalTransform(pTime);
		}

		return lGlobalPosition;
	}

	/////////////////////////////////////////////////////

	void FBXLoader::DrawNodeRecursive(FbxNode* pNode)
	{
		if (pNode->GetNodeAttribute())
		{
			if (pNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
			{
				meshNodes.push_back(pNode);
			}
		}

		const int childCount = pNode->GetChildCount();
		for (int childIndex = 0; childIndex < childCount; ++childIndex)
		{
			DrawNodeRecursive(
				pNode->GetChild(childIndex)
			);
		}
	}

	void FBXLoader::DrawNodeRecursive(
		FbxNode* pNode,
		FbxTime& pTime,
		FbxPose* pPose,
		Tree<Skeleton>* pBoneHierarchy
	) {
		FbxAMatrix pParentGlobalPosition;

		FbxAMatrix globalPosition =
			GetGlobalPosition(
				pNode,
				pTime,
				pPose,
				&pParentGlobalPosition
			);

		Tree<Skeleton>* newSkeleton = nullptr;

		int ParentLevel = -1;
		ParentLevel = pBoneHierarchy->Level;

		if (pNode->GetNodeAttribute())
		{
			if (pNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
			{
				meshNodes.push_back(pNode);
				meshParentGlobalPositions.push_back(globalPosition);
			}
			else if (pNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
			{
				// Insert
				newSkeleton = new Tree<Skeleton>;

				newSkeleton->Name = std::string(pNode->GetNameOnly());
				newSkeleton->Level = ParentLevel + 1;
				newSkeleton->Parent = pBoneHierarchy;

				pBoneHierarchy->Childs.push_back(newSkeleton);

			}
		}

		const int childCount = pNode->GetChildCount();
		for (int childIndex = 0; childIndex < childCount; ++childIndex)
		{
			if (newSkeleton)
			{
				DrawNodeRecursive(
					pNode->GetChild(childIndex),
					pTime,
					pPose,
					newSkeleton
				);
			}
			else
			{
				DrawNodeRecursive(
					pNode->GetChild(childIndex),
					pTime,
					pPose,
					pBoneHierarchy
				);
			}
		}
	}

	void FBXLoader::DrawBoneRecursive(
		FbxNode* pNode,
		std::vector<FbxTime> mStops,
		FbxTime& pTime,
		FbxAMatrix& pGlobalRootPositions,
		std::vector<FbxAMatrix>& pParentGlobalPositions,
		FbxPose* pPose,
		long long perFrame,
		std::ofstream& outFile
	)
	{
		FbxAMatrix pParentGlobalPosition;

		FbxAMatrix globalPosition =
			GetGlobalPosition(
				pNode,
				pTime,
				pPose,
				&pParentGlobalPosition
			);
		pParentGlobalPositions.push_back(globalPosition);

		if (pNode->GetNodeAttribute())
		{
			DrawBone(
				pNode,
				mStops,
				pGlobalRootPositions,
				pParentGlobalPosition,
				perFrame,
				outFile
			);
		}


		const int childCount = pNode->GetChildCount();
		for (int childIndex = 0; childIndex < childCount; ++childIndex)
		{
			DrawBoneRecursive(
				pNode->GetChild(childIndex),
				mStops,
				pTime,
				pGlobalRootPositions,
				pParentGlobalPositions,
				pPose,
				perFrame,
				outFile
			);
		}
	}

	// Draw Skeleton
	void FBXLoader::DrawSkeleton(
		FbxNode* pNode,
		FbxSkeleton* skeleton,
		int targetBoneCount,
		pmx::PmxBone* targetBone,
		std::vector<FbxTime> mStarts,
		std::vector<FbxTime> mStops,
		FbxTime& pTime,
		FbxAnimLayer* pAnimLayer,
		FbxAMatrix& pParentGlobalPosition,
		FbxAMatrix& pGlobalPosition,
		long long perFrame,
		std::ofstream& outFile
	)
	{
		if (skeleton->GetSkeletonType() == FbxSkeleton::eLimbNode &&
			pNode->GetParent() &&
			pNode->GetParent()->GetNodeAttribute() &&
			pNode->GetParent()->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			std::string boneName(pNode->GetName());

			try {
				int transSkeletonPairSize = (int)transSkeletonPairs.at(boneName).size();
				if (transSkeletonPairSize == 0)
					return;
			}
			catch (std::exception&) {
				return;
			}

			// The Bones that is Non-Adapted Animation

			// Global Base Position
			FbxVector4 head = pParentGlobalPosition.GetT();
			// Global End Position
			FbxVector4 tail = pGlobalPosition.GetT();

			// The Bones that is Adapted Animation
			FbxAMatrix fbxAMatrix =
				FbxAMatrix(
					pNode->GetGeometricTranslation(FbxNode::eSourcePivot),
					pNode->GetGeometricRotation(FbxNode::eSourcePivot),
					pNode->GetGeometricScaling(FbxNode::eSourcePivot)
				);

			FbxAMatrix global;
			FbxVector4 globalT;
			FbxVector4 globalR;
			FbxVector4 fbxMatT;

			// Pmx에게 옮길 뼈대의 개수
			int transSkeletonPairsSize = (int)transSkeletonPairs.at(boneName).size();

			outFile.write((char*)&transSkeletonPairsSize, sizeof(int));

			for (int i = 0; i < transSkeletonPairsSize; i++)
			{
				outFile.write((const char*)transSkeletonPairs.at(boneName)[i].c_str(), transSkeletonPairs.at(boneName)[i].size());
				outFile.write("\n", 1);
			}

			int frameCount = (int)mStops[0].GetFrameCount(FbxTime::eFrames30);

			float convBuf;
			for (int animFrame = 0; animFrame < frameCount; animFrame++)
			{
				global =
					GetGlobalPosition(
						pNode,
						animFrame * perFrame,
						NULL,
						&pParentGlobalPosition
					);
				globalT = global.GetT();

				convBuf = (float)globalT[0];
				outFile.write((char*)&convBuf, sizeof(float));
				convBuf = (float)globalT[1];
				outFile.write((char*)&convBuf, sizeof(float));
				convBuf = (float)globalT[2];
				outFile.write((char*)&convBuf, sizeof(float));
			}
		}
	}

	// Deform the vertex array with the shapes contained in the mesh.
	void FBXLoader::ComputeShapeDeformation(
		FbxMesh* pMesh,
		FbxTime& pTime,
		FbxAnimLayer* pAnimLayer,
		FbxVector4* pVertexArray
	)
	{
		int lVertexCount = pMesh->GetControlPointsCount();

		FbxVector4* lSrcVertexArray = pVertexArray;
		FbxVector4* lDstVertexArray = new FbxVector4[lVertexCount];
		memcpy(lDstVertexArray, pVertexArray, lVertexCount * sizeof(FbxVector4));

		int lBlendShapeDeformerCount = pMesh->GetDeformerCount(FbxDeformer::eBlendShape);
		for (int lBlendShapeIndex = 0; lBlendShapeIndex < lBlendShapeDeformerCount; ++lBlendShapeIndex)
		{
			FbxBlendShape* lBlendShape = (FbxBlendShape*)pMesh->GetDeformer(lBlendShapeIndex, FbxDeformer::eBlendShape);

			int lBlendShapeChannelCount = lBlendShape->GetBlendShapeChannelCount();
			for (int lChannelIndex = 0; lChannelIndex < lBlendShapeChannelCount; ++lChannelIndex)
			{
				FbxBlendShapeChannel* lChannel = lBlendShape->GetBlendShapeChannel(lChannelIndex);
				if (lChannel)
				{
					// Get the percentage of influence on this channel.
					FbxAnimCurve* lFCurve = pMesh->GetShapeChannel(lBlendShapeIndex, lChannelIndex, pAnimLayer);
					if (!lFCurve) continue;
					double lWeight = lFCurve->Evaluate(pTime);

					lFCurve->Destroy();

					/*
					If there is only one targetShape on this channel, the influence is easy to calculate:
					influence = (targetShape - baseGeometry) * weight * 0.01
					dstGeometry = baseGeometry + influence

					But if there are more than one targetShapes on this channel, this is an in-between
					blendshape, also called progressive morph. The calculation of influence is different.

					For example, given two in-between targets, the full weight percentage of first target
					is 50, and the full weight percentage of the second target is 100.
					When the weight percentage reach 50, the base geometry is already be fully morphed
					to the first target shape. When the weight go over 50, it begin to morph from the
					first target shape to the second target shape.

					To calculate influence when the weight percentage is 25:
					1. 25 falls in the scope of 0 and 50, the morphing is from base geometry to the first target.
					2. And since 25 is already half way between 0 and 50, so the real weight percentage change to
					the first target is 50.
					influence = (firstTargetShape - baseGeometry) * (25-0)/(50-0) * 100
					dstGeometry = baseGeometry + influence

					To calculate influence when the weight percentage is 75:
					1. 75 falls in the scope of 50 and 100, the morphing is from the first target to the second.
					2. And since 75 is already half way between 50 and 100, so the real weight percentage change
					to the second target is 50.
					influence = (secondTargetShape - firstTargetShape) * (75-50)/(100-50) * 100
					dstGeometry = firstTargetShape + influence
					*/

					// Find the two shape indices for influence calculation according to the weight.
					// Consider index of base geometry as -1.

					int lShapeCount = lChannel->GetTargetShapeCount();
					double* lFullWeights = lChannel->GetTargetShapeFullWeights();

					// Find out which scope the lWeight falls in.
					int lStartIndex = -1;
					int lEndIndex = -1;
					for (int lShapeIndex = 0; lShapeIndex < lShapeCount; ++lShapeIndex)
					{
						if (lWeight > 0 && lWeight <= lFullWeights[0])
						{
							lEndIndex = 0;
							break;
						}
						if (lWeight > lFullWeights[lShapeIndex] && lWeight < lFullWeights[lShapeIndex + 1])
						{
							lStartIndex = lShapeIndex;
							lEndIndex = lShapeIndex + 1;
							break;
						}
					}

					FbxShape* lStartShape = NULL;
					FbxShape* lEndShape = NULL;
					if (lStartIndex > -1)
					{
						lStartShape = lChannel->GetTargetShape(lStartIndex);
					}
					if (lEndIndex > -1)
					{
						lEndShape = lChannel->GetTargetShape(lEndIndex);
					}

					//The weight percentage falls between base geometry and the first target shape.
					if (lStartIndex == -1 && lEndShape)
					{
						double lEndWeight = lFullWeights[0];
						// Calculate the real weight.
						lWeight = (lWeight / lEndWeight) * 100;
						// Initialize the lDstVertexArray with vertex of base geometry.
						memcpy(lDstVertexArray, lSrcVertexArray, lVertexCount * sizeof(FbxVector4));
						for (int j = 0; j < lVertexCount; j++)
						{
							// Add the influence of the shape vertex to the mesh vertex.
							FbxVector4 lInfluence = (lEndShape->GetControlPoints()[j] - lSrcVertexArray[j]) * lWeight * 0.01;
							lDstVertexArray[j] += lInfluence;
						}
					}
					//The weight percentage falls between two target shapes.
					else if (lStartShape && lEndShape)
					{
						double lStartWeight = lFullWeights[lStartIndex];
						double lEndWeight = lFullWeights[lEndIndex];
						// Calculate the real weight.
						lWeight = ((lWeight - lStartWeight) / (lEndWeight - lStartWeight)) * 100;
						// Initialize the lDstVertexArray with vertex of the previous target shape geometry.
						memcpy(lDstVertexArray, lStartShape->GetControlPoints(), lVertexCount * sizeof(FbxVector4));
						for (int j = 0; j < lVertexCount; j++)
						{
							// Add the influence of the shape vertex to the previous shape vertex.
							FbxVector4 lInfluence = (lEndShape->GetControlPoints()[j] - lStartShape->GetControlPoints()[j]) * lWeight * 0.01;
							lDstVertexArray[j] += lInfluence;
						}
					}
				}//If lChannel is valid
			}//For each blend shape channel
		}//For each blend shape deformer

		memcpy(pVertexArray, lDstVertexArray, lVertexCount * sizeof(FbxVector4));

		delete[] lDstVertexArray;
	}

	// Get the geometry offset to a node. It is never inherited by the children.
	FbxAMatrix GetGeometry(FbxNode* pNode)
	{
		const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
		const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

		return FbxAMatrix(lT, lR, lS);
	}

	//Compute the transform matrix that the cluster will transform the vertex.
	void FBXLoader::ComputeClusterDeformation(
		FbxAMatrix& pGlobalPosition,
		FbxMesh* pMesh,
		FbxCluster* pCluster,
		FbxAMatrix& pVertexTransformMatrix,
		FbxTime pTime)
	{
		FbxCluster::ELinkMode lClusterMode = pCluster->GetLinkMode();

		FbxAMatrix lReferenceGlobalInitPosition;
		FbxAMatrix lReferenceGlobalCurrentPosition;
		FbxAMatrix lAssociateGlobalInitPosition;
		FbxAMatrix lAssociateGlobalCurrentPosition;
		FbxAMatrix lClusterGlobalInitPosition;
		FbxAMatrix lClusterGlobalCurrentPosition;

		FbxAMatrix lReferenceGeometry;
		FbxAMatrix lAssociateGeometry;
		FbxAMatrix lClusterGeometry;

		FbxAMatrix lClusterRelativeInitPosition;
		FbxAMatrix lClusterRelativeCurrentPositionInverse;

		// 현재 뼈대의 변환 행렬을 lReferenceGlobalInitPosition에 복제
		pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
		lReferenceGlobalCurrentPosition = pGlobalPosition;
		// Multiply lReferenceGlobalInitPosition by Geometric Transformation

		FbxNode* pNode = pMesh->GetNode();

		lReferenceGeometry = FbxAMatrix(
			pNode->GetGeometricTranslation(FbxNode::eSourcePivot),
			pNode->GetGeometricRotation(FbxNode::eSourcePivot),
			pNode->GetGeometricScaling(FbxNode::eSourcePivot)
		);

		lReferenceGlobalInitPosition *= lReferenceGeometry;

		// Get the link initial global position and the link current global position.
		// 초기 T 포즈에서의 Position과 현재 포즈에서의 Position을 이어주는 Link Matrix를 제공 
		pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);

		// lClusterGlobalCurrentPosition = GetGlobalPosition(pCluster->GetLink(), pTime, NULL);
		lClusterGlobalCurrentPosition = pCluster->GetLink()->EvaluateGlobalTransform(pTime);


		// Compute the initial position of the link relative to the reference.
		// T 포즈인 모든 뼈대를 원점으로 만들어주는 변환 행렬
		lClusterRelativeInitPosition =
			lClusterGlobalInitPosition.Inverse() *
			lReferenceGlobalInitPosition;

		// Compute the current position of the link relative to the reference.
		// 원점 위치의 뼈대를 현재 포즈로 만들어주는 변환 행렬
		lClusterRelativeCurrentPositionInverse =
			lReferenceGlobalCurrentPosition.Inverse() *
			lClusterGlobalCurrentPosition;

		// Compute the shift of the link relative to the reference.
		// 다음 포즈를 만들어주는 변환 행렬을 만들어냅니다.
		pVertexTransformMatrix = lClusterRelativeCurrentPositionInverse * lClusterRelativeInitPosition;
	}

	// Deform the vertex array in classic linear way.
	void FBXLoader::ComputeLinearDeformation(
		FbxAMatrix& pGlobalPosition,
		FbxMesh* pMesh,
		FbxTime& pTime,
		FbxVector4* pVertexArray)
	{
		// All the links must have the same link mode.
		FbxCluster::ELinkMode lClusterMode = ((FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();

		int lVertexCount = pMesh->GetControlPointsCount();
		FbxAMatrix* lClusterDeformation = new FbxAMatrix[lVertexCount];
		memset(lClusterDeformation, 0, lVertexCount * sizeof(FbxAMatrix));

		double* lClusterWeight = new double[lVertexCount];
		memset(lClusterWeight, 0, lVertexCount * sizeof(double));

		if (lClusterMode == FbxCluster::eAdditive)
		{
			for (int i = 0; i < lVertexCount; ++i)
			{
				lClusterDeformation[i].SetIdentity();
			}
		}

		// For all skins and all clusters, accumulate their deformation and weight
		// on each vertices and store them in lClusterDeformation and lClusterWeight.
		int lSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);
		int lSkinIndex = 0;
		FbxSkin* lSkinDeformer = nullptr;
		int lClusterCount = 0;
		int lClusterIndex = 0;
		FbxCluster* lCluster = nullptr;
		// 각 프레임에 따른 뼈대 변형 행렬
		FbxAMatrix lVertexTransformMatrix;
		int lVertexIndexCount;
		int i = 0;
		int k = 0;
		int lIndex = 0;
		double lWeight = 0.0;
		FbxAMatrix lInfluence;

		for (lSkinIndex = 0; lSkinIndex < lSkinCount; ++lSkinIndex)
		{
			// Skin 로드
			lSkinDeformer = (FbxSkin*)pMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin);

			// 한 Skin에 클러스터(뼈대) 개수 로드 
			lClusterCount = lSkinDeformer->GetClusterCount();
			for (lClusterIndex = 0; lClusterIndex < lClusterCount; ++lClusterIndex)
			{
				// 뼈대 로드 
				lCluster = lSkinDeformer->GetCluster(lClusterIndex);
				// 정보가 없다면 패스
				if (!lCluster->GetLink())
					continue;

				// pTime 프레임 에서의 포즈 변환 행렬을 얻어옴.
				ComputeClusterDeformation(pGlobalPosition, pMesh, lCluster, lVertexTransformMatrix, pTime);
				// 정점의 개수 로드
				lVertexIndexCount = lCluster->GetControlPointIndicesCount();

				for (k = 0; k < lVertexIndexCount; ++k)
				{
					// 버텍스가 존재하는 인덱스 위치를 얻어옴
					lIndex = lCluster->GetControlPointIndices()[k];

					// Sometimes, the mesh can have less points than at the time of the skinning
					// because a smooth operator was active when skinning but has been deactivated during export.
					if (lIndex >= lVertexCount)
						continue;

					// Get Bone Weight
					lWeight = lCluster->GetControlPointWeights()[k];
					// Weight가 0.0일 때 Cloth Weight를 업데이트 하지 않음
					if (lWeight == 0.0)
						continue;

					lInfluence = lVertexTransformMatrix;
					MatrixScale(lInfluence, lWeight);

					if (lClusterMode == FbxCluster::eAdditive)
					{
						// Multiply with the product of the deformations on the vertex.
						MatrixAddToDiagonal(lInfluence, 1.0 - lWeight);
						lClusterDeformation[lIndex] = lInfluence * lClusterDeformation[lIndex];

						// Set the link to 1.0 just to know this vertex is influenced by a link.
						lClusterWeight[lIndex] = 1.0;
					}
					else // lLinkMode == FbxCluster::eNormalize || lLinkMode == FbxCluster::eTotalOne
					{
						// Add to the sum of the deformations on the vertex.
						MatrixAdd(lClusterDeformation[lIndex], lInfluence);

						// Add to the sum of weights to either normalize or complete the vertex.
						lClusterWeight[lIndex] += lWeight;
					}
				}//For each vertex		
			}//lClusterCount
		}

		//Actually deform each vertices here by information stored in lClusterDeformation and lClusterWeight
		FbxVector4 lSrcVertex;
		lWeight = 0;
		//double lWeight = 0;

		for (i = 0; i < lVertexCount; i++)
		{
			lSrcVertex = pVertexArray[i];
			FbxVector4& lDstVertex = pVertexArray[i];

			// count of weight == controlPointVertexCount
			lWeight = lClusterWeight[i];

			// Deform the vertex if there was at least a link with an influence on the vertex,
			if (lWeight != 0.0)
			{
				// 기본 IDLE Vertex Position에서 Pose 변형 행렬을 곱하여 결과 Vertex Position을 구한다
				lDstVertex = lClusterDeformation[i].MultT(lSrcVertex);

				if (lClusterMode == FbxCluster::eNormalize)
				{
					// In the normalized link mode, a vertex is always totally influenced by the links. 
					lDstVertex /= lWeight;
				}
				else if (lClusterMode == FbxCluster::eTotalOne)
				{
					// In the total 1 link mode, a vertex can be partially influenced by the links. 
					lSrcVertex *= (1.0 - lWeight);
					lDstVertex += lSrcVertex;
				}
			}
		}

		delete[] lClusterDeformation;
		delete[] lClusterWeight;
	}

	int FBXLoader::DrawMesh(
		FbxNode* pNode,
		MeshData& meshData,
		bool uvMode
	)
	{
		FbxMesh* lMesh = pNode->GetMesh();

		if (!lMesh)
			return 1;

		// FbxMesh
		FbxMesh* mesh = nullptr;
		// Material단위의 Surface
		FbxSurfaceMaterial* smat = nullptr;
		// Mesh 속성
		FbxProperty prop;
		// Material당 Texture  
		FbxLayeredTexture* layered_texture = nullptr;
		FbxFileTexture* fTexture = nullptr;

		// Get a FbxSurfaceMaterials Count
		int mcount = -1;
		// Get a Layer Count
		int lcount = -1;
		const char* file_texture_name = nullptr;

		const int vertexCount = lMesh->GetControlPointsCount();

		if (vertexCount == 0)
		{
			return 1;
		}

		//const bool hasShape = lMesh->GetShapeCount() > 0;
		//const bool hasSkin = lMesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
		//const bool hasDeformation = hasShape || hasSkin;

		//if (!hasDeformation)
		//{
		//	return 1;
		//}

		// Find Texture Path
		mcount = pNode->GetSrcObjectCount<FbxSurfaceMaterial>();

		for (int i = 0; i < mcount; i++)
		{
			smat = (FbxSurfaceMaterial*)pNode->GetSrcObject<FbxSurfaceMaterial>(i);

			if (smat) {
				prop = smat->FindProperty(FbxSurfaceMaterial::sDiffuse);

				int layered_texture_count = prop.GetSrcObjectCount<FbxLayeredTexture>();
				if (layered_texture_count > 0)
				{
					for (int j = 0; j < layered_texture_count; j++)
					{
						layered_texture = FbxCast<FbxLayeredTexture>(prop.GetSrcObject<FbxLayeredTexture>(j));
						lcount = layered_texture->GetSrcObjectCount<FbxFileTexture>();
						for (int k = 0; k < lcount; k++)
						{
							fTexture = FbxCast<FbxFileTexture>(layered_texture->GetSrcObject<FbxFileTexture>(k));

							file_texture_name = fTexture->GetFileName();

							MemoryPool::getInstance().TexturePaths.insert(file_texture_name);
						}
					}
				}
				else
				{
					int texture_count = prop.GetSrcObjectCount<FbxFileTexture>();
					for (int j = 0; j < texture_count; j++)
					{
						fTexture = FbxCast<FbxFileTexture>(prop.GetSrcObject<FbxFileTexture>(j));

						file_texture_name = fTexture->GetFileName();

						MemoryPool::getInstance().TexturePaths.insert(file_texture_name);
					}
				}
			}
		}

		// Get a Textures Path Name
		if (file_texture_name) {
			meshData.mMaterial.BaseColorTexutures.push_back(
				Path2FileName(file_texture_name));
		}

		// GetControlPointVertex
		//if (hasDeformation) 
		{
			meshData.Vertices.resize(vertexCount);

			for (int i = 0; i < vertexCount; i++) {
				meshData.Vertices[i].Position[0] =
					static_cast<float>(lMesh->GetControlPointAt(i)[0]);
				meshData.Vertices[i].Position[1] =
					static_cast<float>(lMesh->GetControlPointAt(i)[1]);
				meshData.Vertices[i].Position[2] =
					static_cast<float>(lMesh->GetControlPointAt(i)[2]);

				meshData.Vertices[i].Normal[0] =
					static_cast<float>(lMesh->GetElementNormal()->GetDirectArray().GetAt(i)[0]);
				meshData.Vertices[i].Normal[1] =
					static_cast<float>(lMesh->GetElementNormal()->GetDirectArray().GetAt(i)[1]);
				meshData.Vertices[i].Normal[2] =
					static_cast<float>(lMesh->GetElementNormal()->GetDirectArray().GetAt(i)[2]);
			}

			if (lMesh->GetElementTangentCount()) {
				for (int i = 0; i < vertexCount; i++) {
					meshData.Vertices[i].TangentU[0] =
						static_cast<float>(lMesh->GetElementTangent()->GetDirectArray().GetAt(i)[0]);
					meshData.Vertices[i].TangentU[1] =
						static_cast<float>(lMesh->GetElementTangent()->GetDirectArray().GetAt(i)[1]);
					meshData.Vertices[i].TangentU[2] =
						static_cast<float>(lMesh->GetElementTangent()->GetDirectArray().GetAt(i)[2]);
				}
			}
			else {
				for (int i = 0; i < vertexCount; i++) {
					meshData.Vertices[i].TangentU[0] = 0.0f;
					meshData.Vertices[i].TangentU[1] = 0.0f;
					meshData.Vertices[i].TangentU[2] = 0.0f;
				}
			}
		}

		// GetControlPointVertexIndices
		// if (hasDeformation)
		{
			unsigned int triCount = lMesh->GetPolygonCount();
			unsigned int vertexIndex = 0;

			std::vector<int> pre;

			unsigned i, j;
			uint32_t numIndices = 1;
			uint32_t polygonVertexSize = 0;

			for (i = 0; i < triCount; ++i)
			{
				polygonVertexSize = lMesh->GetPolygonSize(i) - 2;
				numIndices += polygonVertexSize * 3;
			}

			// Index Count
			uint32_t numIndex = -1;
			meshData.Indices32.resize(numIndices);

			for (i = 0; i < triCount; ++i)
			{
				polygonVertexSize = lMesh->GetPolygonSize(i);

				pre.resize(polygonVertexSize);
				for (j = 0; j < polygonVertexSize; ++j)
					pre[j] = lMesh->GetPolygonVertex(i, j);

				for (UINT indicesIDX = 0; indicesIDX < polygonVertexSize - 2; indicesIDX++) {
					meshData.Indices32[++numIndex] = pre[0];
					meshData.Indices32[++numIndex] = pre[indicesIDX + 1];
					meshData.Indices32[++numIndex] = pre[indicesIDX + 2];
				}

			}

			 pre.clear();

			std::vector<fbxsdk::FbxVector2> uvs;
			{
				//get all UV set names
				FbxStringList lUVSetNameList;
				lMesh->GetUVSetNames(lUVSetNameList);

				//iterating over all uv sets
				for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
				{
					//get lUVSetIndex-th uv set
					const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
					const FbxGeometryElementUV* lUVElement = lMesh->GetElementUV(lUVSetName);

					if (!lUVElement)
						continue;

					// only support mapping mode eByPolygonVertex and eByControlPoint
					if (lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
						lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
						printf("");

					//index array, where holds the index referenced to the uv data
					const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
					const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;

					//iterating through the data by polygon
					const int lPolyCount = lMesh->GetPolygonCount();

					if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
					{
						for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
						{
							// build the max index array that we need to pass into MakePoly
							const int lPolySize = lMesh->GetPolygonSize(lPolyIndex);
							for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
							{
								FbxVector2 lUVValue;
								//get the index of the current vertex in control points array
								int lPolyVertIndex = lMesh->GetPolygonVertex(lPolyIndex, lVertIndex);

								//the UV index depends on the reference mode
								int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;

								lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

								uvs.push_back(lUVValue);
							}
						}
					}
					else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
					{
						int vertexCounter = 0;
						int lPolyIndexCounter = 0;

						for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
						{
							// build the max index array that we need to pass into MakePoly
							const int lPolySize = lMesh->GetPolygonSize(lPolyIndex);
							for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
							{
								if (lPolyIndexCounter < lIndexCount)
								{
									FbxVector2 lUVValue;

									//if (uvMode) 
									//{
									//	int mTextureUVIndex = lMesh->GetTextureUVIndex(lPolyIndex, lVertIndex);

									//	lUVValue = lUVElement->GetDirectArray().GetAt(mTextureUVIndex);

									//	// Convert to floats
									//	meshData.Vertices[meshData.Indices32[vertexCounter]].TexC.x = static_cast<float>(lUVValue[0]);
									//	meshData.Vertices[meshData.Indices32[vertexCounter]].TexC.y = static_cast<float>(lUVValue[1]);

									//	vertexCounter++;
									//}
									//else
									{
										//the UV index depends on the reference mode
										int lUVIndex =
											lUseIndex ?
											lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) :
											lPolyIndexCounter;

										lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

										uvs.push_back(lUVValue);

										lPolyIndexCounter++;
									}
								} // if (lPolyIndexCounter < lIndexCount)
							} // lVertIndex
						} // lPolyIndex
					}
				}
			}

			if (uvs.size() > 0)
			{
				int vertexCounter = 0;

				for (int i = 0; i != lMesh->GetPolygonCount(); ++i) {
					int polygonSize = lMesh->GetPolygonSize(i);

					for (int j = 0; j != polygonSize; ++j) {
						int vertexIndex = lMesh->GetPolygonVertex(i, j);
						meshData.Vertices[vertexIndex].TexCoord0[0] = (float)uvs[vertexCounter][0];
						meshData.Vertices[vertexIndex].TexCoord0[1] = 1.0f - (float)uvs[vertexCounter][1];

						++vertexCounter;
					}
				}
			}
		}

		lMesh->Destroy();

		return 0;
	}

	const UINT kMaxThreadCount = 4;

	// Animation Thread Resource
	DWORD hLoadAnimThreadID[kMaxThreadCount];
	HANDLE hLoadAnimThread[kMaxThreadCount];

	HANDLE hLoadAnimReadEvent[kMaxThreadCount];
	HANDLE hLoadAnimWriteEvent[kMaxThreadCount];

	static float mFrame;

	FbxMesh* mStaticMesh;

	float** mStaticVertexArrays[kMaxThreadCount];

	UINT mStaticVertexCount;

	UINT ClipStartTime[kMaxThreadCount];
	UINT ClipEndTime[kMaxThreadCount];

	// animation * cluster<Matrix>
	std::vector<fbxsdk::FbxAMatrix> lVertexTransformMatrixs[kMaxThreadCount];

	static bool mStopLoadAnimationThread = false;

	DWORD WINAPI LoadAnimationThread(LPVOID prc)
	{
		UINT mThreadIDX = (UINT)prc;

		UINT i, j, k;
		float mTime = 0.0f;

		UINT lClusterCount = 0;
		UINT lClusterIndex = 0;
		std::vector<FbxCluster*> lClusters;

		UINT animFrame = 0;

		fbxsdk::FbxAMatrix* lClusterDeformation;
		lClusterDeformation = new FbxAMatrix[mStaticVertexCount];

		fbxsdk::FbxAMatrix lVertexTransformMatrix;
		UINT lVertexIndexCount;
		int lIndex = 0;
		double lWeight = 0.0;

		FbxSkin* lSkinDeformer = (FbxSkin*)mStaticMesh->GetDeformer(0, FbxDeformer::eSkin);

		// 클러스터 (deform) 개수를 얻어옴
		lClusterCount = lSkinDeformer->GetClusterCount();

		fbxsdk::FbxVector4 x;
		float* mDatas;

		int* mIndices;
		double* mWeights;

		fbxsdk::FbxVector4* iter = mStaticMesh->GetControlPoints();

		UINT mAnimationIndex = 0;
		UINT mClusterIndexByAnimation = 0;

		for (lClusterIndex = 0; lClusterIndex < lClusterCount; ++lClusterIndex)
		{
			lClusters.push_back(lSkinDeformer->GetCluster(lClusterIndex));
		}

		while (true)
		{
			SetEvent(hLoadAnimReadEvent[mThreadIDX]);
			WaitForSingleObject(hLoadAnimWriteEvent[mThreadIDX], INFINITE);

			if (mStopLoadAnimationThread)
				break;

			mAnimationIndex = 0;
			mClusterIndexByAnimation = 0;

			for (animFrame = ClipStartTime[mThreadIDX]; animFrame < ClipEndTime[mThreadIDX]; animFrame++)
			{
				memset(lClusterDeformation, 0, mStaticVertexCount * sizeof(fbxsdk::FbxAMatrix));

				for (lClusterIndex = 0; lClusterIndex < lClusterCount; ++lClusterIndex)
				{
					lVertexTransformMatrix = lVertexTransformMatrixs[mThreadIDX][mClusterIndexByAnimation++];

					lVertexIndexCount = lClusters[lClusterIndex]->GetControlPointIndicesCount();

					mIndices = lClusters[lClusterIndex]->GetControlPointIndices();
					mWeights = lClusters[lClusterIndex]->GetControlPointWeights();

					for (k = 0; k < lVertexIndexCount; ++k)
					{
						lIndex = mIndices[k];
						lWeight = mWeights[k];

						if (lWeight == 1.0)
						{
							lClusterDeformation[lIndex] = lVertexTransformMatrix;
						}
						else
						{
							for (i = 0; i < 4; i++)
								for (j = 0; j < 4; j++)
									lClusterDeformation[lIndex][i][j] += lVertexTransformMatrix[i][j] * lWeight;
						}
					}//For each vertex
				}//lClusterCount

				mDatas = mStaticVertexArrays[mThreadIDX][mAnimationIndex];

				for (i = 0; i < mStaticVertexCount; i++)
				{
					// 현재 버텍스에 animation frame Matrix를 곱하여 다음 애니메이션 포즈에 대응하는 위치로
					// 버텍스가 변형됨.
					x = (lClusterDeformation[i].MultT(iter[i]));

					mDatas[i * 4]		= (float)x.Buffer()[0];
					mDatas[i * 4 + 1]	= (float)x.Buffer()[1];
					mDatas[i * 4 + 2]	= (float)x.Buffer()[2];
					mDatas[i * 4 + 3]	= (float)x.Buffer()[3];
				}

				mAnimationIndex++;
			}

			ResetEvent(hLoadAnimWriteEvent[mThreadIDX]);
		}

		delete[](lClusterDeformation);

		ResetEvent(hLoadAnimWriteEvent[mThreadIDX]);
		SetEvent(hLoadAnimReadEvent[mThreadIDX]);
		return (DWORD)(0);
	}

	void FBXLoader::DrawMesh(
		int submeshIDX,
		FbxNode* pNode,
		ObjectData* objData
	)
	{
		FbxMesh* lMesh = pNode->GetMesh();
		objData->meshFBXDatas.push_back(pNode->GetMesh());

		// Get a FbxSurfaceMaterials Count
		int mcount = -1;
		// Get a Layer Count
		int lcount = -1;
		const char* file_texture_name = nullptr;

		const int vertexCount = lMesh->GetControlPointsCount();

		if (!vertexCount)
			return;

		long long Frame = objData->Sequences[0]->mStops[0].GetFrameCount();
		const bool hasShape = lMesh->GetShapeCount() > 0;
		const bool hasSkin = lMesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
		const bool hasDeformation = hasShape || hasSkin;

		if (!hasDeformation)
			return;

		MeshData meshData;

#pragma region  LoadTexture

		// Find Texture Path
		mcount = pNode->GetSrcObjectCount<FbxSurfaceMaterial>();

		int i = 0;
		int j = 0;
		int k = 0;
		int layered_texture_count;
		int texture_count;

		for (i = 0; i < mcount; i++)
		{
			smat = (FbxSurfaceMaterial*)pNode->GetSrcObject<FbxSurfaceMaterial>(i);

			if (smat) {
				prop = smat->FindProperty(FbxSurfaceMaterial::sDiffuse);

				layered_texture_count = prop.GetSrcObjectCount<FbxLayeredTexture>();
				if (layered_texture_count > 0)
				{
					for (j = 0; j < layered_texture_count; j++)
					{
						layered_texture = FbxCast<FbxLayeredTexture>(prop.GetSrcObject<FbxLayeredTexture>(j));
						lcount = layered_texture->GetSrcObjectCount<FbxFileTexture>();
						for (k = 0; k < lcount; k++)
						{
							fTexture = FbxCast<FbxFileTexture>(layered_texture->GetSrcObject<FbxFileTexture>(k));

							file_texture_name = fTexture->GetFileName();

							MemoryPool::getInstance().TexturePaths.insert(fTexture->GetFileName());
						}
					}
				}
				else
				{
					texture_count = prop.GetSrcObjectCount<FbxFileTexture>();
					for (j = 0; j < texture_count; j++)
					{
						fTexture = FbxCast<FbxFileTexture>(prop.GetSrcObject<FbxFileTexture>(j));

						file_texture_name = fTexture->GetFileName();

						MemoryPool::getInstance().TexturePaths.insert(fTexture->GetFileName());
					}
				}
			}
		}

		// Get a Textures Path Name
		if (file_texture_name) {
			meshData.mMaterial.BaseColorTexutures.push_back(
				Path2FileName(file_texture_name));
		}

#pragma endregion

#pragma region Load Mesh Info

		// GetControlPointVertex
		meshData.Vertices.resize(vertexCount);

		fbxsdk::FbxGeometryElementNormal* norm = lMesh->GetElementNormal();

		for (i = 0; i < vertexCount; i++) {
			meshData.Vertices[i].Position[0] = (float)lMesh->GetControlPoints()[i][0];
			meshData.Vertices[i].Position[1] = (float)lMesh->GetControlPoints()[i][1];
			meshData.Vertices[i].Position[2] = (float)lMesh->GetControlPoints()[i][2];

			switch (norm->GetMappingMode())
			{
			case FbxGeometryElement::eByControlPoint:
				printf("");
				break;
			case FbxGeometryElement::eByPolygonVertex:
				switch (norm->GetReferenceMode())
				{
					case FbxGeometryElement::eDirect:
					{
						meshData.Vertices[i].Normal[0] = 
							(float)norm->GetDirectArray().GetAt(i).mData[0];
						meshData.Vertices[i].Normal[1] = 
							(float)norm->GetDirectArray().GetAt(i).mData[1];
						meshData.Vertices[i].Normal[2] = 
							(float)norm->GetDirectArray().GetAt(i).mData[2];
					}
					break;

					case FbxGeometryElement::eIndexToDirect:
					{
						int index = norm->GetIndexArray().GetAt(i);
						meshData.Vertices[i].Normal[0] = (float)norm->GetDirectArray().GetAt(index)[0];
						meshData.Vertices[i].Normal[1] = (float)norm->GetDirectArray().GetAt(index)[1];
						meshData.Vertices[i].Normal[2] = (float)norm->GetDirectArray().GetAt(index)[2];
					}
					break;
				}
				break;
			}
		}

		if (lMesh->GetElementTangentCount()) {
			for (int i = 0; i < vertexCount; i++) {
				meshData.Vertices[i].TangentU[0] = (float)lMesh->GetElementTangent()->GetDirectArray().GetAt(i)[0];
				meshData.Vertices[i].TangentU[1] = (float)lMesh->GetElementTangent()->GetDirectArray().GetAt(i)[1];
				meshData.Vertices[i].TangentU[2] = (float)lMesh->GetElementTangent()->GetDirectArray().GetAt(i)[2];
			}
		}
		else {
			for (i = 0; i < vertexCount; i++) {
				meshData.Vertices[i].TangentU[0] = 0.0f;
				meshData.Vertices[i].TangentU[1] = 0.0f;
				meshData.Vertices[i].TangentU[2] = 0.0f;
			}
		}

		// GetControlPointVertexIndices
		unsigned int triCount = lMesh->GetPolygonCount();
		unsigned int vertexIndex = 0;

		uint32_t numIndices = 1;
		uint32_t polygonVertexSize = 0;

		for (UINT i = 0; i < triCount; ++i)
		{
			polygonVertexSize = lMesh->GetPolygonSize(i) - 2;
			numIndices += polygonVertexSize * 3;
		}

		// Index Count
		uint32_t numIndex = -1;
		meshData.Indices32.resize(numIndices);
		
		std::vector<int> pre;

		for (UINT i = 0; i < triCount; ++i)
		{
			polygonVertexSize = lMesh->GetPolygonSize(i);

			pre.resize(polygonVertexSize);
			for (UINT j = 0; j < polygonVertexSize; ++j)
				pre[j] = lMesh->GetPolygonVertex(i, j);

			for (UINT indicesIDX = 0; indicesIDX < polygonVertexSize - 2; indicesIDX++) {
				meshData.Indices32[++numIndex] = pre[0];
				meshData.Indices32[++numIndex] = pre[indicesIDX + 1];
				meshData.Indices32[++numIndex] = pre[indicesIDX + 2];
			}
		}

		pre.clear();

		std::vector<fbxsdk::FbxVector2> uvs;

		//get all UV set names
		FbxStringList lUVSetNameList;
		lMesh->GetUVSetNames(lUVSetNameList);

		int uvCount = lUVSetNameList.GetCount();

		//iterating over all uv sets
		for (int lUVSetIndex = 0; lUVSetIndex < uvCount; lUVSetIndex++)
		{
			uvs.clear();
			uvs.resize(0);

			//get lUVSetIndex-th uv set
			const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
			const FbxGeometryElementUV* lUVElement = lMesh->GetElementUV(lUVSetName);

			//if (!lUVElement)
			//	continue;

			//index array, where holds the index referenced to the uv data
			const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
			const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;

			//iterating through the data by polygon
			const int lPolyCount = lMesh->GetPolygonCount();

			if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
				{
					// build the max index array that we need to pass into MakePoly
					const int lPolySize = lMesh->GetPolygonSize(lPolyIndex);
					for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
					{
						FbxVector2 lUVValue;
						//get the index of the current vertex in control points array
						int lPolyVertIndex = lMesh->GetPolygonVertex(lPolyIndex, lVertIndex);

						//the UV index depends on the reference mode
						int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;

						lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

						uvs.push_back(lUVValue);
					}
				}
			}
			else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			{
				int vertexCounter = 0;
				int lPolyIndexCounter = 0;

				int lUVIndex, lPolyIndex, lVertIndex;
				FbxVector2 lUVValue;

				for (lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
				{
					// build the max index array that we need to pass into MakePoly
					const int lPolySize = lMesh->GetPolygonSize(lPolyIndex);
					for (lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
					{
						if (lPolyIndexCounter < lIndexCount)
						{
							//the UV index depends on the reference mode
							lUVIndex =
								lUseIndex ?
								lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) :
								lPolyIndexCounter;

							lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

							uvs.push_back(lUVValue);

							lPolyIndexCounter++;
							
						} // if (lPolyIndexCounter < lIndexCount)
					} // lVertIndex
				} // lPolyIndex
			}

			if (uvs.size() > 0)
			{
				int vertexCounter = 0;
				int polygonSize = 0;
				int vertexIndex = 0;

				for (i = 0; i != lMesh->GetPolygonCount(); ++i) {
					polygonSize = lMesh->GetPolygonSize(i);

					for (j = 0; j != polygonSize; ++j) {
						vertexIndex = lMesh->GetPolygonVertex(i, j);

						if (lUVSetIndex == 0)
						{
							meshData.Vertices[vertexIndex].TexCoord0[0] = (float)uvs[vertexCounter][0];
							meshData.Vertices[vertexIndex].TexCoord0[1] = 1.0f - (float)uvs[vertexCounter][1];
						}
						else if (lUVSetIndex == 1)
						{
							meshData.Vertices[vertexIndex].TexCoord1[0] = (float)uvs[vertexCounter][0];
							meshData.Vertices[vertexIndex].TexCoord1[1] = 1.0f - (float)uvs[vertexCounter][1];
						}
						else if (lUVSetIndex == 2)
						{
							meshData.Vertices[vertexIndex].TexCoord2[0] = (float)uvs[vertexCounter][0];
							meshData.Vertices[vertexIndex].TexCoord2[1] = 1.0f - (float)uvs[vertexCounter][1];
						}
						else if (lUVSetIndex == 3)
						{
							meshData.Vertices[vertexIndex].TexCoord3[0] = (float)uvs[vertexCounter][0];
							meshData.Vertices[vertexIndex].TexCoord3[1] = 1.0f - (float)uvs[vertexCounter][1];
						}
						else
						{
							throw std::runtime_error("UV Overflow");
						}

						++vertexCounter;
					}
				}
			}
		}

		if (meshData.Vertices.size() > 0)
			objData->meshDatas.push_back(meshData);

#pragma endregion

#pragma region Animation

		//we need to get the number of clusters
		const int lSkinCount = lMesh->GetDeformerCount(FbxDeformer::eSkin);
		int lClusterCount = 0;
		int lSkinIndex = 0;

		for (lSkinIndex = 0; lSkinIndex < lSkinCount; ++lSkinIndex)
		{
			lClusterCount +=
				((FbxSkin*)(lMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin)))->GetClusterCount();
		}

		FbxSkin* lSkinDeformer = (FbxSkin*)lMesh->GetDeformer(0, FbxDeformer::eSkin);
		FbxSkin::EType lSkinningType = lSkinDeformer->GetSkinningType();

		// Cluster(뼈대) 개수
		lClusterCount		= lSkinDeformer->GetClusterCount();
		objData->Sequences[0]->mAnimVertexSizes[submeshIDX] = lClusterCount;

		// 애니메이션 버퍼 공간 생성
#pragma region Assigned Animation Buffer Space

		objData->Sequences[0]->mSubmeshSize = objData->SubmeshCount;
		objData->Sequences[0]->mFrameSize = Frame;
		objData->Sequences[0]->mClusterSize = lClusterCount;

		objData->Sequences[0]->mAnimVertexArrays[submeshIDX] = new float** [Frame];
		for (int FrameIDX = 0; FrameIDX < Frame; FrameIDX++)
		{
			objData->Sequences[0]->mAnimVertexArrays[submeshIDX][FrameIDX] = new float* [lClusterCount];
			for (int ClusterIDX = 0; ClusterIDX < lClusterCount; ClusterIDX++)
			{
				objData->Sequences[0]->mAnimVertexArrays[submeshIDX][FrameIDX][ClusterIDX] = new float[16];
			}
		}

#pragma endregion

		// For all skins and all clusters, accumulate their deformation and weight
		// on each vertices and store them in lClusterDeformation and lClusterWeight.
		lCluster = nullptr;
		lClusterIDX = 0;
		FrameIDX = 0;

		lIndex = 0;
		lWeight = 0.0;

		lReferenceGlobalCurrentPosition = objData->pGlobalPosition.Inverse();
		// All the links must have the same link mode.
		lClusterMode = ((FbxSkin*)lMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();
		lVertexCount = lMesh->GetControlPointsCount();

		passCondition =
			lClusterCount &&
			lSkinDeformer &&
			(lSkinningType == FbxSkin::eLinear ||
				lSkinningType == FbxSkin::eRigid);

		mStaticMesh = lMesh;
		mStaticVertexCount = lVertexCount;
		pNode = lMesh->GetNode();

		lReferenceGeometry = fbxsdk::FbxAMatrix(
			pNode->GetGeometricTranslation(FbxNode::eSourcePivot),
			pNode->GetGeometricRotation(FbxNode::eSourcePivot),
			pNode->GetGeometricScaling(FbxNode::eSourcePivot)
		);

		int* mIndices = nullptr;
		double* mWeights = nullptr;

		UINT lVertexIndexCount;

		std::vector<fbxsdk::FbxAMatrix> lClusterRelativeInitPositions(lClusterCount);
		fbxsdk::FbxAMatrix res;

		lWeightAccumulation.clear();
		lWeightAccumulation.resize(lVertexCount);

		for (lClusterIDX = 0; lClusterIDX < lClusterCount; ++lClusterIDX)
		{
			// 클러스터 로드
			lCluster = lSkinDeformer->GetCluster(lClusterIDX);
			// 현 클러스터에서 글로벌 변형 매트릭스를 로드
			lCluster->GetTransformMatrix(lReferenceGlobalInitPosition);

			lReferenceGlobalInitPosition *= lReferenceGeometry;

			// Get the link initial global position and the link current global position.
			// 초기 T 포즈에서의 Position과 현재 포즈에서의 Position을 이어주는 Link Matrix를 제공 
			lCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);

			// Compute the initial position of the link relative to the reference.
			// 초기 포즈에서 모든 뼈대를 원점으로 만들어주는 변형 행렬을 만듦
			lClusterRelativeInitPositions[lClusterIDX] =
				lClusterGlobalInitPosition.Inverse() *
				lReferenceGlobalInitPosition;

			// Calc Weight
			mIndices = lCluster->GetControlPointIndices();
			lVertexIndexCount = lCluster->GetControlPointIndicesCount();

			mWeights = lCluster->GetControlPointWeights();

			for (k = 0; k < lVertexIndexCount; k++)
			{
				lWeightAccumulation[mIndices[k]] += mWeights[k];
			}
		}

		objData->WeightAccumulation.push_back(lWeightAccumulation);

		long long Delta = (objData->Sequences[0]->mStops[0].Get() - objData->Sequences[0]->mStarts[0].Get()) / (long long)(Frame);

		for (FrameIDX = 0; FrameIDX < (int)Frame; FrameIDX++)
		{
			for (lClusterIDX = 0; lClusterIDX < lClusterCount; ++lClusterIDX)
			{
				// 클러스터 로드
				lCluster = lSkinDeformer->GetCluster(lClusterIDX);

				lClusterGlobalCurrentPosition = lCluster->GetLink()->EvaluateGlobalTransform(Delta * FrameIDX);

				// Compute the current position of the link relative to the reference.
				// mTime 프레임 에서의 Pose를 만드는 Matrix
				lClusterRelativeCurrentPositionInverse =
					lReferenceGlobalCurrentPosition *
					lClusterGlobalCurrentPosition;

				// Compute the shift of the link relative to the reference.
				// 결과적으로 T포즈에서 mTime 프레임 에서의 Pose로 한번에 변형되는 Matrix를 계산
				res =
					objData->pGlobalPosition *
					lClusterRelativeCurrentPositionInverse *
					lClusterRelativeInitPositions[lClusterIDX];

				objData->Sequences[0]->mAnimVertexArrays[submeshIDX][FrameIDX][lClusterIDX][0]  = (float)res.mData[0].mData[0] ;
				objData->Sequences[0]->mAnimVertexArrays[submeshIDX][FrameIDX][lClusterIDX][1]  = (float)res.mData[0].mData[1];
				objData->Sequences[0]->mAnimVertexArrays[submeshIDX][FrameIDX][lClusterIDX][2]  = (float)res.mData[0].mData[2];
				objData->Sequences[0]->mAnimVertexArrays[submeshIDX][FrameIDX][lClusterIDX][3]  = (float)res.mData[0].mData[3];

				objData->Sequences[0]->mAnimVertexArrays[submeshIDX][FrameIDX][lClusterIDX][4]  = (float)res.mData[1].mData[0];
				objData->Sequences[0]->mAnimVertexArrays[submeshIDX][FrameIDX][lClusterIDX][5]  = (float)res.mData[1].mData[1];
				objData->Sequences[0]->mAnimVertexArrays[submeshIDX][FrameIDX][lClusterIDX][6]  = (float)res.mData[1].mData[2];
				objData->Sequences[0]->mAnimVertexArrays[submeshIDX][FrameIDX][lClusterIDX][7]  = (float)res.mData[1].mData[3];

				objData->Sequences[0]->mAnimVertexArrays[submeshIDX][FrameIDX][lClusterIDX][8]  = (float)res.mData[2].mData[0];
				objData->Sequences[0]->mAnimVertexArrays[submeshIDX][FrameIDX][lClusterIDX][9]  = (float)res.mData[2].mData[1];
				objData->Sequences[0]->mAnimVertexArrays[submeshIDX][FrameIDX][lClusterIDX][10] = (float)res.mData[2].mData[2];
				objData->Sequences[0]->mAnimVertexArrays[submeshIDX][FrameIDX][lClusterIDX][11] = (float)res.mData[2].mData[3];

				objData->Sequences[0]->mAnimVertexArrays[submeshIDX][FrameIDX][lClusterIDX][12] = (float)res.mData[3].mData[0];
				objData->Sequences[0]->mAnimVertexArrays[submeshIDX][FrameIDX][lClusterIDX][13] = (float)res.mData[3].mData[1];
				objData->Sequences[0]->mAnimVertexArrays[submeshIDX][FrameIDX][lClusterIDX][14] = (float)res.mData[3].mData[2];
				objData->Sequences[0]->mAnimVertexArrays[submeshIDX][FrameIDX][lClusterIDX][15] = (float)res.mData[3].mData[3];
			}
		}

#pragma endregion
	}

	// Draw an oriented camera box where the node is located.
	void FBXLoader::DrawCamera(FbxNode* pNode,
		FbxTime& pTime,
		FbxAnimLayer* pAnimLayer,
		FbxAMatrix& pGlobalPosition)
	{
		FbxAMatrix lCameraGlobalPosition;
		FbxVector4 lCameraPosition, lCameraDefaultDirection, lCameraInterestPosition;

		lCameraPosition = pGlobalPosition.GetT();

		// By default, FBX cameras point towards the X positive axis.
		FbxVector4 lXPositiveAxis(1.0, 0.0, 0.0);
		lCameraDefaultDirection = lCameraPosition + lXPositiveAxis;

		lCameraGlobalPosition = pGlobalPosition;

		// If the camera is linked to an interest, get the interest position.
		if (pNode->GetTarget())
		{
			lCameraInterestPosition = GetGlobalPosition(pNode->GetTarget(), pTime).GetT();

			// Compute the required rotation to make the camera point to it's interest.
			FbxVector4 lCameraDirection;
			FbxVector4::AxisAlignmentInEulerAngle(lCameraPosition,
				lCameraDefaultDirection,
				lCameraInterestPosition,
				lCameraDirection);

			// Must override the camera rotation 
			// to make it point to it's interest.
			lCameraGlobalPosition.SetR(lCameraDirection);
		}

		// Get the camera roll.
		FbxCamera* cam = pNode->GetCamera();
		double lRoll = 0;

		if (cam)
		{
			lRoll = cam->Roll.Get();
			FbxAnimCurve* fc = cam->Roll.GetCurve(pAnimLayer);
			if (fc) fc->Evaluate(pTime);
		}
		// GlDrawCamera(lCameraGlobalPosition, lRoll);
	}

	void FBXLoader::DrawBone(
		FbxNode* pNode,
		std::vector<FbxTime> mStops,
		FbxAMatrix& pOriginGlobalPosition,
		FbxAMatrix& pParentGlobalPosition,
		long long perFrame,
		std::ofstream& outFile
	)
	{
		std::string boneName(pNode->GetName());

		if (boneName == "Shoulder.L" || boneName == "Shoulder.R")
		{
			printf("");
		}

		try {
			int transSkeletonPairSize = transSkeletonPairs.at(boneName).size();
			if (transSkeletonPairSize == 0)
				return;
		}
		catch (std::exception&) {
			return;
		}

		FbxAMatrix global, local, finalM;
		// Default Position
		FbxVector4 localT, localS, deltaT;
		FbxVector4 localR;
		FbxQuaternion localQ;

		global = pOriginGlobalPosition;

		// 현재 뼈대 이름을 key로 갖는 values의 개수를 로드
		int transSkeletonPairsSize = transSkeletonPairs.at(boneName).size();

		outFile.write((char*)&transSkeletonPairsSize, sizeof(int));

		// 현재 뼈대 이름을 key로 갖는 value들의 이름들을 리스트에 적는다.
		for (int i = 0; i < transSkeletonPairsSize; i++)
		{
			outFile.write((const char*)transSkeletonPairs.at(boneName)[i].c_str(), transSkeletonPairs.at(boneName)[i].size());
			outFile.write("\n", 1);
		}

		int frameCount = mStops[0].GetFrameCount(FbxTime::eFrames30);

		FbxVector4 reS;
		reS.mData[0] = 0.1f;
		reS.mData[1] = 0.1f;
		reS.mData[2] = 0.1f;
		reS.mData[3] = 1.0f;

		float convBuf;
		for (int animFrame = 0; animFrame < frameCount; animFrame++)
		{
			// FRAME COUNT 만큼 반복
			local =
				GetGlobalPosition(
					pNode,
					animFrame * perFrame,
					NULL,
					&global
				);

			//////////////////////////
			finalM = local;

			// 로테이션이 초기화 됨.
			// 로테이션이 잘못된 것 같음 고쳐볼 것
			//finalM.SetR(localR);
			finalM.SetS(reS);

			localT = finalM.GetT();
			localQ = finalM.GetQ();
			localS = finalM.GetS();

			localT = localS * localT;

			//////////////////////////

			if (boneName == "Shoulder.L" || boneName == "Shoulder.R")
			{
				localT.mData[0] += 3000000.0f;
			}
			convBuf = (float)localT.mData[0];
			outFile.write((char*)&convBuf, sizeof(float));
			convBuf = (float)localT.mData[1];
			outFile.write((char*)&convBuf, sizeof(float));
			convBuf = (float)localT.mData[2];
			outFile.write((char*)&convBuf, sizeof(float));
			convBuf = 1.0f;
			outFile.write((char*)&convBuf, sizeof(float));

			convBuf = (float)localQ.mData[0];
			outFile.write((char*)&convBuf, sizeof(float));
			convBuf = (float)localQ.mData[1];
			outFile.write((char*)&convBuf, sizeof(float));
			convBuf = (float)localQ.mData[2];
			outFile.write((char*)&convBuf, sizeof(float));
			convBuf = (float)localQ.mData[3];
			outFile.write((char*)&convBuf, sizeof(float));
		}
	}
}