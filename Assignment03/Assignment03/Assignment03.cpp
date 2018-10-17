// Assignment03.cpp : Defines the entry point for the application.

#include "stdafx.h"
#include "Assignment03.h"

// Link opengl32 and glew library
// Supports x32 and x64 build
#pragma comment(lib, "opengl32.lib")
#ifdef _WIN64
#pragma comment(lib, "../../glew/lib/64bit/glew32s.lib")
#else
#pragma comment(lib, "../../glew/lib/86bit/glew32s.lib")
#endif

//
enum SHAPES
{
	HEXPRISM,
	DIAMOND
};

// Constants
const int WIDTH = 800;
const int HEIGHT = 600;
const wchar_t* WINDOW_TITLE = TEXT("Assignment 03");
const GLfloat CLEAR_COLOR[] = {0.0, 0.0, 0.0}; // {R, G, B} - between 0.0 - 1.0
const vmath::mat4 CAMERA_PROJECTION = vmath::perspective(45.0f, 4.0f / 3.0f, 0.0f, 1000.0f);
const vmath::vec3 CAMERA_POS = vmath::vec3(0.0f, 0.0f, 0.0f);
const vmath::vec3 CAMERA_DIRECTION = vmath::vec3(0.0f, 0.0f, -1.0f); // Direction camera is looking at
const vmath::mat4 CAMERA_VIEW = vmath::lookat(CAMERA_POS, CAMERA_DIRECTION, vmath::vec3(0.0f, 1.0f, 0.0f));

const vmath::vec3 LIGHT1_POS   = vmath::vec3( 4.0f, 1.0f,  0.0f);
const vmath::vec3 LIGHT1_COLOR = vmath::vec3( 1.0f, 0.0f,  0.0f);
const vmath::vec3 LIGHT2_POS   = vmath::vec3(-1.0f, 5.0f,  5.0f);
const vmath::vec3 LIGHT2_COLOR = vmath::vec3( 0.0f, 1.0f,  0.0f);
const vmath::vec3 LIGHT3_POS   = vmath::vec3( 2.0f, 3.0f, -5.0f);
const vmath::vec3 LIGHT3_COLOR = vmath::vec3( 0.0f, 0.0f,  1.0f);

// Globals
GLuint shaderProgram;

// Shapes' buffers
GLuint triangle_vao, triangle_vbo, triangle_ebo;
GLuint plane1_vao, plane1_vbo, plane1_ebo;
GLuint plane2_vao, plane2_vbo, plane2_ebo;
GLuint plane3_vao, plane3_vbo, plane3_ebo;
GLuint hexprism_vao, hexprism_vbo, hexprism_ebo;
GLuint diamond_vao, diamond_vbo, diamond, diamond_ebo;

// Uniforms (GLSL)
GLint uniform_modelView, uniform_projection;
GLint uniform_lightPos1, uniform_lightColor1;
GLint uniform_lightPos2, uniform_lightColor2;
GLint uniform_lightPos3, uniform_lightColor3;

// Animations
SHAPES selectedShape = HEXPRISM;
float rotationY = 0.0f;
float rotationX = 0.0f;

// Forward declarations of functions
char* readFile(const char*);
void calculateNormals(const std::vector<GLfloat>&, int, const std::vector<GLuint>&, int, std::vector<GLfloat>&);
bool initOpenGL();
void draw();
void cleanup();
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Code snippet by Dr. Landon
// Just cleaned up for readability
char* readFile(const char* fileName)
{
	FILE* file = fopen(fileName, "rb");

	if (file == NULL)
	{
		return NULL;
	}

	fseek(file, 0L, SEEK_END);
	long size = ftell(file);
	fseek(file, 0L, SEEK_SET);

	char* buffer = new char[size + 1];
	fread(buffer, 1, size, file);
	buffer[size] = '\0';
	fclose(file);

	return buffer;
}

