//=============================================================================
// File:		ModelAsset.cpp
// Created:		2015/02/15
// Last Edited:	2015/02/16
// Copyright:	Daniel Schenker
// Description:	ModelAsset
//=============================================================================

//=============================================================================
//Includes
//=============================================================================

// Daniel Schenker
#include "ModelAsset.h"
#include "Program.h"

//=============================================================================
//Statics
//=============================================================================

//=============================================================================
//Class Definitions
//=============================================================================

//-----------------------------------------------------------------------------
// Constuctors
//-----------------------------------------------------------------------------

ModelAsset::ModelAsset
(
	Program* pProgram,
	bool hasTexture,
	Texture* pTexture,
	unsigned int textureCoordsOffset,
	bool hasColors,
	unsigned int rgbaOffset,
	unsigned int vertexCount,
	unsigned int positionDimensions,
	unsigned int textureDimensions,
	unsigned int colorDimensions,
	GLfloat* pVertices,
	unsigned int elementCount,
	GLuint* pElements,
	GLenum drawType
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
,	mpVertices(nullptr)
,	mVbo(0)
,	mEbo(0)
,	mkElementCount(elementCount)
,	mpElements(nullptr)
,	mDrawType(drawType)
,	mDrawStart(0)
{
	//Deep Copy Data
	
	// Vertex
	mpVertices = new GLfloat[mkVertexCount * mkDataBitsPerVertex];
	for(unsigned int i = 0; i < mkVertexCount * mkDataBitsPerVertex; ++i)
	{
		mpVertices[i] = pVertices[i];
	}

	// Element
	mpElements = new GLuint[mkElementCount];
	for(unsigned int i = 0; i < mkElementCount; ++i)
	{
		mpElements[i] = pElements[i];
	}

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
	glGenBuffers(1, &mEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
	glBufferData
	(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(mpElements[0]) * mkElementCount,
		mpElements,
		GL_STATIC_DRAW
	);


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

ModelAsset::~ModelAsset()
{
	// Rectangle
	if(mpVertices != nullptr)
	{
		delete[] mpVertices;
		mpVertices = nullptr;
	}
	glDeleteBuffers(1, &mEbo);
	if(mpElements != nullptr)
	{
		delete[] mpElements;
		mpElements = nullptr;
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

//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Getters
//-----------------------------------------------------------------------------

GLuint ModelAsset::GetProgramID() const
{
	return mpProgram->GetProgramID();
}

//-----------------------------------------------------------------------------

GLuint ModelAsset::GetTextureObjectID() const
{
	return mpTexture->GetObjectID();
}

//-----------------------------------------------------------------------------

GLuint ModelAsset::GetVao() const
{
	return mVao;
}

//-----------------------------------------------------------------------------

unsigned int ModelAsset::GetElementCount() const
{
	return mkElementCount;
}

//-----------------------------------------------------------------------------

GLenum ModelAsset::GetDrawType() const
{
	return mDrawType;
}

//-----------------------------------------------------------------------------
// Setters
//-----------------------------------------------------------------------------