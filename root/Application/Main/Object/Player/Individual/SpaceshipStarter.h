//=============================================================================
// File:		SpaceshipStarter.h
// Created:		2015/02/27
// Last Edited:	2015/02/27
// Copyright:	Daniel Schenker
// Description:	Object > Player > SpaceshipStarter.
//=============================================================================

#ifndef SPACESHIPSTARTER_H
#define SPACESHIPSTARTER_H

//=============================================================================
//Includes
//=============================================================================

#include "../Player.h"

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

class SpaceshipStarter : public Player
{
public:
	//Constructors
	SpaceshipStarter();
	//Destructor
	virtual ~SpaceshipStarter();

	//Member Functions
public:
	virtual void LoadAsset(DSGraphics::Program* pProgram, DSGraphics::Texture* pTexture = nullptr);
private:
public:
	// Getters
	// Setters

	//Member Variables
private:
};

#endif //#ifndef SPACESHIPSTARTER_H