// Code snippet by Dr. Landon
// Adjusted to use std::vector
void calculateNormals(const std::vector<GLfloat>& vertex_position, int num_verts, const std::vector<GLuint>& vertex_index, int num_faces, std::vector<GLfloat>& normals)
{
	int* tempCount = new int[num_verts];

	for (int i = 0; i < num_verts; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			normals[i * 3 + j] = 0.0f;
		}
		tempCount[i] = 0;
	}

	for (int face = 0; face < num_faces; face++)
	{
		vmath::vec3 A = vmath::vec3(vertex_position[vertex_index[face * 3 + 0] * 3 + 0],
			vertex_position[vertex_index[face * 3 + 0] * 3 + 1],
			vertex_position[vertex_index[face * 3 + 0] * 3 + 2]);

		vmath::vec3 B = vmath::vec3(vertex_position[vertex_index[face * 3 + 1] * 3 + 0],
			vertex_position[vertex_index[face * 3 + 1] * 3 + 1],
			vertex_position[vertex_index[face * 3 + 1] * 3 + 2]);

		vmath::vec3 C = vmath::vec3(vertex_position[vertex_index[face * 3 + 2] * 3 + 0],
			vertex_position[vertex_index[face * 3 + 2] * 3 + 1],
			vertex_position[vertex_index[face * 3 + 2] * 3 + 2]);

		vmath::vec3 normal = vmath::normalize(vmath::cross(B - A, C - A));

		normals[vertex_index[face * 3 + 0] * 3 + 0] += normal[0];
		normals[vertex_index[face * 3 + 0] * 3 + 1] += normal[1];
		normals[vertex_index[face * 3 + 0] * 3 + 2] += normal[2];
		tempCount[vertex_index[face * 3 + 0]]++;

		normals[vertex_index[face * 3 + 1] * 3 + 0] += normal[0];
		normals[vertex_index[face * 3 + 1] * 3 + 1] += normal[1];
		normals[vertex_index[face * 3 + 1] * 3 + 2] += normal[2];
		tempCount[vertex_index[face * 3 + 1]]++;

		normals[vertex_index[face * 3 + 2] * 3 + 0] += normal[0];
		normals[vertex_index[face * 3 + 2] * 3 + 1] += normal[1];
		normals[vertex_index[face * 3 + 2] * 3 + 2] += normal[2];
		tempCount[vertex_index[face * 3 + 2]]++;
	}

	for (int i = 0; i < num_verts; i++)
	{
		normals[i * 3 + 0] /= (float)tempCount[i];
		normals[i * 3 + 1] /= (float)tempCount[i];
		normals[i * 3 + 2] /= (float)tempCount[i];
	}
}

bool initOpenGL()
{
	GLenum error = glewInit();

	if (GLEW_OK != error)
	{
		return false;
	}

#pragma region Shader Setup
	// Read shader files
	char* vs_source = readFile("vertex.vert");
	char* fs_source = readFile("fragment.frag");

	// Used to check for shader compile/program link result
	GLint result = GL_FALSE;

	// Create and compile vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vs_source, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE)
	{
		char error[4096];
		glGetShaderInfoLog(vertexShader, 4096, NULL, error);

		OutputDebugStringA("Vertex Shader");
		OutputDebugStringA(error);
	}

	// Create and compile fragment shader
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, (const GLchar**)&fs_source, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE)
	{
		char error[4096];
		glGetShaderInfoLog(fragShader, 4096, NULL, error);

		OutputDebugStringA("Fragment Shader");
		OutputDebugStringA(error);
	}

	// Attach shaders to program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);

	// Link program to gpu
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	if (result != GL_TRUE)
	{
		char error[4096];
		glGetProgramInfoLog(shaderProgram, 4096, NULL, error);

		OutputDebugStringA("Program Linking");
		OutputDebugStringA(error);
	}

	// Get locations of uniforms in shader
	uniform_projection = glGetUniformLocation(shaderProgram, "projection");
	uniform_modelView = glGetUniformLocation(shaderProgram, "modelView");
	uniform_lightPos1 = glGetUniformLocation(shaderProgram, "lightPos1");
	uniform_lightColor1 = glGetUniformLocation(shaderProgram, "lightColor1");
	uniform_lightPos2 = glGetUniformLocation(shaderProgram, "lightPos2");
	uniform_lightColor2 = glGetUniformLocation(shaderProgram, "lightColor2");
	uniform_lightPos3 = glGetUniformLocation(shaderProgram, "lightPos3");
	uniform_lightColor3 = glGetUniformLocation(shaderProgram, "lightColor3");
#pragma endregion

	GLuint in_position;
	GLuint in_color;
	GLuint in_normal;

