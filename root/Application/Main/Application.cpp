//=============================================================================
// File:		Application.cpp
// Created:		2015/02/10
// Last Edited:	2015/02/19
// Copyright:	Daniel Schenker
// Description:	Application
//=============================================================================

//=============================================================================
//Includes
//=============================================================================

// Standard C++ Libraries
#include <iostream>//check
#include <stdio.h>//check
#include <time.h>
#include <vector>

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
,	mpProgramColorOnly(nullptr)
,	mpProgramTexAndColor(nullptr)
//Textures
,	mpTextureSpaceship(nullptr)
//Models
,	mpModelAssetWall(nullptr)
,	mpModelAssetSpaceship(nullptr)
//Individual Objects
,	kDCPerM(0.01f)//REMEMBER: Change back to 0.01f
,	mDegreesRotated(0.0f)
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
			Load();
			CreateInitialInstances();
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
		//Make sure OpenGL version 4.0 API is available
		if(!GLEW_VERSION_4_0)
		{
			throw std::runtime_error("ERROR: OpenGL 4.0 API is not available.");
		}

		//Print out some info about the graphics drivers
		std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
		std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

		//OpenGL Settings
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

void Application::Load()
{
		LoadCamera();
		LoadShaders();
		LoadTextures();
		LoadAssets();
}

//-----------------------------------------------------------------------------

void Application::LoadCamera()
{
	//Camera
	if(mpCamera == nullptr)
	{
		mpCamera = new DSGraphics::Camera();
		mpCamera->SetPosition(glm::vec3(100.0f * kDCPerM, 100.0f * kDCPerM, 100.0f * kDCPerM));
		mpCamera->SetOrientation(glm::vec3(60.0f, 0.0f, 0.0f));
		//mpCamera->OffsetOrientation(0.0f, 45.0f);//temp
		mpCamera->SetViewportAspectRatio(mWindowSize.x / mWindowSize.y);
	}
	else
	{
		fprintf(stderr, "WARNING: mpCamera is being used due to it not having a default value of nullptr. Attempting to continue regardless. Camera values may be incorrect unless this is intentional.\n");
	}
}

//-----------------------------------------------------------------------------

void Application::LoadShaders()
{
	// Wall
	if(mpProgramColorOnly == nullptr)
	{
		mpProgramColorOnly = CreateProgram("Shaders/ColorOnly.VertexShader", "Shaders/ColorOnly.FragmentShader");
	}
	else
	{
		fprintf(stderr, "WARNING: mpProgramColorOnly is being used due to it not having a default value of nullptr. Attempting to continue regardless. Incorrect rendering may occur unless this is intentional.\n");
	}

	//Tex and Color
	if(mpProgramTexAndColor == nullptr)
	{
		mpProgramTexAndColor = CreateProgram("Shaders/TexAndColor.VertexShader", "Shaders/TexAndColor.FragmentShader");
	}
	else
	{
		fprintf(stderr, "WARNING: mpProgramTexAndColor is being used due to it not having a default value of nullptr. Attempting to continue regardless. Incorrect rendering may occur unless this is intentional.\n");
	}
}

//-----------------------------------------------------------------------------

DSGraphics::Program* Application::CreateProgram(const char* vertexShaderFile, const char* fragmentShaderFile)
{
	std::vector<DSGraphics::Shader> shaders;
	shaders.push_back(DSGraphics::Shader::CreateShaderFromFile(vertexShaderFile, GL_VERTEX_SHADER));
	shaders.push_back(DSGraphics::Shader::CreateShaderFromFile(fragmentShaderFile, GL_FRAGMENT_SHADER));
	return new DSGraphics::Program(shaders);
}

//-----------------------------------------------------------------------------

void Application::LoadTextures()
{
	//Textures
	if(mpTextureSpaceship == nullptr)
	{
		mpTextureSpaceship = new DSGraphics::Texture("../../Resources/Textures/stripes.png");
	}
	else
	{
		fprintf(stderr, "WARNING: mpTextureSpaceship is being used due to it not having a default value of nullptr. Attempting to continue regardless. Incorrect texture may appear unless this is intentional.\n");
	}
}

//-----------------------------------------------------------------------------

void Application::LoadAssets()
{
	//Models
	LoadModelAssets();
}

//-----------------------------------------------------------------------------

void Application::LoadModelAssets()
{
	//Wall
	LoadModelAssetWall();

	//Spaceship
	LoadModelAssetSpaceship();
}

//-----------------------------------------------------------------------------

