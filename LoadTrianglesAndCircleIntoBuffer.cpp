#include "global.h"

// This copies the triangles into the vertex buffer. 
// Note: In the render, the size of the vecTriangles indicates how 
//       far into the vertex buffer we draw
// The numberOfTrianglesInBall is used to calculate how "smooth"
//  the ball will be (more triangles = smoother ball)

bool LoadTrianglesAndCircleIntoBuffer( std::vector< CTriangle > &vecTriangles, int numberOfTrianglesInBall, std::wstring &error )
{
	HRESULT hr = S_OK;
	error = L"OK";

	// We are going to set the colour of the triangles and balls here...
	SimpleVertex tempTriVertex;
	tempTriVertex.Color.x = 0.0f;
	tempTriVertex.Color.y = 1.0f;	// Green triangles.
	tempTriVertex.Color.z = 0.0f;
	tempTriVertex.Color.w = 1.0f;

	SimpleVertex tempBallVertex;
	tempBallVertex.Color.x = 1.0f;	// Red ball.
	tempBallVertex.Color.y = 0.0f;
	tempBallVertex.Color.z = 0.0f;
	tempBallVertex.Color.w = 1.0f;	

	g_pd3dDevice->IASetInputLayout( g_pVertexLayout );

	// NOTE: There is a little fidling that we have to do because
	//	this list of triangles isn't indexed as a regularly
	//	indexed model would be. In essence, there isn't 
	//	much point to using an index buffer here, but we are 
	//	because virtually all models are indexed.	
	
	// Create a buffer big enough to hold all the triangles...
	// (the "* 2" is to make sure the buffer is more than big enough)
	g_sizeOfTriangleIndexBuffer = vecTriangles.size() * 3;
	int numberOfVerticesInBuffer = g_sizeOfTriangleIndexBuffer * 2;
	SimpleVertex* vertices = new SimpleVertex[ numberOfVerticesInBuffer ];
	memset( vertices, 0, numberOfVerticesInBuffer * sizeof( SimpleVertex ));

	// Note: Here, the number of elements is the same as the number
	//	of vertices (each index = a vertex)
	DWORD* indices = new DWORD[ numberOfVerticesInBuffer ];
	// Clear the buffer.
	memset( indices, 0, numberOfVerticesInBuffer * sizeof(DWORD) );

	// Keep these to point to where in the index and vertex 
	//	buffers we are pointing...
	int vertexOffset = 0;
	int indexOffset = 0;

	// First step: load all the vertices into the vertex buffer...
	std::vector< CTriangle >::iterator itTriangle;
	for ( itTriangle = vecTriangles.begin(); itTriangle != vecTriangles.end(); itTriangle++ )
	{	
		// Note that we are only using the "start" locations of the lines
		
		// Here's how this works:
		// The vertex buffer is loaded with the three points
		//	that make up the triangle. 
		// At the same time, we are loading the index buffer 
		//	with the locations of these vertices in the vertex buffer.
		// 
		// Huh? 
		// 
		// For example, let's look at the first triangle.
		// 
		// It has three points, A, B, C, each with an x & y location.
		// Since these are only 2D, we set the z = 0.0f;
		// 
		// We load point A into "vertexOffset" (the "zeroth" location)
		// So the INDEX location is zero (0).
		// 
		// Next, we load point B into "vertexOffset + 1" 
		//	(location 1 in the vertex array)
		// So we load the index buffer with 1
		// 
		// Next, point C into "vertexOffset + 2" and index location 2.
		// 
		// Then we increment the "vertexOffset" by three and repeat
		//	for the next triangle. 
		// Note that the index locations for these three vertices
		//	will be 3, 4, and 5. 
		
		vertices[ vertexOffset ].Pos.x = itTriangle->A.start.x;
		vertices[ vertexOffset ].Pos.y = itTriangle->A.start.y;
		vertices[ vertexOffset ].Pos.z = 0.0f;	// We aren't using z...
		vertices[ vertexOffset ].Color = tempTriVertex.Color;

		vertices[ vertexOffset + 1 ].Pos.x = itTriangle->B.start.x;
		vertices[ vertexOffset + 1 ].Pos.y = itTriangle->B.start.y;
		vertices[ vertexOffset + 1 ].Pos.z = 0.0f;
		vertices[ vertexOffset + 1 ].Color = tempTriVertex.Color;

		vertices[ vertexOffset + 2 ].Pos.x = itTriangle->C.start.x;
		vertices[ vertexOffset + 2 ].Pos.y = itTriangle->C.start.y;
		vertices[ vertexOffset + 2 ].Pos.z = 0.0f;
		vertices[ vertexOffset + 2 ].Color = tempTriVertex.Color;
		
		// Now the index locations...
		indices[ indexOffset ] = vertexOffset;
		indices[ indexOffset + 1 ] = vertexOffset + 1;
		indices[ indexOffset + 2 ] = vertexOffset + 2;
		
		// Now we increment the offsets by three 
		// (three vertices and three indices)
		vertexOffset += 3;
		indexOffset += 3;
	}

	// Now copy these arrays into the DirectX vertex and index buffers
    D3D10_BUFFER_DESC bd;
    bd.Usage = D3D10_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( SimpleVertex ) * numberOfVerticesInBuffer;
    bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = vertices;
    hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &g_pVertexBufferTriangles );
    if( FAILED( hr ) )
	{
		error = L"ERROR: Unable to create vertex buffer for triangle list.";
		return false;
	}

	bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( DWORD ) * numberOfVerticesInBuffer;       
    bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    InitData.pSysMem = indices;
    hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &g_pIndexBufferTriangles );
    if( FAILED( hr ) )
	{
		error = L"ERROR: Unable to create index buffer for triangle list.";
		return false;
	}

	// Get rid of the arrays...
	delete [] vertices;
	delete [] indices;

	// **************************************************
	// Now we do the same sort of thing for the circle...
	// **************************************************

	// The circle is a set of triangles with the "top"
	//	of the triangle in the centre and the "bottom" 
	//	edge of the triangle arranged around the outside.
	// Think of a pizza, where you cut off the "rounded"
	//	part of each slice to make a bunch of triangles. 
	// The more "slices" you have, the more rounded 
	//	the circle is. 

	// x = r*cos(t)
	// y = r*sin(t)
	// 
	// Where t is the angle around the circle
	float PI = 3.141597f;
	float TWOPI = 2.0f * PI;
	float stepAngle = TWOPI / numberOfTrianglesInBall;

	std::vector< CTriangle > vecCircle;

	for ( float angle = 0.0f; angle <= TWOPI; angle += stepAngle )
	{
		CTriangle tempTriangle;
		// "Top" of triangle is at centre...
		tempTriangle.A.start.x = 0.0f;
		tempTriangle.A.start.y = 0.0f;

		// "Bottom" is near edge of circle...
		tempTriangle.B.start.x = (float)cos( angle );
		tempTriangle.B.start.y = (float)sin( angle );
		
		tempTriangle.C.start.x = (float)cos( angle + stepAngle );
		tempTriangle.C.start.y = (float)sin( angle + stepAngle );

		tempTriangle.A.end = tempTriangle.B.start;
		tempTriangle.B.end = tempTriangle.C.start;
		tempTriangle.C.end = tempTriangle.A.start;

		vecCircle.push_back( tempTriangle );

	}

	// Make the actual buffer 2x bigger just in case
	g_sizeOfBallIndexBuffer = vecCircle.size() * 3;
	numberOfVerticesInBuffer = g_sizeOfBallIndexBuffer * 2;
	//SimpleVertex* 
	vertices = new SimpleVertex[ numberOfVerticesInBuffer ];
	memset( vertices, 0, numberOfVerticesInBuffer * sizeof( SimpleVertex ));

	// Note: Here, the number of elements is the same as the number
	//	of vertices (each index = a vertex)
	//DWORD* 
	indices = new DWORD[ numberOfVerticesInBuffer ];
	// Clear the buffer.
	memset( indices, 0, numberOfVerticesInBuffer * sizeof(DWORD) );

	// Keep these to point to where in the index and vertex 
	//	buffers we are pointing...
	vertexOffset = 0;
	indexOffset = 0;

	// First step: load all the vertices into the vertex buffer...
	//std::vector< CTriangle >::iterator itTriangle;
	for ( itTriangle = vecCircle.begin(); itTriangle != vecCircle.end(); itTriangle++ )
	{			
		vertices[ vertexOffset ].Pos.x = itTriangle->A.start.x;
		vertices[ vertexOffset ].Pos.y = itTriangle->A.start.y;
		vertices[ vertexOffset ].Pos.z = 0.0f;	// We aren't using z...
		vertices[ vertexOffset ].Color = tempBallVertex.Color;

		vertices[ vertexOffset + 1 ].Pos.x = itTriangle->C.start.x;
		vertices[ vertexOffset + 1 ].Pos.y = itTriangle->C.start.y;
		vertices[ vertexOffset + 1 ].Pos.z = 0.0f;
		vertices[ vertexOffset + 1 ].Color = tempBallVertex.Color;

		vertices[ vertexOffset + 2 ].Pos.x = itTriangle->B.start.x;
		vertices[ vertexOffset + 2 ].Pos.y = itTriangle->B.start.y;
		vertices[ vertexOffset + 2 ].Pos.z = 0.0f;
		vertices[ vertexOffset + 2 ].Color = tempBallVertex.Color;
		
		// Now the index locations...
		indices[ indexOffset ] = vertexOffset;
		indices[ indexOffset + 1 ] = vertexOffset + 1;
		indices[ indexOffset + 2 ] = vertexOffset + 2;
		
		// Now we increment the offsets by three 
		// (three vertices and three indices)
		vertexOffset += 3;
		indexOffset += 3;
	}

	// Now copy these arrays into the DirectX vertex and index buffers
    //D3D10_BUFFER_DESC 	bd;
    bd.Usage = D3D10_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( SimpleVertex ) * numberOfVerticesInBuffer;
    bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    //D3D10_SUBRESOURCE_DATA 
	InitData;
    InitData.pSysMem = vertices;
    hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &g_pVertexBufferBall );
    if( FAILED( hr ) )
	{
		error = L"ERROR: Unable to create vertex buffer for circle.";
		return false;
	}

	bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( DWORD ) * numberOfVerticesInBuffer;       
    bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    InitData.pSysMem = indices;
    hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &g_pIndexBufferBall );
    if( FAILED( hr ) )
	{
		error = L"ERROR: Unable to create index buffer for circle";
		return false;
	}

	// Get rid of the arrays...
	delete [] vertices;
	delete [] indices;


	return true;
}