#pragma region Mesh - Triangle
	// Vertex Array Object
	glGenVertexArrays(1, &triangle_vao);
	glBindVertexArray(triangle_vao);

	// Vertex Buffer Object
	glGenBuffers(1, &triangle_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo);

	// Element Buffer Object
	glGenBuffers(1, &triangle_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_ebo);

	glBufferData(GL_ARRAY_BUFFER,
		TRIANGLE_VERTICES.size() * sizeof(GLfloat) + TRIANGLE_COLORS.size() * sizeof(GLfloat),
		NULL,
		GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, TRIANGLE_VERTICES.size() * sizeof(GLfloat), &TRIANGLE_VERTICES[0]); // Vertices
	glBufferSubData(GL_ARRAY_BUFFER, TRIANGLE_VERTICES.size() * sizeof(GLfloat), TRIANGLE_COLORS.size() * sizeof(GLfloat), &TRIANGLE_COLORS[0]); // Colors

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, TRIANGLE_INDICES.size() * sizeof(GLuint), &TRIANGLE_INDICES[0], GL_STATIC_DRAW); // Indices

	in_position = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(in_position, 3, GL_FLOAT, GL_FALSE, 0, 0); // position

	in_color = glGetAttribLocation(shaderProgram, "color");
	glVertexAttribPointer(in_color, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(TRIANGLE_VERTICES.size() * sizeof(GLfloat)));
#pragma endregion

#pragma region Mesh - Plane 1
	//calculateNormals(PLANE1_VERTICES, PLANE1_VERTICES.size() / 3, PLANE1_INDICES, PLANE1_INDICES.size() / 3, PLANE1_NORMALS);

	// Vertex Array Object
	glGenVertexArrays(1, &plane1_vao);
	glBindVertexArray(plane1_vao);

	// Vertex Buffer Object
	glGenBuffers(1, &plane1_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, plane1_vbo);

	// Element Buffer Object
	glGenBuffers(1, &plane1_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, plane1_ebo);

	glBufferData(GL_ARRAY_BUFFER,
		PLANE1_VERTICES.size() * sizeof(GLfloat) + PLANE1_COLORS.size() * sizeof(GLfloat) + PLANE1_NORMALS.size() * sizeof(GLfloat),
		NULL,
		GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, PLANE1_VERTICES.size() * sizeof(GLfloat), &PLANE1_VERTICES[0]); // Vertices
	glBufferSubData(GL_ARRAY_BUFFER, PLANE1_VERTICES.size() * sizeof(GLfloat), PLANE1_COLORS.size() * sizeof(GLfloat), &PLANE1_COLORS[0]); // Colors
	glBufferSubData(GL_ARRAY_BUFFER, PLANE1_VERTICES.size() * sizeof(GLfloat) + PLANE1_COLORS.size() * sizeof(GLfloat), PLANE1_NORMALS.size() * sizeof(GLfloat), &PLANE1_NORMALS[0]); // Normals

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, PLANE1_INDICES.size() * sizeof(GLuint), &PLANE1_INDICES[0], GL_STATIC_DRAW); // Indices

	in_position = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(in_position, 3, GL_FLOAT, GL_FALSE, 0, 0); // position

	in_color = glGetAttribLocation(shaderProgram, "color");
	glVertexAttribPointer(in_color, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(PLANE1_VERTICES.size() * sizeof(GLfloat))); // color

	in_normal = glGetAttribLocation(shaderProgram, "normal");
	glVertexAttribPointer(in_normal, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(PLANE1_VERTICES.size() * sizeof(GLfloat) + PLANE1_COLORS.size() * sizeof(GLfloat)));
#pragma endregion

