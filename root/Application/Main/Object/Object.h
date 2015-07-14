//=============================================================================
// File:		Object.h
// Created:		2015/02/27
// Last Edited:	2015/02/27
// Copyright:	Daniel Schenker
// Description:	Object is abstract class which is a parent of all object types.
//=============================================================================

#ifndef OBJECT_H
#define OBJECT_H

//=============================================================================
//Includes
//=============================================================================

#include "../DSGraphics/ModelAsset.h"

//=============================================================================
//Forward Declarations
//=============================================================================

//=============================================================================
//Namespace
//=============================================================================

//=============================================================================
//Enums
//=============================================================================

//=============================================================================
//Class Declarations
//=============================================================================

class Object
{
protected:
	//Constructors
	Object();
	//Destructor
	virtual ~Object();

	//Member Functions
protected:
	virtual void LoadAsset(DSGraphics::Program* pProgram) = 0;
public:
	// Getters
	DSGraphics::ModelAsset* GetModelAsset();
	bool GetIsModelAssetLoaded();
	// Setters

	//Member Variables
	// Statics
public:
	static float sDCPerM;//Device Coordinates / meter
protected:
	DSGraphics::ModelAsset* mpModelAsset;//TODO: Change this to a smart pointer, in case misuse of ModelAsset occurs and the ModelAsset gets deleted before the ModelInstance pointing to the ModelAsset gets deleted.
	bool mIsModelAssetLoaded;
};

#endif //#ifndef OBJECT_H