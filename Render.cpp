#include "global.h"

static int totalIndicesToDraw = 3;

void Render()	// 60Hz because of the SWAPCHAIN setttings...
{


    // Just clear the backbuffer
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha
    g_pd3dDevice->ClearRenderTargetView( g_pRenderTargetView, ClearColor );


    // Clear the depth buffer to 1.0 (max depth)
    g_pd3dDevice->ClearDepthStencilView( g_pDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0 );


    // Initialize the view matrix
    D3DXVECTOR3 Eye( 0.0f, 1.0f, -4.0f );
    D3DXVECTOR3 At( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 Up( 0.0f, 1.0f, 0.0f );

	D3DXMatrixLookAtLH( &g_View, &Eye, &At, &Up );
	// ********************************************
    g_pViewVariable->SetMatrix( ( float* )&g_View );




    // Initialize the projection matrix
    D3DXMatrixPerspectiveFovLH( &g_Projection, 
							    static_cast<float>(D3DX_PI) * 0.5f, 
		                        static_cast<float>(g_windowWidth) / static_cast<float>(g_windowHeight), 
								0.1f, 100.0f );
    g_pProjectionVariable->SetMatrix( ( float* )&g_Projection );

	// ADD loop to draw all our objects....

	std::vector<CObject>::iterator itObjects;
	for ( itObjects = g_vecObjects.begin(); 
		  itObjects != g_vecObjects.end(); itObjects++ )
	{
		// Initialize the world matrix
		D3DXMatrixIdentity( &g_World );

		// Take the data from the Object iterator and load 
		//	a bunch of matrices with those values and set the 
		//	world transform...
		::D3DXMATRIX matTranslate;
		::D3DXMATRIX matRotateX, matRotateY, matRotateZ;
		// Also: D3DXMATRIX matScale... (don't use yet)
		::D3DXMATRIX matFinal;	// Final result of all the mults..

		D3DXMatrixIdentity( &matTranslate );	// 'clear' 
		D3DXMatrixIdentity( &matRotateX );
		D3DXMatrixIdentity( &matRotateY );
		D3DXMatrixIdentity( &matRotateZ );


		// Translation matrix...
		D3DXMatrixTranslation( &matTranslate, 
			                   itObjects->PosX, 
							   itObjects->PosY, 
							   itObjects->PosZ );
		// Three rotational matrices...
		D3DXMatrixRotationX( &matRotateX, itObjects->RotX );
		D3DXMatrixRotationY( &matRotateY, itObjects->RotY );
		D3DXMatrixRotationZ( &matRotateZ, itObjects->RotZ );

		::D3DXMatrixIdentity( &matFinal );


		D3DXMATRIX matScale;
		D3DXMatrixIdentity( &matScale );
		::D3DXMatrixScaling( &matScale, 0.01f, 0.01f, 0.01f );
		::D3DXMatrixMultiply( &matFinal, &matFinal, &matScale );

		
		::D3DXMatrixMultiply( &matFinal, &matFinal, &matRotateX );
		::D3DXMatrixMultiply( &matFinal, &matFinal, &matRotateY );
		::D3DXMatrixMultiply( &matFinal, &matFinal, &matRotateZ );

		::D3DXMatrixMultiply( &matFinal, &matFinal, &matTranslate );

		

		// Apply this object matrix to the world transform...
		::D3DXMatrixMultiply( &g_World, &g_World, &matFinal );

		g_pWorldVariable->SetMatrix( ( float* )&g_World );

		// Point the renderer to the appropriate vertex 
		//	and index buffer
		UINT stride = sizeof( SimpleVertex );
		UINT offset = 0;
		g_pd3dDevice->IASetVertexBuffers( 0, 1, &g_pVertexBuffer, &stride, &offset );
	    g_pd3dDevice->IASetIndexBuffer( g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

		// Set primitive topology
		g_pd3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		// ***********************************************************************
		// Insert amazing code to change the colour of the bunny in the shader...
		g_objectColour.x = itObjects->red;		// 1.0f;	// Red
		g_objectColour.y = itObjects->green;	// 0.0f;	// Green
		g_objectColour.z = itObjects->blue;		// 0.0f;	// Blue
		g_objectColour.w = 1.0f;
		// ***********************************************************************
		// Update corresponding variable inside shader
		// (i.e. what the g_pObjectColour is pointing to)
		// Like this:
		// 		g_pWorldVariable->SetMatrix( ( float* )&g_World );
		// 
		::g_pObjectColour->SetFloatVector( (float*) &g_objectColour );

		D3D10_TECHNIQUE_DESC techDesc;	
		g_pTechnique->GetDesc( &techDesc );
	
		for( UINT p = 0; p < techDesc.Passes; ++p )
		{
			g_pTechnique->GetPassByIndex( p )->Apply( 0 );
			g_pd3dDevice->DrawIndexed( 1024 * 3, 
									0,
									0 );        // 36 vertices needed for 12 triangles in a triangle list
		}
	}//for ( itObjects = g_vecObjects.begin(); 

    g_pSwapChain->Present( 0, 0 );
}

























































// Top secret copy and paste area - SHHH!! Don't tell anyone!!

		////// Set colour of bunny in shader...
		////D3DXVECTOR4 bunnycolour( itObjects->red, itObjects->green, itObjects->blue, 1.0f );

		////::g_pBunnyColourVariable->SetFloatVector( (float*) bunnycolour );