#pragma region Mesh - Plane 2
	// Vertex Array Object
	glGenVertexArrays(1, &plane2_vao);
	glBindVertexArray(plane2_vao);

	// Vertex Buffer Object
	glGenBuffers(1, &plane2_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, plane2_vbo);

	// Element Buffer Object
	glGenBuffers(1, &plane2_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, plane2_ebo);

	glBufferData(GL_ARRAY_BUFFER,
		PLANE2_VERTICES.size() * sizeof(GLfloat) + PLANE2_COLORS.size() * sizeof(GLfloat) + PLANE2_NORMALS.size() * sizeof(GLfloat),
		NULL,
		GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, PLANE2_VERTICES.size() * sizeof(GLfloat), &PLANE2_VERTICES[0]); // Vertices
	glBufferSubData(GL_ARRAY_BUFFER, PLANE2_VERTICES.size() * sizeof(GLfloat), PLANE2_COLORS.size() * sizeof(GLfloat), &PLANE2_COLORS[0]); // Colors
	glBufferSubData(GL_ARRAY_BUFFER, PLANE2_VERTICES.size() * sizeof(GLfloat) + PLANE2_COLORS.size() * sizeof(GLfloat), PLANE2_NORMALS.size() * sizeof(GLfloat), &PLANE2_NORMALS[0]); // Normals

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, PLANE2_INDICES.size() * sizeof(GLuint), &PLANE2_INDICES[0], GL_STATIC_DRAW); // Indices

	in_position = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(in_position, 3, GL_FLOAT, GL_FALSE, 0, 0); // position

	in_color = glGetAttribLocation(shaderProgram, "color");
	glVertexAttribPointer(in_color, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(PLANE2_VERTICES.size() * sizeof(GLfloat))); // color

	in_normal = glGetAttribLocation(shaderProgram, "normal");
	glVertexAttribPointer(in_normal, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(PLANE2_VERTICES.size() * sizeof(GLfloat) + PLANE2_COLORS.size() * sizeof(GLfloat)));
#pragma endregion

#pragma region Mesh - Plane 3
	// Vertex Array Object
	glGenVertexArrays(1, &plane3_vao);
	glBindVertexArray(plane3_vao);

	// Vertex Buffer Object
	glGenBuffers(1, &plane3_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, plane3_vbo);

	// Element Buffer Object
	glGenBuffers(1, &plane3_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, plane3_ebo);

	glBufferData(GL_ARRAY_BUFFER,
		PLANE3_VERTICES.size() * sizeof(GLfloat) + PLANE3_COLORS.size() * sizeof(GLfloat) + PLANE3_NORMALS.size() * sizeof(GLfloat),
		NULL,
		GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, PLANE3_VERTICES.size() * sizeof(GLfloat), &PLANE3_VERTICES[0]); // Vertices
	glBufferSubData(GL_ARRAY_BUFFER, PLANE3_VERTICES.size() * sizeof(GLfloat), PLANE3_COLORS.size() * sizeof(GLfloat), &PLANE3_COLORS[0]); // Colors
	glBufferSubData(GL_ARRAY_BUFFER, PLANE3_VERTICES.size() * sizeof(GLfloat) + PLANE3_COLORS.size() * sizeof(GLfloat), PLANE3_NORMALS.size() * sizeof(GLfloat), &PLANE3_NORMALS[0]); // Normals

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, PLANE3_INDICES.size() * sizeof(GLuint), &PLANE3_INDICES[0], GL_STATIC_DRAW); // Indices

	in_position = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(in_position, 3, GL_FLOAT, GL_FALSE, 0, 0); // position

	in_color = glGetAttribLocation(shaderProgram, "color");
	glVertexAttribPointer(in_color, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(PLANE3_VERTICES.size() * sizeof(GLfloat))); // color

	in_normal = glGetAttribLocation(shaderProgram, "normal");
	glVertexAttribPointer(in_normal, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(PLANE3_VERTICES.size() * sizeof(GLfloat) + PLANE3_COLORS.size() * sizeof(GLfloat)));
#pragma endregion

#pragma region Mesh - Hexagonal Prism
	calculateNormals(HEXPRISM_VERTICES, HEXPRISM_VERTICES.size() / 3, HEXPRISM_INDICES, HEXPRISM_INDICES.size() / 3, HEXPRISM_NORMALS);

	// Vertex Array Object
	glGenVertexArrays(1, &hexprism_vao);
	glBindVertexArray(hexprism_vao);

	// Vertex Buffer Object
	glGenBuffers(1, &hexprism_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, hexprism_vbo);

	// Element Buffer Object
	glGenBuffers(1, &hexprism_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hexprism_ebo);

	glBufferData(GL_ARRAY_BUFFER,
		HEXPRISM_VERTICES.size() * sizeof(GLfloat) + HEXPRISM_COLORS.size() * sizeof(GLfloat) + HEXPRISM_NORMALS.size() * sizeof(GLfloat),
		NULL,
		GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, HEXPRISM_VERTICES.size() * sizeof(GLfloat), &HEXPRISM_VERTICES[0]); // Vertices
	glBufferSubData(GL_ARRAY_BUFFER, HEXPRISM_VERTICES.size() * sizeof(GLfloat), HEXPRISM_COLORS.size() * sizeof(GLfloat), &HEXPRISM_COLORS[0]); // Colors
	glBufferSubData(GL_ARRAY_BUFFER, HEXPRISM_VERTICES.size() * sizeof(GLfloat) + HEXPRISM_COLORS.size() * sizeof(GLfloat), HEXPRISM_NORMALS.size() * sizeof(GLfloat), &HEXPRISM_NORMALS[0]); // Normals

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, HEXPRISM_INDICES.size() * sizeof(GLuint), &HEXPRISM_INDICES[0], GL_STATIC_DRAW); // Indices

	in_position = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(in_position, 3, GL_FLOAT, GL_FALSE, 0, 0); // position

	in_color = glGetAttribLocation(shaderProgram, "color");
	glVertexAttribPointer(in_color, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(HEXPRISM_VERTICES.size() * sizeof(GLfloat))); // color

	in_normal = glGetAttribLocation(shaderProgram, "normal");
	glVertexAttribPointer(in_normal, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(HEXPRISM_VERTICES.size() * sizeof(GLfloat) + HEXPRISM_COLORS.size() * sizeof(GLfloat)));
