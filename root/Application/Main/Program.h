//=============================================================================
// File:		Program.h
// Created:		2015/02/11
// Last Edited:	2015/02/12
// Copyright:	Daniel Schenker
// Description:	Program
//=============================================================================

#ifndef PROGRAM_H
#define PROGRAM_H

//=============================================================================
//Includes
//=============================================================================

// Standard C++ Libraries
#include <vector>

// Daniel Schenker
#include "Shader.h"

//=============================================================================
//Forward Declarations
//=============================================================================

//=============================================================================
//Enums
//=============================================================================

//=============================================================================
//Class Declarations
//=============================================================================

class Program
{
public:
	//Constructors
	Program(const std::vector<Shader>& shaders);
	//Destructor
	~Program();

	//Member Functions

private:
	//Disable Copy Constructor
	Program(const Program&);
	const Program& operator=(const Program&);

public:
	// Getters
	GLuint GetProgramID() const;
	GLint GetAttrib(const GLchar* pAttribName) const;
	GLint GetUniform(const GLchar* pUniformName) const;
	// Setters

	//Member Variables
private:
	GLuint mProgramID;
};

#endif //#ifndef PROGRAM_H