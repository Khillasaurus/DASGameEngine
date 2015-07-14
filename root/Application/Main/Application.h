//=============================================================================
// File:		Application.h
// Created:		2015/02/10
// Last Edited:	2015/02/12
// Copyright:	Daniel Schenker
// Description:	Application
//=============================================================================

#ifndef APPLICATION_H
#define APPLICATION_H

//=============================================================================
//Includes
//=============================================================================

// Third-Party Libraries
//  GLEW (must be included before the OpenGL headers or GLFW)
#define GLEW_STATIC
#include <GL/glew.h>

//  GLFW
#include <GLFW/glfw3.h>

//  GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Standard C++ Libraries

// Daniel Schenker

//=============================================================================
// Forward Declarations
//=============================================================================

class Program;
class Texture;

//=============================================================================
//Enums
//=============================================================================

//=============================================================================
//Class Declarations
//=============================================================================

class Application
{
public:
	//Constructors
	Application();
	//Destructor
	~Application();

	//Member Functions
private:
	// General
	void Initialize();
	void Run();
	void Terminate();
	void CleanUp();

	// GLFW
	bool InitializeGLFW();
	void TerminateGLFW();

	// GLEW
	bool InitializeGLEW();

	// Initialize Sub-Functions
	void LoadRectangle();
	void LoadShaders();

	// Run Sub-Functions
	void Render();

	//Member Variables
private:
	//create a variable that holds the default data path of "../../Resources/"
	
	// General
	bool mQuit;
	
	// Window
	GLFWwindow* mpWindow;
	glm::vec2 mWindowSize;
	const char* mWindowTitle;

	// Data
	//  Programs
	Program* mpProgram001;
	//  Rectangle
	//   VAO
	GLuint mRectVao;
	//   Model Vertices
	GLfloat* mpRectVertices;//TODO: change this to be a vector or something that knows the length of the array
	//   Model-View-Projection (mvp) Matrix
	glm::mat4 mRectMvp;
	//    Model Matrix
	glm::mat4 mRectModel;
	//     Scale
	glm::mat4 mRectScale;
	//     Rotate
	glm::mat4 mRectRotate;
	glm::vec3 mRectRotationAxis;
	//     Translate
	glm::mat4 mRectTranslate;
	//    View Matrix
	glm::mat4 mRectView;
	//    Projection Matrix
	glm::mat4 mRectProj;
	//   MVP Uniform Handle
	GLuint mRectUniMvp;
	//   VBO
	GLuint mRectVbo;
	//   EBO
	GLuint mRectEbo;
	GLuint* mpRectElements;//TODO: make same change as with mpRectVertices
	//  Textures
	Texture* mpTex;

};

#endif //#ifndef APPLICATION_H