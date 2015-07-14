//=============================================================================
// File:		Shader.cpp
// Created:		2015/02/10
// Last Edited:	2015/02/17
// Copyright:	Daniel Schenker
// Description:	Shader
//=============================================================================

//=============================================================================
//Includes
//=============================================================================

// Standard C++ Libraries
#include <cassert>
#include <fstream>
#include <sstream>

// Daniel Schenker
#include "Shader.h"

//=============================================================================
//Statics
//=============================================================================

//=============================================================================
//Class Definitions
//=============================================================================

//-----------------------------------------------------------------------------
// Constuctors
//-----------------------------------------------------------------------------

DSGraphics::Shader::Shader(const std::string& shaderCode, GLenum shaderType)
:	mShaderID(0)
,	mpRefCount(nullptr)
{
	//create the shader object
	mShaderID = glCreateShader(shaderType);
	if(mShaderID == 0)
	{
		throw std::runtime_error("ERROR: glCreateShader failed.");
	}

	//set the source code
	const char* code = shaderCode.c_str();
	glShaderSource(mShaderID, 1, (const GLchar**)&code, NULL);

	//compile
	glCompileShader(mShaderID);

	//throw exception if compile error occurred
	GLint status;
	glGetShaderiv(mShaderID, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE)
	{
		std::string msg("Compile failure in shader:\n");

		GLint infoLogLength;
		glGetShaderiv(mShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* strInfoLog = new char[infoLogLength + 1];
		glGetShaderInfoLog(mShaderID, infoLogLength, NULL, strInfoLog);
		msg += strInfoLog;
		delete[] strInfoLog;
		strInfoLog = nullptr;

		glDeleteShader(mShaderID);
		mShaderID = 0;
		throw std::runtime_error(msg);
	}

	mpRefCount = new unsigned;
	*mpRefCount = 1;
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------

DSGraphics::Shader::~Shader()
{
	//_refCount will be nullptr if constructor failed and threw an exception
	if(mpRefCount != nullptr)
	{
		Release();
	}
}

//-----------------------------------------------------------------------------
// Copy Constructor
//-----------------------------------------------------------------------------

DSGraphics::Shader::Shader(const Shader& other)
:	mShaderID(other.mShaderID)
,	mpRefCount(other.mpRefCount)
{
	Retain();
}

//-----------------------------------------------------------------------------
// Copy Assignment Operator
//-----------------------------------------------------------------------------

DSGraphics::Shader& DSGraphics::Shader::operator = (const Shader& other)
{
	Release();
	mShaderID = other.mShaderID;
	mpRefCount = other.mpRefCount;
	Retain();

	return *this;
}

//-----------------------------------------------------------------------------
// Public Member Functions
//-----------------------------------------------------------------------------
//  General

DSGraphics::Shader DSGraphics::Shader::CreateShaderFromFile(const std::string& filePath, GLenum shaderType)
{
	//open file
	std::ifstream f;
	f.open(filePath.c_str(), std::ios::in | std::ios::binary);
	
	if(!f.is_open())
	{
		throw std::runtime_error(std::string("ERROR: Failed to open file: ") + filePath);
	}

	//read whole file into stringstream buffer
	std::stringstream buffer;
	buffer << f.rdbuf();

	//return new shader
	Shader shader(buffer.str(), shaderType);

	return shader;
}

//-----------------------------------------------------------------------------
// Private Member Functions
//-----------------------------------------------------------------------------
//  General

void DSGraphics::Shader::Retain()
{
	assert(mpRefCount);
	*mpRefCount += 1;
}

//-----------------------------------------------------------------------------

void DSGraphics::Shader::Release()
{
	assert(mpRefCount && *mpRefCount > 0);
	*mpRefCount -= 1;

	if(*mpRefCount == 0)
	{
		glDeleteShader(mShaderID);
		mShaderID = 0;

		delete mpRefCount;
		mpRefCount= nullptr;
	}
}

//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Getters
//-----------------------------------------------------------------------------

GLuint DSGraphics::Shader::GetShaderID() const
{
	return mShaderID;
}

//-----------------------------------------------------------------------------
// Setters
//-----------------------------------------------------------------------------