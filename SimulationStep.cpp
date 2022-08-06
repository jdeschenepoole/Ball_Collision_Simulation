// SimulationStep.cpp
// This will update the world (i.e. the objects)
// Note that this is *separate* from the 
//    Render() function....

#include "global.h"

void SimulationStep( std::vector< CTriangle > &vecTriangles, std::vector< CBall > &vecBalls )
{
	CHRTimer simTimer;
	simTimer.Reset();
	simTimer.Start();


	// Determine how many seconds have gone by
	//	since the last call to this function...
	::g_TheTimer.Stop();

	float deltaSeconds = ::g_TheTimer.GetElapsedSeconds();

	const float MIN_TIMESTEP = 0.001f;
	if ( deltaSeconds > MIN_TIMESTEP )
	{
		deltaSeconds = MIN_TIMESTEP;
	}

	::g_TheTimer.Reset();
	::g_TheTimer.Start();

	// Update the ball positions... #1
	{
		std::vector<CBall>::iterator itBall;
		for ( itBall = vecBalls.begin(); itBall != vecBalls.end(); itBall++ )
		{
			// 
			itBall->velocityOLD = itBall->velocity;	// Save old velocity
			itBall->locationOLD = itBall->location;	// Save old location (in case we have to go back)
			//
			itBall->location.x += itBall->velocity.x * deltaSeconds;
			itBall->location.y += itBall->velocity.y * deltaSeconds;
			// Apply gravity...
			itBall->velocity.y += ::g_GRAVITY * deltaSeconds;

		}
	}


	std::vector< CBall >::iterator itBall;
	for ( itBall = vecBalls.begin(); itBall != vecBalls.end(); itBall++ )
	{	// Update the position of the balls. 

		//Ball On Ball
		for (std::vector< CBall >::iterator itBallX = vecBalls.begin(); itBallX != vecBalls.end(); itBallX++)
		{
			bool coll = testBallBallCollision(itBall,itBallX);
			if (coll)
			{
				handleBallBallCollision(itBall, itBallX);
			}
		}

		//Ball On Triangle
		for (std::vector< CTriangle >::iterator itTri = vecTriangles.begin(); itTri != vecTriangles.end(); itTri++)
		{
			// Sphere-triangle collision
			// Check for triangle-shpere intersection
			float t = 0.0f;
			CPoint pClosest;	//CPoint pLineStart;	CPoint pLineEnd;
			CVector lineNormal;

			// These were added for the pretty drawing
			bool bClosestToLineA = false;
			bool bClosestToLineB = false;
			bool bClosestToLineC = false;

			if ( TestSphereTriangle( *(itBall), itTri->A.start, itTri->B.start, itTri->C.start, pClosest ) )
			{	// The sphere is 'inside' or touching the triangle.
				// Determine which side is closest to the triangle (that's the side its touching)
					
				CPoint pCloseA;
				ClosestPointPointSegment( itBall->location, itTri->A.start, itTri->A.end, t, pCloseA );

				CPoint pCloseB;
				ClosestPointPointSegment( itBall->location, itTri->B.start, itTri->B.end, t, pCloseB );

				CPoint pCloseC;
				ClosestPointPointSegment( itBall->location, itTri->C.start, itTri->C.end, t, pCloseC );

				// Which is shortest distance?
				// Note that we are using distance squared, instead of distance to avoid
				//	the sqrt() function. Because we are just comparing them (i.e. we don't 
				//	really need the *actual* distance), this is fine
				float distA = CVector::DistanceSquared( CVector( itBall->location.x, itBall->location.y, 0.0f ),
														CVector( pCloseA.x, pCloseA.y, 0.0f ) );
				float distB = CVector::DistanceSquared( CVector( itBall->location.x, itBall->location.y, 0.0f ),
														CVector( pCloseB.x, pCloseB.y, 0.0f ) );
				float distC = CVector::DistanceSquared( CVector( itBall->location.x, itBall->location.y, 0.0f ),
														CVector( pCloseC.x, pCloseC.y, 0.0f ) );
				// Closest to line A?
				if ( ( distA <= distB ) && ( distA <= distC ))
				{
					//pLineStart = itTri->A.start;	pLineEnd = itTri->A.end;
					lineNormal = itTri->nA;
					bClosestToLineA = true;
				}
				// Closest to line B?
				else if ( ( distB <= distA ) && ( distB <= distC ))
				{
					//pLineStart = itTri->B.start;	pLineEnd = itTri->B.end;
					lineNormal = itTri->nB;
					bClosestToLineB = true;
				}
				// Closest to line C? (NOTE: This should be the only other option....
				else if ( ( distC <= distA ) && ( distC <= distB ) )
				{
					//pLineStart = itTri->C.start;	pLineEnd = itTri->C.end;
					lineNormal = itTri->nC;
					bClosestToLineC = true;
				}
				else	// Something is very wrong...
				{
					MessageBox(0 ,L"Something is Wrong", L"Physics Failed", MB_OK) ;
				}

				// Now we compute the reflected vector based on the normal and the plane
				CVector vReflectDir = (itBall->velocity) + lineNormal 
									    * (-2.0f * CVector::DotProduct( lineNormal, (itBall->velocity)) );
				vReflectDir.Normalize();

				//float normalFactor = CVector::DotProduct( lineNormal, vReflectDir );

				float length = (itBall->velocity).Magnitude();

				CVector vNewVel = vReflectDir * length;

				itBall->velocity = -vNewVel;	// Update to the new velocity
				//
				// HACK: move the ball to previous position, avoiding penetration
				itBall->location = itBall->locationOLD;

			}
		}//for ( itTri = ...

		// Did we hit the limit of the screen? 
		//Ball on walls
		if ( itBall->location.x > ::g_WorldLimitRight )
		{	
			itBall->velocity.x = -abs(itBall->velocity.x);
		}
		if ( itBall->location.x < ::g_WorldLimitLeft )
		{
			itBall->velocity.x = abs(itBall->velocity.x);
		}
		if ( itBall->location.y < ::g_WorldLimitBottom )
		{
			itBall->velocity.y = abs(itBall->velocity.y);
		}
		if ( itBall->location.y > ::g_WorldLimitTop )
		{
			itBall->velocity.y = -abs(itBall->velocity.y);
		}
	}

	// End
	simTimer.Stop();
	float timeSpend = simTimer.GetElapsedSeconds();
	int ImASexyBreakPoint = 0;
}