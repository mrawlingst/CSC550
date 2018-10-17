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

// Plane #1 - Bottom
const vmath::vec3 PLANE1_POS = vmath::vec3(0.0f, -1.0f, 0.0f);
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

std::vector<GLfloat> PLANE1_NORMALS = std::vector<GLfloat>(PLANE1_VERTICES.size(), 1.0f);

// Plane #2 - Left
const vmath::vec3 PLANE2_POS = vmath::vec3(-10.0f, 0.0f, 0.0f);
const std::vector<GLfloat> PLANE2_VERTICES =
{
	0.0f, -50.0f, -50.0f, // 0 - bottom left
	0.0f,  50.0f, -50.0f, // 1 - top left
	0.0f, -50.0f,  50.0f, // 2 - bottom right
	0.0f,  50.0f,  50.0f  // 3 - top right
};

const std::vector<GLuint> PLANE2_INDICES =
{
	0, 1, 2,
	2, 1, 3
};

const std::vector<GLfloat> PLANE2_COLORS =
{
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f
};

std::vector<GLfloat> PLANE2_NORMALS = std::vector<GLfloat>(PLANE2_VERTICES.size(), 1.0f);

// Plane #3 - Right
const vmath::vec3 PLANE3_POS = vmath::vec3(10.0f, 0.0f, 0.0f);
const std::vector<GLfloat> PLANE3_VERTICES =
{
	0.0f, -50.0f, -50.0f, // 0 - bottom left
	0.0f,  50.0f, -50.0f, // 1 - top left
	0.0f, -50.0f,  50.0f, // 2 - bottom right
	0.0f,  50.0f,  50.0f  // 3 - top right
};

const std::vector<GLuint> PLANE3_INDICES =
{
	0, 1, 2,
	2, 1, 3
};

const std::vector<GLfloat> PLANE3_COLORS =
{
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f
};

std::vector<GLfloat> PLANE3_NORMALS = std::vector<GLfloat>(PLANE3_VERTICES.size(), 1.0f);

// Hexagonal Prism
const vmath::vec3 HEXPRISM_POS = vmath::vec3(0.0f, 0.0f, -2.0f);
const std::vector<GLfloat> HEXPRISM_VERTICES =
{
	-0.25f,  0.00f,  0.00f,
	-0.25f,  0.25f, -0.25f,
	-0.25f,  0.00f, -0.50f,
	-0.25f, -0.25f, -0.25f,
	-0.25f, -0.25f,  0.25f,
	-0.25f,  0.00f,  0.50f,
	-0.25f,  0.25f,  0.25f,
	 0.25f,  0.00f,  0.00f,
	 0.25f,  0.25f, -0.25f,
	 0.25f,  0.00f, -0.50f,
	 0.25f, -0.25f, -0.25f,
	 0.25f, -0.25f,  0.25f,
	 0.25f,  0.00f,  0.50f,
	 0.25f,  0.25f,  0.25f
};

const std::vector<GLuint> HEXPRISM_INDICES =
{
	// Left side
	0, 1, 2,
	0, 2, 3,
	0, 3, 4,
	0, 4, 5,
	0, 5, 6,
	0, 1, 6,

	// Bar
	1, 6, 8,
	6, 8, 13,
	5, 6, 13,
	5, 12, 13,
	4, 5, 12,
	4, 11, 12,
	3, 4, 11,
	3, 10, 11,
	2, 3, 10,
	2, 9, 10,
	1, 2, 9,
	1, 8, 9,

	// Right side
	7, 8, 9,
	7, 9, 10,
	7, 10, 11,
	7, 11, 12,
	7, 12, 13,
	7, 8, 13
};

const std::vector<GLfloat> HEXPRISM_COLORS =
{
	0.0f, 0.5f, 0.5f,
	0.0f, 0.5f, 0.5f,
	0.0f, 0.5f, 0.5f,
	0.0f, 0.5f, 0.5f,
	0.0f, 0.5f, 0.5f,
	0.0f, 0.5f, 0.5f,
	0.0f, 0.5f, 0.5f,
	0.0f, 0.5f, 0.5f,
	0.0f, 0.5f, 0.5f,
	0.0f, 0.5f, 0.5f,
	0.0f, 0.5f, 0.5f,
	0.0f, 0.5f, 0.5f,
	0.0f, 0.5f, 0.5f,
	0.0f, 0.5f, 0.5f,
};

std::vector<GLfloat> HEXPRISM_NORMALS = std::vector<GLfloat>(HEXPRISM_VERTICES.size());

// Diamond
const vmath::vec3 DIAMOND_POS = vmath::vec3(-0.75f, 0.0f, -2.0f);
const std::vector<GLfloat> DIAMOND_VERTICES =
{
	 0.000f,  0.5f,  0.000f,
	 0.000f,  0.0f, -0.250f,
	-0.125f,  0.0f, -0.125f,
	-0.250f,  0.0f,  0.000f,
	-0.125f,  0.0f,  0.125f,
	 0.000f,  0.0f,  0.250f,
	 0.125f,  0.0f,  0.125f,
	 0.250f,  0.0f,  0.000f,
	 0.125f,  0.0f, -0.125f,
	 0.000f, -0.5f,  0.000f,
};

const std::vector<GLuint> DIAMOND_INDICES =
{
	// Top
	0, 1, 2,
	0, 2, 3,
	0, 3, 4,
	0, 4, 5,
	0, 5, 6,
	0, 6, 7,
	0, 7, 8,
	0, 1, 8,

	// Bottom
	9, 1, 2,
	9, 2, 3,
	9, 3, 4,
	9, 4, 5,
	9, 5, 6,
	9, 6, 7,
	9, 7, 8,
	9, 1, 8
};

const std::vector<GLfloat> DIAMOND_COLORS =
{
	0.3f, 0.7f, 0.7f,
	0.3f, 0.7f, 0.7f,
	0.3f, 0.7f, 0.7f,
	0.3f, 0.7f, 0.7f,
	0.3f, 0.7f, 0.7f,
	0.3f, 0.7f, 0.7f,
	0.3f, 0.7f, 0.7f,
	0.3f, 0.7f, 0.7f,
	0.3f, 0.7f, 0.7f,
	0.3f, 0.7f, 0.7f,
};

std::vector<GLfloat> DIAMOND_NORMALS = std::vector<GLfloat>(DIAMOND_VERTICES.size());
