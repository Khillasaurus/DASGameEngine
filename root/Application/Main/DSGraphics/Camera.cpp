//=============================================================================
// File:		Camera.cpp
// Created:		2015/02/12
// Last Edited:	2015/04/10
// Copyright:	Daniel Schenker
// Description:	Camera
//=============================================================================

//=============================================================================
//Includes
//=============================================================================

// Third-Party Libraries
//  GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>//remember to remove this once all dependency on this class is removed

// Standard C++ Libraries
#define _USE_MATH_DEFINES
#include <cmath>

//debugging
#include <stdio.h>

// Daniel Schenker
#include "Camera.h"

#define TOLERANCE 0.00001f

//=============================================================================
//Statics
//=============================================================================

//=============================================================================
//Class Definitions
//=============================================================================

//-----------------------------------------------------------------------------
// Constuctors
//-----------------------------------------------------------------------------

DSGraphics::Camera::Camera
(
	glm::vec3 position,
	const DSMathematics::Quaternion& orientation,
	float fov,
	float nearPlane,
	float farPlane,
	float viewportAspectRatio
)
:	mPosition(position)
,	mOrientation(orientation)
,	mFov(fov)
,	mNearPlane(nearPlane)
,	mFarPlane(farPlane)
,	mViewportAspectRatio(viewportAspectRatio)
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
//  Usage

/*
Description:
	Teleports the camera to the specified position. Interpolation implementation is left to the user of this function.
	Can set the position both locally (relative to the current position) as well as globally (using world coordinates).
*/
void DSGraphics::Camera::SetPosition(glm::vec3 pos, bool moveLocally)
{
	//Local Coordinates
	if(moveLocally == true)
	{
		mPosition += pos;
	}
	//World Coordinates
	else
	{
		mPosition = pos;
	}
}

//-----------------------------------------------------------------------------

/*
Description:
	Moves the camera based on the orientation of the camera.
	i.e. Moves the camera by the specified vector rotated to match the local axis.

	Always moves with relation to local space.
	If this transformation is desired using world coordinates:
		1. Save current position.
		2. Calculate new position.
		3. Subtract the two to get the vector that represents the change in position, and then use SetPosition.
*/
void DSGraphics::Camera::Move(glm::vec3 pos)
{
	glm::vec3 relativeDirection = GetOrientation().Rotate(pos);
	mPosition += relativeDirection;
}

//-----------------------------------------------------------------------------

//Set Orientation doesn't care if it's over time or immediately.//absolute vs relative
/*
Description:
	Snaps the camera to the specified orientation. Interpolation implementation is left to the user of this function.
	Can set the orientation both relative to the current orientation, as well as absolutely to the specified orientation.
*/
void DSGraphics::Camera::SetOrientation(DSMathematics::Quaternion orientation, bool relative)
{
	//Relative
	if(relative == true)
	{
		mOrientation = mOrientation.Multiply(orientation);
	}
	//Absolute
	else
	{
		mOrientation = orientation;
	}
}

//-----------------------------------------------------------------------------
// Getters
//-----------------------------------------------------------------------------
//  Camera Rigid Body Displacement

const glm::vec3& DSGraphics::Camera::GetPosition() const
{
	return mPosition;
}

//-----------------------------------------------------------------------------

const DSMathematics::Quaternion& DSGraphics::Camera::GetOrientation() const
{
	return mOrientation;
}

//-----------------------------------------------------------------------------
//  Direction
/*
Notes:
	In order to find a direction, simply rotate the desired direction by the orientation of the camera.
Example:
		Camera has been rotated 90 degrees around the y-axis and is now facing left instead of forward.
		The x-axis is now pointing in the negative z direction.
		When polling for "direction right", it should return (0, 0, -1) instead of (1, 0, 0).
		mOrientation.Rotate(desired direction) = result.
*/

glm::vec3 DSGraphics::Camera::GetDirectionRight() const
{
	return mOrientation.Rotate(glm::vec3(1.0f, 0.0f, 0.0f));
}

//-----------------------------------------------------------------------------

glm::vec3 DSGraphics::Camera::GetDirectionUp() const
{
	return mOrientation.Rotate(glm::vec3(0.0f, 1.0f, 0.0f));
}

//-----------------------------------------------------------------------------

glm::vec3 DSGraphics::Camera::GetDirectionForward() const
{
	return mOrientation.Rotate(glm::vec3(0.0f, 0.0f, -1.0f));
}

//-----------------------------------------------------------------------------

glm::vec3 DSGraphics::Camera::GetDirectionBackward() const
{
	return mOrientation.Rotate(glm::vec3(0.0f, 0.0f, 1.0f));
}

//-----------------------------------------------------------------------------
//  Model View Matrices

