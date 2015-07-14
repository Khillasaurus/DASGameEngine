//=============================================================================
// File:		Main.cpp
// Created:		2015/01/28
// Last Edited:	2015/03/01
// Copyright:	Daniel Schenker
//				All files in this solution are copyright Daniel Schenker.
//				Please contact Daniel Schenker at D.A.Schenker@gmail.com to request file usage.
// Description:	Main.cpp
//=============================================================================

//=============================================================================
//Includes
//=============================================================================

// Daniel Schenker
#include "Application.h"

//=============================================================================
//Function Prototypes
//=============================================================================

//=============================================================================
//Main
//=============================================================================
int main()
{
	//Allocating entire application here for when I create a memory manager in the future
	Application* pApp= new Application();
	if(pApp != nullptr)
	{
		delete pApp;
		pApp = nullptr;
	}

	return 0;
}

//=============================================================================
//Helper Functions
//=============================================================================