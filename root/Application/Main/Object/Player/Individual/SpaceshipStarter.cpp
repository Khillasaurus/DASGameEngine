//=============================================================================
// File:		SpaceshipStarter.cpp
// Created:		2015/02/27
// Last Edited:	2015/02/27
// Copyright:	Daniel Schenker
// Description:	Object > Player > SpaceshipStarter.
//=============================================================================

//=============================================================================
//Includes
//=============================================================================

// Daniel Schenker
#include "SpaceshipStarter.h"

//=============================================================================
//Statics
//=============================================================================

//=============================================================================
//Class Definitions
//=============================================================================

//-----------------------------------------------------------------------------
// Constuctors
//-----------------------------------------------------------------------------

SpaceshipStarter::SpaceshipStarter()
{
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------

SpaceshipStarter::~SpaceshipStarter()
{
}

//-----------------------------------------------------------------------------
// Public Member Functions
//-----------------------------------------------------------------------------

void SpaceshipStarter::LoadAsset(DSGraphics::Program* pProgram, DSGraphics::Texture* pTexture)
{
	//Vertices

	const unsigned int kVertexCount = 23;
	const unsigned int kPositionDimensions = 3;
	const unsigned int kTextureDimensions = 2;
	const unsigned int kColorDimensions = 4;
	const unsigned int kDataBitsPerVertex = kPositionDimensions + kTextureDimensions + kColorDimensions;

	GLfloat* pVertices = new GLfloat[kVertexCount * kDataBitsPerVertex]
	{
		/*
		OVERALL
		//TODO: Draw the overall ship design using ascii art.

		FLOOR
						2		_
		   /\		   /\		|		Length:	 8m
		  /  \		  /  \		|	}	Width:	 4m
		 /____\		0/____\1	|		Height:	-1m
								-


		MAIN LEVEL
											_
						6					|
					  /   \					|
					 /  5  \				|
					/ 3   4 \				|		Length:	10m	
				   / 1     2 \				|	}	Width:	8m	
				 9/           \0			|		Height:	2.5m
			    /   7       8   \			|						//Note: Vertices 7 and 8 are slightly elevated.
			 3/______4_____5______\6		|
											-


		UPPER ACCENTS

						x
					  /   \
					 /  x  \
					/ x1 2x \
				   / x 9 0 x \
				 x/           \x
			    / 7 x       x 8 \
			 x/______x_____x______\x

		*/

		//Position																								Texture						Color
		//x					y					z																							r		g		b		a

		//FLOOR
		-2.0f * Object::sDCPerM,	-1.0f * Object::sDCPerM,	 4.0f * Object::sDCPerM,						1.0f,	1.0f,				1.0f,	0.0f,	0.0f,	1.0f,	//0
		 2.0f * Object::sDCPerM,	-1.0f * Object::sDCPerM,	 4.0f * Object::sDCPerM,						1.0f,	1.0f,				1.0f,	0.0f,	0.0f,	1.0f,	//1
		 0.0f,						-1.0f * Object::sDCPerM,	-4.0f * Object::sDCPerM,						1.0f,	1.0f,				1.0f,	0.0f,	0.0f,	1.0f,	//2

		//MAIN LEVEL
		-4.0f * Object::sDCPerM,	 0.0f,						 5.0f * Object::sDCPerM,						1.0f,	1.0f,				1.0f,	0.0f,	1.0f,	1.0f,	//3
		-1.0f * Object::sDCPerM,	 0.0f,						 5.0f * Object::sDCPerM,						1.0f,	1.0f,				1.0f,	0.0f,	1.0f,	1.0f,	//4
		 1.0f * Object::sDCPerM,	 0.0f,						 5.0f * Object::sDCPerM,						1.0f,	1.0f,				1.0f,	0.0f,	1.0f,	1.0f,	//5
		 4.0f * Object::sDCPerM,	 0.0f,						 5.0f * Object::sDCPerM,						1.0f,	1.0f,				1.0f,	0.0f,	1.0f,	1.0f,	//6
		-1.5f * Object::sDCPerM,	 0.5f * Object::sDCPerM,	 3.0f * Object::sDCPerM,						1.0f,	1.0f,				1.0f,	0.0f,	1.0f,	1.0f,	//7
		 1.5f * Object::sDCPerM,	 0.5f * Object::sDCPerM,	 3.0f * Object::sDCPerM,						1.0f,	1.0f,				1.0f,	0.0f,	1.0f,	1.0f,	//8
		-2.0f * Object::sDCPerM,	 0.0f,						 1.0f * Object::sDCPerM,						1.0f,	1.0f,				1.0f,	0.0f,	1.0f,	1.0f,	//9
		 2.0f * Object::sDCPerM,	 0.0f,						 1.0f * Object::sDCPerM,						1.0f,	1.0f,				1.0f,	0.0f,	1.0f,	1.0f,	//10
		-0.8f * Object::sDCPerM,	 0.0f,						 0.5f * Object::sDCPerM,						1.0f,	1.0f,				0.0f,	0.0f,	1.0f,	1.0f,	//11
		 0.8f * Object::sDCPerM,	 0.0f,						 0.5f * Object::sDCPerM,						1.0f,	1.0f,				0.0f,	0.0f,	1.0f,	1.0f,	//12
		-1.0f * Object::sDCPerM,	 0.0f,						-0.75f * Object::sDCPerM,						1.0f,	1.0f,				0.0f,	0.0f,	1.0f,	1.0f,	//13
		 1.0f * Object::sDCPerM,	 0.0f,						-0.75f * Object::sDCPerM,						1.0f,	1.0f,				0.0f,	0.0f,	1.0f,	1.0f,	//14
		 0.0f,						 0.0f,						-2.0f * Object::sDCPerM,						1.0f,	1.0f,				0.0f,	0.0f,	1.0f,	1.0f,	//15
		 0.0f,						 0.0f,						-5.0f * Object::sDCPerM,						1.0f,	1.0f,				1.0f,	0.0f,	1.0f,	1.0f,	//16

		//UPPER ACCENTS
		-2.5f * Object::sDCPerM,	 1.5f * Object::sDCPerM,	 3.67f * Object::sDCPerM,						1.0f,	1.0f,				1.0f,	0.0f,	0.0f,	1.0f,	//17
		 2.5f * Object::sDCPerM,	 1.5f * Object::sDCPerM,	 3.67f * Object::sDCPerM,						1.0f,	1.0f,				1.0f,	0.0f,	0.0f,	1.0f,	//18
		-0.5f * Object::sDCPerM,	 1.0f * Object::sDCPerM,	 0.0f,											1.0f,	1.0f,				0.0f,	1.0f,	1.0f,	1.0f,	//19
		 0.5f * Object::sDCPerM,	 1.0f * Object::sDCPerM,	 0.0f,											1.0f,	1.0f,				0.0f,	1.0f,	1.0f,	1.0f,	//20
		-0.5f * Object::sDCPerM,	 1.0f * Object::sDCPerM,	-0.67f * Object::sDCPerM,						1.0f,	1.0f,				0.0f,	1.0f,	1.0f,	1.0f,	//21
		 0.5f * Object::sDCPerM,	 1.0f * Object::sDCPerM,	-0.67f * Object::sDCPerM,						1.0f,	1.0f,				0.0f,	1.0f,	1.0f,	1.0f	//22
	};

	//Elements

	const unsigned int kElementsPerDrawType = 3;//Triangle
	const unsigned int kDrawTypeElements = 42;
	const unsigned int kElementCount = kElementsPerDrawType * kDrawTypeElements;

	GLuint* pElements = new GLuint[kElementCount]
	{
		//FLOOR (Count: 1)
		0, 1, 2,

		//FLOOR - MAIN LEVEL (Count: 10)
		0, 3, 4,
		0, 3, 9,
		0, 2, 9,
		0, 4, 5,
		1, 5, 0,
		1, 5, 6,
		1, 10, 6,
		1, 10, 2,
		2, 16, 9,
		2, 16, 10,

		//ROOF (Count: 31)
		// Left Wing (Count: 4)
		17, 3, 4,
		17, 7, 4,
		17, 7, 9,
		17, 3, 9,
		// Right Wing (Count: 4)
		18, 6, 5,
		18, 8, 5,
		18, 8, 10,
		18, 6, 10,
		// Rear Flat Middle (Count: 2)
		4, 5, 7,
		8, 5, 7,
		// Center (Count: 4)
		8, 11, 7,
		8, 11, 12,
		8, 10, 12,
		9, 11, 7,
		// Front Body (Count: 6)
		11, 13, 9,
		16, 13, 9,
		16, 13, 15,
		16, 14, 15,
		16, 14, 10,
		12, 14, 10,
		// Captain's Windows (Count: 11)
		19, 11, 12,	//Back (part 1)
		19, 11, 13,	//Left (part 1)
		19, 21, 13,	//Left (part 2)
		21, 15, 13,	//Front Left
		21, 15, 22,	//Front Center
		14, 15, 22,	//Front Right
		14, 12, 22,	//Right (part 1)
		20, 12, 22,	//Right (part 2)
		20, 12, 19,	//Back (part 2)
		20, 21, 19,	//Roof (part 1)
		20, 21, 22	//Roof (part 2)
	};

	//ModelAsset Creation

	if(mpModelAsset == nullptr)
	{
		mpModelAsset = new DSGraphics::ModelAsset
		(
			pProgram,									//Program
			true,										//Has Texture?
			pTexture,									//Texture
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