//=============================================================================
// File:		Program.cpp
// Created:		2015/02/11
// Last Edited:	2015/02/12
// Copyright:	Daniel Schenker
// Description:	Program
//=============================================================================

//=============================================================================
//Includes
//=============================================================================

// Daniel Schenker
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

Program::Program(const std::vector<Shader>& shaders)
:	mProgramID(0)
{
	if(shaders.size() <= 0)
	{
		throw std::runtime_error("ERROR: No shaders were provided to create the program.");
	}

	//Create the program object
	mProgramID = glCreateProgram();
	
	if(mProgramID == 0)
	{
		throw std::runtime_error("ERROR: glCreateProgram failed.");
	}

	//attach all the shaders
	for(unsigned i = 0; i < shaders.size(); ++i)
	{
		glAttachShader(mProgramID, shaders[i].GetShaderID());
	}

	//link the shaders together
	glLinkProgram(mProgramID);

	//detach all the shaders
	for(unsigned i = 0; i < shaders.size(); ++i)
	{
		glDetachShader(mProgramID, shaders[i].GetShaderID());
	}

	//throw exception if linking failed
	GLint status;
	glGetProgramiv(mProgramID, GL_LINK_STATUS, &status);
	if(status == GL_FALSE)
	{
		std::string msg("Program linking failure: ");

		GLint infoLogLength;
		glGetProgramiv(mProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* strInfoLog = new char[infoLogLength + 1];
		glGetProgramInfoLog(mProgramID, infoLogLength, NULL, strInfoLog);
		msg += strInfoLog;
		delete[] strInfoLog;

		glDeleteProgram(mProgramID);
		mProgramID = 0;
		throw std::runtime_error(msg);
	}
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------

Program::~Program()
{
	//might be 0 if ctor fails by throwing exception
	if(mProgramID != 0)
	{
		glDeleteProgram(mProgramID);
		mProgramID = 0;
	}
}

//-----------------------------------------------------------------------------
// Public Member Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Private Member Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Getters
//-----------------------------------------------------------------------------

GLuint Program::GetProgramID() const
{
	return mProgramID;
}

//-----------------------------------------------------------------------------

GLint Program::GetAttrib(const GLchar* pAttribName) const
{
	if(pAttribName != nullptr)
	{
		GLint attrib = glGetAttribLocation(mProgramID, pAttribName);
		if(attrib == -1)
		{
			throw std::runtime_error(std::string("ERROR: Program attribute not found: ") + pAttribName);
		}

		return attrib;
	}
	else
	{
		throw std::runtime_error("ERROR: pAttribName was NULL.");
	}
}

//-----------------------------------------------------------------------------

GLint Program::GetUniform(const GLchar* pUniformName) const
{
	if(pUniformName != nullptr)
	{
		GLint uniform = glGetUniformLocation(mProgramID, pUniformName);
		if(uniform == -1)
		{
			throw std::runtime_error(std::string("ERROR: Program uniform not found: ") + pUniformName);
		}

		return uniform;
	}
	else
	{
		throw std::runtime_error("ERROR: pUniformName was NULL.");
	}
}

//-----------------------------------------------------------------------------
// Setters
//-----------------------------------------------------------------------------