#pragma endregion

#pragma region Mesh - Diamond
	calculateNormals(DIAMOND_VERTICES, DIAMOND_VERTICES.size() / 3, DIAMOND_INDICES, DIAMOND_INDICES.size() / 3, DIAMOND_NORMALS);

	// Vertex Array Object
	glGenVertexArrays(1, &diamond_vao);
	glBindVertexArray(diamond_vao);

	// Vertex Buffer Object
	glGenBuffers(1, &diamond_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, diamond_vbo);

	// Element Buffer Object
	glGenBuffers(1, &diamond_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, diamond_ebo);

	glBufferData(GL_ARRAY_BUFFER,
		DIAMOND_VERTICES.size() * sizeof(GLfloat) + DIAMOND_COLORS.size() * sizeof(GLfloat) + DIAMOND_NORMALS.size() * sizeof(GLfloat),
		NULL,
		GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, DIAMOND_VERTICES.size() * sizeof(GLfloat), &DIAMOND_VERTICES[0]); // Vertices
	glBufferSubData(GL_ARRAY_BUFFER, DIAMOND_VERTICES.size() * sizeof(GLfloat), DIAMOND_COLORS.size() * sizeof(GLfloat), &DIAMOND_COLORS[0]); // Colors
	glBufferSubData(GL_ARRAY_BUFFER, DIAMOND_VERTICES.size() * sizeof(GLfloat) + DIAMOND_COLORS.size() * sizeof(GLfloat), DIAMOND_NORMALS.size() * sizeof(GLfloat), &DIAMOND_NORMALS[0]); // Normals

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, DIAMOND_INDICES.size() * sizeof(GLuint), &DIAMOND_INDICES[0], GL_STATIC_DRAW); // Indices

	in_position = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(in_position, 3, GL_FLOAT, GL_FALSE, 0, 0); // position

	in_color = glGetAttribLocation(shaderProgram, "color");
	glVertexAttribPointer(in_color, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(DIAMOND_VERTICES.size() * sizeof(GLfloat))); // color

	in_normal = glGetAttribLocation(shaderProgram, "normal");
	glVertexAttribPointer(in_normal, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(DIAMOND_VERTICES.size() * sizeof(GLfloat) + DIAMOND_COLORS.size() * sizeof(GLfloat)));
#pragma endregion

	glEnable(GL_CULL_FACE);

	return true;
}

