// Assignment03.cpp : Defines the entry point for the application.

#include "stdafx.h"
#include "Assignment04.h"

// Link opengl32 and glew library
// Supports x32 and x64 build
#pragma comment(lib, "opengl32.lib")
#ifdef _WIN64
#pragma comment(lib, "../../glew/lib/64bit/glew32s.lib")
#else
#pragma comment(lib, "../../glew/lib/86bit/glew32s.lib")
#endif

// Constants
const int WIDTH = 800;
const int HEIGHT = 600;
const wchar_t* WINDOW_TITLE = TEXT("Assignment 04");
const GLfloat CLEAR_COLOR[] = { 0.0, 0.0, 0.0 }; // {R, G, B} - between 0.0 - 1.0
const vmath::mat4 CAMERA_PROJECTION = vmath::perspective(45.0f, 4.0f / 3.0f, 0.0f, 1000.0f);
const vmath::vec3 CAMERA_POSITION = vmath::vec3(0.0f, 0.0f, -6.0f);
const vmath::vec3 CAMERA_TARGET = vmath::vec3(0.0f, 0.0f, 0.0f);
const vmath::mat4 CAMERA_VIEW = vmath::lookat(CAMERA_POSITION, CAMERA_TARGET, vmath::vec3(0.0f, 1.0f, 0.0f));

// Globals
GLuint shaderProgram;

Model* model;
char* gModelToLoad;

// Uniforms (GLSL)
GLint uniform_mv, uniform_proj;

// Forward declarations of functions
char* readFile(const char*);
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

bool initOpenGL()
{
	GLenum error = glewInit();

	if (GLEW_OK != error)
	{
		return false;
	}

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

		OutputDebugStringA("Vertex Shader ");
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

		OutputDebugStringA("Fragment Shader ");
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
	uniform_mv = glGetUniformLocation(shaderProgram, "mv_matrix");
	uniform_proj = glGetUniformLocation(shaderProgram, "proj_matrix");

	model = new Model(gModelToLoad);
	model->setShaderProgram(shaderProgram);

	return true;
}

// Draw everything here - called in loop
void draw()
{
	glClearColor(CLEAR_COLOR[0], CLEAR_COLOR[1], CLEAR_COLOR[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);

	vmath::mat4 world = vmath::mat4::identity();
	model->draw(world, CAMERA_VIEW, CAMERA_PROJECTION);
}

// Clean up and delete any resources
void cleanup()
{
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	gModelToLoad = lpCmdLine;

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
	pfd.cRedBits = 8; // R
	pfd.cGreenBits = 8; // G
	pfd.cBlueBits = 8; // B
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
