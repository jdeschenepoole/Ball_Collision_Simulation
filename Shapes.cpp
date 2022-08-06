#include "Shapes.h"
#include "BasicTypes.h"

CBall::CBall()	
{
	// this->location   set to zero by constructor
	// this->velocity	set to zero by constructor
	this->radius = 1.0f;
}

CBall::~CBall()	{}

CLine::CLine() {}

CLine::~CLine() {}

CColour::CColour() 
{
	this->red = this->green = this->blue = 1.0f;	// White
	this->alpha = 1.0f;	// solid
}

CColour::CColour( float red, float green, float blue, float alpha )
{
	this->red = red;	this->green = green;	this->blue = blue;
	this->alpha = alpha;
}

CColour::CColour( const CColour &v )	// Copy constructor
{
	this->red = v.red;	this->green = v.green;	this->blue = v.blue;
	this->alpha = v.alpha;
}

CColour::~CColour() {}

// Constructor you have to call (other is private)
CLinesThatFade::CLinesThatFade( const float timeToFade, const CColour &colour )
{
	this->m_timeToFade = timeToFade;		// How long to exist
	this->m_timeLeft = this->m_timeToFade;
	this->colour = colour;
}

float CLinesThatFade::getTimeLeftPercentage( void )
{
	float timeLeftPer = this->m_timeLeft / this->m_timeToFade;
	return timeLeftPer;
}

CLinesThatFade::CLinesThatFade() {}		// Can't call this one

CLinesThatFade::~CLinesThatFade() {}

void CLinesThatFade::Fade( float deltaTime )
{
	this->m_timeLeft -= deltaTime;
	float colourRatio = ( this->m_timeLeft / this->m_timeToFade );	
	// HACK - rather than figure out a correct fade palatte...
	colourRatio *= 5.0f;
	if ( colourRatio > 1.0f )	colourRatio = 1.0f;
	//
	this->colour.alpha *= colourRatio; 
	this->colour.red *= colourRatio; 
	this->colour.green *= colourRatio;
	this->colour.blue *= colourRatio;
}

float CLinesThatFade::getTimeLeft( void )
{
	return this->m_timeLeft;
}

CTriangle::CTriangle() 
{
	this->NormalLineDrawingScale = 1.0f;
}

CTriangle::~CTriangle() {}

float Test2D( const CVector &v1, const CVector &v2 )
{
	return v1.x * v2.y - v1.y * v1.x;
}

bool CTriangle::bIsClockwise(void)
{
	CVector A( this->A.start.x, this->A.start.y, 0.0f );
	CVector B( this->B.start.x, this->B.start.y, 0.0f );
	CVector C( this->C.start.x, this->C.start.y, 0.0f );

	float cross = Test2D( B - A, C - A );
	if ( cross > 0 )	
		return true;
	return false;
}

float midPoint( const float x, const float y )
{
	float mid = 0.0f;
	if ( x < y )
	{
		mid = ((y - x) / 2.0f) + x;
	}
	else
	{
		mid = ((x - y) / 2.0f) + y;
	}
	return mid;
}

CVector GetNormal( const CPoint &pLineStart, const CPoint &pLineEnd )
{
	CVector cLineStart( pLineStart.x, pLineStart.y, 0.0f );
	CVector cLineEnd( pLineEnd.x, pLineEnd.y, -0.0f );
	CVector linePlane = cLineStart - cLineEnd;
	CVector lineNormal( -linePlane.y, linePlane.x, 0.0f );
	lineNormal.Normalize();
	return lineNormal;
}

void CTriangle::Update(void)
{
	this->midAB.x = midPoint( A.start.x, B.start.x );
	this->midAB.y = midPoint( A.start.y, B.start.y );
	// 
	this->midBC.x = midPoint( B.start.x, C.start.x );
	this->midBC.y = midPoint( B.start.y, C.start.y );
	//
	this->midCA.x = midPoint( C.start.x, A.start.x );
	this->midCA.y = midPoint( C.start.y, A.start.y );
	//
	// Update normals...
	// Calculate the normal to that line. Note that if we do the cross product, which 
	//	would work for 3D, we get zeros because the z values are zero.
	// 
	// A work around is explained well here: http://stackoverflow.com/questions/1243614/how-do-i-calculate-the-normal-vector-of-a-line-segment
	//
	// Another way to think of it is to calculate the unit vector for a given direction and 
	//	then apply a 90 degree counterclockwise rotation to get the normal vector.
	//
	// The matrix representation of the general 2D transformation looks like this:
	//
	// x' = x cos(t) - y sin(t)
	// y' = x sin(t) + y cos(t)
	//
	// where (x,y) are the components of the original vector and (x', y') are the transformed 
	//	components.
	//
	// If t = 90 degrees, then cos(90) = 0 and sin(90) = 1. Substituting and multiplying it 
	//	out gives:
	//
	// x' = -y
	// y' = +x
	//
	this->nA = GetNormal( this->A.start, this->B.start );
	this->nB = GetNormal( this->B.start, this->C.start );
	this->nC = GetNormal( this->C.start, this->A.start );
	// 
	// Update the pretty drawing normals...
	this->nALine.start = CPoint( this->midAB.x, this->midAB.y );
	CVector nABig = CVector( this->midAB.x, this->midAB.y, 0.0f ) + (this->nA * this->NormalLineDrawingScale);	// Scale it
	this->nALine.end = CPoint( nABig.x, nABig.y );

	this->nBLine.start = CPoint( this->midBC.x, this->midBC.y );
	CVector nBBig = CVector( this->midBC.x, this->midBC.y, 0.0f ) + (this->nB * this->NormalLineDrawingScale);	// Scale it
	this->nBLine.end = CPoint( nBBig.x, nBBig.y );

	this->nCLine.start = CPoint( this->midCA.x, this->midCA.y );
	CVector nCBig = CVector( this->midCA.x, this->midCA.y, 0.0f ) + (this->nC * this->NormalLineDrawingScale);	// Scale it
	this->nCLine.end = CPoint( nCBig.x, nCBig.y );
}
