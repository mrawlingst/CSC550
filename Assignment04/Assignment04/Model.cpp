#include "stdafx.h"
#include "Model.h"
#include "Helpers.h"
#include <fbxsdk.h>
#include <fstream>
#include <string>

Model::Model()
{
}

Model::Model(char* fileName)
	: mFileNameToLoad(fileName), mHasNormal(false), mHasTexCoords(false), mByControlPoint(true)
{
	char* extension = strrchr(fileName, '.');

	// '.' not found
	if (extension == NULL)
	{
		err_log_("Invalid file");
		exit(1);
	}

	// If .obj is found - loadOBJ()
	if (strcmp(extension, ".obj") == 0)
	{
		//loadOBJ();
		err_log_("Tried to load %s - OBJ loading disabled", mFileNameToLoad);
	}
	// If .fbx is found - loadFBX()
	else if (strcmp(extension, ".fbx") == 0)
	{
		log_("FBX - Loading %s model", mFileNameToLoad);
		loadFBX();
	}
	// Otherwise, the extension isn't supported
	else
	{
		err_log_("Unsupported file type: %s", extension);
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
		err_log_("Could not load model - %s", mFileNameToLoad);
		return;
	}

	log_("Loading model - %s", mFileNameToLoad);

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

	log_("Loaded model - %s", mFileNameToLoad);
}

