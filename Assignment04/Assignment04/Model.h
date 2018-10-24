#pragma once

#pragma warning(disable:4996)

#include "../../include/GL/glew.h"
#include "../../include/vmath.h"
#include <vector>

enum class ModelFileType
{
	OBJ,
	FBX
};

class Model
{
public:
	Model();
	Model(const char* fileName, ModelFileType modelType = ModelFileType::OBJ);

	~Model();

	void draw(vmath::mat4 world, vmath::mat4 view, vmath::mat4 projection);
	void setShaderProgram(GLuint program);

private:
	std::vector<GLfloat> mVertices; // Vector of vertices in model
	std::vector<GLuint> mIndices; // Vector of indices in model
	std::vector<GLfloat> mNormals; // Vector of normals in model
	std::vector<GLfloat> mColors; // Vector of colors in model
	std::vector<GLfloat> mTexCoords; // Vector of tex coords in model

	const char* mFileNameToLoad;

	// Handle IDs
	GLuint mVAO, mVBO, mEBO;
	GLuint mShaderProgram;
	GLint mUniformProjection, mUniformModelView;

private:
	void loadOBJ();
	void loadFBX();
};
