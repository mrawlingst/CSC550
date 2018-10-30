#pragma once

#pragma warning(disable:4996)

#include "../../include/GL/glew.h"
#include "../../include/vmath.h"
#include <vector>

class Model
{
public:
	Model();
	Model(char* fileName);

	~Model();

	void draw(vmath::mat4 world, vmath::mat4 view, vmath::mat4 projection);
	void setShaderProgram(GLuint program);

private:
	std::vector<GLfloat> mVertices; // Vector of vertices in model
	std::vector<GLuint> mIndices; // Vector of indices in model
	std::vector<GLfloat> mNormals; // Vector of normals in model
	std::vector<GLfloat> mColors; // Vector of colors in model
	std::vector<GLfloat> mTexCoords; // Vector of tex coords in model

	bool mHasNormal;
	bool mHasTexCoords;
	bool mByControlPoint;

	const char* mFileNameToLoad;

	// Handle IDs
	GLuint mVAO, mVBO, mEBO;
	GLuint mShaderProgram;
	GLint mUniformProjection, mUniformModelView;

	enum
	{
		VERTEX_VBO,
		NORMAL_VBO,
		UV_VBO,
		INDEX_VBO,
		VBO_COUNT
	};

	GLuint mVBONames[VBO_COUNT];

private:
	void loadOBJ();
	void loadFBX();
};
