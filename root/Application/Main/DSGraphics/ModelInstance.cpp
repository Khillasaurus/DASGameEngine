//=============================================================================
// File:		ModelInstance.cpp
// Created:		2015/02/15
// Last Edited:	2015/02/18
// Copyright:	Daniel Schenker
// Description:	ModelInstance
//=============================================================================

//=============================================================================
//Includes
//=============================================================================

// Third-Party Libraries
//  GLM
#include <glm/gtc/matrix_transform.hpp>
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
//Externally Accessible (not encapsulated)
:	mpAsset(pAsset)
,	mSize(0.0f)
,	mOrientationAngle(0.0f)
,	mOrientationAxis(0.0f)
//,	mOrientation(0.0f)
,	mPosition(0.0f)
,	mpCamera(pCamera)
//Externally Inaccessible (encapsulated)
,	mTransform(1.0f)
,	mScale(1.0f)
,	mSizeUpdated(false)
,	mRotate(1.0f)
//,	mRotationAxis(0.0f)
,	mOrientationUpdated(false)
,	mTranslate(1.0f)
,	mPositionUpdated(false)
,	mUniformModel(0)
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
	UpdateScale();
	UpdateRotate();
	UpdateTranslate();

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
	if(mpAsset->GetHasTexture() == true)
	{
		glUniform1i(glGetUniformLocation(mpAsset->GetProgramID(), "tex"), 0);

	//Bind
		// Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mpAsset->GetTextureObjectID());
	}
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

void DSGraphics::ModelInstance::UpdateScale()
{
	if(mSizeUpdated == true)
	{
		mScale = glm::scale(glm::mat4(1.0f), mSize);

		mSizeUpdated = false;
	}
}

//-----------------------------------------------------------------------------

void DSGraphics::ModelInstance::UpdateRotate()
{
	if(mOrientationUpdated == true)
	{
		//TODO: TOFIX: Can't remember if any orientation can be represented by the sum or multiplication of rotating around each axis... ??? or something totally different

		//Rotate around x
		//mRotate = glm::rotate(glm::mat4(1.0f), glm::radians(mOrientation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		//Rotate around y
		//Rotate around z

		mRotate = glm::rotate(glm::mat4(1.0f), mOrientationAngle, mOrientationAxis);

		mOrientationUpdated = false;
	}
}

//-----------------------------------------------------------------------------

void DSGraphics::ModelInstance::UpdateTranslate()
{
	if(mPositionUpdated == true)
	{
		mTranslate = glm::translate(glm::mat4(1.0f), mPosition);

		mPositionUpdated = false;
	}
}

//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Getters
//-----------------------------------------------------------------------------

glm::vec3 DSGraphics::ModelInstance::GetSize() const
{
	return mSize;
}

//-----------------------------------------------------------------------------

/*
glm::vec3 DSGraphics::ModelInstance::GetOrientation() const
{
	return mOrientation;
}
*/

//-----------------------------------------------------------------------------

GLfloat DSGraphics::ModelInstance::GetOrientationAngleInRadians() const
{
	return mOrientationAngle;
}

//-----------------------------------------------------------------------------

glm::vec3 DSGraphics::ModelInstance::GetOrientationAxis() const
{
	return mOrientationAxis;
}

//-----------------------------------------------------------------------------

glm::vec3 DSGraphics::ModelInstance::GetPosition() const
{
	return mPosition;
}

//-----------------------------------------------------------------------------
// Setters
//-----------------------------------------------------------------------------

void DSGraphics::ModelInstance::SetSize(const glm::vec3& size)
{
	mSize = size;
	mSizeUpdated = true;
}

//-----------------------------------------------------------------------------

void DSGraphics::ModelInstance::SetOrientationAngle(GLfloat radians)
{
	mOrientationAngle = radians;
	mOrientationUpdated = true;
}

//-----------------------------------------------------------------------------

void DSGraphics::ModelInstance::SetOrientationAxis(const glm::vec3& rotationAxis)
{
	mOrientationAxis = rotationAxis;
	mOrientationUpdated = true;
}

//-----------------------------------------------------------------------------

/*
void DSGraphics::ModelInstance::SetOrientation(const glm::vec3& orientation)
{
	mOrientation = orientation;
	mOrientationUpdated = true;
}
*/

//-----------------------------------------------------------------------------

void DSGraphics::ModelInstance::SetPosition(const glm::vec3& position)
{
	mPosition = position;
	mPositionUpdated = true;
}
