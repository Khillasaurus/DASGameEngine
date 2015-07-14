//=============================================================================
// File:		Environmental.h
// Created:		2015/02/23
// Last Edited:	2015/02/23
// Copyright:	Daniel Schenker
// Description:	Object > Solid > Environmental. Environmental is an abstract class which does not inherit from anything.
//=============================================================================

#ifndef ENVIRONMENTAL_H
#define ENVIRONMENTAL_H

//=============================================================================
//Includes
//=============================================================================

//=============================================================================
//Forward Declarations
//=============================================================================

class ModelInstance;

//=============================================================================
//Namespace
//=============================================================================

//=============================================================================
//Enums
//=============================================================================

//=============================================================================
//Class Declarations
//=============================================================================

class Environmental
{
public:
	//Constructors
	Environmental();
	//Destructor
	virtual ~Environmental();

	//Member Functions
public:
private:
public:
	// Getters
	// Setters

	//Member Variables
private:
	ModelInstance* mpInstance;

};

#endif //#ifndef ENVIRONMENTAL_H