#ifndef _Shapes_HG_
#define _Shapes_HG_

#include "BasicTypes.h"

class CBall
{
public:
	CBall();
	virtual ~CBall();
	::CPoint location;
	::CPoint locationOLD;
	::CVector velocity;
	::CVector velocityOLD;
	float radius;
};

class CLine
{
public:
	CLine();
	virtual ~CLine();
	::CPoint start;
	::CPoint end;
};

class CColour
{
public:
	CColour();
	CColour( float red, float green, float blue, float alpha );
	CColour( const CColour &v );	// Copy constructor
	virtual ~CColour();
	float red;
	float green;
	float blue;
	float alpha;
};

// Used to show normals, etc. that fade out over time... lovely
class CLinesThatFade : public CLine
{
public:
	CLinesThatFade( const float timeToFade, const CColour &colour );
	virtual ~CLinesThatFade();
	void Fade( float deltaTime );
	float getTimeLeft( void );
	float getTimeLeftPercentage( void );		// time left as percentage of total time (for fading)
	CColour colour;
protected:
	CLinesThatFade();		// Can't call
	float m_timeToFade;		// How long to exist
	float m_timeLeft;		// when zero, we can remove them.
};

class CTriangle
{
public:
	CTriangle();
	virtual ~CTriangle();
	CLine A;
	CLine B;
	CLine C;
	// Added these...
	CVector nA;
	CVector nB;
	CVector nC;
	// 
	CLine nALine;	// From mid-point out... (for debug draw)
	CLine nBLine;
	CLine nCLine;
	float NormalLineDrawingScale;	// Used to scale the normals from a unit vector to something you can see
	//
	CPoint midAB;
	CPoint midBC;
	CPoint midCA;
	//
	void Update(void);
	bool bIsClockwise(void);
};

#endif