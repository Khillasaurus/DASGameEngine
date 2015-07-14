//=============================================================================
// File:		Wall.cpp
// Created:		2015/02/27
// Last Edited:	2015/02/27
// Copyright:	Daniel Schenker
// Description:	Object > Environmental > Wall.
//=============================================================================

//=============================================================================
//Includes
//=============================================================================

// Daniel Schenker
#include "Wall.h"

//=============================================================================
//Statics
//=============================================================================

//=============================================================================
//Class Definitions
//=============================================================================

//-----------------------------------------------------------------------------
// Constuctors
//-----------------------------------------------------------------------------

Wall::Wall()
{
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------

Wall::~Wall()
{
}

//-----------------------------------------------------------------------------
// Public Member Functions
//-----------------------------------------------------------------------------

void Wall::LoadAsset(DSGraphics::Program* pProgram, DSGraphics::Texture* pTexture)
{
	//Vertices

	const unsigned int kVertexCount = 6;
	const unsigned int kPositionDimensions = 3;
	const unsigned int kColorDimensions = 4;
	const unsigned int kDataBitsPerVertex = kPositionDimensions + kColorDimensions;

	GLfloat* pVertices = new GLfloat[kVertexCount * kDataBitsPerVertex]
	{
		/*
			3---4---5
			|       |	2x1m wall
			0---1---2
		*/

		//Position													Color
		//x							 y					 z		/**/r		g		b		a

		//Outside
		 0.0f,						 0.0f,				 0.0f,	/**/1.0f,	0.0f,	0.0f,	1.0f,	//0
		 Object::sDCPerM,			 0.0f,				 0.0f,	/**/1.0f,	1.0f,	0.0f,	1.0f,	//1
		 2.0f * Object::sDCPerM,	 0.0f,				 0.0f,	/**/1.0f,	0.0f,	0.0f,	1.0f,	//2
		 0.0f,						 Object::sDCPerM,	 0.0f,	/**/1.0f,	0.0f,	1.0f,	1.0f,	//3
		 Object::sDCPerM,			 Object::sDCPerM,	 0.0f,	/**/0.0f,	0.0f,	1.0f,	1.0f,	//4
		 2.0f * Object::sDCPerM,	 Object::sDCPerM,	 0.0f,	/**/1.0f,	0.0f,	1.0f,	1.0f	//5
	};

	//Elements

	const unsigned int kElementsPerDrawType = 3;//Triangle
	const unsigned int kDrawTypeElements = 2 * 2;//2 triangles per square * 2 squares per 2x1 wall
	const unsigned int kElementCount = kElementsPerDrawType * kDrawTypeElements;

	GLuint* pElements = new GLuint[kElementCount]
	{
		4, 3, 0,
		4, 1, 0,
		4, 1, 2,
		4, 5, 2
	};

	//ModelAsset Creation

	if(mpModelAsset == nullptr)
	{
		mpModelAsset = new DSGraphics::ModelAsset
		(
			pProgram,			//Program
			false,				//Has Texture?
			nullptr,			//Texture
			0,					//Texture Coords Offset
			true,				//Has Colors?
			kPositionDimensions,//rgba Offset
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
		fprintf(stderr, "WARNING: mpModelAsset is being used due to it not having a default value of nullptr. Attempting to continue regardless. Incorrect assets may appear unless this is intentional.\n");
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

	mIsModelAssetLoaded = true;
}

//-----------------------------------------------------------------------------
// Private Member Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Getters
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Setters
//-----------------------------------------------------------------------------