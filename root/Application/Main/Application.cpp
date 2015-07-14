//=============================================================================
// File:		Application.cpp
// Created:		2015/02/10
// Last Edited:	2015/02/12
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
,	mpWindow(nullptr)
,	mWindowSize(800, 600)
,	mWindowTitle("Daniel Schenker's Application")
	//Data
	// Shaders
,	mpProgram001(nullptr)
	// Rectangle
,	mRectVao(0)
,	mpRectVertices(nullptr)
,	mRectMvp(1.0f)
,	mRectModel(1.0f)
,	mRectScale(1.0f)
,	mRectRotate(1.0f)
,	mRectRotationAxis(0.0f, 0.0f, 0.0f)
,	mRectTranslate(1.0f)
,	mRectView(1.0f)
,	mRectProj(1.0f)
,	mRectUniMvp(0)
,	mRectVbo(0)
,	mRectEbo(0)
,	mpRectElements(nullptr)
	// Textures
,	mpTex(nullptr)
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

			//Shaders
			LoadShaders();

			//Individual Objects

			// Rectangle
			LoadRectangle();
		}
	}
}

//-----------------------------------------------------------------------------

//   2. Main game loop
void Application::Run()
{
	//[Closed] Event Loop
	while(mQuit == false)//while(glfwWindowShouldClose(mpWindow) == false)
	{
		//Input
		
		// Retrieve
		glfwPollEvents();
		
		// Handle
		//  If key pressed: escape
		if(glfwGetKey(mpWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			mQuit = true;
		}


		//Render
		Render();
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
	mpRectVertices = new GLfloat[28]
	{
	//	Position							Color							TexCoords
		-0.5f,  0.5f, /* left	top */		1.0f,  0.0f,  0.0f, /* RGB */	0.0f, 1.0f,
		 0.5f,  0.5f, /* right	top */		0.0f,  1.0f,  0.0f, /* RGB */	1.0f, 1.0f,
		 0.5f, -0.5f, /* right	bottom */	0.0f,  0.0f,  1.0f, /* RGB */	1.0f, 0.0f,
		-0.5f, -0.5f, /* left	bottom */	1.0f,  1.0f,  1.0f, /* RGB */	0.0f, 0.0f
	};

	mpRectElements = new GLuint[6]
	{
		0, 1, 2,	//LT, RT, RB
		2, 3, 0		//RB, LB, LT
	};

	// Calculate model-view-projection (mvp) matrix
	//Random Notes: model = glm::rotate(model, glm::radians(0.f), rotationAxis) * glm::scale(model, glm::vec3(5.0f, 5.0f, 1.0f));
	//  Model

	//   Scale
	mRectScale = glm::mat4(1.0f);//not doing any scaling at the moment
	
	//   Rotate
	mRectRotate = glm::mat4(1.0f);//not doing any rotating at the moment
	mRectRotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	
	//   Translate
	mRectTranslate  = glm::mat4(1.0f);//not doing any translating at the moment

	//Combine SRT into Model
	mRectModel = mRectTranslate * mRectRotate * mRectScale;

	//  View
	mRectView = glm::lookAt
	(
		glm::vec3(1.0f, 1.0f, 1.0f),	//camera position
		glm::vec3(0.0f, 0.0f, 0.0f),	//focal point
		glm::vec3(0.0f, 1.0f, 0.0f)		//up axis
	);

	//  Projection
	mRectProj = glm::perspective
	(
		45.0f,				//Field of View (FoV) in degrees
		800.0f / 600.0f,	//aspect ratio
		1.0f,				//near plane
		10.0f				//far plane
	);

	mRectMvp = mRectProj * mRectView * mRectModel;


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
		sizeof(mpRectVertices[0]) * 4 * 7,	//size of vertices data in bytes
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mpRectElements[0]) * 2 * 3, mpRectElements, GL_STATIC_DRAW);


	//Get a handle for our mvp uniform
	if(mRectUniMvp != 0)
	{
		fprintf(stderr, "WARNING: mRectUniMvp is being used due to it not having a default value of GLuint == 0. Attempting to continue regardless.\n");
	}
	mRectUniMvp = glGetUniformLocation(mpProgram001->GetProgramID(), "mvp");


	//Specify the layout of the vertex data
	
	// Position
	GLint posAttrib = glGetAttribLocation(mpProgram001->GetProgramID(), "vsInPosition");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer
	(
		posAttrib,				//which attribute? in this case, referencing vsInPosition.
		2,						//number of values (size) for attribute (input)? in this case, 2 for x, y
		GL_FLOAT,				//type of each component
		GL_FALSE,				//should attribute (input) values be normalized?
		7 * sizeof(GLfloat),	//stride (how many bytes between each position attribute in the array)
		0						//array buffer offset
	);

	// Color
	GLint colAttrib = glGetAttribLocation(mpProgram001->GetProgramID(), "vsInColor");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer
	(
		colAttrib,
		3,
		GL_FLOAT,
		GL_FALSE,
		7 * sizeof(float),
		(void*)(2 * sizeof(GLfloat))
	);

	//  Texture
	GLint texAttrib = glGetAttribLocation(mpProgram001->GetProgramID(), "vsInTexCoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
	
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

	//int texWidth = 0;
	//int texHeight = 0;

	//GLuint texture = LoadTexture("../../Resources/Textures/test.png", texWidth, texHeight);
	//GLuint texture2 = LoadTexture("../../Resources/Textures/pngexample.png", texWidth, texHeight);//I shouldn't reuse texWidth and texHeight, but this is just for testing's sake

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
}

//-----------------------------------------------------------------------------
// Run Sub-Functions
//-----------------------------------------------------------------------------

void Application::Render()
{
	// Background
	// Clear the screen to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(mpProgram001->GetProgramID());

	// Individual Objects
		
	//  Rectangle
	//   Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mpTex->GetObjectID());
	glUniform1i(glGetUniformLocation(mpProgram001->GetProgramID(), "tex"), 0);

	//   VAO
	glBindVertexArray(mRectVao);

	//   Transforms (should be in physics I believe (after input))
	mRectRotate = glm::rotate
	(
		glm::mat4(1.0f),
		((GLfloat)clock() / (GLfloat)CLOCKS_PER_SEC) * glm::radians(180.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
	mRectModel = mRectTranslate * mRectRotate * mRectScale;
	mRectMvp = mRectProj * mRectView * mRectModel;
	glUniformMatrix4fv(mRectUniMvp, 1, GL_FALSE, glm::value_ptr(mRectMvp));

	//   Draw Elements
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//  Unbind
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Unbind the Program
	glUseProgram(0);

	// Swap the back buffer and the front buffer
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