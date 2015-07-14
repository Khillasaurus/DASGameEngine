//=============================================================================
// File:		ModelAssetBgMesh.h
// Created:		2015/02/19
// Last Edited:	2015/02/19
// Copyright:	Daniel Schenker
// Description:	ModelAsset for the Background Mesh Only
//=============================================================================

#ifndef MODELASSETBGMESH_H
#define MODELASSETBGMESH_H

//=============================================================================
//Includes
//=============================================================================

// Third-Party Libraries
//  GLEW
#include <GL/glew.h>

//  GLM
#include <glm/glm.hpp>

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

	class ModelAssetBgMesh
	{
	public:
		//Constructors
		ModelAssetBgMesh
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
			glm::vec3* pVerticesVec3,
			glm::vec3 bgMeshDistanceBetweenVertices
		);
		//Destructor
		~ModelAssetBgMesh();

		//Member Functions
	private:
		void CalculateVertices();
		void CalculateVerticesPerLine();
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
		glm::vec3* mpVerticesVec3;
		GLfloat* mpVertices;//TODO: change this to be a vector or something that knows the length of the array
		// VBO
		GLuint mVbo;
		// Elements
		const bool mkHasElements;
		GLuint mEbo;
		const unsigned int mkElementCountTotal;
		GLuint* mpElements;//TODO: make same change as with mpRectVertices
		std::vector<unsigned int> mVerticesPerHorizontalLineList;
		std::vector<unsigned int> mVerticesPerVerticalLineList;
		//unsigned int* mVerticesPerHorizontalLine;
		//unsigned int* mVerticesPerVerticalLine;
		// Draw Info
		GLenum mDrawType;
		GLint mDrawStart;

		//Data
		glm::vec3 mBgMeshDistanceBetweenVertices;
	};

}//namespace DSGraphics

#endif //#ifndef MODELASSETBGMESH_H