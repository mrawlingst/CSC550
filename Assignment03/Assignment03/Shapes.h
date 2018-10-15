#pragma once

#include "Assignment03.h"

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

const std::vector<GLfloat> PLANE1_VERTICES =
{
	-1.0f, -1.0f,  5.0f, // 0 - bottom left
	-1.0f,  1.0f, -5.0f, // 1 - top left
	 1.0f, -1.0f,  5.0f, // 2 - bottom right
	 1.0f,  1.0f, -5.0f  // 3 - top right
};

const std::vector<GLuint> PLANE1_INDICES =
{
	0, 1, 2,
	2, 1, 3
};
