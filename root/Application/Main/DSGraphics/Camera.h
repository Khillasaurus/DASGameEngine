//=============================================================================
// File:		Camera.h
// Created:		2015/02/12
// Last Edited:	2015/04/10
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
//#include <glm/gtc/quaternion.hpp>

// Daniel Schenker
#include "../DSMathematics/Quaternion.h"

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
			const DSMathematics::Quaternion& orientation = DSMathematics::Quaternion(),
			float fov = 90.0f,
			float nearPlane = 0.01f,
			float farPlane = 1000.0f,
			float viewportAspectRatio = 16.0f/9.0f
		);
		//Destructor
		~Camera();

	public:
		//Member Functions
		
		// Usage
		void SetPosition(glm::vec3 pos, bool moveLocally = true);
		void Move(glm::vec3 pos);
		void SetOrientation(DSMathematics::Quaternion orientation, bool relative = true);
	
		// Getters
		//  Camera Rigid Body Displacement
		const glm::vec3& GetPosition() const;
		const DSMathematics::Quaternion& GetOrientation() const;

		//  Direction - TODO: Move this to the Quaternion class, or some math helper class, as this functionality isn't exclusive to cameras alone
		glm::vec3 GetDirectionRight() const;
		glm::vec3 GetDirectionUp() const;
		glm::vec3 GetDirectionForward() const;
		glm::vec3 GetDirectionBackward() const;

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
		//  Camera Settings
		void SetFoV(float fov);
		void SetNearPlane(float nearPlane);
		void SetFarPlane(float farPlane);
		void SetPlanes(float nearPlane, float farPlane);
		void SetViewportAspectRatio(float viewportAspectRatio);

	private:
		// Helper
		glm::mat4 QuaternionToMatrix() const;


		//Member Variables
	private:
		glm::vec3 mPosition;
		DSMathematics::Quaternion mOrientation;
		float mFov;
		float mNearPlane;
		float mFarPlane;
		float mViewportAspectRatio;
	};

}//namespace DSGraphics

#endif //#ifndef CAMERA_H