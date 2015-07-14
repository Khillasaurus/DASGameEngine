//=============================================================================
// File:		Camera.h
// Created:		2015/02/12
// Last Edited:	2015/02/13
// Copyright:	Daniel Schenker
// Description:	Camera
//=============================================================================

#ifndef CAMERA_H
#define CAMERA_H

//=============================================================================
//Includes
//=============================================================================

// Third-Party Libraries
//  GLM
#include <glm/glm.hpp>

//=============================================================================
//Forward Declarations
//=============================================================================

//=============================================================================
//Enums
//=============================================================================

//=============================================================================
//Class Declarations
//=============================================================================

class Camera
{
public:
	//Constructors
	Camera();
	//Destructor
	~Camera();

	//Member Functions
public:
	void OffsetPosition(const glm::vec3& offset);
	void OffsetOrientation(float rightAngle, float upAngle);
	void LookAt(glm::vec3 position);
private:
	void NormalizeAngles();


public:
	// Getters
	const glm::vec3& GetPosition() const;
	float GetFov() const;
	float GetNearPlane() const;
	float GetFarPlane() const;
	float GetViewportAspectRatio() const;
	glm::mat4 GetOrientation() const;
	glm::vec3 GetDirectionForward() const;
	glm::vec3 GetDirectionRight() const;
	glm::vec3 GetDirectionUp() const;
	glm::mat4 GetMatrix() const;
	glm::mat4 GetView() const;
	glm::mat4 GetProj() const;
	// Setters
	void SetPosition(const glm::vec3& position);
	void SetFoV(float fov);
	void SetNearPlane(float nearPlane);
	void SetFarPlane(float farPlane);
	void SetPlanes(float nearPlane, float farPlane);
	void SetViewportAspectRatio(float viewportAspectRatio);

	//Member Variables
private:
	glm::vec3 mPosition;
	float mPitch;	//Rotation around x axis
	float mYaw;		//Rotation around y axis
	float mRoll;	//Rotation around z axis
	float mFov;
	float mNearPlane;
	float mFarPlane;
	float mViewportAspectRatio;

};

#endif //#ifndef CAMERA_H