glm::mat4 DSGraphics::Camera::GetView() const
{
	/*
	The purpose of the view transform is to place the camera at the origin and
	aim it in the directinon of the negative Z-axis, with the Y-axis pointing
	upwards and the X-axis pointing to the right.
	(Right-handed coordinate system)

	In order to relocate the camera to the origin, we need to "undo" its
	previous transformations in the following order:
	1. Rotation around the origin.
	2. Translation to its position (viewer position).

	This is done by applying the matrix M^-1 = (T.R)^-1
	These operations result in the following matrix:

																-1
	[	rightx		up'x		viewDirx		viewPosx	]
	[	righty		up'y		viewDiry		viewPosy	]
	[	rightz		up'z		viewDirz		viewPosz	]
	[	0			0			0				1			]

	viewPos	= view position
	up		= the up camera vector. If this is non-perpendicular to the view
			  direction, it needs to be recomputed.
	viewDir	= the camera view direction
	right	= right vector

	The enumarated vectors are calculated as follows:
	viewDir	= viewPos - lookAtPos
	right	= Normalize(up X viewDir)
	up'		= Normalize(viewDir X right)

	where:
	lookAtPos	= position at which the camera is looking

	Since the above rotation matrix is an orthonormal matrix, the inverse of
	the matrix is equal to its transpose.
	Translation matrix simply gets inversed signs in the 4th column.
	Therefore the view matrix is = (R^t.T^-1)
	It looks like the following:

	[	rightx		righty		rightz			-dot(viewPos, right)	]
	[	upx			upy			upz				-dot(viewPos, up)		]
	[	viewDirx	viewDiry	viewDirz		-dot(viewPos, viewDir)	]
	[	0			0			0				1						]
	*/
	/*
	glm::vec3 viewDir	= mPosition - GetDirectionForward();
	glm::vec3 right		= glm::normalize(glm::cross(GetDirectionUp(), viewDir));
	glm::vec3 up		= glm::normalize(glm::cross(viewDir, GetDirectionRight()));

	glm::mat4 viewMatrix = 
	{
		right.x,	right.y,	right.z,	-glm::dot(mPosition, right),
		up.x,		up.y,		up.z,		-glm::dot(mPosition, up),
		viewDir.x,	viewDir.y,	viewDir.z,	-glm::dot(mPosition, viewDir),
		0,			0,			0,			1
	};
	*/

	/*
	glm::vec3 viewDir	= glm::normalize(GetDirectionForward() - mPosition);
	glm::vec3 right		= glm::normalize(glm::cross(viewDir, GetDirectionUp()));
	glm::vec3 up		= glm::normalize(glm::cross(GetDirectionRight(), viewDir));

	glm::mat4 viewMatrix = 
	{
		right.x,	right.y,	right.z,	-glm::dot(right, mPosition),
		up.x,		up.y,		up.z,		-glm::dot(up, mPosition),
		-viewDir.x,	-viewDir.y,	-viewDir.z,	glm::dot(viewDir, mPosition),
		0,			0,			0,			1
	};

	return viewMatrix;
	*/

	/*
	glm::mat4 orientation = QuaternionToMatrix();

	return glm::lookAt
	(
		mPosition,
		GetDirectionForward(),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	*/

	//return glm::translate(glm::mat4(), -mPosition) * glm::mat4_cast(mOrientation);

	//I believe it should be done in this order (reverse order), because we are undoing the effects (namely rotation followed by translation) applied to the camera.
	//Since they were applied rotation first, translation second, we must undo the operations in the correct order (namely translation first, rotation second).
	return glm::mat4_cast(glm::quat(mOrientation.Invert().mW, mOrientation.Invert().mV.x, mOrientation.Invert().mV.y, mOrientation.Invert().mV.z)) * glm::translate(glm::mat4(), -mPosition);
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

glm::mat4 DSGraphics::Camera::GetMatrix() const
{
	return GetProj() * GetView();
}

//-----------------------------------------------------------------------------
//  Camera Settings

float DSGraphics::Camera::GetFoV() const
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
// Setters
//-----------------------------------------------------------------------------
//  Camera Settings

void DSGraphics::Camera::SetFoV(float fov)
{
	mFov = fov;
}

//-----------------------------------------------------------------------------

void DSGraphics::Camera::SetNearPlane(float nearPlane)
{
	mNearPlane = nearPlane;
}

//-----------------------------------------------------------------------------

void DSGraphics::Camera::SetFarPlane(float farPlane)
{
	mFarPlane = farPlane;
}

//-----------------------------------------------------------------------------

void DSGraphics::Camera::SetPlanes(float nearPlane, float farPlane)
{
	mNearPlane = nearPlane;
	mFarPlane = farPlane;
}

//-----------------------------------------------------------------------------

void DSGraphics::Camera::SetViewportAspectRatio(float viewportAspectRatio)
{
	mViewportAspectRatio = viewportAspectRatio;
}

//-----------------------------------------------------------------------------
// Private Member Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------

glm::mat4 DSGraphics::Camera::QuaternionToMatrix() const
{
	/*
	float x2 = mOrientation.x * mOrientation.x;
	float y2 = mOrientation.y * mOrientation.y;
	float z2 = mOrientation.z * mOrientation.z;
	float xy = mOrientation.x * mOrientation.y;
	float xz = mOrientation.x * mOrientation.z;
	float yz = mOrientation.y * mOrientation.z;
	float wx = mOrientation.w * mOrientation.x;
	float wy = mOrientation.w * mOrientation.y;
	float wz = mOrientation.w * mOrientation.z;
 
	//Note: This calculation would be a lot more complicated for non-unit length quaternions
	
	return glm::mat4
			(
				1.0f - 2.0f * (y2 + z2),	2.0f * (xy - wz),			2.0f * (xz + wy),			0.0f,
				2.0f * (xy + wz),			1.0f - 2.0f * (x2 + z2),	2.0f * (yz - wx),			0.0f,
				2.0f * (xz - wy),			2.0f * (yz + wx),			1.0f - 2.0f * (x2 + y2),	0.0f,
				0.0f,						0.0f,						0.0f,						1.0f
			);
	*/

	//return glm::mat4_cast(mOrientation);

	return glm::mat4_cast(glm::quat(mOrientation.mW, mOrientation.mV.x, mOrientation.mV.y, mOrientation.mV.z));
}