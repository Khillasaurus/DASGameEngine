//=============================================================================
// File:		Application.cpp
// Created:		2015/02/10
// Last Edited:	2015/02/17
// Copyright:	Daniel Schenker
// Description:	Application
//=============================================================================

//=============================================================================
//Includes
//=============================================================================

// Standard C++ Libraries
#include <iostream>//check
#include <stdio.h>//check
#include <time.h>//check
#include <vector>//check

// Daniel Schenker
#include "Application.h"

//=============================================================================
//Function Prototypes
//=============================================================================

void OnError(int errorCode, const char* msg);

//=============================================================================
//Statics
//=============================================================================

//=============================================================================
//Class Definitions
//=============================================================================

//-----------------------------------------------------------------------------
// Constuctors
//-----------------------------------------------------------------------------

Application::Application()
	//General
:	mQuit(false)
	//Window
,	mpWindow(nullptr)
,	mWindowSize(1280, 720)
,	mWindowTitle("Daniel Schenker's Application")
	//Time
,	mLastTime(0.0)
,	mThisTime(0.0)
,	mElapsedTime(0.0)
	//Camera
,	mpCamera(nullptr)
,	mUniformCamera(0)
	//Shader Programs
,	mpProgramCube(nullptr)
	//Textures
,	mpTextureCube(nullptr)
	//Data
	//  Models
,	mpModelAssetCube(nullptr)
//,	mListModelInstances()//not sure how to initialize this yet

,	mRectUniformModel(0)
	//temp
