/**********************************
Programmer: Joseph Deschene-Poole
Project:	Physics and Simulation Project 1
Due:		October 8th, 2011
*///*******************************

#include "global.h"

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
 
	// Save the "handle to the instance." a.k.a. the number that windows uses to identify this application instance
	::g_hInst = hInstance;

	std::wstring error;
	if ( !InitDirectXAndCreateWindow( hInstance, nCmdShow, L"Physics 1 - wireframe example", 800, 600, error ) )
	{
		MessageBox( NULL, error.c_str(), L"Something is Wrong", MB_OK );
		exit( -1 );
	}

	if ( !LoadAndPrepareShaderEffectFile( L"HLSL_WireFrame.fx", "Render", error ) )
	{
		MessageBox( NULL, error.c_str(), L"Something is wrong", MB_OK );
		exit( -1 );
	}

	//// **************************************************************
	//if ( !LoadModelIntoBuffer( L"ply\\Utah_Teapot.ply", error ) )
	//// **************************************************************
	//{
	//	MessageBox( NULL, error.c_str(), L"Something is wrong", MB_OK );
	//	exit( -1 );
	//}


	// *************************
	// Create a number of triangles and balls...
	std::vector< CTriangle >	vecTriangles;
	std::vector< CBall >		vecBalls;

	//Adds triangles one to each bottom corner
	CTriangle tempTriangle;
	tempTriangle.A.start.x = ::g_WorldLimitRight;
	tempTriangle.A.start.y = ::g_WorldLimitBottom;

	tempTriangle.B.start.x = ::g_WorldLimitRight - 1.5f;
	tempTriangle.B.start.y = ::g_WorldLimitBottom;
		
	tempTriangle.C.start.x = ::g_WorldLimitRight;
	tempTriangle.C.start.y = ::g_WorldLimitBottom + 2.5f;

	tempTriangle.A.end = tempTriangle.B.start;
	tempTriangle.B.end = tempTriangle.C.start;
	tempTriangle.C.end = tempTriangle.A.start;

	vecTriangles.push_back( tempTriangle );

	tempTriangle.A.start.x = ::g_WorldLimitLeft;
	tempTriangle.A.start.y = ::g_WorldLimitBottom;

	tempTriangle.B.start.x = ::g_WorldLimitLeft + 2.5f;
	tempTriangle.B.start.y = ::g_WorldLimitBottom;
		
	tempTriangle.C.start.x = ::g_WorldLimitLeft;
	tempTriangle.C.start.y = ::g_WorldLimitBottom + 1.5f;

	tempTriangle.A.end = tempTriangle.B.start;
	tempTriangle.B.end = tempTriangle.C.start;
	tempTriangle.C.end = tempTriangle.A.start;

	vecTriangles.push_back( tempTriangle );

	tempTriangle.A.start.x = ::g_WorldLimitLeft + 4.6f;
	tempTriangle.A.start.y = ::g_WorldLimitBottom + 5.0f;

	tempTriangle.B.start.x = ::g_WorldLimitLeft + 7.5f;
	tempTriangle.B.start.y = ::g_WorldLimitBottom + 7.1f;
		
	tempTriangle.C.start.x = ::g_WorldLimitLeft + 5.0f;
	tempTriangle.C.start.y = ::g_WorldLimitBottom + 6.5f;

	tempTriangle.A.end = tempTriangle.B.start;
	tempTriangle.B.end = tempTriangle.C.start;
	tempTriangle.C.end = tempTriangle.A.start;

	vecTriangles.push_back( tempTriangle );

	tempTriangle.A.start.x = ::g_WorldLimitLeft + 2.6f;
	tempTriangle.A.start.y = ::g_WorldLimitBottom + 3.0f;

	tempTriangle.B.start.x = ::g_WorldLimitLeft + 3.5f;
	tempTriangle.B.start.y = ::g_WorldLimitBottom + 5.1f;
		
	tempTriangle.C.start.x = ::g_WorldLimitLeft + 4.0f;
	tempTriangle.C.start.y = ::g_WorldLimitBottom + 4.5f;

	tempTriangle.A.end = tempTriangle.B.start;
	tempTriangle.B.end = tempTriangle.C.start;
	tempTriangle.C.end = tempTriangle.A.start;

	vecTriangles.push_back( tempTriangle );

	tempTriangle.A.start.x = ::g_WorldLimitRight - 4.6f;
	tempTriangle.A.start.y = ::g_WorldLimitBottom + 4.0f;

	tempTriangle.B.start.x = ::g_WorldLimitLeft + 7.5f;
	tempTriangle.B.start.y = ::g_WorldLimitBottom + 2.1f;
		
	tempTriangle.C.start.x = ::g_WorldLimitRight - 5.0f;
	tempTriangle.C.start.y = ::g_WorldLimitBottom + 2.5f;

	tempTriangle.A.end = tempTriangle.B.start;
	tempTriangle.B.end = tempTriangle.C.start;
	tempTriangle.C.end = tempTriangle.A.start;

	vecTriangles.push_back( tempTriangle );

	//creates 0 random triangles
	int numbeOfTriangles = 0;
	for ( int count = 0; count != numbeOfTriangles; count++ )
	{
		CTriangle tempTriangle;
		tempTriangle.A.start.x = getRand<float>( ::g_WorldLimitLeft, ::g_WorldLimitRight );
		tempTriangle.A.start.y = getRand<float>( ::g_WorldLimitTop, ::g_WorldLimitBottom );

		tempTriangle.B.start.x = getRand<float>( ::g_WorldLimitLeft, ::g_WorldLimitRight );
		tempTriangle.B.start.y = getRand<float>( ::g_WorldLimitTop, ::g_WorldLimitBottom );
		
		tempTriangle.C.start.x = getRand<float>( ::g_WorldLimitLeft, ::g_WorldLimitRight );
		tempTriangle.C.start.y = getRand<float>( ::g_WorldLimitTop, ::g_WorldLimitBottom );

		// The triangle is made up of three lines, so set these
		// Note that I'm not using these "end" locations for 
		// the rendering... (so I really don't have to set them here)
		tempTriangle.A.end = tempTriangle.B.start;
		tempTriangle.B.end = tempTriangle.C.start;
		tempTriangle.C.end = tempTriangle.A.start;

		bool bTriangleOK = false;

		// Making things 'pretty'...
		// Double check that the triangles don't intersect with any existing triangles...
		std::vector<CTriangle>::iterator itTri;
		for ( itTri = vecTriangles.begin(); itTri != vecTriangles.end(); itTri++ )
		{
			if ( ::TestPointTriangle( tempTriangle.A.start, itTri->A.start, itTri->B.start, itTri->C.start ) )
			{	// Point is inside triangle
				bTriangleOK = false;
			}
			if ( ::TestPointTriangle( tempTriangle.B.start, itTri->A.start, itTri->B.start, itTri->C.start ) )
			{	// Point is inside triangle
				bTriangleOK = false;
			}
			if ( ::TestPointTriangle(tempTriangle.C.start, itTri->A.start, itTri->B.start, itTri->C.start ) )
			{	// Point is inside triangle
				bTriangleOK = false;
			}
		}

		vecTriangles.push_back( tempTriangle );
	}

	// Place these triangles into the vertex buffer...
	int numberOfTrianglesInBall = 20;
	if ( !LoadTrianglesAndCircleIntoBuffer( vecTriangles, numberOfTrianglesInBall, error ) )
	{
		MessageBox( NULL, error.c_str(), L"Error", MB_OK );
		return -1;
	}

	// Create a bunch of balls (circles) to simulate:
	int numberOfBalls = 11;
	for ( int count = 0; count != numberOfBalls; count++ )
	{
		CBall tempBall;
		float maxVelocity = 5.0f;
		float minRadius = 0.10f;
		float maxRadius = 0.50f;
		bool bBallOK = false;

		while ( ! bBallOK )
		{
			bBallOK = true;	// Assume the triangle is OK to start...

			tempBall.location.x = getRand< float >( ::g_WorldLimitLeft , ::g_WorldLimitRight );
			tempBall.location.y = getRand< float >( ::g_WorldLimitTop, ::g_WorldLimitTop );
			tempBall.velocity.x = getRand< float >( -maxVelocity, maxVelocity );
			tempBall.velocity.y = getRand< float >( -maxVelocity, maxVelocity );
			
			//
			tempBall.velocityOLD = tempBall.velocity;	// So doesn't go from 0.0 to something...
			tempBall.locationOLD = tempBall.location;	// So doesn't go from 0.0 to something...

			// Ball radius
			tempBall.radius = getRand< float > ( minRadius, maxRadius );
			
			// 
			// Now check to see if the ball is inside any of the triangles...
			std::vector<CTriangle>::iterator itTri;
			for	( itTri = vecTriangles.begin(); itTri != vecTriangles.end(); itTri++ )
			{
				// Newer, triangle-sphere test
				CPoint pClosest;
				if ( TestSphereTriangle( tempBall, itTri->A.start, itTri->B.start, itTri->C.start, pClosest ) )
				{	// Inside a triangle...
					bBallOK = false;	// mark the triangle as 'bad' so we can generate another
				}
			}
		}//while ( ! bTiangleOK )

		vecBalls.push_back( tempBall );
	}


	// Start the timer...
	::g_TheTimer.Start();
	
	// Main message loop
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
			// Collision Detection
			//CollisionDectection( vecTriangles, vecBalls );

			// Update simulation...
			SimulationStep( vecTriangles, vecBalls );

            //Render();
			RenderTrianglesAndBalls( vecTriangles, vecBalls );
        }
    }

	// Shut'er down!
	if( g_pd3dDevice ) g_pd3dDevice->ClearState();

    if( g_pRenderTargetView ) g_pRenderTargetView->Release();
    if( g_pSwapChain ) g_pSwapChain->Release();
    if( g_pd3dDevice ) g_pd3dDevice->Release();

    return ( int )msg.wParam;
}




