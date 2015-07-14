//=============================================================================
// File:		Application.h
// Created:		2015/02/10
// Last Edited:	2015/02/15
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
#include <list>

// Daniel Schenker

//=============================================================================
// Forward Declarations
//=============================================================================

class Camera;
class ModelAsset;
class ModelInstance;
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

	// GLFW
	bool InitializeGLFW();
	void TerminateGLFW();

	// GLEW
	bool InitializeGLEW();

	// Initialize Sub-Functions
	void LoadAssets();
		void LoadModelAssets();
			void LoadModelAssetCube();
	Program* LoadShaders(const char* vertexShaderFile, const char* fragmentShaderFile);

	// Run Sub-Functions
	void Input();
	void AI();
	void Physics();
	void Render();


	// Terminate Sub-Functions
	void CleanUp();
		void CleanUpAssets();
		void CleanUpInstances();

	//Test Functions
	void CreateInstances();

	//Member Variables
private:
	//create a variable that holds the default data path of "../../Resources/"
	
	// General
	bool mQuit;
	
	// Window
	GLFWwindow* mpWindow;
	glm::vec2 mWindowSize;
	const char* mWindowTitle;

	// Time
	double mLastTime;
	double mThisTime;
	double mElapsedTime;

	// Camera
	Camera* mpCamera;
	GLuint mUniformCamera;

	// Shader Programs
	Program* mpProgramCube;

	// Textures
	Texture* mpTextureCube;

	// Assets & Instances
	//  Models
	ModelAsset* mpModelAssetCube;
	std::list<ModelInstance> mListModelInstances;

	
	GLuint mRectUniformModel;
	//temp
	GLfloat mRectDegreesRotated;
};

#endif //#ifndef APPLICATION_H