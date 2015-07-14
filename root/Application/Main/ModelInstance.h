//=============================================================================
// File:		ModelInstance.h
// Created:		2015/02/15
// Last Edited:	2015/02/16
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

//=============================================================================
//Forward Declarations
//=============================================================================

class Camera;
class ModelAsset;

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
	ModelInstance(ModelAsset* mpAsset, Camera* mpCamera);
	//Destructor
	~ModelInstance();

	//Member Functions
public:
	//General
	void UpdateTransform();
	void Render();
	// Getters
	glm::mat4 GetScale() const;
	glm::mat4 GetRotate() const;
	glm::vec3 GetRotationAxis() const;
	glm::mat4 GetTranslate() const;
	// Setters
	void SetScale(const glm::mat4& scale);
	void SetRotate(const glm::mat4& rotate);
	void SetRotationAxis(const glm::vec3& rotationAxis);
	void SetTranslate(const glm::mat4& translate);

	//Member Variables
private:
	// Model Asset
	ModelAsset* mpAsset;
	// Model Matrix
	glm::mat4 mTransform;
	//  Scale
	glm::mat4 mScale;
	//  Rotate
	glm::mat4 mRotate;
	glm::vec3 mRotationAxis;
	//  Translate
	glm::mat4 mTranslate;
	//  Shader Uniform
	GLuint mUniformModel;
	// Camera
	Camera* mpCamera;
	//  Shader Uniform
	GLuint mUniformCamera;
};

#endif //#ifndef MODELINSTANCE_H