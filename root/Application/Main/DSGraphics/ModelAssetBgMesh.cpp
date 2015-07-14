//=============================================================================
// File:		ModelAssetBgMesh.cpp
// Created:		2015/02/19
// Last Edited:	2015/02/19
// Copyright:	Daniel Schenker
// Description:	ModelAsset Bg Mesh
//=============================================================================

//=============================================================================
//Includes
//=============================================================================

// Daniel Schenker
#include "ModelAssetBgMesh.h"

//=============================================================================
//Statics
//=============================================================================

//=============================================================================
//Class Definitions
//=============================================================================

//-----------------------------------------------------------------------------
// Constuctors
//-----------------------------------------------------------------------------

DSGraphics::ModelAssetBgMesh::ModelAssetBgMesh
(
	DSGraphics::Program* pProgram,
	bool hasTexture,
	DSGraphics::Texture* pTexture,
	unsigned int textureCoordsOffset,
	bool hasColors,
	unsigned int rgbaOffset,
	unsigned int vertexCount,
	unsigned int positionDimensions,
	unsigned int textureDimensions,
	unsigned int colorDimensions,
	glm::vec3* pVerticesVec3,
	glm::vec3 bgMeshDistanceBetweenVertices
)
:	mpProgram(pProgram)
,	mkHasTexture(hasTexture)
,	mpTexture(pTexture)
,	mkTextureCoordsOffset(textureCoordsOffset)
,	mkHasColors(hasColors)
,	mkRgbaOffset(rgbaOffset)
,	mVao(0)
,	mkVertexCount(vertexCount)
,	mkDataBitsPerVertex(positionDimensions + textureDimensions + colorDimensions)
,	mkPositionDimensions(positionDimensions)
,	mkTextureDimensions(textureDimensions)
,	mkColorDimensions(colorDimensions)
,	mpVerticesVec3(nullptr)
,	mpVertices(nullptr)
,	mVbo(0)
,	mkHasElements(true)
,	mEbo(0)
,	mkElementCountTotal(0)
,	mpElements(nullptr)
,	mDrawType(GL_LINE_STRIP)
,	mDrawStart(0)
,	mBgMeshDistanceBetweenVertices(bgMeshDistanceBetweenVertices)
{
	//Deep Copy Data
	
	// Vertex
	if(positionDimensions != 3)
	{
		printf("ERROR: Not 3 dimensions: xyz\n");//TODO: Make this error more informative
	}
	mpVerticesVec3 = new glm::vec3[mkVertexCount * positionDimensions];
	for(unsigned int i = 0; i < mkVertexCount * positionDimensions; ++i)
	{
		mpVerticesVec3[i] = pVerticesVec3[i];
	}

	CalculateVertices();
	CalculateVerticesPerLine();

	//Vertex Array Object (VAO)
	//Note:	VAOs store all of the links between attributes and their corresponding VBOs with raw vertex data.
	// Creation
	glGenVertexArrays(1, &mVao);
	// Enable Usage (by binding)
	glBindVertexArray(mVao);


	//Vertex Buffer Object (VBO)
	//Note:	Since OpenGL manages memory, GLuint vboTriangle is a positive number reference to the memory that the VBO will occupy.
	// Creation
	glGenBuffers(1, &mVbo);
	// Set a specific VBO as the active object in the array buffer
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);
	// Copy vertex data to the active object previously specified to the array buffer
	glBufferData
	(
		GL_ARRAY_BUFFER,												//currently active array buffer
		sizeof(mpVertices[0]) * mkVertexCount * mkDataBitsPerVertex,	//size of vertices data in bytes
		mpVertices,														//actual vertices data
		GL_STATIC_DRAW													//usage of vertex data
	);


	//Element  Buffer Objects (EBO)
	if(mkHasElements == true)
	{
		glGenBuffers(1, &mEbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
		glBufferData
		(
			GL_ELEMENT_ARRAY_BUFFER,
			sizeof(mpElements[0]) * mkElementCountTotal,
			mpElements,
			GL_STATIC_DRAW
		);
	}


	//Specify the layout of the vertex data
	
	// Position
	GLint posAttrib = glGetAttribLocation(mpProgram->GetProgramID(), "vsInPosition");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer
	(
		posAttrib,								//which attribute? in this case, referencing vsInPosition.
		mkPositionDimensions,					//number of values (size) for attribute (vsInPosition). in this case, 3 for x, y, z
		GL_FLOAT,								//type of each component
		GL_FALSE,								//should attribute (input) values be normalized?
		mkDataBitsPerVertex * sizeof(GLfloat),	//stride (how many bytes between each position attribute in the array)
		0										//array buffer offset
	);

	//  Texture
	if(mkHasTexture == true)
	{
		GLint texAttrib = glGetAttribLocation(mpProgram->GetProgramID(), "vsInTexCoord");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer
		(
			texAttrib,											//which attribute? in this case, referencing vsInTexCoord.
			mkTextureDimensions,								//number of values (size) for attribute (vsInTexCoord). in this case, 2 for u, v
			GL_FLOAT,											//type of each component
			GL_FALSE,											//should attribute (input) values be normalized?
			mkDataBitsPerVertex * sizeof(GLfloat),				//stride (how many bytes between each position attribute in the array)
			(void*)(mkTextureCoordsOffset * sizeof(GLfloat))	//array buffer offset
		);
	}

	// Color
	if(mkHasColors)
	{
		GLint colAttrib = glGetAttribLocation(mpProgram->GetProgramID(), "vsInColor");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer
		(
			colAttrib,								//which attribute? in this case, referencing vsInColor.
			mkColorDimensions,						//number of values (size) for attribute (vsInColor). in this case, 4 for r, g, b, a
			GL_FLOAT,								//type of each component
			GL_FALSE,								//should attribute (input) values be normalized?
			mkDataBitsPerVertex * sizeof(float),	//stride (how many bytes between each position attribute in the array)
			(void*)(mkRgbaOffset * sizeof(GLfloat))	//array buffer offset
		);
	}

	//Unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------

DSGraphics::ModelAssetBgMesh::~ModelAssetBgMesh()
{
	// Rectangle
	if(mpVertices != nullptr)
	{
		delete[] mpVertices;
		mpVertices = nullptr;
	}
	if(mkHasElements == true)
	{
		glDeleteBuffers(1, &mEbo);
		if(mpElements != nullptr)
		{
			delete[] mpElements;
			mpElements = nullptr;
		}
	}
	glDeleteBuffers(1, &mVbo);
	glDeleteVertexArrays(1, &mVao);
}

//-----------------------------------------------------------------------------
// Public Member Functions
//-----------------------------------------------------------------------------
// General

//-----------------------------------------------------------------------------
// Private Member Functions
//-----------------------------------------------------------------------------

void DSGraphics::ModelAssetBgMesh::CalculateVertices()
{
}

//-----------------------------------------------------------------------------

void DSGraphics::ModelAssetBgMesh::CalculateVerticesPerLine()
{
	printf("Setting pElements initial values.\n");

	//HORIZONTAL

	// Calculate how many vertices there are per horizontal line

	unsigned int verticesInThisLine = 1;

	//for vertical calculation later on in order to determine how many vertical lines there are
	//initialize as being set to first vertex in the background (bottom left)
	glm::vec3 furthestNegXVertex = mpVerticesVec3[0];
	glm::vec3 furthestPosXVertex = furthestNegXVertex;


	//since no general relativity effects have been applied yet, it's safe to say that the z (vertical offset) values of each vertex in the same horizontal line should be identical to each other
	for(unsigned int i = 0; i < mkVertexCount - 1; ++i)//TODO: double check the - 1
	{
		// For vertical calculations later on
		//Furthest negative x
		if(mpVerticesVec3[i].x < furthestNegXVertex.x)
		{
			furthestNegXVertex = mpVerticesVec3[i];
		}
		//Furthest positive x
		else if(mpVerticesVec3[i].x > furthestPosXVertex.x)
		{
			furthestPosXVertex = mpVerticesVec3[i];
		}

		//if the next vertex in the positive x direction has the same z value as the current vertex
		if(mpVerticesVec3[i + 1].z == mpVerticesVec3[i].z)
		{
			++verticesInThisLine;
		}
		//since the z values are different, this means that we are on the next horizontal line
		else
		{
			//store the count of how many vertices there were in the previous line
			mVerticesPerHorizontalLineList.push_back(verticesInThisLine);

			//prepare for testing the next line
			verticesInThisLine = 1;
		}
	}
	//we now know how many vertices there are in each horizontal line


	//Vertical

	// Calculate how many vertices there are per vertical line

	verticesInThisLine = 1;
	unsigned int curHorLine = 0;
	unsigned int vertexToCheck = 0;

	//# of vertical lines = (distance between furthestPosXVertex.x and furthestNegXVertex.x) / horizontal distance between each vertex
	fabsf(furthestPosXVertex.x - furthestNegXVertex.x) / mBgMeshDistanceBetweenVertices;

	//Start with the furthest left (most negative x) vertex, and work our way right (towards positive x)
	//!!!CONTINUE HERE


	//since no general relativity effects have been applied yet, it's safe to say that the x (horizontal offset) values of each vertex in the same vetical line should be identical to each other
	for(unsigned int i = 0; i < mkVertexCount - 1; ++i)//TODO: double check the - 1
	{
		//skip ahead to the horizontal line above us
		vertexToCheck += mVerticesPerHorizontalLineList[curHorLine]


		////if the next vertex in the positive x direction has the same z value as the current vertex, we are still in the same horizontal line, so continue until we get to the next line
		if(mpVerticesVec3[i + 1].z == mpVerticesVec3[i].z)
		{
			++verticesInThisHorizontalLine;
		}
		//since the z values are different, this means that we are on the next horizontal line
		else
		{
			//store the count of how many vertices there were in the previous line
			mVerticesPerHorizontalLineList.push_back(verticesInThisHorizontalLine);

			//prepare for testing the next line
			verticesInThisHorizontalLine = 1;
		}
	}
	//we now know how many vertices there are in each horizontal line

	// Calculate mpElements
	//use mpVerticesVec3 as mpElements, because there are the same # of elements as there are vertices in the game world grid
	/*
	//there are mVerticesPerHorizontalLineList.size() horizontal lines
	._._.
	._.
	._._.
	3 lines
	*/








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
}

//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Getters
//-----------------------------------------------------------------------------

GLuint DSGraphics::ModelAssetBgMesh::GetProgramID() const
{
	return mpProgram->GetProgramID();
}

//-----------------------------------------------------------------------------

bool DSGraphics::ModelAssetBgMesh::GetHasTexture() const
{
	return mkHasTexture;
}

//-----------------------------------------------------------------------------

GLuint DSGraphics::ModelAssetBgMesh::GetTextureObjectID() const
{
	return mpTexture->GetObjectID();
}

//-----------------------------------------------------------------------------

GLuint DSGraphics::ModelAssetBgMesh::GetVao() const
{
	return mVao;
}

//-----------------------------------------------------------------------------

unsigned int DSGraphics::ModelAssetBgMesh::GetVertexCount() const
{
	return mkVertexCount;
}

//-----------------------------------------------------------------------------

bool DSGraphics::ModelAssetBgMesh::GetHasElements() const
{
	return mkHasElements;
}

//-----------------------------------------------------------------------------

unsigned int DSGraphics::ModelAssetBgMesh::GetElementCountTotal() const
{
	return mkElementCountTotal;
}

//-----------------------------------------------------------------------------

GLenum DSGraphics::ModelAssetBgMesh::GetDrawType() const
{
	return mDrawType;
}

//-----------------------------------------------------------------------------
// Setters
//-----------------------------------------------------------------------------