,	mRectDegreesRotated(0.0f)
{
	Initialize();
	Run();
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------

Application::~Application()
{
	Terminate();
}

//-----------------------------------------------------------------------------
// Public Member Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Private Member Functions
//-----------------------------------------------------------------------------
//  General

//   1. Game initalization
void Application::Initialize()
{

	if(InitializeGLFW() == true)
	{
		if(InitializeGLEW() == true)
		{
			//General
			// print out some info about the graphics drivers
			std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
			std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
			std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
			std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

			//OpenGL Settings
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//Camera
			if(mpCamera == nullptr)
			{
				mpCamera = new DSGraphics::Camera();
				mpCamera->SetPosition(glm::vec3(0.0f, 0.0f, 4.0f));
				mpCamera->SetViewportAspectRatio(mWindowSize.x / mWindowSize.y);
			}
			else
			{
				fprintf(stderr, "WARNING: mpCamera is being used due to it not having a default value of nullptr. Attempting to continue regardless. A memory leak may occur.\n");
			}

			//Shaders
			if(mpProgramCube == nullptr)
			{
				mpProgramCube = LoadShaders("Shaders/vertexShader.txt", "Shaders/fragmentShader.txt");
			}
			else
			{
				fprintf(stderr, "WARNING: mpProgramCube is being used due to it not having a default value of nullptr. Attempting to continue regardless. A memory leak may occur.\n");
			}

			//Textures
			if(mpTextureCube == nullptr)
			{
				mpTextureCube = new DSGraphics::Texture("../../Resources/Textures/stripes2.png");
			}
			else
			{
				fprintf(stderr, "WARNING: mpTextureCube is being used due to it not having a default value of nullptr. Attempting to continue regardless. A memory leak may occur.\n");
			}

			//Individual Objects
			// Assets
			//  Models
			LoadAssets();
			//  Instances
			CreateInstances();
		}
	}
}

//-----------------------------------------------------------------------------

//   2. Main game loop
void Application::Run()
{
	mLastTime = glfwGetTime();

	//[Closed] Event Loop
	while(mQuit == false)//while(glfwWindowShouldClose(mpWindow) == false)
	{
		//Time
		mThisTime = glfwGetTime();
		mElapsedTime = mThisTime - mLastTime;

		Input();
		AI();
		Physics();
		Render();

		mLastTime = mThisTime;
	}

	//Close the window
	glfwSetWindowShouldClose(mpWindow, GL_TRUE);
}

//-----------------------------------------------------------------------------

//   3. Game shutdown
void Application::Terminate()
{
	CleanUp();
	TerminateGLFW();
}

//-----------------------------------------------------------------------------
//  GLFW

bool Application::InitializeGLFW()
{
	glfwSetErrorCallback(OnError);
	if(glfwInit() == GL_TRUE)
	{
		//  Configuration

		//Minimum Version 3.2
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		//is the window resizeable? No
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		//create the actual window
		mpWindow = glfwCreateWindow(static_cast<int>(mWindowSize.x), static_cast<int>(mWindowSize.y), mWindowTitle, nullptr, nullptr);//Windowed
		//mpWindow = glfwCreateWindow(static_cast<int>(mWindowSize.x), static_cast<int>(mWindowSize.y), mWindowTitle, glfwGetPrimaryMonitor(), nullptr);
		if(mpWindow == nullptr)
		{
			throw std::runtime_error("ERROR: Failed to open a GLFW window. Please update your graphics card drivers to a version that supports OpenGL 4.0 or higher.");
			mQuit = false;

			return false;
		}

		//OpenGL context - set pWindow to active
		glfwMakeContextCurrent(mpWindow);

		return true;
	}
	else
	{
		throw std::runtime_error("ERROR: Failed to initialize GLFW.");
		mQuit = false;

		return false;
	}
}

//-----------------------------------------------------------------------------

void Application::TerminateGLFW()
{
	glfwTerminate();
	if(mpWindow != nullptr)
	{
		//no need to delete data contained in mpWindow since glfwTerminate() takes care of that.
		mpWindow = nullptr;
	}
}

//-----------------------------------------------------------------------------
//  GLEW

bool Application::InitializeGLEW()
{
	//Force GLEW to use a modern OpenGL method for checking if a function is available in case the supported extensions given by experimental or pre-release drivers are incorrect or lacking info.
	glewExperimental = GL_TRUE;
	if(glewInit() == GLEW_OK)
	{
		// make sure OpenGL version 4.0 API is available
		if(!GLEW_VERSION_4_0)
		{
			throw std::runtime_error("ERROR: OpenGL 4.0 API is not available.");
		}

		return true;
	}
	else
	{
		throw std::runtime_error("ERROR: Failed to initialize GLEW.");

		return false;
	}
}

//-----------------------------------------------------------------------------
// Initialize Sub-Functions
//-----------------------------------------------------------------------------

void Application::LoadAssets()
{
	//Models
	LoadModelAssets();
}

//-----------------------------------------------------------------------------

void Application::LoadModelAssets()
{
	//Cube
	LoadModelAssetCube();
}

//-----------------------------------------------------------------------------

void Application::LoadModelAssetCube()
{
	//  Cube
	const unsigned int kVertexCount = 8;
	const unsigned int kDataBitsPerVertex = 9;

	GLfloat* pVertices = new GLfloat[kVertexCount * kDataBitsPerVertex]
	{
		//	  2---3
		//	 /|  /|
		//	0---1 |
		//	| 6-|-7
		//	|/  |/
		//	4---5

		//Position				TexCoords		Color: RGBA
		
		//Top
		-0.5f,  0.5f,  0.5f,	0.0f, 1.0f,		1.0f,  0.0f,  0.0f, 1.0f,	//0 0.0f,	
		 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,		1.0f,  0.5f,  0.0f, 1.0f,	//1 1.0f,	
		-0.5f,  0.5f, -0.5f,	0.0f, 0.0f,		0.0f,  1.0f,  0.0f, 1.0f,	//2 0.0f,	
		 0.5f,  0.5f, -0.5f,	1.0f, 0.0f,		1.0f,  1.0f,  0.0f, 1.0f,	//3 0.7f,	

		//Bottom
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,		0.0f,  1.0f,  1.0f, 1.0f,	//4 1.0f,	
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,		0.0f,  0.0f,  1.0f, 1.0f,	//5 0.0f,	
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,		1.0f,  0.0f,  1.0f, 1.0f,	//6 0.5f,	
		 0.5f, -0.5f, -0.5f,	1.0f, 1.0f,		0.5f,  0.0f,  1.0f, 1.0f	//7 0.5f	
	};

	const unsigned int kElementCount = 3 * 2 * 6;
	GLuint* pElements = new GLuint[kElementCount]
	{
		//Top
		0, 1, 2,
		3, 2, 1,

		//Front
		4, 5, 0,
		1, 0, 5,

		//Right
		5, 7, 1,
		3, 1, 7,

		//Back
		7, 6, 3,
		2, 3, 6,

		//Left
		6, 4, 2,
		0, 2, 4,

		//Bot
		6, 7, 4,
		5, 4, 7
	};

	mpModelAssetCube = new DSGraphics::ModelAsset
	(
		mpProgramCube,
		true,
		mpTextureCube,
		3,
		true,
		5,
		kVertexCount,
		3,
		2,
		4,
		pVertices,
		true,
		kElementCount,
		pElements,
		GL_LINES
	);

	// Clean up temp variables
	if(pVertices != nullptr)
	{
		delete[] pVertices;
		pVertices = nullptr;
	}
	if(pElements != nullptr)
	{
		delete[] pElements;
		pElements = nullptr;
	}
}

//-----------------------------------------------------------------------------

DSGraphics::Program* Application::LoadShaders(const char* vertexShaderFile, const char* fragmentShaderFile)
{
	std::vector<DSGraphics::Shader> shaders;
	shaders.push_back(DSGraphics::Shader::CreateShaderFromFile(vertexShaderFile, GL_VERTEX_SHADER));
	shaders.push_back(DSGraphics::Shader::CreateShaderFromFile(fragmentShaderFile, GL_FRAGMENT_SHADER));
	return new DSGraphics::Program(shaders);
}

//-----------------------------------------------------------------------------
// Run Sub-Functions
//-----------------------------------------------------------------------------

void Application::Input()
{
	//Retrieve
	glfwPollEvents();


	//Camera
	const float kMoveSpeed = 2.0f; //units per second

	// Reset
	if(glfwGetKey(mpWindow, '`'))
	{
		mpCamera->SetPosition(glm::vec3(0.0f, 0.0f, 4.0f));
	}

	//Poll in counter clockwise direction, starting at 0 radians/degrees (right)
	// Right
	else if(glfwGetKey(mpWindow, 'D'))
	{
		mpCamera->OffsetPosition(static_cast<float>(mElapsedTime) * kMoveSpeed * mpCamera->GetDirectionRight());
	}
	// Forward
	else if(glfwGetKey(mpWindow, 'W'))
	{
		mpCamera->OffsetPosition(static_cast<float>(mElapsedTime) * kMoveSpeed * mpCamera->GetDirectionForward());
	}
	// Left
	else if(glfwGetKey(mpWindow, 'A'))
	{
		mpCamera->OffsetPosition(static_cast<float>(mElapsedTime) * kMoveSpeed * (-mpCamera->GetDirectionRight()));
	}
	// Backwards
	else if(glfwGetKey(mpWindow, 'S'))
	{
		mpCamera->OffsetPosition(static_cast<float>(mElapsedTime) * kMoveSpeed * (-mpCamera->GetDirectionForward()));
	}
	// Up
	else if(glfwGetKey(mpWindow, 'E'))
	{
		mpCamera->OffsetPosition(static_cast<float>(mElapsedTime) * kMoveSpeed * mpCamera->GetDirectionUp());
	}
	// Down
	else if(glfwGetKey(mpWindow, 'Q'))
	{
		mpCamera->OffsetPosition(static_cast<float>(mElapsedTime) * kMoveSpeed * (-mpCamera->GetDirectionUp()));
	}


	//Individual Objects
	// Rect
	if(glfwGetKey(mpWindow, 'X'))
	{
		mListModelInstances.front().SetRotationAxis(glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else if(glfwGetKey(mpWindow, 'Y'))
	{
		mListModelInstances.front().SetRotationAxis(glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if(glfwGetKey(mpWindow, 'Z'))
	{
		mListModelInstances.front().SetRotationAxis(glm::vec3(0.0f, 0.0f, 1.0f));
	}


	//If key pressed: escape
	if(glfwGetKey(mpWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		mQuit = true;
	}
}

//-----------------------------------------------------------------------------

void Application::AI()
{
}

//-----------------------------------------------------------------------------

void Application::Physics()
{
	//Individual Objects
	// Rect
	const GLfloat kDegreesPerSecond = 90.0f;
	mRectDegreesRotated += static_cast<float>(mElapsedTime) * kDegreesPerSecond;
	while(mRectDegreesRotated > 360.0f)
	{
		mRectDegreesRotated -= 360.0f;
	}
	//  Update model based on changes that may have occured during the input stage
	//TODO: Add static ID to instances in order to keep track of which is which, so that I don't do the following risky business
	mListModelInstances.front().SetRotate(glm::rotate(glm::mat4(1.0f), glm::radians(mRectDegreesRotated), mListModelInstances.front().GetRotationAxis()));
	mListModelInstances.front().UpdateTransform();
}

//-----------------------------------------------------------------------------

void Application::Render()
{
	//Background
	// Clear the screen to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Instances
	//TODO: learn what kind of iterator I need here
	std::list<DSGraphics::ModelInstance>::iterator it;
	for(it = mListModelInstances.begin(); it != mListModelInstances.end(); ++it)
	{
		it->Render();
	}

	//Swap the back buffer and the front buffer
	glfwSwapBuffers(mpWindow);
}

//-----------------------------------------------------------------------------
// Terminate Sub-Functions
//-----------------------------------------------------------------------------

void Application::CleanUp()
{
	//Camera
	if(mpCamera != nullptr)
	{
		delete mpCamera;
		mpCamera = nullptr;
	}

	//Shaders
	if(mpProgramCube != nullptr)
	{
		delete mpProgramCube;
		mpProgramCube = nullptr;
	}
	//Textures
	if(mpTextureCube != nullptr)
	{
		delete mpTextureCube;
		mpTextureCube = nullptr;
	}

	//Assets
	CleanUpAssets();

	//Instances
	CleanUpInstances();
}

//-----------------------------------------------------------------------------

void Application::CleanUpAssets()
{
	//Cube
	if(mpModelAssetCube != nullptr)
	{
		delete mpModelAssetCube;
		mpModelAssetCube = nullptr;
	}
}

//-----------------------------------------------------------------------------

void Application::CleanUpInstances()
{
	mListModelInstances.clear();
}

//-----------------------------------------------------------------------------
// Test Functions
//-----------------------------------------------------------------------------

void Application::CreateInstances()
{
	DSGraphics::ModelInstance cube0001(mpModelAssetCube, mpCamera);
	mListModelInstances.push_back(cube0001);

	DSGraphics::ModelInstance cube0002(mpModelAssetCube, mpCamera);
	cube0002.SetScale(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));
	cube0002.SetTranslate(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -1.5f, -2.5f)));
	cube0002.UpdateTransform();
	mListModelInstances.push_back(cube0002);

	//Set initial rotation axis for cube00001
	mListModelInstances.front().SetRotationAxis(glm::vec3(0.0f, 1.0f, 0.0f));
}

//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------

void OnError(int errorCode, const char* msg)
{
	throw std::runtime_error(msg);
}

//-----------------------------------------------------------------------------
// Getters
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Setters
//-----------------------------------------------------------------------------