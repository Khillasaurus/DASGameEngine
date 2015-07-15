//=============================================================================
// File:		Application.cpp
// Created:		2015/02/10
// Last Edited:	2015/04/10
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
#include "DSMathematics/Quaternion.h"

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
//Objects
// Abstracts
// Aesthetics
// Environmentals
,	mpWall(nullptr)
// Player
,	mpSpaceshipStarter(nullptr)
// Units
//Individual Objects
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
		LoadObjects();
}

//-----------------------------------------------------------------------------

void Application::LoadCamera()
{
	//Camera
	if(mpCamera == nullptr)
	{
		mpCamera = new DSGraphics::Camera(glm::vec3(100.0f * Object::sDCPerM, 100.0f * Object::sDCPerM, 100.0f * Object::sDCPerM), DSMathematics::Quaternion(), 90.0f, 0.01f, 1000.0f, mWindowSize.x / mWindowSize.y);
		/*
		Note: TOCHECK
			Since OpenGL defaults the camera to face (0, 0, -1) (or possibly I have set this myself and am unaware), future rotations must keep this in mind.
			Therefore tilting the camera down 60 degrees means rotating 60 degrees around the x-axis, which is unintuitive.
			This is as if the camera uses eyes on the back of its head instead.
			In theory, since the camera is rotated 180 degrees around the y-axis by default, any rotation that isn't around the y-axis must be applied oppositely (namely, times negative one), but instead applying ALL rotations oppositely works instead for some bizarre reason.

			Currently implemented quick fix to be able to ignore the above non-sense:
				Internal to the camera class, all orientation related functions are inverted. This way logical vectors can be used in outside implementation.
		*/
		mpCamera->SetOrientation(DSMathematics::Quaternion(glm::radians(-60.0f), glm::vec3(1.0f, 0.0f, 0.0f)), false);
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

void Application::LoadObjects()
{
	LoadObjectsAbstracts();
	LoadObjectsAesthetics();
	LoadObjectsEnvironmentals();
	LoadObjectsPlayers();
	LoadObjectsUnits();
}

//-----------------------------------------------------------------------------

void Application::LoadObjectsAbstracts()
{
}

//-----------------------------------------------------------------------------

void Application::LoadObjectsAesthetics()
{
}

//-----------------------------------------------------------------------------

void Application::LoadObjectsEnvironmentals()
{
	//Wall
	if(mpWall == nullptr)
	{
		mpWall = new Wall();
		mpWall->LoadAsset(mpProgramColorOnly, nullptr);
	}
}

//-----------------------------------------------------------------------------

void Application::LoadObjectsPlayers()
{
	//SpaceshipStarter
	if(mpSpaceshipStarter == nullptr)
	{
		mpSpaceshipStarter = new SpaceshipStarter();
		mpSpaceshipStarter->LoadAsset(mpProgramTexAndColor, mpTextureSpaceship);
	}
}

//-----------------------------------------------------------------------------

void Application::LoadObjectsUnits()
{
}

//-----------------------------------------------------------------------------

void Application::CreateInitialInstances()
{
	CreateInitialInstancesAbstracts();
	CreateInitialInstancesAesthetics();
	CreateInitialInstancesEnvironmentals();
	CreateInitialInstancesPlayers();
	CreateInitialInstancesUnits();
}

//-----------------------------------------------------------------------------

void Application::CreateInitialInstancesAbstracts()
{
}

//-----------------------------------------------------------------------------

void Application::CreateInitialInstancesAesthetics()
{
}

//-----------------------------------------------------------------------------

void Application::CreateInitialInstancesEnvironmentals()
{
	//Wall
	if(mpWall != nullptr)
	{
		if(mpWall->GetIsModelAssetLoaded() == true)
		{
			//Left
			DSGraphics::ModelInstance wallLeft(mpWall->GetModelAsset(), mpCamera);
			wallLeft.SetSize(glm::vec3(100.0f, 6.0f, 1.0f));//multiplies size by scaling, hence no need for multiplying by the scale ratio (device coordinates per meter), since the ModelAsset is already constructed using the scale.
			wallLeft.SetOrientationAxis(glm::vec3(0.0f, 1.0f, 0.0f));//TODO: Change this to new DSGraphics::Quaternion based rotation.
			wallLeft.SetOrientationAngle(glm::radians(270.0f));
			wallLeft.Move(glm::vec3(-15.0f, 0.0f, -15.0f), Object::sDCPerM);
			wallLeft.UpdateTransform();
			mEnvironmentalsInstanceList.push_back(wallLeft);

			//Top
			DSGraphics::ModelInstance wallTop(mpWall->GetModelAsset(), mpCamera);
			wallTop.SetSize(glm::vec3(100.0f, 6.0f, 1.0f));//multiplies size by scaling, hence no need for multiplying by the scale ratio (device coordinates per meter), since the ModelAsset is already constructed using the scale.
			wallTop.Move(glm::vec3(-15.0f, 0.0f, -15.0f), Object::sDCPerM);
			wallTop.UpdateTransform();
			mEnvironmentalsInstanceList.push_back(wallTop);
		}
	}
}

//-----------------------------------------------------------------------------

void Application::CreateInitialInstancesPlayers()
{
	//SpaceshipStarter
	if(mpSpaceshipStarter != nullptr)
	{
		if(mpSpaceshipStarter->GetIsModelAssetLoaded() == true)
		{
			//Player1
			DSGraphics::ModelInstance player1(mpSpaceshipStarter->GetModelAsset(), mpCamera);
			mPlayersInstanceList.push_back(player1);
		}
	}
}

//-----------------------------------------------------------------------------

void Application::CreateInitialInstancesUnits()
{
}

//-----------------------------------------------------------------------------
// Run Sub-Functions
//-----------------------------------------------------------------------------

void Application::Input()
{
	//Retrieve
	glfwPollEvents();


	//Camera
	const float kMoveSpeed = 2.0f;//units per second

	// Reset
	//	Puts the camera at the world's origin and faces it in the negative z direction (0, 0, -1).
	if(glfwGetKey(mpWindow, '`'))
	{
		mpCamera->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f), false);
		mpCamera->SetOrientation(DSMathematics::Quaternion(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)), false);
	}
	//Look at origin
	else if(glfwGetKey(mpWindow, 'O'))
	{
		//TODO:
		//mpCamera->LookAt(glm::vec3(0.0f));
	}
	
	//Tilt Up
	else if(glfwGetKey(mpWindow, 'R'))
	{
		//Roll around global x-axis
		//mpCamera->SetOrientation(DSMathematics::Quaternion(/*static_cast<float>(mElapsedTime) * */glm::radians(0.01f), glm::vec3(1.0f, 0.0f, 0.0f)), true);

		//Roll around local x-axis
		mpCamera->SetOrientation(DSMathematics::Quaternion(/*static_cast<float>(mElapsedTime) * */glm::radians(0.01f), mpCamera->GetDirectionRight()), true);
	}
	//Tilt Down
	else if(glfwGetKey(mpWindow, 'F'))
	{
		//Roll around global x-axis
		//mpCamera->SetOrientation(DSMathematics::Quaternion(/*static_cast<float>(mElapsedTime) * */glm::radians(-0.01f), glm::vec3(1.0f, 0.0f, 0.0f)), true);

		//Roll around local x-axis
		mpCamera->SetOrientation(DSMathematics::Quaternion(/*static_cast<float>(mElapsedTime) * */glm::radians(-0.01f), mpCamera->GetDirectionRight()), true);
	}
	//Turn Left
	else if(glfwGetKey(mpWindow, 'T'))
	{
		//Roll around global y-axis
		//mpCamera->SetOrientation(DSMathematics::Quaternion(/*static_cast<float>(mElapsedTime) * */glm::radians(0.01f), glm::vec3(0.0f, 1.0f, 0.0f)), true);

		//Roll around local y-axis
		mpCamera->SetOrientation(DSMathematics::Quaternion(/*static_cast<float>(mElapsedTime) * */glm::radians(0.01f), mpCamera->GetDirectionUp()), true);
	}
	//Turn Right
	else if(glfwGetKey(mpWindow, 'Y'))
	{
		//Roll around global y-axis
		//mpCamera->SetOrientation(DSMathematics::Quaternion(/*static_cast<float>(mElapsedTime) * */glm::radians(-0.01f), glm::vec3(0.0f, 1.0f, 0.0f)), true);

		//Roll around local y-axis
		mpCamera->SetOrientation(DSMathematics::Quaternion(/*static_cast<float>(mElapsedTime) * */glm::radians(-0.01f), mpCamera->GetDirectionUp()), true);
	}
	//Roll Left
	else if(glfwGetKey(mpWindow, 'G'))
	{
		//Roll around global z-axis
		//mpCamera->SetOrientation(DSMathematics::Quaternion(/*static_cast<float>(mElapsedTime) * */glm::radians(0.01f), glm::vec3(0.0f, 0.0f, 1.0f)), true);

		//Roll around local z-axis
		mpCamera->SetOrientation(DSMathematics::Quaternion(/*static_cast<float>(mElapsedTime) * */glm::radians(0.01f), mpCamera->GetDirectionBackward()), true);
	}
	//Roll Right
	else if(glfwGetKey(mpWindow, 'H'))
	{
		//Roll around global z-axis
		//mpCamera->SetOrientation(DSMathematics::Quaternion(/*static_cast<float>(mElapsedTime) * */glm::radians(-0.01f), glm::vec3(0.0f, 0.0f, 1.0f)), true);

		//Roll around local z-axis
		mpCamera->SetOrientation(DSMathematics::Quaternion(/*static_cast<float>(mElapsedTime) * */glm::radians(-0.01f), mpCamera->GetDirectionBackward()), true);
	}



	//Poll in counter clockwise direction, starting at 0 radians/degrees (right)
	// East
	else if(glfwGetKey(mpWindow, 'D'))
	{
		mpCamera->SetPosition(static_cast<float>(mElapsedTime) * kMoveSpeed * glm::vec3(1.0f, 0.0f, 0.0f), true);
	}
	// North
	else if(glfwGetKey(mpWindow, 'W'))
	{
		mpCamera->SetPosition(static_cast<float>(mElapsedTime) * kMoveSpeed * glm::vec3(0.0f, 0.0f, -1.0f), true);
	}
	// West
	else if(glfwGetKey(mpWindow, 'A'))
	{
		mpCamera->SetPosition(static_cast<float>(mElapsedTime) * kMoveSpeed * glm::vec3(-1.0f, 0.0f, 0.0f), true);
	}
	// South
	else if(glfwGetKey(mpWindow, 'S'))
	{
		mpCamera->SetPosition(static_cast<float>(mElapsedTime) * kMoveSpeed * glm::vec3(0.0f, 0.0f, 1.0f), true);
	}

	// Up
	else if(glfwGetKey(mpWindow, 'E'))
	{
		mpCamera->SetPosition(static_cast<float>(mElapsedTime) * kMoveSpeed * glm::vec3(0.0f, 1.0f, 0.0f), true);
	}
	// Down
	else if(glfwGetKey(mpWindow, 'Q'))
	{
		mpCamera->SetPosition(static_cast<float>(mElapsedTime) * kMoveSpeed * glm::vec3(0.0f, -1.0f, 0.0f), true);
	}

	// Local Right
	else if(glfwGetKey(mpWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		mpCamera->Move(static_cast<float>(mElapsedTime) * kMoveSpeed * glm::vec3(1.0f, 0.0f, 0.0f));
	}
	// Local Forward
	else if(glfwGetKey(mpWindow, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mpCamera->Move(static_cast<float>(mElapsedTime) * kMoveSpeed * glm::vec3(0.0f, 0.0f, -1.0f));
	}
	// Local Left
	else if(glfwGetKey(mpWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		mpCamera->Move(static_cast<float>(mElapsedTime) * kMoveSpeed * glm::vec3(-1.0f, 0.0f, 0.0f));
	}
	// Local Backward
	else if(glfwGetKey(mpWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mpCamera->Move(static_cast<float>(mElapsedTime) * kMoveSpeed * glm::vec3(0.0f, 0.0f, 1.0f));
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
	//TODO: Add static ID to instances in order to keep track of which is which, so that I don't do the following risky business
	mPlayersInstanceList.front().SetOrientationAxis(glm::vec3(0.0f, 1.0f, 0.0f));
	mPlayersInstanceList.front().SetOrientationAngle(glm::radians(mDegreesRotated));
	mPlayersInstanceList.front().UpdateTransform();
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
	// Abstracts
	for(it = mAbstractsInstanceList.begin(); it != mAbstractsInstanceList.end(); ++it)
	{
		it->Render();
	}
	// Aesthetics
	for(it = mAestheticsInstanceList.begin(); it != mAestheticsInstanceList.end(); ++it)
	{
		it->Render();
	}
	// Environmentals
	for(it = mEnvironmentalsInstanceList.begin(); it != mEnvironmentalsInstanceList.end(); ++it)
	{
		it->Render();
	}
	// Player
	for(it = mPlayersInstanceList.begin(); it != mPlayersInstanceList.end(); ++it)
	{
		it->Render();
	}
	// Units
	for(it = mUnitsInstanceList.begin(); it != mUnitsInstanceList.end(); ++it)
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
	// Color Only
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

	//Objects
	CleanUpObjects();

	//Instances
	CleanUpInstances();
}

//-----------------------------------------------------------------------------

void Application::CleanUpObjects()
{
	//Abstracts
	
	
	//Aesthetics
	

	//Environmentals
	
	// Wall
	if(mpWall != nullptr)
	{
		delete mpWall;
		mpWall = nullptr;
	}
	

	//Player
	
	// SpaceshipStarter
	if(mpSpaceshipStarter != nullptr)
	{
		delete mpSpaceshipStarter;
		mpSpaceshipStarter = nullptr;
	}


	//Units
}

//-----------------------------------------------------------------------------

void Application::CleanUpInstances()
{
	mAbstractsInstanceList.clear();
	mAestheticsInstanceList.clear();
	mEnvironmentalsInstanceList.clear();
	mPlayersInstanceList.clear();
	mUnitsInstanceList.clear();
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