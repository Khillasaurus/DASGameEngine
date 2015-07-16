//=============================================================================
// File:		ModelAsset.h
// Created:		2015/02/15
// Last Edited:	2015/02/19
// Copyright:	Daniel Schenker
// Description:	ModelAsset
//=============================================================================

#ifndef MODELASSET_H
#define MODELASSET_H

//=============================================================================
//Includes
//=============================================================================

// Third-Party Libraries
//  GLEW
#include <GL/glew.h>

// Daniel Schenker
#include "Program.h"
#include "Texture.h"

//=============================================================================
//Forward Declarations
//=============================================================================

//=============================================================================
//Namespace
//=============================================================================

namespace DSGraphics
{

	//=============================================================================
	//Enums
	//=============================================================================

	//=============================================================================
	//Class Declarations
	//=============================================================================

	class ModelAsset
	{
	public:
		//Constructors
		ModelAsset
		(
			//TODO: Change const correctness for Program and Texture
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
			GLfloat* pVertices,
			bool hasElements,
			unsigned int elementCountTotal,
			GLuint* pElements,
			GLenum drawType,
			unsigned int elementCountPerDrawType
		);
		//Destructor
		~ModelAsset();

		//Member Functions
	public:
		// Getters
		GLuint GetProgramID() const;
		bool GetHasTexture() const;
		GLuint GetTextureObjectID() const;
		GLuint GetVao() const;
		unsigned int GetVertexCount() const;
		bool GetHasElements() const;
		unsigned int GetElementCountTotal() const;
		GLenum GetDrawType() const;
		unsigned int GetElementCountPerDrawType() const;
		// Setters

		//Member Variables
	private:
		// Program
		DSGraphics::Program* mpProgram;
		// Texture
		const unsigned int mkTextureCount;
		DSGraphics::Texture* mpTexture;
		const unsigned int mkTextureCoordsOffset;
		// Colors
		const bool mkHasColors;
		const unsigned int mkRgbaOffset;
		// VAO
		GLuint mVao;
		// Vertex Data
		const unsigned int mkVertexCount;
		const unsigned int mkDataBitsPerVertex;
		const unsigned int mkPositionDimensions;
		const unsigned int mkTextureDimensions;
		const unsigned int mkColorDimensions;
		GLfloat* mpVertices;//TODO: change this to be a vector or something that knows the length of the array
		// VBO
		GLuint mVbo;
		// Elements
		const bool mkHasElements;
		GLuint mEbo;
		const unsigned int mkElementCountTotal;
		GLuint* mpElements;//TODO: make same change as with mpVertices
		// Draw Info
		GLenum mDrawType;
		unsigned int mElementCountPerDrawType;//CHECK: is this safe for all type? eg, I doubt it's safe for line segments created from rectangles that are not square
		GLint mDrawStart;
	};

}//namespace DSGraphics

#endif //#ifndef MODELASSET_H