// Draw everything here - called in loop
void draw()
{
	glClearColor(CLEAR_COLOR[0], CLEAR_COLOR[1], CLEAR_COLOR[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);

	// Uniforms
	//glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, projection);
	glProgramUniform3fv(shaderProgram, uniform_lightPos1, 1, LIGHT1_POS);
	glProgramUniform3fv(shaderProgram, uniform_lightColor1, 1, LIGHT1_COLOR);
	glProgramUniform3fv(shaderProgram, uniform_lightPos2, 1, LIGHT2_POS);
	glProgramUniform3fv(shaderProgram, uniform_lightColor2, 1, LIGHT2_COLOR);
	glProgramUniform3fv(shaderProgram, uniform_lightPos3, 1, LIGHT3_POS);
	glProgramUniform3fv(shaderProgram, uniform_lightColor3, 1, LIGHT3_COLOR);

	vmath::mat4 projection;
	vmath::mat4 modelView;
	GLuint in_position;
	GLuint in_color;
	GLuint in_normal;

	// Draw triangle
	/*projection = CAMERA_PROJECTION * CAMERA_VIEW * vmath::translate(TRIANGLE_POS);
	glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, projection);
	glBindVertexArray(triangle_vao);
	in_position = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(in_position);
	in_color = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(in_color);
	glDrawElements(GL_TRIANGLES, TRIANGLE_INDICES.size(), GL_UNSIGNED_INT, NULL);
	glDisableVertexAttribArray(in_position);
	glDisableVertexAttribArray(in_color);*/

	// Draw Plane 2
	modelView = CAMERA_VIEW * vmath::translate(PLANE2_POS);
	projection = CAMERA_PROJECTION * modelView;
	glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, projection);
	glUniformMatrix4fv(uniform_modelView, 1, GL_FALSE, modelView);
	glBindVertexArray(plane2_vao);
	in_position = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(in_position);
	in_color = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(in_color);
	in_normal = glGetAttribLocation(shaderProgram, "normal");
	glEnableVertexAttribArray(in_normal);
	glDrawElements(GL_TRIANGLES, PLANE2_INDICES.size(), GL_UNSIGNED_INT, NULL);
	glDisableVertexAttribArray(in_position);
	glDisableVertexAttribArray(in_color);
	glDisableVertexAttribArray(in_normal);

	// Draw Plane 3
	modelView = CAMERA_VIEW * vmath::translate(PLANE3_POS);
	projection = CAMERA_PROJECTION * modelView;
	glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, projection);
	glUniformMatrix4fv(uniform_modelView, 1, GL_FALSE, modelView);
	glBindVertexArray(plane3_vao);
	in_position = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(in_position);
	in_color = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(in_color);
	in_normal = glGetAttribLocation(shaderProgram, "normal");
	glEnableVertexAttribArray(in_normal);
	glDrawElements(GL_TRIANGLES, PLANE3_INDICES.size(), GL_UNSIGNED_INT, NULL);
	glDisableVertexAttribArray(in_position);
	glDisableVertexAttribArray(in_color);
	glDisableVertexAttribArray(in_normal);

	// Draw Plane 1
	modelView = CAMERA_VIEW * vmath::translate(PLANE1_POS);
	projection = CAMERA_PROJECTION * modelView;
	glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, projection);
	glUniformMatrix4fv(uniform_modelView, 1, GL_FALSE, modelView);
	glBindVertexArray(plane1_vao);
	in_position = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(in_position);
	in_color = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(in_color);
	in_normal = glGetAttribLocation(shaderProgram, "normal");
	glEnableVertexAttribArray(in_normal);
	glDrawElements(GL_TRIANGLES, PLANE1_INDICES.size(), GL_UNSIGNED_INT, NULL);
	glDisableVertexAttribArray(in_position);
	glDisableVertexAttribArray(in_color);
	glDisableVertexAttribArray(in_normal);

	// Draw Hexagonal Prism
	modelView = CAMERA_VIEW * vmath::translate(HEXPRISM_POS);
	if (selectedShape == HEXPRISM)
	{
		modelView *= vmath::rotate(rotationX, 1.0f, 0.0f, 0.0f) * vmath::rotate(rotationY, 0.0f, 1.0f, 0.0f);
	}
	projection = CAMERA_PROJECTION * modelView;
	glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, projection);
	glUniformMatrix4fv(uniform_modelView, 1, GL_FALSE, modelView);
	glBindVertexArray(hexprism_vao);
	in_position = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(in_position);
	in_color = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(in_color);
	in_normal = glGetAttribLocation(shaderProgram, "normal");
	glEnableVertexAttribArray(in_normal);
	glDrawElements(GL_TRIANGLES, HEXPRISM_INDICES.size(), GL_UNSIGNED_INT, NULL);
	glDisableVertexAttribArray(in_position);
	glDisableVertexAttribArray(in_color);
	glDisableVertexAttribArray(in_normal);

	// Draw Diamond
	projection = CAMERA_PROJECTION * CAMERA_VIEW * vmath::translate(DIAMOND_POS);
	if (selectedShape == DIAMOND)
	{
		projection *= vmath::rotate(rotationX, 1.0f, 0.0f, 0.0f) *vmath::rotate(rotationY, 0.0f, 1.0f, 0.0f);
	}
	glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, projection);
	glUniformMatrix4fv(uniform_modelView, 1, GL_FALSE, modelView);
	glBindVertexArray(diamond_vao);
	in_position = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(in_position);
	in_color = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(in_color);
	in_normal = glGetAttribLocation(shaderProgram, "normal");
	glEnableVertexAttribArray(in_normal);
	glDrawElements(GL_TRIANGLES, DIAMOND_INDICES.size(), GL_UNSIGNED_INT, NULL);
	glDisableVertexAttribArray(in_position);
	glDisableVertexAttribArray(in_color);
	glDisableVertexAttribArray(in_normal);
}

