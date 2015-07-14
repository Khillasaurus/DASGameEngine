//=============================================================================
// File:		Quaternion.cpp
// Created:		2015/02/25
// Last Edited:	2015/02/25
// Copyright:	Daniel Schenker
// Description:	Quaternion
//=============================================================================

//=============================================================================
//Includes
//=============================================================================

// Daniel Schenker
#include "Quaternion.h"

//=============================================================================
//Statics
//=============================================================================

//=============================================================================
//Class Definitions
//=============================================================================

//-----------------------------------------------------------------------------
// Constuctors
//-----------------------------------------------------------------------------

DSMathematics::Quaternion::Quaternion()
:	mW(1.0f)
,	mV(0.0f, 0.0f, 0.0f)
{
}

//-----------------------------------------------------------------------------

DSMathematics::Quaternion::Quaternion(float w, float x, float y, float z)
:	mW(w)
,	mV(x, y, z)
{
}

//-----------------------------------------------------------------------------

DSMathematics::Quaternion::Quaternion(float a, const glm::vec3& n)
{
	mW = cos(a / 2);
	mV.x = n.x * sin(a / 2);
	mV.y = n.y * sin(a / 2);
	mV.z = n.z * sin(a / 2);
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------

DSMathematics::Quaternion::~Quaternion()
{
}

//-----------------------------------------------------------------------------
// Copy Constructor
//-----------------------------------------------------------------------------

DSMathematics::Quaternion::Quaternion(const DSMathematics::Quaternion& rhs)
{
	mW = rhs.mW;
	//mV = rhs.mV
	mV.x = rhs.mV.x;
	mV.y = rhs.mV.y;
	mV.z = rhs.mV.z;
}

//-----------------------------------------------------------------------------
// Copy-Assignment Operator
//-----------------------------------------------------------------------------

DSMathematics::Quaternion& DSMathematics::Quaternion::operator= (const DSMathematics::Quaternion& rhs)
{
	//Check for self-assignment by comparing the address of the implicit object and the paramter
	if(this == &rhs)
	{
		return *this;
	}

	mW = rhs.mW;
	//mV = rhs.mV
	mV.x = rhs.mV.x;
	mV.y = rhs.mV.y;
	mV.z = rhs.mV.z;

	return *this;
}

//-----------------------------------------------------------------------------
// Public Member Functions
//-----------------------------------------------------------------------------
//  General

/*
Description:
	Returns the inverted form of this quaternion.
	Does not modify original quaternion, rather returns a new one.
*/
DSMathematics::Quaternion DSMathematics::Quaternion::Invert() const
{
	DSMathematics::Quaternion i;
	i.mW = mW;
	i.mV = -mV;
	/*
	//same as above for i.mV, just in case the vector copy-assignment operator is not supported.
	i.mV.x = -mV.x;
	i.mV.y = -mV.y;
	i.mV.z = -mV.z;
	*/

	return i;
}

//-----------------------------------------------------------------------------

/*
Description:
	Rotates a vector using this quaternion.
	This is equivalent to the multiplication of this quaternion with a vector.
	Does not modify original vector, rather returns a new one.
*/
glm::vec3 DSMathematics::Quaternion::Rotate(const glm::vec3& v) const
{
	/*
	Where q refers to *this quaternion...
	The basic equation is:
		(q)(p)(q.Invert())
	Can also be described as:
		qpq^i where ^i means inverse
	A simplified version of the basic equation is used instead for optimization.
	*/

	DSMathematics::Quaternion p;
	p.mW = 0;
	p.mV = v;

	//Above mentioned optimized form:
	glm::vec3 mVCrossv = glm::cross(mV, v);
	return v + (mVCrossv * (2.0f * mW)) + (glm::cross(mV, mVCrossv) * 2.0f);

	/*
	Above mentioned basic equation form:

	const Quaternion& q = (*this);
	return (q * p * q.Invert()).mV;
	*/
}

//-----------------------------------------------------------------------------
//  Operator Overloading Style, Done Through Functions Instead

/*
Description:
	Returns the multiplication of this quaternion with another quaternion.
	Does not modify original quaternion, rather returns a new one.
*/
DSMathematics::Quaternion DSMathematics::Quaternion::Multiply(const DSMathematics::Quaternion& q) const
{
	DSMathematics::Quaternion m;

	m.mW = (mW * q.mW) - glm::dot(mV, q.mV);
	m.mV = (mV * q.mW) + (q.mV * mW) + glm::cross(mV, q.mV);
	/*
	//same as above for m.mV, just in case vector times scalar is not supported.
	glm::vec3 p1 = mV;
	p1.x *= q.mW;
	p1.y *= q.mW;
	p1.z *= q.mW;
	glm::vec3 p2 = q.mV;
	p2.x *= mW;
	p2.y *= mW;
	p2.z *= mW;
	glm::vec3 p3 = glm::cross(mV, q.mV);
	m.mV = p1 + p2 + p3;
	*/

	return m;
}

//-----------------------------------------------------------------------------
// Private Member Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Getters
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Setters
//-----------------------------------------------------------------------------