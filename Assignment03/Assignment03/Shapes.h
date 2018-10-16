#pragma once

#include "Assignment03.h"

const vmath::vec3 TRIANGLE_POS = vmath::vec3(0.0f, 1.0f, -10.0f);
const std::vector<GLfloat> TRIANGLE_VERTICES =
{
	-1.0f, -1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	 0.0f,  1.0f, 0.0f
};

const std::vector<GLuint> TRIANGLE_INDICES =
{
	0, 1, 2
};

const std::vector<GLfloat> TRIANGLE_COLORS =
{
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f
};

// Plane #1
const vmath::vec3 PLANE1_POS = vmath::vec3(0.0, -1.0f, 0.0f);
const std::vector<GLfloat> PLANE1_VERTICES =
{
	-100.0f, -1.0f,  50.0f, // 0 - bottom left
	-100.0f,  1.0f, -50.0f, // 1 - top left
	 100.0f, -1.0f,  50.0f, // 2 - bottom right
	 100.0f,  1.0f, -50.0f  // 3 - top right
};

const std::vector<GLuint> PLANE1_INDICES =
{
	0, 1, 2,
	2, 1, 3
};

const std::vector<GLfloat> PLANE1_COLORS =
{
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f
};
