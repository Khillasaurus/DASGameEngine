//=============================================================================
// File:		ModelInstance.cpp
// Created:		2015/02/15
// Last Edited:	2015/02/23
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

//#define BUFFER_OFFSET(i) ((unsigned int*)NULL + (i))

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
,	mSize(1.0f)
,	mOrientationAngle(0.0f)
,	mOrientationAxis(0.0f, 1.0f, 0.0f)
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
	if(mpCamera != nullptr)
	{
		//Camera
		mUniformCamera = glGetUniformLocation(mpAsset->GetProgramID(), "camera");
		glUniformMatrix4fv(mUniformCamera, 1, GL_FALSE, glm::value_ptr(mpCamera->GetMatrix()));
		//Model
		mUniformModel = glGetUniformLocation(mpAsset->GetProgramID(), "model");
		glUniformMatrix4fv(mUniformModel, 1, GL_FALSE, glm::value_ptr(mTransform));
	}
	//  Texture
	if(mpAsset->GetHasTexture() == true)
	{
		glUniform1i(glGetUniformLocation(mpAsset->GetProgramID(), "tex"), 0);

	//Bind
		// Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mpAsset->GetTextureObjectID());
		//TODO:	The above line is where I specify which texture to use when drawing.
		//		Currently mpAsset only holds one texture, so the GetTexture function chooses the only one.
		//		Instead the GetTexture function should ask the instance which texture to use based on the state of the instance.
		//		This means that the mpAsset holds a list of textures for every possible state of the instance, and can choose whichever one is necessary based on the passed in texture number.
		//		This also means that animation is done by swapping out which texture is used. This means that all textures must use the same texture coordinates, which causes problems in two ways:
		//			1.	Texture sheets with multiple textures crammed into one sheet for efficiency (eg. numbers 0-9 all squeezed into one image for scoreboard usage) can not be used
		//				since that requires being able to specify texture coordinates on the fly.
		//			2.	If the instance's shape changes (eg. a crumpled car fender), the texture map may no longer matches the shape of the object, and display incorrectly stretched or squeezed textures.
		//		There are two losses that occur by not remedying this:
		//			1.	Efficiency. Texture swapping is usually less efficient than specifying new texture coordinates.
		//			2.	Changing a model's shape requires changing texture coordinates as well, so this cannot be done.
		//		Thankfully, loss #2 is the only loss that mandates fixing this problem, and loss #2 only occurs in soft body physics systems, which aren't implemented at all yet.
		//		In order to convert the current system into a soft body complient system:
		//		Make the assets "masters" that represent initial state of an object, and make the instances not instances, but rather individual assets themselves.
		//		This way the individual assets can each have their own vertex coordinates, allowing for crumpling (soft body physics), as well as their own texture and colour coordinates to go along with the vertex coordinates, for on the fly updating.
		//		This is because every "instance" in a soft body physics system is its own asset, since no two instances are alike, compared to rigid body physics where two objects (eg. a coffee mug) are identical, with the exception of SRT (scale, rotation and translation), because their model and texture/colour are the same.
	}
	// VAO
	glBindVertexArray(mpAsset->GetVao());

	//Draw
	// Elements
	if(mpAsset->GetHasElements() == true)
	{
		unsigned int elementCountPerDrawType = mpAsset->GetElementCountPerDrawType();
		unsigned int elementCountTotal = mpAsset->GetElementCountTotal();

		if(elementCountPerDrawType == 0)
		{
			glDrawElements(mpAsset->GetDrawType(), mpAsset->GetElementCountTotal(), GL_UNSIGNED_INT, 0);
		}
		else
		{
			for(unsigned int drawn = 0; drawn < elementCountTotal; drawn += elementCountPerDrawType)
			{
				//glDrawElements(mpAsset->GetDrawType(), elementCountPerDrawType, GL_UNSIGNED_INT, BUFFER_OFFSET(drawn));
				glDrawElements(mpAsset->GetDrawType(), elementCountPerDrawType, GL_UNSIGNED_INT, reinterpret_cast<void*>(drawn * sizeof(GLuint)));
				//glDrawElements(mpAsset->GetDrawType(), elementCountPerDrawType, GL_UNSIGNED_INT, drawn * sizeof(GLuint));
			}
		}
	}
	// Arrays
	else
	{
		glDrawArrays(mpAsset->GetDrawType(), 0, mpAsset->GetVertexCount());
	}

	//Unbind
	glBindVertexArray(0);
	if(mpAsset->GetHasTexture() == true)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glUseProgram(0);
}

//-----------------------------------------------------------------------------
//  Locomotion

void DSGraphics::ModelInstance::Move(glm::vec3 displacement, float deviceCoordinatesPerMeter)
{
	mPosition += displacement * deviceCoordinatesPerMeter;
	mPositionUpdated = true;
}

//-----------------------------------------------------------------------------

void DSGraphics::ModelInstance::Spin(float radians)
{
	mRotate = glm::rotate(mRotate, radians, mOrientationAxis);
	
}

//-----------------------------------------------------------------------------
// Private Member Functions
//-----------------------------------------------------------------------------
//  UpdateTransform Sub-Functions

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
