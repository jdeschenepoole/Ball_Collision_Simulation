#include "global.h"

void RenderTrianglesAndBalls( std::vector< CTriangle > &vecTriangles, std::vector< CBall > &vecBalls )
{

	// Just clear the backbuffer
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha
    g_pd3dDevice->ClearRenderTargetView( g_pRenderTargetView, ClearColor );


    // Clear the depth buffer to 1.0 (max depth)
    g_pd3dDevice->ClearDepthStencilView( g_pDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0 );

    // Initialize the view matrix
    D3DXVECTOR3 Eye( 0.0f, 0.0f, -4.0f );
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

	// Initialize the world matrix
	D3DXMatrixIdentity( &g_World );

	g_pWorldVariable->SetMatrix( ( float* )&g_World );

	// Point the renderer to the appropriate vertex 
	//	and index buffer
	UINT stride = sizeof( SimpleVertex );
	UINT offset = 0;
	g_pd3dDevice->IASetVertexBuffers( 0, 1, &g_pVertexBufferTriangles, &stride, &offset );
	g_pd3dDevice->IASetIndexBuffer( g_pIndexBufferTriangles, DXGI_FORMAT_R32_UINT, 0 );

	// Set primitive topology
	g_pd3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	D3D10_TECHNIQUE_DESC techDesc;	
	g_pTechnique->GetDesc( &techDesc );
	
	for( UINT p = 0; p < techDesc.Passes; ++p )
	{
		g_pTechnique->GetPassByIndex( p )->Apply( 0 );
		g_pd3dDevice->DrawIndexed( g_sizeOfTriangleIndexBuffer, 
								   0,
								   0 );        
	}

	// Now we draw the circles...(from the other buffer)
	g_pd3dDevice->IASetVertexBuffers( 0, 1, &g_pVertexBufferBall, &stride, &offset );
	g_pd3dDevice->IASetIndexBuffer( g_pIndexBufferBall, DXGI_FORMAT_R32_UINT, 0 );


	for ( std::vector< CBall >::iterator itBall = vecBalls.begin(); itBall != vecBalls.end(); itBall++ )
	{
		// Initialize the world matrix
		D3DXMatrixIdentity( &g_World );

		::D3DXMATRIX matTranslate;
		::D3DXMATRIX matScale;
		::D3DXMATRIX matFinal;	// Final result of all the mults..

		D3DXMatrixIdentity( &matTranslate );
		D3DXMatrixIdentity( &matScale );
		D3DXMatrixIdentity( &matFinal );

		// Translation matrix...
		D3DXMatrixTranslation( &matTranslate, 
			                   itBall->location.x, itBall->location.y, 0.0f );

		D3DXMatrixScaling( &matScale, itBall->radius, itBall->radius, 1.0f );

		// Combine...
		::D3DXMatrixMultiply( &matFinal, &matFinal, &matScale );
		::D3DXMatrixMultiply( &matFinal, &matFinal, &matTranslate );

		// Apply this object matrix to the world transform...
		::D3DXMatrixMultiply( &g_World, &g_World, &matFinal );

		g_pWorldVariable->SetMatrix( ( float* )&g_World );		

		for( UINT p = 0; p < techDesc.Passes; ++p )
		{
			g_pTechnique->GetPassByIndex( p )->Apply( 0 );
			g_pd3dDevice->DrawIndexed( g_sizeOfBallIndexBuffer, 
									   0,
									   0 );        
		}
	}//for ( std::vector< CBall >::iterator itBall ...

    g_pSwapChain->Present( 0, 0 );
}