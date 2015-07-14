//=============================================================================
// File:		Camera.cpp
// Created:		2015/02/12
// Last Edited:	2015/02/17
// Copyright:	Daniel Schenker
// Description:	Camera
//=============================================================================

//=============================================================================
//Includes
//=============================================================================

// Third-Party Libraries
//  GLM
#include <glm/gtc/matrix_transform.hpp>

// Standard C++ Libraries
#define _USE_MATH_DEFINES
#include <cmath>

// Daniel Schenker
#include "Camera.h"

//=============================================================================
//Statics
//=============================================================================

//Must be less than 90 to avoid gimbal lock
static const float sMaxVerticalAngle = 85.0f;

//=============================================================================
//Class Definitions
//=============================================================================

//-----------------------------------------------------------------------------
// Constuctors
//-----------------------------------------------------------------------------

DSGraphics::Camera::Camera()
:	mPosition(0.0f, 0.0f, 1.0f)
,	mPitch(0.0f)
,	mYaw(0.0f)
,	mRoll(0.0f)
,	mFov(90.0f)
,	mNearPlane(0.01f)
,	mFarPlane(100.0f)
,	mViewportAspectRatio(16.0f / 9.0f)
{
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------

DSGraphics::Camera::~Camera()
{
}

//-----------------------------------------------------------------------------
// Public Member Functions
//-----------------------------------------------------------------------------

void DSGraphics::Camera::OffsetPosition(const glm::vec3& offset)
{
	mPosition += offset;
}

//-----------------------------------------------------------------------------

void DSGraphics::Camera::OffsetOrientation(float rightAngle, float upAngle)
{
	mYaw += rightAngle;
	mPitch += upAngle;
	NormalizeAngles();
}

//-----------------------------------------------------------------------------

void DSGraphics::Camera::LookAt(glm::vec3 position)
{
	assert(position != mPosition);

	glm::vec3 direction = glm::normalize(position - mPosition);
	mPitch = glm::radians(asinf(-direction.y));
	mYaw = -glm::radians(atan2f(-direction.x, -direction.z));
	NormalizeAngles();
}

//-----------------------------------------------------------------------------
// Private Member Functions
//-----------------------------------------------------------------------------

void DSGraphics::Camera::NormalizeAngles()
{
	mYaw = fmodf(mYaw, 360.0f);
	
	//fmodf can return negative values, but this will make them all positive
	while(mYaw < 0.0f)
	{
		mYaw += 360.0f;
	}

	if(mPitch > sMaxVerticalAngle)	
	{
		mPitch = sMaxVerticalAngle;
	}
	else if(mPitch < -sMaxVerticalAngle)
	{
		mPitch = -sMaxVerticalAngle;
	}
}

//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Getters
//-----------------------------------------------------------------------------

const glm::vec3& DSGraphics::Camera::GetPosition() const
{
	return mPosition;
}

//-----------------------------------------------------------------------------

float DSGraphics::Camera::GetFov() const
{
	return mFov;
}

//-----------------------------------------------------------------------------

float DSGraphics::Camera::GetNearPlane() const
{
	return mNearPlane;
}

//-----------------------------------------------------------------------------

float DSGraphics::Camera::GetFarPlane() const
{
	return mFarPlane;
}

//-----------------------------------------------------------------------------

float DSGraphics::Camera::GetViewportAspectRatio() const
{
	return mViewportAspectRatio;
}

//-----------------------------------------------------------------------------

glm::mat4 DSGraphics::Camera::GetOrientation() const
{
	glm::mat4 orientation(1.0f);

	orientation = glm::rotate(orientation, glm::radians(mPitch), glm::vec3(1, 0, 0));
	orientation = glm::rotate(orientation, glm::radians(mYaw), glm::vec3(0, 1, 0));

	return orientation;
}

//-----------------------------------------------------------------------------

glm::vec3 DSGraphics::Camera::GetDirectionForward() const
{
	glm::vec4 forward = glm::inverse(GetOrientation()) * glm::vec4(0, 0, -1, 1);

	return glm::vec3(forward);
}

//-----------------------------------------------------------------------------

glm::vec3 DSGraphics::Camera::GetDirectionRight() const
{
	glm::vec4 right = glm::inverse(GetOrientation()) * glm::vec4(1, 0, 0, 1);

	return glm::vec3(right);
}

//-----------------------------------------------------------------------------

glm::vec3 DSGraphics::Camera::GetDirectionUp() const
{
	glm::vec4 up = glm::inverse(GetOrientation()) * glm::vec4(0, 1, 0, 1);

	return glm::vec3(up);
}

//-----------------------------------------------------------------------------

glm::mat4 DSGraphics::Camera::GetMatrix() const
{
	return GetProj() * GetView();
}

//-----------------------------------------------------------------------------

glm::mat4 DSGraphics::Camera::GetView() const
{
	/*
	glm::mat4 test(1.0f);
	test = glm::lookAt
	(
		glm::vec3(1.0f, 1.5f, 2.0f),	//camera position
		glm::vec3(0.0f, 0.0f, 0.0f),	//focal point
		glm::vec3(0.0f, 1.0f, 0.0f)		//up axis
	);
	return test;
	*/
	return GetOrientation() * glm::translate(glm::mat4(), -mPosition);
}

//-----------------------------------------------------------------------------

glm::mat4 DSGraphics::Camera::GetProj() const
{
	return
		glm::perspective
		(
			glm::radians(mFov),		//Vertical Field of View (FoV) in degrees
			mViewportAspectRatio,	//aspect ratio
			mNearPlane,				//near plane
			mFarPlane				//far plane
		);
}

//-----------------------------------------------------------------------------
// Setters
//-----------------------------------------------------------------------------

void DSGraphics::Camera::SetPosition(const glm::vec3& position)
{
	mPosition = position;
}

//-----------------------------------------------------------------------------

void DSGraphics::Camera::SetFoV(float fov)
{
	assert(fov > 0.0f && fov < 180.0f);

	mFov = fov;
}

//-----------------------------------------------------------------------------

void DSGraphics::Camera::SetNearPlane(float nearPlane)
{
	assert((nearPlane > 0.0f) && (nearPlane < mFarPlane));

	mNearPlane = nearPlane;
}

//-----------------------------------------------------------------------------

void DSGraphics::Camera::SetFarPlane(float farPlane)
{
	assert(farPlane > mNearPlane);

	mFarPlane = farPlane;
}

//-----------------------------------------------------------------------------

void DSGraphics::Camera::SetPlanes(float nearPlane, float farPlane)
{
	assert(nearPlane > 0.0f);
	assert(farPlane > nearPlane);

	mNearPlane = nearPlane;
	mFarPlane = farPlane;
}

//-----------------------------------------------------------------------------

void DSGraphics::Camera::SetViewportAspectRatio(float viewportAspectRatio)
{
	assert(viewportAspectRatio > 0.0f);

	mViewportAspectRatio = viewportAspectRatio;
}