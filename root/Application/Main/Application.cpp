//=============================================================================
// File:		Application.cpp
// Created:		2015/02/10
// Last Edited:	2015/02/13
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
#include "Camera.h"
#include "Program.h"
#include "Texture.h"

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
	//Data
	// Shaders
,	mpProgram001(nullptr)
	// Rectangle
,	mRectVao(0)
,	mpRectVertices(nullptr)
,	mRectModel(1.0f)
,	mRectUniformModel(0)
,	mRectScale(1.0f)
,	mRectRotate(1.0f)
,	mRectRotationAxis(0.0f, 0.0f, 0.0f)
,	mRectTranslate(1.0f)
,	mRectVbo(0)
,	mRectEbo(0)
,	mpRectElements(nullptr)
	// Textures
,	mpTex(nullptr)
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

			//Shaders
			LoadShaders();

			//Individual Objects

			// Rectangle
			LoadRectangle();

			//Camera
			if(mpCamera == nullptr)
			{
				//do stuff
				mpCamera = new Camera();
				mpCamera->SetPosition(glm::vec3(0.0f, 0.0f, 4.0f));
				mpCamera->SetViewportAspectRatio(mWindowSize.x / mWindowSize.y);
			}
			else
			{
				fprintf(stderr, "WARNING: mpCamera is being used due to it not having a default value of nullptr. Attempting to continue regardless. A memory leak may occur.\n");
			}
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

