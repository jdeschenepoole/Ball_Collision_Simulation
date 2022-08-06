#ifndef _global_HG_
#define _global_HG_

#include <string>
#include "CObject.h"	// Add this...
#include <vector>		// Add this...
#include <windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <string>
#include "Physics.h"

// Added:
#include "Shapes.h"

#include "CHRTimer.h"

extern CHRTimer g_TheTimer;

class CObject;		// Forward declaration...

// Will contain all our objects...
extern std::vector<CObject> g_vecObjects;	// Add this...

extern HINSTANCE   g_hInst;	// = NULL;
extern HWND        g_hWnd;	// = NULL;
// DX variables...
extern ID3D10Device*           g_pd3dDevice;		// = NULL;
extern IDXGISwapChain*         g_pSwapChain;		// = NULL;
extern ID3D10RenderTargetView* g_pRenderTargetView;	// = NULL;

extern UINT g_windowWidth;
extern UINT g_windowHeight;

template <class T>
T getRand(T lowRange, T highRange)
{
	if ( lowRange > highRange )
	{
		T temp = lowRange;
		lowRange = highRange;
		highRange = temp;
	}
	T delta = highRange - lowRange;
	delta = ( static_cast<T>( rand() ) / static_cast<T>(RAND_MAX) ) * delta;
	return delta + lowRange;
}

// **********************************************
extern ID3D10Effect*               g_pEffect;				// = NULL;
extern ID3D10EffectTechnique*      g_pTechnique;			// = NULL;
extern ID3D10InputLayout*          g_pVertexLayout;		// = NULL;
//extern ID3D10Buffer*               g_pVertexBuffer;		// = NULL;
//extern ID3D10Buffer*               g_pIndexBuffer;			// = NULL;
extern ID3D10EffectMatrixVariable* g_pWorldVariable;		// = NULL;
extern ID3D10EffectMatrixVariable* g_pViewVariable;		// = NULL;
extern ID3D10EffectMatrixVariable* g_pProjectionVariable;	// = NULL;

// ADD Your bunny colour variable here...
extern ID3D10EffectVectorVariable*  g_pObjectColour;
extern D3DXVECTOR4					g_objectColour;

extern D3DXMATRIX                  g_World;
extern D3DXMATRIX                  g_View;
extern D3DXMATRIX                  g_Projection;
// **********************************************
extern ID3D10Texture2D*			g_pDepthStencilBuffer;
extern ID3D10DepthStencilView*	g_pDepthStencilView;
// **********************************************

bool InitDirectXAndCreateWindow( HINSTANCE hInstance, int nCmdShow, 
	                             std::wstring title, 
								 UINT width, UINT height, 
								 std::wstring &error );
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );

//void SimulationStep();	// Added Week 3, day 1
//void Render();

bool LoadAndPrepareShaderEffectFile( std::wstring shaderFileName, std::string effectName, std::wstring &error );

// **********************************************
bool LoadTrianglesAndCircleIntoBuffer( std::vector< CTriangle > &vecTriangles, int numberOfTrianglesInBall, std::wstring &error ); 
// This is now many triangles I need to draw in the ball vertex buffer
extern ID3D10Buffer*   g_pVertexBufferTriangles;// = NULL;
extern ID3D10Buffer*   g_pIndexBufferTriangles;// = NULL;
extern int			   g_sizeOfTriangleIndexBuffer;

extern ID3D10Buffer*	g_pVertexBufferBall;// = NULL;
extern ID3D10Buffer*	g_pIndexBufferBall;// = NULL;
extern int              g_sizeOfBallIndexBuffer;

void RenderTrianglesAndBalls( std::vector< CTriangle > &vecTriangles, std::vector< CBall > &vecBalls );
void SimulationStep( std::vector< CTriangle > &vecTriangles, std::vector< CBall > &vecBalls );
//void CollisionDectection ( std::vector< CTriangle > &vecTriangles, std::vector< CBall > &vecBalls );
//bool Between(float v1, float v2, float b);

extern float g_WorldLimitLeft;
extern float g_WorldLimitRight;
extern float g_WorldLimitTop;
extern float g_WorldLimitBottom;

// **********************************************

// **********************************
struct SimpleVertex
{
    D3DXVECTOR3 Pos;
    D3DXVECTOR4 Color;
};
// **********************************
#endif 




















































// What are you looking here, for? Really? There's nothing for you to copy and paste here...

// extern ID3D10EffectVectorVariable* g_pBunnyColourVariable;// = NULL;