//=============================================================================
// File:		Application.h
// Created:		2015/02/10
// Last Edited:	2015/02/27
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
//  DSGraphics
#include "DSGraphics/Camera.h"
#include "DSGraphics/ModelAsset.h"
#include "DSGraphics/ModelInstance.h"
#include "DSGraphics/Program.h"
#include "DSGraphics/Texture.h"
//  Object
//   Abstracts
//   Aesthetics
//   Environmentals
#include "Object/Environmental/Individual/Wall.h"
//   Player
#include "Object/Player/Individual/SpaceshipStarter.h"
//   Units

//=============================================================================
// Forward Declarations
//=============================================================================

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
	void Load();
		void LoadCamera();
		void LoadShaders();
			DSGraphics::Program* CreateProgram(const char* vertexShaderFile, const char* fragmentShaderFile);
		void LoadTextures();
		void LoadObjects();
			void LoadObjectsAbstracts();
			void LoadObjectsAesthetics();
			void LoadObjectsEnvironmentals();
			void LoadObjectsPlayers();
			void LoadObjectsUnits();
	void CreateInitialInstances();
		void CreateInitialInstancesAbstracts();
		void CreateInitialInstancesAesthetics();
		void CreateInitialInstancesEnvironmentals();
		void CreateInitialInstancesPlayers();
		void CreateInitialInstancesUnits();


	// Run Sub-Functions
	void Input();
	void AI();
	void Physics();
	void Render();


	// Terminate Sub-Functions
	void CleanUp();
		void CleanUpObjects();
		void CleanUpInstances();


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
	DSGraphics::Camera* mpCamera;
	GLuint mUniformCamera;

	// Shader Programs
	DSGraphics::Program* mpProgramColorOnly;
	DSGraphics::Program* mpProgramTexAndColor;

	// Textures
	DSGraphics::Texture* mpTextureSpaceship;

	// Objects
	//  Abstracts
	//  Aesthetics
	//  Environmentals
	Wall* mpWall;
	//  Player
	SpaceshipStarter* mpSpaceshipStarter;
	//  Units
	
	// Instance Lists
	std::vector<DSGraphics::ModelInstance> mAbstractsInstanceList;
	std::vector<DSGraphics::ModelInstance> mAestheticsInstanceList;
	std::vector<DSGraphics::ModelInstance> mEnvironmentalsInstanceList;
	std::vector<DSGraphics::ModelInstance> mPlayersInstanceList;
	std::vector<DSGraphics::ModelInstance> mUnitsInstanceList;

	// Individual Objects
	//  Spaceship
	float mDegreesRotated;
};

#endif //#ifndef APPLICATION_H