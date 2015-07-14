//=============================================================================
// File:		Camera.h
// Created:		2015/02/12
// Last Edited:	2015/02/24
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
#include <glm/gtc/quaternion.hpp>

//=============================================================================
//Forward Declarations
//=============================================================================

//=============================================================================
//Namespace
//=============================================================================

namespace DSGraphics
{

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
		Camera::Camera
		(
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f),
			glm::quat orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
			float fov = 90.0f,
			float nearPlane = 0.01f,
			float farPlane = 1000.0f,
			float viewportAspectRatio = 16.0f/9.0f
		);
		//Destructor
		~Camera();

		//Member Functions
	public:
		// General
		void MoveWorld(const glm::vec3& offset);//Moves camera using world coordinates
		//void MoveLocal(const glm::vec3& offset);//Moves camera based on the cameras current orientation//TODO
		void Rotate(float angle, const glm::vec3& axis);
		//void LookAt(glm::vec3 position);//TODO
	private:
		// Helper
		void NormalizeQuaternion();
		glm::mat4 QuaternionToMatrix() const;
		//glm::vec3 ApplyQuatToVec(const glm::quat& q, const glm::vec3& v) const;//TODO

	public:
		// Getters
		//  Camera Rigid Body Displacement
		const glm::vec3& GetPosition() const;
		const glm::quat& GetOrientation() const;
		float GetAngle() const;
		const glm::vec3& GetAxis() const;
		glm::vec3 GetDirectionRight() const;
		glm::vec3 GetDirectionUp() const;
		glm::vec3 GetDirectionForward() const;
		//  Model View Matrices
		glm::mat4 GetView() const;
		glm::mat4 GetProj() const;
		glm::mat4 GetMatrix() const;
		//  Camera Settings
		float GetFoV() const;
		float GetNearPlane() const;
		float GetFarPlane() const;
		float GetViewportAspectRatio() const;

		// Setters
		//  Camera Rigid Body Displacement
		void SetPosition(const glm::vec3& position);
		void SetOrientation(const glm::quat& orientation);
		void SetOrientation(float angle, const glm::vec3& axis);
		//void SetOrientation(const glm::vec3& right, const glm::vec3& up);//TODO
		void SetAngle(float angle);
		void SetAxis(const glm::vec3& axis);
		//  Camera Settings
		void SetFoV(float fov);
		void SetNearPlane(float nearPlane);
		void SetFarPlane(float farPlane);
		void SetPlanes(float nearPlane, float farPlane);
		void SetViewportAspectRatio(float viewportAspectRatio);

		//Member Variables
	private:
		glm::vec3 mPosition;
		glm::quat mOrientation;
		float mFov;
		float mNearPlane;
		float mFarPlane;
		float mViewportAspectRatio;
	};

}//namespace DSGraphics

#endif //#ifndef CAMERA_H