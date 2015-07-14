//=============================================================================
// File:		ModelInstance.h
// Created:		2015/02/15
// Last Edited:	2015/02/19
// Copyright:	Daniel Schenker
// Description:	ModelInstance
//=============================================================================

#ifndef MODELINSTANCE_H
#define MODELINSTANCE_H

//=============================================================================
//Includes
//=============================================================================

// Third-Party Libraries
//  GLM
#include <glm/glm.hpp>

// Daniel Schenker
#include "Camera.h"
#include "ModelAsset.h"

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

	class ModelInstance
	{
	public:
		//Constructors
		ModelInstance(DSGraphics::ModelAsset* mpAsset, DSGraphics::Camera* mpCamera = nullptr);
		//Destructor
		~ModelInstance();

		//Member Functions
	public:
		//General
		void UpdateTransform();
		void Render();
		//TODO: Move, Turn, etc
	private:
		//UpdateTransform Sub-Functions
		void UpdateScale();
		void UpdateRotate();
		void UpdateTranslate();
	public:
		// Getters
		glm::vec3 GetSize() const;
		//glm::vec3 GetOrientation() const;
		GLfloat GetOrientationAngleInRadians() const;
		glm::vec3 GetOrientationAxis() const;
		glm::vec3 GetPosition() const;
		// Setters
		void SetSize(const glm::vec3& size);
		//void SetOrientation(const glm::vec3& orientation);
		void SetOrientationAngle(GLfloat radians);
		void SetOrientationAxis(const glm::vec3& rotationAxis);
		void SetPosition(const glm::vec3& position);

		//Member Variables
	private:
		//Externally Accessible (not encapsulated)
			// Model Asset
			DSGraphics::ModelAsset* mpAsset;

			glm::vec3 mSize;
			//glm::vec3 mOrientation;
			GLfloat mOrientationAngle;
			glm::vec3 mOrientationAxis;
			glm::vec3 mPosition;

			// Camera
			DSGraphics::Camera* mpCamera;

		//Externally Inaccessible (encapsulated)
			// Model Matrix
			glm::mat4 mTransform;
			//  Scale
			glm::mat4 mScale;
			bool mSizeUpdated;
			//  Rotate
			glm::mat4 mRotate;
			//glm::vec3 mRotationAxis;
			bool mOrientationUpdated;
			//  Translate
			glm::mat4 mTranslate;
			bool mPositionUpdated;
			//  Shader Uniform
			GLuint mUniformModel;
		
			//  Shader Uniform
			GLuint mUniformCamera;
	};

}//namespace DSGraphics

#endif //#ifndef MODELINSTANCE_H