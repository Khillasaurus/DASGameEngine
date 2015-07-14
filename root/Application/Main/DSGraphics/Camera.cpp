//=============================================================================
// File:		Camera.cpp
// Created:		2015/02/12
// Last Edited:	2015/02/25
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
	const DSMathematics::Quaternion& orientation,///
	///glm::quat orientation,
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
// General

void DSGraphics::Camera::MoveWorld(const glm::vec3& offset)
{
	mPosition += offset;
}

//-----------------------------------------------------------------------------

//TOFIX
void DSGraphics::Camera::MoveLocal(const glm::vec3& offset)
{
	glm::vec3 right = glm::normalize(GetDirectionRight());
	glm::vec3 up = glm::normalize(GetDirectionUp());
	glm::vec3 backward = -glm::normalize(GetDirectionForward());

	right.x *= offset.x;
	right.y *= offset.x;
	right.z *= offset.x;
	up.x *= offset.y;
	up.y *= offset.y;
	up.z *= offset.y;
	backward.x *= offset.z;
	backward.y *= offset.z;
	backward.z *= offset.z;
	mPosition += right;
	mPosition += up;
	mPosition += backward;
}

//-----------------------------------------------------------------------------

void DSGraphics::Camera::Rotate(const DSMathematics::Quaternion& rotation)
{
	mOrientation.Multiply(rotation);
}

//-----------------------------------------------------------------------------

/*
void DSGraphics::Camera::Rotate(float angle, const glm::vec3& axis)
{
	if(axis.length() != 1.0f)
	{
		glm::normalize(axis);
	}
	mOrientation = glm::rotate(mOrientation, angle, axis);
}
*/

//-----------------------------------------------------------------------------

/*
void DSGraphics::Camera::LookAt(glm::vec3 position)
{
	//TODO
}
*/

//-----------------------------------------------------------------------------
// Private Member Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------

/*
void DSGraphics::Camera::NormalizeQuaternion()
{
	//Don't normalize if we don't have to
	float mag2 =
				(mOrientation.w * mOrientation.w) +
				(mOrientation.x * mOrientation.x) +
				(mOrientation.y * mOrientation.y) +
				(mOrientation.z * mOrientation.z);

	if(fabsf(mag2) > TOLERANCE && fabsf(mag2 - 1.0f) > TOLERANCE)
	{
		float mag = sqrt(mag2);
		mOrientation.w /= mag;
		mOrientation.x /= mag;
		mOrientation.y /= mag;
		mOrientation.z /= mag;
	}
}
*/

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

//-----------------------------------------------------------------------------

// Multiplying a quaternion q with a vector v applies the q-rotation to v
/*
glm::vec3 DSGraphics::Camera::ApplyQuatToVec(const glm::quat& q, const glm::vec3& v) const
{
	//TODO

	glm::vec3 vn(v);
	vn = glm::normalize(vn);
 
	glm::quat vecQuat, resQuat;
	vecQuat.x = vn.x;
	vecQuat.y = vn.y;
	vecQuat.z = vn.z;
	vecQuat.w = 0.0f;
 
	resQuat = vecQuat * glm::quat(-vecQuat.x, -vecQuat.y, -vecQuat.z, vecQuat.w)
	resQuat = *this * resQuat;
 
	return (Vector3(resQuat.x, resQuat.y, resQuat.z));
}
*/

//-----------------------------------------------------------------------------
// Getters
//-----------------------------------------------------------------------------

//  Camera Rigid Body Displacement

const glm::vec3& DSGraphics::Camera::GetPosition() const
{
	return mPosition;
}

//-----------------------------------------------------------------------------

const DSMathematics::Quaternion& DSGraphics::Camera::GetOrientation() const///
{
	return mOrientation;
}

/*
const glm::quat& DSGraphics::Camera::GetOrientation() const
{
	return mOrientation;
}
*/

//-----------------------------------------------------------------------------