// Clean up and delete any resources
void cleanup()
{
	glDeleteVertexArrays(1, &triangle_vao);
	glDeleteBuffers(1, &triangle_vbo);
	glDeleteBuffers(1, &triangle_ebo);

	glDeleteVertexArrays(1, &plane1_vao);
	glDeleteBuffers(1, &plane1_vbo);
	glDeleteBuffers(1, &plane1_ebo);

	glDeleteVertexArrays(1, &plane2_vao);
	glDeleteBuffers(1, &plane2_vbo);
	glDeleteBuffers(1, &plane2_ebo);

	glDeleteVertexArrays(1, &plane3_vao);
	glDeleteBuffers(1, &plane3_vbo);
	glDeleteBuffers(1, &plane3_ebo);

	glDeleteVertexArrays(1, &hexprism_vao);
	glDeleteBuffers(1, &hexprism_vbo);
	glDeleteBuffers(1, &hexprism_ebo);

	glDeleteVertexArrays(1, &diamond_vao);
	glDeleteBuffers(1, &diamond_vbo);
	glDeleteBuffers(1, &diamond_ebo);
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	WNDCLASS windowClass;

	ZeroMemory(&windowClass, sizeof(windowClass));

	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = &WndProc;
	windowClass.hInstance = ::GetModuleHandle(NULL);
	windowClass.lpszClassName = WINDOW_TITLE;

	RegisterClass(&windowClass);

	// Create window
	HWND window = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		WINDOW_TITLE, WINDOW_TITLE,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		(GetSystemMetrics(SM_CXSCREEN) / 2) - (WIDTH / 2), (GetSystemMetrics(SM_CYSCREEN) / 2) - (HEIGHT / 2), // Position the window at center of monitor
		WIDTH, HEIGHT,
		NULL, NULL,
		hInstance,
		NULL);

	if (!window)
	{
		// Failed to create window
		return FALSE;
	}

	HDC deviceContext = GetDC(window);

	PIXELFORMATDESCRIPTOR pfd;

	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24; // Size of RGB (8, 8, 8)
	pfd.cRedBits   =  8; // R
	pfd.cGreenBits =  8; // G
	pfd.cBlueBits  =  8; // B
	pfd.cDepthBits = 32;

	int iPixelFormat = ChoosePixelFormat(deviceContext, &pfd);
	SetPixelFormat(deviceContext, iPixelFormat, &pfd);

	HGLRC renderingContext = wglCreateContext(deviceContext);
	wglMakeCurrent(deviceContext, renderingContext);

	if (!initOpenGL())
	{
		return FALSE;
	}

	// Get events (window, inputs, etc.)
	MSG message;
	for (;;)
	{
		if (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			switch (message.wParam)
			{
			case VK_LEFT:
				rotationY -= 0.5f;
				break;

			case VK_RIGHT:
				rotationY += 0.5f;
				break;

			case VK_UP:
				rotationX -= 0.5f;
				break;

			case VK_DOWN:
				rotationX += 0.5f;
				break;

			case 0x31:
				selectedShape = HEXPRISM;
				break;

			case 0x32:
				selectedShape = DIAMOND;
				break;

			case 0x33:
				break;

			default:
				break;
			}

			// Pressing X button to close
			if (message.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		draw();

		// Double Buffering
		SwapBuffers(deviceContext);
	}

	// Done with the program, clean up
	cleanup();
	ReleaseDC(window, deviceContext);
	DestroyWindow(window);
	UnregisterClass(WINDOW_TITLE, GetModuleHandle(NULL));

	return (int)message.wParam;
}

// Handle any window events
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_SIZE:
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
    }

	return DefWindowProc(hWnd, message, wParam, lParam);
}