void Application::LoadModelAssetWall()
{
	//Vertices

	const unsigned int kVertexCount = 4;
	const unsigned int kPositionDimensions = 3;
	const unsigned int kColorDimensions = 4;
	const unsigned int kDataBitsPerVertex = kPositionDimensions + kColorDimensions;

	GLfloat* pVertices = new GLfloat[kVertexCount * kDataBitsPerVertex]
	{
		/*
			2---3
			|   |		1m^2 wall
			0---1
		*/

		//Position								Color
		//x			y			z				r		g		b		a

		//Outside
		 0.0f,		 0.0f,		 0.0f,			1.0f,	0.0f,	0.0f,	1.0f,	//0
		 kDCPerM,	 0.0f,		 0.0f,			1.0f,	1.0f,	0.0f,	1.0f,	//1
		 0.0f,		 kDCPerM,	 0.0f,			1.0f,	0.0f,	1.0f,	1.0f,	//2
		 kDCPerM,	 kDCPerM,	 0.0f,			0.0f,	0.0f,	1.0f,	1.0f	//3
	};

	//Elements

	const unsigned int kElementsPerDrawType = 3;//Triangle
	const unsigned int kDrawTypeElements = 2;//2 triangles per square
	const unsigned int kElementCount = kElementsPerDrawType * kDrawTypeElements;

	GLuint* pElements = new GLuint[kElementCount]
	{
		0, 1, 2,
		3, 2, 1
	};

	//ModelAsset Creation

	if(mpModelAssetWall == nullptr)
	{
		mpModelAssetWall = new DSGraphics::ModelAsset
		(
			mpProgramColorOnly,	//Program
			false,				//Has Texture?
			nullptr,			//Texture
			0,					//Texture Coords Offset
			true,				//Has Colors?
			3,					//rgba Offset
			kVertexCount,		//Vertex Count
			kPositionDimensions,//Position Dimensions
			0,					//Texture Dimensions
			kColorDimensions,	//Color Dimensions
			pVertices,			//Vertices
			true,				//Has Elements?
			kElementCount,		//Element Count
			pElements,			//Elements,
			GL_TRIANGLES,		//Draw Type
			0
		);
	}
	else
	{
		fprintf(stderr, "WARNING: mpModelAssetWall is being used due to it not having a default value of nullptr. Attempting to continue regardless. Incorrect assets may appear unless this is intentional.\n");
	}

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

void Application::LoadModelAssetSpaceship()
{
	//Vertices

	const unsigned int kVertexCount = 10;
	const unsigned int kPositionDimensions = 3;
	const unsigned int kTextureDimensions = 2;
	const unsigned int kColorDimensions = 4;
	const unsigned int kDataBitsPerVertex = kPositionDimensions + kTextureDimensions + kColorDimensions;

	GLfloat* pVertices = new GLfloat[kVertexCount * kDataBitsPerVertex]
	{
		/*
											_														//complicated version for later
		      .				      1			|							      .						|      .
		     / \			     / \		|		Length:	10m			     /|\					|     / \
		    / . \			    8 0 2		|	}	Width:	8m			    /_|_\					|   _/   \_
		  /_     _\			  7_6 9 4_3		|		Height:	2.5m		  /__\|/__\					|  / _   _ \
		     \./			     \5/		|							     \./					|  \/ \./ \/
											-
		*/

		//Position																								Texture						Color
		//x					y					z																							r		g		b		a

		 0.0f,				 2.5f * kDCPerM,	 0.0f,															1.0f,	1.0f,				0.4f,	0.9f,	1.0f,	1.0f,	//0:	Cockpit
		 0.0f,				 0.0f,				-5.0f * kDCPerM,												1.0f,	1.0f,				0.8f,	0.2f,	1.0f,	1.0f,	//1:	Bow (Nose)
		 2.0f * kDCPerM,	 0.0f,				 0.0f,															1.0f,	1.0f,				0.4f,	1.0f,	0.55f,	1.0f,	//2:	Starboard (Right)
		 4.0f * kDCPerM,	 0.0f,				 2.5f * kDCPerM,												1.0f,	1.0f,				0.9f,	0.9f,	0.1f,	1.0f,	//3:	Right Wing Tip
		 2.0f * kDCPerM,	 0.0f,				 2.5f * kDCPerM,												1.0f,	1.0f,				1.0f,	0.5f,	0.5f,	1.0f,	//4:
		 0.0f,				 0.0f,				 5.0f * kDCPerM,												1.0f,	1.0f,				1.0f,	0.35f,	0.1f,	1.0f,	//5:	Stern (Tail)
		-2.0f * kDCPerM,	 0.0f,				 2.5f * kDCPerM,												1.0f,	1.0f,				1.0f,	0.5f,	0.5f,	1.0f,	//6:
		-4.0f * kDCPerM,	 0.0f,				 2.5f * kDCPerM,												1.0f,	1.0f,				0.9f,	0.9f,	0.1f,	1.0f,	//7:	Left Wing Tip
		-2.0f * kDCPerM,	 0.0f,				 0.0f,															1.0f,	1.0f,				0.4f,	1.0f,	0.55f,	1.0f,	//8:	Port (Left)
		 0.0f,				 0.0f,				 2.5f * kDCPerM,												1.0f,	1.0f,				0.4f,	0.35f,	0.45f,	1.0f	//9:	Amidships
	};

	//Elements

	const unsigned int kElementsPerDrawType = 3;//Triangle
	const unsigned int kDrawTypeElements = 7;
	const unsigned int kElementCount = kElementsPerDrawType * kDrawTypeElements;

	GLuint* pElements = new GLuint[kElementCount]
	{
		0, 2, 1,
		0, 8, 1,
		0, 9, 2,
		0, 9, 8,
		9, 3, 2,
		9, 7, 8,
		4, 6, 5
	};

	//ModelAsset Creation

	if(mpModelAssetSpaceship == nullptr)
	{
		mpModelAssetSpaceship = new DSGraphics::ModelAsset
		(
			mpProgramTexAndColor,						//Program
			true,										//Has Texture?
			mpTextureSpaceship,							//Texture
			kPositionDimensions,						//Texture Coords Offset
			true,										//Has Colors?
			kPositionDimensions + kTextureDimensions,	//rgba Offset
			kVertexCount,								//Vertex Count
			kPositionDimensions,						//Position Dimensions
			kTextureDimensions,							//Texture Dimensions
			kColorDimensions,							//Color Dimensions
			pVertices,									//Vertices
			true,										//Has Elements?
			kElementCount,								//Element Count
			pElements,									//Elements,
			GL_TRIANGLES,								//Draw Type
			0
		);
	}
	else
	{
		fprintf(stderr, "WARNING: mpModelAssetSpaceship is being used due to it not having a default value of nullptr. Attempting to continue regardless. Incorrect assets may appear unless this is intentional.\n");
	}

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

void Application::CreateInitialInstances()
{
	//Walls
	DSGraphics::ModelInstance wallLeft(mpModelAssetWall, mpCamera);
	wallLeft.SetSize(glm::vec3(100.0f, 6.0f, 1.0f));//multiplies size by scaling, hence no need for multiplying by kDCPerM, since the ModelAsset is already constructed using the kDCPerM scale.
	wallLeft.SetOrientationAxis(glm::vec3(0.0f, 1.0f, 0.0f));
	wallLeft.SetOrientationAngle(glm::radians(270.0f));
	wallLeft.Move(glm::vec3(-15.0f, 0.0f, -15.0f), kDCPerM);
	wallLeft.UpdateTransform();
	mModelInstancesListWalls.push_back(wallLeft);

	DSGraphics::ModelInstance wallTop(mpModelAssetWall, mpCamera);
	wallTop.SetSize(glm::vec3(100.0f, 6.0f, 1.0f));
	wallTop.Move(glm::vec3(-15.0f, 0.0f, -15.0f), kDCPerM);
	wallTop.UpdateTransform();
	mModelInstancesListWalls.push_back(wallTop);

	//General
	// Spaceship
	DSGraphics::ModelInstance spaceshipPlayer(mpModelAssetSpaceship, mpCamera);
	mModelInstancesList.push_back(spaceshipPlayer);
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
		mpCamera->OffsetOrientation(0.0f, 0.0f);
		mpCamera->SetPosition(glm::vec3(0.0f, 0.0f, 4.0f));
	}
	//Look at origin
	else if(glfwGetKey(mpWindow, 'O'))
	{
		mpCamera->LookAt(glm::vec3(0.0f));
	}
	//Tilt Up
	else if(glfwGetKey(mpWindow, 'R'))
	{
		mpCamera->OffsetOrientation(0.0f, 30.0f);
	}
	//Tilt Down
	else if(glfwGetKey(mpWindow, 'F'))
	{
		mpCamera->OffsetOrientation(0.0f, -30.0f);
	}

	//Poll in counter clockwise direction, starting at 0 radians/degrees (right)
	// East
	else if(glfwGetKey(mpWindow, 'D'))
	{
		mpCamera->OffsetPosition(static_cast<float>(mElapsedTime) * kMoveSpeed * glm::vec3(1.0f, 0.0f, 0.0f));

		//Right
		//mpCamera->OffsetPosition(static_cast<float>(mElapsedTime) * kMoveSpeed * mpCamera->GetDirectionRight());
	}
	// North
	else if(glfwGetKey(mpWindow, 'W'))
	{
		//North
		mpCamera->OffsetPosition(static_cast<float>(mElapsedTime) * kMoveSpeed * glm::vec3(0.0f, 0.0f, -1.0f));

		//Forward
		//mpCamera->OffsetPosition(static_cast<float>(mElapsedTime) * kMoveSpeed * mpCamera->GetDirectionForward());
	}
	// West
	else if(glfwGetKey(mpWindow, 'A'))
	{
		mpCamera->OffsetPosition(static_cast<float>(mElapsedTime) * kMoveSpeed * glm::vec3(-1.0f, 0.0f, 0.0f));

		//Left
		//mpCamera->OffsetPosition(static_cast<float>(mElapsedTime) * kMoveSpeed * (-mpCamera->GetDirectionRight()));
	}
	// South
	else if(glfwGetKey(mpWindow, 'S'))
	{
		mpCamera->OffsetPosition(static_cast<float>(mElapsedTime) * kMoveSpeed * glm::vec3(0.0f, 0.0f, 1.0f));

		//Backwards
		//mpCamera->OffsetPosition(static_cast<float>(mElapsedTime) * kMoveSpeed * (-mpCamera->GetDirectionForward()));
	}
	// Up
	else if(glfwGetKey(mpWindow, 'E'))
	{
		//Up relative to global up (like on Earth, towards the sky)
		mpCamera->OffsetPosition(static_cast<float>(mElapsedTime) * kMoveSpeed * glm::vec3(0.0f, 1.0f, 0.0f));

		//Up based on camera orientation (like in Space)
		//mpCamera->OffsetPosition(static_cast<float>(mElapsedTime) * kMoveSpeed * mpCamera->GetDirectionUp());
	}
	// Down
	else if(glfwGetKey(mpWindow, 'Q'))
	{
		//Down relative to global Down (like on Earth, towards center of planet)
		mpCamera->OffsetPosition(static_cast<float>(mElapsedTime) * kMoveSpeed * glm::vec3(0.0f, -1.0f, 0.0f));

		//Down based on camera orientation (like in Space)
		//mpCamera->OffsetPosition(static_cast<float>(mElapsedTime) * kMoveSpeed * (-mpCamera->GetDirectionUp()));
	}


	//Individual Objects


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
	// Spaceship
	const float kDegreesPerSecond = -20.0f;
	mDegreesRotated += static_cast<float>(mElapsedTime) * kDegreesPerSecond;
	while(mDegreesRotated > 360.0f)
	{
		mDegreesRotated -= 360.0f;
	}
	mModelInstancesList.front().SetOrientationAxis(glm::vec3(0.0f, 1.0f, 0.0f));
	mModelInstancesList.front().SetOrientationAngle(glm::radians(mDegreesRotated));
	mModelInstancesList.front().UpdateTransform();

	
	//TODO: Add static ID to instances in order to keep track of which is which, so that I don't do the following risky business
}

//-----------------------------------------------------------------------------

void Application::Render()
{
	//Background
	// Clear the screen to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Camera
	//glm::vec4 posToLookAt = mListModelInstancesBackground.begin()->GetTranslate() * glm::vec4(1.0f);
	//mpCamera->LookAt(glm::vec3(posToLookAt));
	//mpCamera->LookAt(glm::vec3(0.0f, 0.0f, 0.0f));

	//Instance Lists
	std::vector<DSGraphics::ModelInstance>::iterator it;
	
	// Background

	// Walls
	for(it = mModelInstancesListWalls.begin(); it != mModelInstancesListWalls.end(); ++it)
	{
		it->Render();
	}

	// General
	for(it = mModelInstancesList.begin(); it != mModelInstancesList.end(); ++it)
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
	if(mpProgramColorOnly != nullptr)
	{
		delete mpProgramColorOnly;
		mpProgramColorOnly = nullptr;
	}
	// Tex and Color
	if(mpProgramTexAndColor != nullptr)
	{
		delete mpProgramTexAndColor;
		mpProgramTexAndColor = nullptr;
	}

	//Textures
	if(mpTextureSpaceship != nullptr)
	{
		delete mpTextureSpaceship;
		mpTextureSpaceship = nullptr;
	}

	//Assets
	CleanUpAssets();

	//Instances
	CleanUpInstances();
}

//-----------------------------------------------------------------------------

void Application::CleanUpAssets()
{
	//Wall
	if(mpModelAssetWall != nullptr)
	{
		delete mpModelAssetWall;
		mpModelAssetWall = nullptr;
	}

	//Spaceship
	if(mpModelAssetSpaceship != nullptr)
	{
		delete mpModelAssetSpaceship;
		mpModelAssetSpaceship = nullptr;
	}
}

//-----------------------------------------------------------------------------

void Application::CleanUpInstances()
{
	mModelInstancesListWalls.clear();
	mModelInstancesList.clear();
}

//-----------------------------------------------------------------------------
// Test Functions
//-----------------------------------------------------------------------------

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