float DSGraphics::Camera::GetAngle() const
{

	return glm::angle(glm::quat(mOrientation.mW, mOrientation.mV.x, mOrientation.mV.y, mOrientation.mV.z));
}

//-----------------------------------------------------------------------------

const glm::vec3& DSGraphics::Camera::GetAxis() const
{
	return glm::axis(glm::quat(mOrientation.mW, mOrientation.mV.x, mOrientation.mV.y, mOrientation.mV.z));
}

//-----------------------------------------------------------------------------

glm::vec3 DSGraphics::Camera::GetDirectionRight() const
{
	return mOrientation.Rotate(glm::vec3(1.0f, 0.0f, 0.0f));
	
	//glm::vec4 right = mOrientation * glm::vec4(1, 0, 0, 1);

	//return glm::vec3(right);

	//return glm::vec3(QuaternionToMatrix()[0]);
}

//-----------------------------------------------------------------------------

glm::vec3 DSGraphics::Camera::GetDirectionUp() const
{
	return mOrientation.Rotate(glm::vec3(0.0f, 1.0f, 0.0f));

	//glm::vec4 up = mOrientation * glm::vec4(0, 1, 0, 1);

	//return glm::vec3(up);
	//return glm::vec3(QuaternionToMatrix()[1]);
}

//-----------------------------------------------------------------------------

