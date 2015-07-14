//=============================================================================
// File:		Shader.h
// Created:		2015/02/10
// Last Edited:	2015/02/11
// Copyright:	Daniel Schenker
// Description:	Shader
//=============================================================================

#ifndef SHADER_H
#define SHADER_H

//=============================================================================
//Includes
//=============================================================================

// Third-Party Libraries
//  GLEW
#include <GL/glew.h>

// Standard C++ Libraries
#include <string>

//=============================================================================
//Forward Declarations
//=============================================================================

//=============================================================================
//Enums
//=============================================================================

//=============================================================================
//Class Declarations
//=============================================================================

class Shader
{
public:
	//Constructors
	Shader(const std::string& shaderCode, GLenum shaderType);
	//Destructor
	~Shader();
	//Note: Shader objects can be copied and assigned because they are reference counted like a shared pointer
	//Copy Constructor
	Shader(const Shader& other);
	//Copy Assignment Operator
	Shader& operator =(const Shader& other);

	//Member Functions
public:
	// General
	static Shader CreateShaderFromFile(const std::string& filePath, GLenum shaderType);

private:
	// General
	void Retain();
	void Release();
	//void ReadShader(const char* pShaderFile, std::string& shaderData);

public:
	// Getters
	GLuint GetShaderID() const;
	// Setters

	//Member Variables
private:
	GLuint mShaderID;
	unsigned* mpRefCount;
};

#endif //#ifndef SHADER_H