void Model::loadFBX()
{
	FbxManager* manager = FbxManager::Create();
	FbxIOSettings* ioSettings = FbxIOSettings::Create(manager, IOSROOT);
	FbxImporter* fbxImporter = FbxImporter::Create(manager, "");

	if (!fbxImporter->Initialize(mFileNameToLoad, -1, manager->GetIOSettings()))
	{
		err_log_("Could not load FBX model - %s", mFileNameToLoad);
		return;
	}

	FbxScene* scene = FbxScene::Create(manager, "myScene");
	fbxImporter->Import(scene);

	auto rootNode = scene->GetRootNode();

	int count = rootNode->GetChildCount();
	for (int i = 0; i < count; i++)
	{
		auto child = rootNode->GetChild(i);
		auto mesh = child->GetMesh();

		if (!mesh)
			continue;

		const int polyCount = mesh->GetPolygonCount();

		mHasNormal = mesh->GetElementNormalCount() > 0;
		mHasTexCoords = mesh->GetElementUVCount() > 0;
		
		auto normalMapMode = FbxGeometryElement::eNone;
		auto texCoordMapMode = FbxGeometryElement::eNone;

		if (mHasNormal)
		{
			normalMapMode = mesh->GetElementNormal(0)->GetMappingMode();
			if (normalMapMode == FbxGeometryElement::eNone)
				mHasNormal = false;

			if (mHasNormal && normalMapMode != FbxGeometryElement::eByControlPoint)
				mByControlPoint = false;
		}

		if (mHasTexCoords)
		{
			texCoordMapMode = mesh->GetElementUV(0)->GetMappingMode();
			if (texCoordMapMode == FbxGeometryElement::eNone)
				mHasTexCoords = false;

			if (mHasTexCoords && texCoordMapMode != FbxGeometryElement::eByControlPoint)
				mByControlPoint = false;
		}

		int polyVertexCount = mesh->GetControlPointsCount();
		if (!mByControlPoint)
		{
			polyVertexCount = polyCount * 3;
		}
		mVertices.resize(polyVertexCount * 4, 0.0f);
		mIndices.resize(polyCount * 3, 0);

		if (mHasNormal)
			mNormals.resize(polyVertexCount * 3, 0.0f);

		FbxStringList uvNames;
		mesh->GetUVSetNames(uvNames);
		const char* uvName = NULL;
		if (mHasTexCoords && uvNames.GetCount())
		{
			mTexCoords.resize(polyVertexCount * 2, 0.0f);
			uvName = uvNames[0];
		}

		const auto controlPoints = mesh->GetControlPoints();
		FbxVector4 currentVert;
		FbxVector4 currentNorm;
		FbxVector2 currentTexCoord;

		if (mByControlPoint)
		{
			const FbxGeometryElementNormal* normalElement = NULL;
			const FbxGeometryElementUV* uvElement = NULL;

			if (mHasNormal)
			{
				normalElement = mesh->GetElementNormal(0);
			}

			if (mHasTexCoords)
			{
				uvElement = mesh->GetElementUV(0);
			}

			for (int index = 0; index < polyVertexCount; ++index)
			{
				currentVert = controlPoints[index];
				mVertices[index * 4] = static_cast<GLfloat>(currentVert[0]);
				mVertices[index * 4 + 1] = static_cast<GLfloat>(currentVert[1]);
				mVertices[index * 4 + 2] = static_cast<GLfloat>(currentVert[2]);
				mVertices[index * 4 + 3] = 1.0f;

				if (mHasNormal)
				{
					int normalIndex = index;
					if (normalElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
					{
						normalIndex = normalElement->GetIndexArray().GetAt(index);
					}

					currentNorm = normalElement->GetDirectArray().GetAt(normalIndex);
					mNormals[index * 3] = static_cast<GLfloat>(currentNorm[0]);
					mNormals[index * 3 + 1] = static_cast<GLfloat>(currentNorm[1]);
					mNormals[index * 3 + 2] = static_cast<GLfloat>(currentNorm[2]);
				}

				if (mHasTexCoords)
				{
					int uvIndex = index;
					if (uvElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
					{
						uvIndex = uvElement->GetIndexArray().GetAt(index);
					}

					currentTexCoord = uvElement->GetDirectArray().GetAt(uvIndex);
					mNormals[index * 2] = static_cast<GLfloat>(currentTexCoord[0]);
					mNormals[index * 2 + 1] = static_cast<GLfloat>(currentTexCoord[1]);
				}
			}
		}

		int vertCount = 0;
		for (int polyIndex = 0; polyIndex < polyCount; ++polyIndex)
		{
			for (int vertIndex = 0; vertIndex < 3; ++vertIndex)
			{
				const auto controlPointIndex = mesh->GetPolygonVertex(polyIndex, vertIndex);

				if (mByControlPoint)
				{
					mIndices[vertIndex] = static_cast<GLuint>(controlPointIndex);
				}
				else
				{
					mIndices[vertIndex] = static_cast<GLuint>(vertCount);

					currentVert = controlPoints[controlPointIndex];
					mVertices[vertCount * 4] = static_cast<GLfloat>(currentVert[0]);
					mVertices[vertCount * 4 + 1] = static_cast<GLfloat>(currentVert[1]);
					mVertices[vertCount * 4 + 2] = static_cast<GLfloat>(currentVert[2]);
					mVertices[vertCount * 4 + 3] = 1.0f;

					if (mHasNormal)
					{
						mesh->GetPolygonVertexNormal(polyIndex, vertIndex, currentNorm);
						mNormals[vertCount * 3] = static_cast<GLfloat>(currentNorm[0]);
						mNormals[vertCount * 3 + 1] = static_cast<GLfloat>(currentNorm[1]);
						mNormals[vertCount * 3 + 2] = static_cast<GLfloat>(currentNorm[2]);
					}

					if (mHasTexCoords)
					{
						bool unmap;
						mesh->GetPolygonVertexUV(polyIndex, vertIndex, uvName, currentTexCoord, unmap);
						mTexCoords[vertCount * 2] = static_cast<GLfloat>(currentTexCoord[0]);
						mTexCoords[vertCount * 2 + 1] = static_cast<GLfloat>(currentTexCoord[1]);
					}
				}

				++vertCount;
			}
		}

		glGenBuffers(VBO_COUNT, mVBONames);

		glBindBuffer(GL_ARRAY_BUFFER, mVBONames[VERTEX_VBO]);
		glBufferData(GL_ARRAY_BUFFER, polyVertexCount * 4 * sizeof(GLfloat), mVertices.data(), GL_STATIC_DRAW);

		if (mHasNormal)
		{
			glBindBuffer(GL_ARRAY_BUFFER, mVBONames[NORMAL_VBO]);
			glBufferData(GL_ARRAY_BUFFER, polyVertexCount * 3 * sizeof(GLfloat), mNormals.data(), GL_STATIC_DRAW);
		}

		if (mHasTexCoords)
		{
			glBindBuffer(GL_ARRAY_BUFFER, mVBONames[UV_VBO]);
			glBufferData(GL_ARRAY_BUFFER, polyVertexCount * 2 * sizeof(GLfloat), mTexCoords.data(), GL_STATIC_DRAW);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVBONames[INDEX_VBO]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, polyCount * 3 * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);
	}
}
