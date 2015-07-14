//=============================================================================
// File:		ModelAsset.h
// Created:		2015/02/15
// Last Edited:	2015/02/18
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
			unsigned int elementCount,
			GLuint* pElements,
			GLenum drawType
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
		unsigned int GetElementCount() const;
		GLenum GetDrawType() const;
		// Setters

		//Member Variables
	private:
		// Program
		DSGraphics::Program* mpProgram;
		// Texture
		const bool mkHasTexture;
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
		const unsigned int mkElementCount;
		GLuint* mpElements;//TODO: make same change as with mpRectVertices
		// Draw Info
		GLenum mDrawType;
		GLint mDrawStart;
	};

}//namespace DSGraphics

#endif //#ifndef MODELASSET_H