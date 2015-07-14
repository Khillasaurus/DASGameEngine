//=============================================================================
// File:		Texture.h
// Created:		2015/02/12
// Last Edited:	2015/02/17
// Copyright:	Daniel Schenker
// Description:	Texture
//=============================================================================

#ifndef TEXTURE_H
#define TEXTURE_H

//=============================================================================
//Includes
//=============================================================================

// Third-Party Libraries
//  GLEW
#include <GL/glew.h>

//  zlib
#include <zlib.h>

//  libpng
#include <png.h>

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

	class Texture
	{
	public:
		//Constructors
		Texture(const char* pImageFile, GLint minMagFiler = GL_LINEAR, GLint wrapMode = GL_CLAMP_TO_EDGE);
		//Destructor
		~Texture();

		//Member Functions

	private:
		//Disable Copy Constructor
		Texture(const Texture&);
		const Texture& operator=(const Texture&);

	public:
		// Getters
		bool GetIsTextureLoaded() const;
		GLuint GetObjectID() const;
		png_uint_32 GetWidth() const;
		png_uint_32 GetHeight() const;
		// Setters

		//Member Variables
	private:
		bool mIsTextureLoaded;
		GLuint mObjectID;
		png_uint_32 mWidth;
		png_uint_32 mHeight;
	};

}//namespace DSGraphics

#endif //#ifndef TEXTURE_H