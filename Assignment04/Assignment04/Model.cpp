#include "stdafx.h"
#include "Model.h"
#include <fstream>
#include <string>

Model::Model()
{
}

Model::Model(const char* fileName, ModelFileType modelType)
	: mFileNameToLoad(fileName)
{
	switch (modelType)
	{
	case ModelFileType::FBX:
		break;

	default:
		loadOBJ();
		break;
	}
}

Model::~Model()
{
}

void Model::draw()
{
}

void Model::loadOBJ()
{
	mVertices.clear();
	mIndices.clear();
	mNormals.clear();
	mColors.clear();
	mTexCoords.clear();

	FILE* file = fopen(mFileNameToLoad, "r");

	if (file == NULL)
	{
		OutputDebugStringA("Could not load model :");
		OutputDebugStringA(mFileNameToLoad);
		OutputDebugStringA("\n");
		return;
	}

	OutputDebugStringA("Loading model: ");
	OutputDebugStringA(mFileNameToLoad);
	OutputDebugStringA("\n");

	// http://www.martinreddy.net/gfx/3d/OBJ.spec
	// Specs of an OBJ file
	char line[128];
	GLfloat vertX = 0, vertY = 0, vertZ = 0;

	while (fscanf(file, "%s", line) != EOF)
	{
		switch (line[0])
		{
		// v is part of vertex data
		case 'v':
			// Check for 2nd letter to determine what kind of vertex data
			switch (line[1])
			{
			// 'v' - straight up vertex (x, y, z)
			case '\0':
				fscanf(file, "%f %f %f", &vertX, &vertY, &vertZ);
				mVertices.push_back(vertX);
				mVertices.push_back(vertY);
				mVertices.push_back(vertZ);
				break;

			// 'vt' - texture vertex (x, y)
			case 't':
				fscanf(file, "%f %f", &vertX, &vertY);
				mTexCoords.push_back(vertX);
				mTexCoords.push_back(vertY);
				break;

			// 'vn' - vertex normal (x, y, z)
			case 'n':
				fscanf(file, "%f %f %f", &vertX, &vertY, &vertZ);
				mNormals.push_back(vertX);
				mNormals.push_back(vertY);
				mNormals.push_back(vertZ);
				break;

			// Unknown token
			default:
				break;
			}
			break;

		// f is face
		// According to manual spec, the format is: v/vt/vn (vertex/texture/normal)
		// but if face does not have texture, the format is: v//vn (vertex//normal)
		// example 1: f 1/1/1 2/2/1 3/3/1	-- with texture
		// example 2: f 1//1 2//2 3//3		-- no texture
		case 'f':
			GLuint index = 0;
			char line[128];
			fscanf(file, "%s", line);
			
			// Check if the face only has 2 values (v//vn)
			if (strstr(line, "//"))
			{
				sscanf(line, "%d//%d", &index);
				mIndices.push_back(index - 1);
				while (fscanf(file, "%d//%d", &index) > 0)
				{
					mIndices.push_back(index - 1);
				}
			}
			// Check if the face has 3 values (v/vt/vn)
			else if (sscanf(line, "%d/%d/%d", &index) == 3)
			{
				mIndices.push_back(index - 1);
				while (fscanf(file, "%d/%d/%d", &index) > 0)
				{
					mIndices.push_back(index - 1);
				}
			}
			break;
		}
	} // Done reading OBJ file

	// Resize colors vector to same size as vertices vector
	mColors.resize(mVertices.size(), 0.0f);

	// Vertex Array Object
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	// Vertex Buffer Object
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	// Element Buffer Object
	glGenBuffers(1, &mEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);

	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(mVertices) + sizeof(mColors) + sizeof(mNormals) + sizeof(mTexCoords),
		NULL,
		GL_STATIC_DRAW);
	
	// Vertices
	glBufferSubData(GL_ARRAY_BUFFER,
		0,
		sizeof(mVertices),
		&mVertices[0]);

	// Colors
	glBufferSubData(GL_ARRAY_BUFFER,
		sizeof(mVertices),
		sizeof(mColors),
		&mColors[0]);

	// Normals
	glBufferSubData(GL_ARRAY_BUFFER,
		sizeof(mVertices) + sizeof(mColors),
		sizeof(mNormals),
		&mNormals[0]);

}

void Model::loadFBX()
{
}
