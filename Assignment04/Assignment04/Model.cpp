#include "stdafx.h"
#include "Model.h"
#include "Helpers.h"
#include <fstream>
#include <string>

Model::Model()
{
}

Model::Model(char* fileName)
	: mFileNameToLoad(fileName)
{
	char* extension = strrchr(fileName, '.');

	// '.' not found
	if (extension == NULL)
	{
		OutputDebugStringA("Invalid file\n");
		exit(1);
	}

	// If .obj is found - loadOBJ()
	if (strcmp(extension, ".obj") == 0)
	{
		//loadOBJ();
		OutputDebugStringA("OBJ loading disabled\n");
		err_log_("Tried to load %s - OBJ loading disabled", mFileNameToLoad);
	}
	// If .fbx is found - loadFBX()
	else if (strcmp(extension, ".fbx") == 0)
	{
		loadFBX();
	}
	// Otherwise, the extension isn't supported
	else
	{
		OutputDebugStringA("Unsupported file type: ");
		OutputDebugStringA(extension);
		OutputDebugStringA("\n");
		exit(1);
	}
}

Model::~Model()
{
}

void Model::draw(vmath::mat4 world, vmath::mat4 view, vmath::mat4 projection)
{
	glUseProgram(mShaderProgram);
	glBindVertexArray(mVAO);

	vmath::mat4 modelView = view * world;

	glUniformMatrix4fv(mUniformProjection, 1, GL_FALSE, projection);
	glUniformMatrix4fv(mUniformModelView, 1, GL_FALSE, modelView);

	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, NULL);
}

void Model::setShaderProgram(GLuint program)
{
	mShaderProgram = program;

	// in vec3 position;
	GLuint in_position = glGetAttribLocation(mShaderProgram, "position");
	glVertexAttribPointer(in_position, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(in_position);

	// in vec3 color;
	GLuint in_color = glGetAttribLocation(mShaderProgram, "color");
	glVertexAttribPointer(in_color, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(GLfloat) * mVertices.size()));
	glEnableVertexAttribArray(in_color);

	// in vec3 normal;
	GLuint in_normal = glGetAttribLocation(mShaderProgram, "normal");
	glVertexAttribPointer(in_normal, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(GLfloat) * (mVertices.size() + mColors.size())));
	glEnableVertexAttribArray(in_normal);

	// in vec2 texCoord;
	GLuint in_texCoord = glGetAttribLocation(mShaderProgram, "texCoord");
	glVertexAttribPointer(in_texCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(GLfloat) * (mVertices.size() + mColors.size() + mNormals.size())));
	glEnableVertexAttribArray(in_texCoord);

	// Uniforms
	mUniformProjection = glGetUniformLocation(mShaderProgram, "projection");
	mUniformModelView = glGetUniformLocation(mShaderProgram, "modelView");
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
	mColors.resize(mVertices.size(), 0.5f);

	// Vertex Array Object
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	// Vertex Buffer Object
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	// Element Buffer Object - Indices
	glGenBuffers(1, &mEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(GLfloat) * (mVertices.size() + mColors.size() + mNormals.size() + mTexCoords.size()),
		NULL,
		GL_STATIC_DRAW);
	
	// Vertices
	glBufferSubData(GL_ARRAY_BUFFER,
		0,
		sizeof(GLfloat) * mVertices.size(),
		mVertices.data());

	// Colors
	glBufferSubData(GL_ARRAY_BUFFER,
		sizeof(GLfloat) * mVertices.size(),
		sizeof(GLfloat) * mColors.size(),
		mColors.data());

	// Normals
	glBufferSubData(GL_ARRAY_BUFFER,
		sizeof(GLfloat) * (mVertices.size() + mColors.size()),
		sizeof(GLfloat) * mNormals.size(),
		mNormals.data());

	// Tex Coords
	glBufferSubData(GL_ARRAY_BUFFER,
		sizeof(GLfloat) * (mVertices.size() + mColors.size() + mNormals.size()),
		sizeof(GLfloat) * mTexCoords.size(),
		mTexCoords.data());

	OutputDebugStringA("Loaded model: ");
	OutputDebugStringA(mFileNameToLoad);
	OutputDebugStringA("\n");
}

void Model::loadFBX()
{
	OutputDebugStringA("FBX Loading not implemented yet.\n");
}
