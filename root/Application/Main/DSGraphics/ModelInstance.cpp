//=============================================================================
// File:		ModelInstance.cpp
// Created:		2015/02/15
// Last Edited:	2015/02/17
// Copyright:	Daniel Schenker
// Description:	ModelInstance
//=============================================================================

//=============================================================================
//Includes
//=============================================================================

// Third-Party Libraries
//  GLM
#include <glm/gtc/type_ptr.hpp>

// Daniel Schenker
#include "ModelInstance.h"
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

DSGraphics::ModelInstance::ModelInstance(DSGraphics::ModelAsset* pAsset, DSGraphics::Camera* pCamera)
:	mpAsset(pAsset)
,	mTransform(1.0f)
,	mScale(1.0f)
,	mRotate(1.0f)
,	mRotationAxis(0.0f)
,	mTranslate(1.0f)
,	mUniformModel(0)
,	mpCamera(pCamera)
,	mUniformCamera(0)
{
	//TODO: Error checking stuff
	//if(mpCamera != nullptr)
	//{
		//mUniformCamera = glGetUniformLocation(mpAsset->mpProgram->GetProgramID(), "camera");
		//mUniformModel = glGetUniformLocation(mpAsset->mpProgram->GetProgramID(), "model");
	//}
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------

DSGraphics::ModelInstance::~ModelInstance()
{
}

//-----------------------------------------------------------------------------
// Public Member Functions
//-----------------------------------------------------------------------------
//  General

//-----------------------------------------------------------------------------

void DSGraphics::ModelInstance::UpdateTransform()
{
	mTransform = mTranslate * mRotate * mScale;
}

//-----------------------------------------------------------------------------

void DSGraphics::ModelInstance::Render()
{
	//Bind the shaders
	glUseProgram(mpAsset->GetProgramID());

	//Set the Shader Uniforms
	//Camera
	mUniformCamera = glGetUniformLocation(mpAsset->GetProgramID(), "camera");
	glUniformMatrix4fv(mUniformCamera, 1, GL_FALSE, glm::value_ptr(mpCamera->GetMatrix()));
	//Model
	mUniformModel = glGetUniformLocation(mpAsset->GetProgramID(), "model");
	glUniformMatrix4fv(mUniformModel, 1, GL_FALSE, glm::value_ptr(mTransform));
	//  Texture
	glUniform1i(glGetUniformLocation(mpAsset->GetProgramID(), "tex"), 0);

	//Bind
	// Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mpAsset->GetTextureObjectID());
	// VAO
	glBindVertexArray(mpAsset->GetVao());

	//Draw
	// Elements
	if(mpAsset->GetHasElements() == true)
	{
		glDrawElements(mpAsset->GetDrawType(), mpAsset->GetElementCount(), GL_UNSIGNED_INT, 0);
	}
	// Arrays
	else
	{
		glDrawArrays(mpAsset->GetDrawType(), 0, mpAsset->GetVertexCount());
	}

	//Unbind
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
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

glm::mat4 DSGraphics::ModelInstance::GetScale() const
{
	return mScale;
}

//-----------------------------------------------------------------------------

glm::mat4 DSGraphics::ModelInstance::GetRotate() const
{
	return mRotate;
}

//-----------------------------------------------------------------------------

glm::vec3 DSGraphics::ModelInstance::GetRotationAxis() const
{
	return mRotationAxis;
}

//-----------------------------------------------------------------------------

glm::mat4 DSGraphics::ModelInstance::GetTranslate() const
{
	return mTranslate;
}

//-----------------------------------------------------------------------------
// Setters
//-----------------------------------------------------------------------------

void DSGraphics::ModelInstance::SetScale(const glm::mat4& scale)
{
	mScale = scale;
}

//-----------------------------------------------------------------------------

void DSGraphics::ModelInstance::SetRotate(const glm::mat4& rotate)
{
	mRotate = rotate;
}

//-----------------------------------------------------------------------------

void DSGraphics::ModelInstance::SetRotationAxis(const glm::vec3& rotationAxis)
{
	mRotationAxis = rotationAxis;
}

//-----------------------------------------------------------------------------

void DSGraphics::ModelInstance::SetTranslate(const glm::mat4& translate)
{
	mTranslate = translate;
}