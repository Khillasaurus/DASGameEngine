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
,	mpProgramBgMesh(nullptr)
,	mpProgramCube(nullptr)
//Textures
,	mpTextureCube(nullptr)
//Models
,	mpModelAssetBgMesh(nullptr)
,	mpModelAssetCube(nullptr)
//Instance Lists
	//,	mListModelInstancesBackground()
	//,	mListModelInstances()//not sure how to initialize this yet
//Individual Objects
,	mBgMeshSize(4.0f, 4.0f)
,	mBgMeshDistanceBetweenVertices(0.1f, 0.0f, 0.1f)
,	mBgMeshLengthX(0.1f)
,	mBgMeshLengthY(0.0f)
,	mBgMeshLengthZ(0.1f)
,	mBgMeshInstanceCount(0)

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
		mpCamera->SetPosition(glm::vec3(0.0f, 4.0f, 0.0f));
		mpCamera->OffsetOrientation(0.0f, 60.0f);//temp
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
	//Shaders
	
	// Background Mesh
	if(mpProgramBgMesh == nullptr)
	{
		mpProgramBgMesh = CreateProgram("Shaders/BackgroundMesh/bgmesh.vertexshader", "Shaders/BackgroundMesh/bgmesh.fragmentshader");
	}
	else
	{
		fprintf(stderr, "WARNING: mpProgramCube is being used due to it not having a default value of nullptr. Attempting to continue regardless. Incorrect rendering may occur unless this is intentional.\n");
	}

	// Cube
	if(mpProgramCube == nullptr)
	{
		mpProgramCube = CreateProgram("Shaders/vertexShader.txt", "Shaders/fragmentShader.txt");
	}
	else
	{
		fprintf(stderr, "WARNING: mpProgramCube is being used due to it not having a default value of nullptr. Attempting to continue regardless. Incorrect rendering may occur unless this is intentional.\n");
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
	if(mpTextureCube == nullptr)
	{
		mpTextureCube = new DSGraphics::Texture("../../Resources/Textures/stripes2.png");
	}
	else
	{
		fprintf(stderr, "WARNING: mpTextureCube is being used due to it not having a default value of nullptr. Attempting to continue regardless. Incorrect texture may appear unless this is intentional.\n");
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
	//Background Mesh
	LoadModelAssetBgMesh();

	//Cube
	LoadModelAssetCube();
}

//-----------------------------------------------------------------------------

void Application::LoadModelAssetBgMesh()
{
	const unsigned int kPositionDimensions = 3;

	printf("Setting pVerticesVec3 initial values.\n");

	glm::vec3* pVerticesVec3 = new glm::vec3[static_cast<int>(mBgMeshSize.x * mBgMeshSize.y)];

	for(int row = 0; row < static_cast<int>(mBgMeshSize.y); ++row)
	{
		for(int col = 0; col < static_cast<int>(mBgMeshSize.x); ++col)
		{
			pVerticesVec3[(static_cast<int>(mBgMeshSize.x) * row) + col] = glm::vec3(col * mBgMeshLengthX, 0.0f, row * (-mBgMeshLengthZ));
			printf("pVerticesVec3[%u] = x: %f, y: %f, z: %f\n", ((static_cast<int>(mBgMeshSize.x) * row) + col), (col * mBgMeshLengthX), 0.0f, (row * (-mBgMeshLengthZ)));
		}
	}

	printf("\nConverting pVerticesVec3 to pVertices.\n");

	//Convert pVerticesVec3 to GLfloat by creating a new array 3 times as big and deep copying (TODO: make this more efficient)
	const unsigned int kVertexCount = static_cast<int>(3 * mBgMeshSize.x * mBgMeshSize.y);
	GLfloat* pVertices = new GLfloat[kVertexCount];
	
	GLfloat* pVerticesIterator = nullptr;
	for(int i = 0; i < static_cast<int>(mBgMeshSize.x * mBgMeshSize.y); ++i)
	{
		pVerticesIterator = &pVertices[i * 3];
		*pVerticesIterator = pVerticesVec3[i].x;
		++pVerticesIterator;
		*pVerticesIterator = pVerticesVec3[i].y;
		++pVerticesIterator;
		*pVerticesIterator = pVerticesVec3[i].z;
		//pVertices[i * 3] = pVerticesVec3[i].x;
		printf("pVertices[%u] = x: %f\n", (i * 3), pVerticesVec3[i].x);
	
		//pVertices[(i * 3) + 1] = pVerticesVec3[i].y;
		printf("pVertices[%u] = x: %f\n", ((i * 3) + 1), pVerticesVec3[i].y);
		
		//pVertices[(i * 3) + 2] = pVerticesVec3[i].z;
		printf("pVertices[%u] = x: %f\n", ((i * 3) + 1), pVerticesVec3[i].z);
	}
	pVerticesIterator = nullptr;


	/*
	===
	Q1: How many line segments in an x * y vertex grid?
	===

	SQUARES													RECTANGLES

	12 _ 13 _ 14 _ 15										 4 _  5 _  6
	 |    |    |    |										 |    |    |
	 8 _  9 _ 10 _ 11										 0 _  1 _  2
	 |    |    |    |
	 4 _  5 _  6 _  7
	 |    |    |    |
	 0 _  1 _  2 _  3

	HORIZONTAL												HORIZONTAL
	There are (col - 1) line segments PER row				There are (col - 1) line segments PER row
	= row * (col - 1)										= row * (col - 1)
	= 4   * (4   - 1)										= 2   * (3   - 1)
	= 12													= 4

	VERTICAL												VERTICAL
	There are (row - 1) line segments PER col				There are (row - 1) line segments PER col
	= col * (row - 1)										= col * (row - 1)
	= 4   * (4   - 1)										= 3   * (2   - 1)
	= 12													= 3

	TOTAL													TOTAL
	= Horizontal + Vertical									= Horizontal + Vertical
	= 12         + 12										= 4          + 3
	= 24													= 7

	= (row * (col - 1)) + (col * (row - 1))					= (row * (col - 1)) + (col * (row - 1))
	= (4   * (4   - 1)) + (4   * (4   - 1))					= (2   * (3   - 1)) + (3   * (2   - 1))
	= (4 * 3) + (4 * 3)										= (2 * 2) + (3 * 1)
	= 12 + 12												= 4 + 3
	= 24													= 7

	OR (more efficiently)									OR (more efficiently)

	= (col * row * 2) - row - col							= (col * row * 2) - row - col
	= (4   * 4   * 2) - 4   - 4								= (3   * 2   * 2) - 2   - 3
	= (16 * 2) - 8											= (6 * 2) - 5
	= 32 - 8												= 12 - 5
	= 24													= 7

	===
	Q2: How to calculate elements of line segments in an x * y vertex grid?
	===
	*/

	//const unsigned int kElementCount = static_cast<int>((mBgMeshSize.y * (mBgMeshSize.x - 1)) + (mBgMeshSize.x * (mBgMeshSize.y - 1)));
	//const unsigned int kElementCount = (mBgMeshSize.x * mBgMeshSize.y * 2) - mBgMeshSize.y - mBgMeshSize.x;

	printf("Setting pElements initial values.\n");

	//Note: I realised I can use line segment strips instead of line segments, as it uses fewer elements, so I am doing so for now. Later I may need to return to line segments for efficient general relativity animation.
	//const unsigned int verticesPerLineStripHorizontal = static_cast<int>(mBgMeshSize.x);
	//const unsigned int verticesPerLineStripVertical = static_cast<int>(mBgMeshSize.y);
	//const unsigned int kElementCount = verticesPerLineStrip * (mBgMeshSize.x + mBgMeshSize.y)//square calculation. rectangle calculation nets same result. this is more efficient, but works for squares only.
	const unsigned int kElementCount = static_cast<int>((mBgMeshSize.y * mBgMeshSize.x) + (mBgMeshSize.x * mBgMeshSize.y));//const unsigned int kElementCount = (mBgMeshSize.y * verticesPerLineStripHorizontal) + (mBgMeshSize.x * verticesPerLineStripVertical)//alternate way of writing

	GLuint* pElements = new GLuint[kElementCount];

	//Horizontal Line Strips
	printf("- Horizontal Line Strips.\n");
	//pass in one horizontal line strip at a time
	for(int row = 0; row < static_cast<int>(mBgMeshSize.y); ++row)
	{
		for(int col = 0; col < static_cast<int>(mBgMeshSize.x); ++col)
		{
			pElements[(static_cast<int>(mBgMeshSize.x) * row) + col] = (static_cast<int>(mBgMeshSize.x) * row) + col;
			printf("pElements[%u] = %u\n", ((static_cast<int>(mBgMeshSize.x) * row) + col), ((static_cast<int>(mBgMeshSize.x) * row) + col));
			//pElements[(static_cast<int>(mBgMeshSize.x) * row) + col] = (col * row) + col;
			//pElements[(col * row) + col] = (col * row) + col;
		}
	}
	int skipFromHToVLineStrips = static_cast<int>(mBgMeshSize.y * mBgMeshSize.x);
	//Vertical Line Strips
	printf("- Vertical Line Strips.\n");
	//pass in one vertical line strip at a time
	for(int col = 0; col < static_cast<int>(mBgMeshSize.x); ++col)
	{
		for(int row = 0; row < static_cast<int>(mBgMeshSize.y); ++row)
		{
			pElements[skipFromHToVLineStrips + ((static_cast<int>(mBgMeshSize.y) * col) + row)] = static_cast<int>(col + (mBgMeshSize.x * row));
			//pElements[skipFromHToVLineStrips + (col * row) + col] = static_cast<int>(col + (mBgMeshSize.x * row));
			printf("pElements[%u] = %u\n", (skipFromHToVLineStrips + ((static_cast<int>(mBgMeshSize.y) * col) + row)), (static_cast<int>(col + (mBgMeshSize.x * row))));
		}
	}


	if(mpModelAssetBgMesh == nullptr)
	{
		mpModelAssetBgMesh = new DSGraphics::ModelAsset
		(
			mpProgramBgMesh,	//Program
			false,				//Has Texture?
			nullptr,			//Texture
			0,					//Texture Coords Offset
			false,				//Has Colors?
			0,					//rgba Offset
			kVertexCount,		//Vertex Count
			kPositionDimensions,//Position Dimensions
			0,					//Texture Dimensions
			0,					//Color Dimensions
			pVertices,			//Vertices
			true,				//Has Elements?
			kElementCount,		//Element Count Total
			pElements,			//Elements
			GL_LINE_STRIP,		//Draw Type
			mBgMeshSize.x		//Element Count Per Draw Type if variable
		);
	}
	else
	{
		fprintf(stderr, "WARNING: mpModelAssetBgMesh is being used due to it not having a default value of nullptr. Attempting to continue regardless. Incorrect assets may appear unless this is intentional.\n");
	}

	// Clean up temp variables
	if(pVerticesVec3 != nullptr)
	{
		delete[] pVerticesVec3;
		pVerticesVec3 = nullptr;
	}
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

	if(mpModelAssetCube == nullptr)
	{
		mpModelAssetCube = new DSGraphics::ModelAsset
		(
			mpProgramCube,	//Program
			true,			//Has Texture?
			mpTextureCube,	//Texture
			3,				//Texture Coords Offset
			true,			//Has Colors?
			5,				//rgba Offset
			kVertexCount,	//Vertex Count
			3,				//Position Dimensions
			2,				//Texture Dimensions
			4,				//Color Dimensions
			pVertices,		//Vertices
			true,			//Has Elements?
			kElementCount,	//Element Count
			pElements,		//Elements,
			GL_LINES,		//Draw Type
			0
		);
	}
	else
	{
		fprintf(stderr, "WARNING: mpModelAssetCube is being used due to it not having a default value of nullptr. Attempting to continue regardless. Incorrect assets may appear unless this is intentional.\n");
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
	//Background
	// Mesh
	DSGraphics::ModelInstance bgMesh(mpModelAssetBgMesh, mpCamera);
	mModelInstancesBackgroundList.push_back(bgMesh);


	//Cube
	DSGraphics::ModelInstance cube0001(mpModelAssetCube, mpCamera);
	mModelInstancesList.push_back(cube0001);

	DSGraphics::ModelInstance cube0002(mpModelAssetCube, mpCamera);
	cube0002.SetSize(glm::vec3(0.5f));
	cube0002.SetPosition(glm::vec3(2.0f, -1.5f, -2.5f));
	cube0002.UpdateTransform();
	mModelInstancesList.push_back(cube0002);

	//Set initial rotation axis for cube00001
	mModelInstancesList.front().SetOrientationAxis(glm::vec3(0.0f, 1.0f, 0.0f));
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
	// Rect
	if(glfwGetKey(mpWindow, 'X'))
	{
		mModelInstancesList.front().SetOrientationAxis(glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else if(glfwGetKey(mpWindow, 'Y'))
	{
		mModelInstancesList.front().SetOrientationAxis(glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if(glfwGetKey(mpWindow, 'Z'))
	{
		mModelInstancesList.front().SetOrientationAxis(glm::vec3(0.0f, 0.0f, 1.0f));
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
	mModelInstancesList[0].SetOrientationAngle(glm::radians(mRectDegreesRotated));//this is the same as the next line, just written differently
	//mModelInstancesList.front().SetOrientationAngle(glm::radians(mRectDegreesRotated));
	//mListModelInstances.front().SetRotate(glm::rotate(glm::mat4(1.0f), glm::radians(mRectDegreesRotated), mListModelInstances.front().GetRotationAxis()));
	mModelInstancesList.front().UpdateTransform();
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

	//Instance Lists
	
	// Background
	std::vector<DSGraphics::ModelInstance>::iterator it;
	for(it = mModelInstancesBackgroundList.begin(); it != mModelInstancesBackgroundList.end(); ++it)
	{
		it->Render();
	}

	// Cube
	//TODO: learn what kind of iterator I need here
	//std::list<DSGraphics::ModelInstance>::iterator it;
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
	//Background Mesh
	if(mpModelAssetBgMesh != nullptr)
	{
		delete mpModelAssetBgMesh;
		mpModelAssetBgMesh = nullptr;
	}

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
	mModelInstancesBackgroundList.clear();
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