glm::vec3 DSGraphics::Camera::GetDirectionForward() const
{
	return mOrientation.Rotate(glm::vec3(0.0f, 0.0f, -1.0f));

	//glm::vec4 forward = mOrientation * glm::vec4(0, 0, -1, 1);

	//return glm::vec3(forward);
	/*
	glm::mat4 matFromQuat = QuaternionToMatrix();

	return glm::vec3(-matFromQuat[2][0], -matFromQuat[2][1], -matFromQuat[2][2]);
	*/
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
	return glm::mat4_cast(glm::quat(mOrientation.mW, mOrientation.mV.x, mOrientation.mV.y, mOrientation.mV.z)) * glm::translate(glm::mat4(), -mPosition);
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

//  Camera Rigid Body Displacement
void DSGraphics::Camera::SetPosition(const glm::vec3& position)
{
	mPosition = position;
}

//-----------------------------------------------------------------------------

void DSGraphics::Camera::SetOrientation(const DSMathematics::Quaternion& orientation)///
{
	mOrientation = orientation;
}

/*
void DSGraphics::Camera::SetOrientation(const glm::quat& orientation)
{
	mOrientation = orientation;
}
*/

//-----------------------------------------------------------------------------

void DSGraphics::Camera::SetOrientation(float a, const glm::vec3& n)///
{
	mOrientation = DSMathematics::Quaternion(a, n);
}

/*
void DSGraphics::Camera::SetOrientation(float angle, const glm::vec3& axis)
{
	glm::vec3 unitVector = axis;

	//Normalize axis for use in quaternion
	float magnitude = axis.length();
	if(magnitude != 1.0f)
	{
		unitVector  = glm::normalize(axis);
	}

	mOrientation = glm::angleAxis(angle, unitVector);
}
*/

//-----------------------------------------------------------------------------

void DSGraphics::Camera::SetOrientation(float w, float x, float y, float z)///
{
	mOrientation = DSMathematics::Quaternion(w, x, y, z);
}

/*
void DSGraphics::Camera::SetOrientation(const glm::vec3& right, const glm::vec3& up)
{
	//TODO
}
*/

//-----------------------------------------------------------------------------

/*
void DSGraphics::Camera::SetAngle(float angle)
{
	while(angle > 360.0f)
	{
		angle -= 360.0f;
	}
	while(angle < -360.0f)
	{
		angle += 360.0f;
	}
	
	mOrientation = glm::angleAxis(angle, GetAxis());
}
*/

//-----------------------------------------------------------------------------

/*
void DSGraphics::Camera::SetAxis(const glm::vec3& axis)
{
	glm::vec3 unitVector = axis;

	//Normalize axis for use in quaternion
	float magnitude = axis.length();
	if(magnitude != 1.0f)
	{
		unitVector  = glm::normalize(axis);
	}

	mOrientation = glm::angleAxis(GetAngle(), unitVector);
}
*/

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

////-----------------------------------------------------------------------------
////OLDOLDOLDOLDOLDOLDOLDOLDOLDOLDOLDOLDOLDOLD
//
//void DSGraphics::Camera::OffsetPosition(const glm::vec3& offset)
//{
//	mPosition += offset;
//}
//
////-----------------------------------------------------------------------------
//
//void DSGraphics::Camera::OffsetOrientation(float rightAngle, float upAngle)
//{
//	mYaw += rightAngle;
//	mPitch += upAngle;
//	NormalizeAngles();
//}
//
////-----------------------------------------------------------------------------
//
//void DSGraphics::Camera::LookAt(glm::vec3 position)
//{
//	assert(position != mPosition);
//
//	glm::vec3 direction = glm::normalize(position - mPosition);
//	mPitch = glm::radians(asinf(-direction.y));
//	mYaw = -glm::radians(atan2f(-direction.x, -direction.z));
//	NormalizeAngles();
//}
//
////-----------------------------------------------------------------------------
//// Private Member Functions
////-----------------------------------------------------------------------------
//
//void DSGraphics::Camera::NormalizeAngles()
//{
//	mYaw = fmodf(mYaw, 360.0f);
//	
//	//fmodf can return negative values, but this will make them all positive
//	if(mPitch < 0.0f)
//	{
//		while(mPitch < 0.0f)
//		{
//			mPitch += 360.0f;
//		}
//	}
//	else
//	{
//		while(mPitch > 360.0f)
//		{
//			mPitch -= 360.0f;
//		}
//	}
//	if(mYaw < 0.0f)
//	{
//		while(mYaw < 0.0f)
//		{
//			mYaw += 360.0f;
//		}
//	}
//	else
//	{
//		while(mYaw > 360.0f)
//		{
//			mYaw -= 360.0f;
//		}
//	}
//	if(mRoll < 0.0f)
//	{
//		while(mRoll < 0.0f)
//		{
//			mRoll += 360.0f;
//		}
//	}
//	else
//	{
//		while(mRoll > 360.0f)
//		{
//			mRoll -= 360.0f;
//		}
//	}
//
//	if(mIsFirstPersonStyle == true)
//	{
//		if(mPitch > sMaxVerticalAngle)	
//		{
//			mPitch = sMaxVerticalAngle;
//		}
//		else if(mPitch < -sMaxVerticalAngle)
//		{
//			mPitch = -sMaxVerticalAngle;
//		}
//	}
//}
//
////-----------------------------------------------------------------------------
//// Helper Functions
////-----------------------------------------------------------------------------
//
////-----------------------------------------------------------------------------
//// Getters
////-----------------------------------------------------------------------------
//
//const glm::vec3& DSGraphics::Camera::GetPosition() const
//{
//	return mPosition;
//}
//
////-----------------------------------------------------------------------------
//
//float DSGraphics::Camera::GetFov() const
//{
//	return mFov;
//}
//
////-----------------------------------------------------------------------------
//
//float DSGraphics::Camera::GetNearPlane() const
//{
//	return mNearPlane;
//}
//
////-----------------------------------------------------------------------------
//
//float DSGraphics::Camera::GetFarPlane() const
//{
//	return mFarPlane;
//}
//
////-----------------------------------------------------------------------------
//
//float DSGraphics::Camera::GetViewportAspectRatio() const
//{
//	return mViewportAspectRatio;
//}
//
////-----------------------------------------------------------------------------
//
//glm::mat4 DSGraphics::Camera::GetOrientation() const
//{
//	glm::mat4 orientation(1.0f);
//
//	orientation = glm::rotate(orientation, glm::radians(mPitch), glm::vec3(1, 0, 0));
//	//TODO: Fix this from rotating clockwise (should always be counterclockwise)
//	orientation = glm::rotate(orientation, glm::radians(-mYaw), glm::vec3(0, 1, 0));
//	orientation = glm::rotate(orientation, glm::radians(mRoll), glm::vec3(0, 0, 1));
//
//	return orientation;
//}
//
////-----------------------------------------------------------------------------
//
//glm::vec3 DSGraphics::Camera::GetDirectionForward() const
//{
//	glm::vec4 forward = glm::inverse(GetOrientation()) * glm::vec4(0, 0, -1, 1);
//
//	return glm::vec3(forward);
//}
//
////-----------------------------------------------------------------------------
//
//glm::vec3 DSGraphics::Camera::GetDirectionRight() const
//{
//	glm::vec4 right = glm::inverse(GetOrientation()) * glm::vec4(1, 0, 0, 1);
//
//	return glm::vec3(right);
//}
//
////-----------------------------------------------------------------------------
//
//glm::vec3 DSGraphics::Camera::GetDirectionUp() const
//{
//	glm::vec4 up = glm::inverse(GetOrientation()) * glm::vec4(0, 1, 0, 1);
//
//	return glm::vec3(up);
//}
//
////-----------------------------------------------------------------------------
//
//glm::mat4 DSGraphics::Camera::GetMatrix() const
//{
//	return GetProj() * GetView();
//}
//
////-----------------------------------------------------------------------------
//
//glm::mat4 DSGraphics::Camera::GetView() const
//{
//	return GetOrientation() * glm::translate(glm::mat4(), -mPosition);
//
//	/*
//	return glm::lookAt
//	(
//		mPosition,						//camera position
//		glm::vec3(0.0f, 0.0f, 0.0f),	//focal point
//		glm::vec3(0.0f, 1.0f, 0.0f)		//up axis
//	);
//	*/
//}
//
////-----------------------------------------------------------------------------
//
//glm::mat4 DSGraphics::Camera::GetProj() const
//{
//	return
//		glm::perspective
//		(
//			glm::radians(mFov),		//Vertical Field of View (FoV) in degrees
//			mViewportAspectRatio,	//aspect ratio
//			mNearPlane,				//near plane
//			mFarPlane				//far plane
//		);
//}
//
////-----------------------------------------------------------------------------
//// Setters
////-----------------------------------------------------------------------------
//
//void DSGraphics::Camera::SetPosition(const glm::vec3& position)
//{
//	mPosition = position;
//}
//
////-----------------------------------------------------------------------------
//
//void DSGraphics::Camera::SetOrientation(const glm::vec3& orientation)
//{
//	mPitch	= orientation.x;
//	mYaw	= orientation.y;
//	mRoll	= orientation.z;
//	NormalizeAngles();
//}
//
////-----------------------------------------------------------------------------
//
//void DSGraphics::Camera::SetFoV(float fov)
//{
//	assert(fov > 0.0f && fov < 180.0f);
//
//	mFov = fov;
//}
//
////-----------------------------------------------------------------------------
//
//void DSGraphics::Camera::SetNearPlane(float nearPlane)
//{
//	assert((nearPlane > 0.0f) && (nearPlane < mFarPlane));
//
//	mNearPlane = nearPlane;
//}
//
////-----------------------------------------------------------------------------
//
//void DSGraphics::Camera::SetFarPlane(float farPlane)
//{
//	assert(farPlane > mNearPlane);
//
//	mFarPlane = farPlane;
//}
//
////-----------------------------------------------------------------------------
//
//void DSGraphics::Camera::SetPlanes(float nearPlane, float farPlane)
//{
//	assert(nearPlane > 0.0f);
//	assert(farPlane > nearPlane);
//
//	mNearPlane = nearPlane;
//	mFarPlane = farPlane;
//}
//
////-----------------------------------------------------------------------------
//
//void DSGraphics::Camera::SetViewportAspectRatio(float viewportAspectRatio)
//{
//	assert(viewportAspectRatio > 0.0f);
//
//	mViewportAspectRatio = viewportAspectRatio;
//}