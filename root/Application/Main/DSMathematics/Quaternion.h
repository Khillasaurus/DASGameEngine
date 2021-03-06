//=============================================================================
// File:		Quaternion.h
// Created:		2015/02/25
// Last Edited:	2015/04/10
// Copyright:	Daniel Schenker
// Description:	Quaternion
//=============================================================================

#ifndef QUATERNION_H
#define QUATERNION_H

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
//Namespace
//=============================================================================

namespace DSMathematics
{

	//=============================================================================
	//Enums
	//=============================================================================

	//=============================================================================
	//Class Declarations
	//=============================================================================

	class Quaternion
	{
	public:
		//Constructors
		Quaternion();
		Quaternion(float w, float x, float y, float z);
		Quaternion(float a, const glm::vec3& n);
		//Destructor
		virtual ~Quaternion();
		//Copy Constructor
		Quaternion(const Quaternion& rhs);
		//Copy-Assignment Operator
		Quaternion& operator= (const Quaternion& rhs);

		//Member Functions
		// General
		Quaternion Invert() const;
		Quaternion Multiply(const Quaternion& q) const;
		glm::vec3 Rotate(const glm::vec3& v) const;
		Quaternion Slerp(const Quaternion& r, float t) const;
		Quaternion Power(float t) const;
		// Conversions
		void ToAxisAngle(glm::vec3& axis, float& a) const;
	

		//Member Variables
	public:
		float mW;
		glm::vec3 mV;
	};

}//namespace DSMathematics

#endif //#ifndef QUATERNION_H