void Application::CleanUp()
{
	//Camera
	if(mpCamera != nullptr)
	{
		delete mpCamera;
		mpCamera = nullptr;
	}

	//Shaders
	if(mpProgram001 != nullptr)
	{
		delete mpProgram001;
		mpProgram001 = nullptr;
	}

	//Individual Objects
	// Rectangle
	if(mpRectVertices != nullptr)
	{
		delete[] mpRectVertices;
		mpRectVertices = nullptr;
	}
	glDeleteBuffers(1, &mRectEbo);
	if(mpRectElements != nullptr)
	{
		delete[] mpRectElements;
		mpRectElements = nullptr;
	}
	glDeleteBuffers(1, &mRectVbo);
	glDeleteVertexArrays(1, &mRectVao);

	//Textures
	if(mpTex != nullptr)
	{
		delete mpTex;
		mpTex = nullptr;
	}
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
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		//is the window resizeable? No
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		//create the actual window
		mpWindow = glfwCreateWindow(static_cast<int>(mWindowSize.x), static_cast<int>(mWindowSize.y), mWindowTitle, nullptr, nullptr);//Windowed
		//mpWindow = glfwCreateWindow(static_cast<int>(mWindowSize.x), static_cast<int>(mWindowSize.y), mWindowTitle, glfwGetPrimaryMonitor(), nullptr);
		if(mpWindow == nullptr)
		{
			throw std::runtime_error("ERROR: Failed to open a GLFW window. Please update your graphics card drivers to a version that supports OpenGL 3.2 or higher.");
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
		// make sure OpenGL version 3.2 API is available
		if(!GLEW_VERSION_3_2)
		{
			throw std::runtime_error("ERROR: OpenGL 3.2 API is not available.");
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

void Application::LoadRectangle()
{
	//Data

	// Vertices in Object Coordinates
	mpRectVertices = new GLfloat[8 * 8]
	{
		//	  2---3
		//	 /|  /|
		//	0---1 |
		//	| 6-|-7
		//	|/  |/
		//	4---5

	//	Position				TexCoords		Color
		
		//Top
		-0.5f,  0.5f,  0.5f,	0.0f, 0.0f,		1.0f,  0.0f,  0.0f,	//0
		 0.5f,  0.5f,  0.5f,	0.0f, 0.0f,		1.0f,  0.5f,  0.0f,	//1
		-0.5f,  0.5f, -0.5f,	0.0f, 0.0f,		0.0f,  1.0f,  0.0f,	//2
		 0.5f,  0.5f, -0.5f,	0.0f, 0.0f,		1.0f,  1.0f,  0.0f,	//3

		//Bottom
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,		0.0f,  1.0f,  1.0f,	//4
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,		0.0f,  0.0f,  1.0f,	//5
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,		1.0f,  0.0f,  1.0f,	//6
		 0.5f, -0.5f, -0.5f,	1.0f, 1.0f,		0.5f,  0.0f,  1.0f	//7
	};

	mpRectElements = new GLuint[3 * 2 * 6]
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

	// Model
	//   Scale
	mRectScale = glm::mat4(1.0f);//not doing any scaling at the moment
	
	//   Rotate
	mRectRotate = glm::mat4(1.0f);//not doing any rotating at the moment
	mRectRotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	
	//   Translate
	mRectTranslate  = glm::mat4(1.0f);//not doing any translating at the moment

	//Combine SRT into Model
	mRectModel = mRectTranslate * mRectRotate * mRectScale;


	//Vertex Array Object (VAO)
	//Note:	VAOs store all of the links between attributes and their corresponding VBOs with raw vertex data.
	// Creation
	if(mRectVao != 0)
	{
		fprintf(stderr, "WARNING: mRectVao is being used due to it not having a default value of GLuint == 0. Attempting to continue regardless.\n");
	}
	glGenVertexArrays(1, &mRectVao);
	// Enable Usage (by binding)
	glBindVertexArray(mRectVao);


	//Vertex Buffer Object (VBO)
	//Note:	Since OpenGL manages memory, GLuint vboTriangle is a positive number reference to the memory that the VBO will occupy.
	// Creation
	if(mRectVbo != 0)
	{
		fprintf(stderr, "WARNING: mRectVbo is being used due to it not having a default value of GLuint == 0. Attempting to continue regardless.\n");
	}
	glGenBuffers(1, &mRectVbo);
	// Set a specific VBO as the active object in the array buffer
	glBindBuffer(GL_ARRAY_BUFFER, mRectVbo);
	// Copy vertex data to the active object previously specified to the array buffer
	glBufferData
	(
		GL_ARRAY_BUFFER,					//currently active array buffer
		sizeof(mpRectVertices[0]) * 8 * 8,	//size of vertices data in bytes
		mpRectVertices,						//actual vertices data
		GL_STATIC_DRAW						//usage of vertex data
	);


	//Element  Buffer Objects (EBO)
	if(mRectEbo != 0)
	{
		fprintf(stderr, "WARNING: mRectEbo is being used due to it not having a default value of GLuint == 0. Attempting to continue regardless.\n");
	}
	glGenBuffers(1, &mRectEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRectEbo);
	glBufferData
	(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(mpRectElements[0]) * 3 * 2 * 6,
		mpRectElements,
		GL_STATIC_DRAW
	);


	//Specify the layout of the vertex data
	
	// Position
	GLint posAttrib = glGetAttribLocation(mpProgram001->GetProgramID(), "vsInPosition");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer
	(
		posAttrib,				//which attribute? in this case, referencing vsInPosition.
		3,						//number of values (size) for attribute (vsInPosition). in this case, 3 for x, y, z
		GL_FLOAT,				//type of each component
		GL_FALSE,				//should attribute (input) values be normalized?
		8 * sizeof(GLfloat),	//stride (how many bytes between each position attribute in the array)
		0						//array buffer offset
	);

	//  Texture
	GLint texAttrib = glGetAttribLocation(mpProgram001->GetProgramID(), "vsInTexCoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer
	(
		texAttrib,						//which attribute? in this case, referencing vsInTexCoord.
		2,								//number of values (size) for attribute (vsInTexCoord). in this case, 2 for u, v
		GL_FLOAT,						//type of each component
		GL_FALSE,						//should attribute (input) values be normalized?
		8 * sizeof(GLfloat),			//stride (how many bytes between each position attribute in the array)
		(void*)(3 * sizeof(GLfloat))	//array buffer offset
	);

	// Color
	GLint colAttrib = glGetAttribLocation(mpProgram001->GetProgramID(), "vsInColor");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer
	(
		colAttrib,						//which attribute? in this case, referencing vsInColor.
		3,								//number of values (size) for attribute (vsInColor). in this case, 3 for r, g, b
		GL_FLOAT,						//type of each component
		GL_FALSE,						//should attribute (input) values be normalized?
		8 * sizeof(float),				//stride (how many bytes between each position attribute in the array)
		(void*)(5 * sizeof(GLfloat))	//array buffer offset
	);
	
	//   Load
	if(mpTex != nullptr)
	{
		fprintf(stderr, "WARNING: mpTex is being used Overwriting texture. Potential memory leak may occur.\n");
	}
	//glGenTextures(1, &mTex);
	mpTex = new Texture("../../Resources/Textures/pngexample.png");
	if(mpTex->GetIsTextureLoaded() == false)
	{
		fprintf(stderr, "WARNING: mpTex texture not loaded. Attempting to continue regardless.\n");
	}

	//Unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//-----------------------------------------------------------------------------

void Application::LoadShaders()
{
	std::vector<Shader> shaders;
	shaders.push_back(Shader::CreateShaderFromFile("Shaders/vertexShader.txt", GL_VERTEX_SHADER));
	shaders.push_back(Shader::CreateShaderFromFile("Shaders/fragmentShader.txt", GL_FRAGMENT_SHADER));
	mpProgram001 = new Program(shaders);

	/*
	glUseProgram(mpProgram001->GetProgramID());

	//MVP (Model View Projection)
	//Note: model matrices are calculated individually for each instance, but the view and projection matrices are the same for all instances.

	// View
	mView = glm::lookAt
	(
		glm::vec3(1.0f, 1.5f, 2.0f),	//camera position
		glm::vec3(0.0f, 0.0f, 0.0f),	//focal point
		glm::vec3(0.0f, 1.0f, 0.0f)		//up axis
	);
	mUniformView = glGetUniformLocation(mpProgram001->GetProgramID(), "view");
	glUniformMatrix4fv(mUniformView, 1, GL_FALSE, glm::value_ptr(mView));

	// Projection
	mProj = glm::perspective
	(
		glm::radians(90.0f),			//Vertical Field of View (FoV) in degrees
		mWindowSize.x / mWindowSize.y,	//aspect ratio
		0.1f,							//near plane
		100.0f							//far plane
	);
	mUniformProj = glGetUniformLocation(mpProgram001->GetProgramID(), "proj");
	glUniformMatrix4fv(mUniformProj, 1, GL_FALSE, glm::value_ptr(mProj));

	glUseProgram(0);
	*/
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
		mRectRotationAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else if(glfwGetKey(mpWindow, 'Y'))
	{
		mRectRotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	else if(glfwGetKey(mpWindow, 'Z'))
	{
		mRectRotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
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
	const GLfloat kDegreesPerSecond = 180.0f;
	mRectDegreesRotated += static_cast<float>(mElapsedTime) * kDegreesPerSecond;
	while(mRectDegreesRotated > 360.0f)
	{
		mRectDegreesRotated -= 360.0f;
	}
	//  Update model based on changes that may have occured during the input stage
	mRectRotate = glm::rotate(glm::mat4(1.0f), glm::radians(mRectDegreesRotated), mRectRotationAxis);
	mRectModel = mRectTranslate * mRectRotate * mRectScale;
}

//-----------------------------------------------------------------------------

void Application::Render()
{
	//Background
	// Clear the screen to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(mpProgram001->GetProgramID());

	//Camera
	mUniformCamera = glGetUniformLocation(mpProgram001->GetProgramID(), "camera");
	glUniformMatrix4fv(mUniformCamera, 1, GL_FALSE, glm::value_ptr(mpCamera->GetMatrix()));

	//Individual Objects

	// Rectangle

	//  Model
	mRectUniformModel = glGetUniformLocation(mpProgram001->GetProgramID(), "model");
	glUniformMatrix4fv(mRectUniformModel, 1, GL_FALSE, glm::value_ptr(mRectModel));

	//  Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mpTex->GetObjectID());
	glUniform1i(glGetUniformLocation(mpProgram001->GetProgramID(), "tex"), 0);

	//  VAO
	glBindVertexArray(mRectVao);

	//  Draw Elements
	glDrawElements(GL_TRIANGLES, 3 * 2 * 6, GL_UNSIGNED_INT, 0);

	//Unbind
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Unbind the Program
	glUseProgram(0);

	//Swap the back buffer and the front buffer
	glfwSwapBuffers(mpWindow);
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