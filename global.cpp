#include "global.h"

// High Res timer
CHRTimer g_TheTimer;

// Add this...
std::vector<CObject> g_vecObjects;

HINSTANCE   g_hInst = NULL;
HWND        g_hWnd = NULL;
// DX variables...
ID3D10Device*           g_pd3dDevice = NULL;
IDXGISwapChain*         g_pSwapChain = NULL;
ID3D10RenderTargetView* g_pRenderTargetView = NULL;

UINT g_windowWidth = 0;
UINT g_windowHeight = 0;

// **********************************************
ID3D10Effect*               g_pEffect = NULL;
ID3D10EffectTechnique*      g_pTechnique = NULL;
ID3D10InputLayout*          g_pVertexLayout = NULL;

ID3D10EffectMatrixVariable* g_pWorldVariable = NULL;
ID3D10EffectMatrixVariable* g_pViewVariable = NULL;
ID3D10EffectMatrixVariable* g_pProjectionVariable = NULL;
// ADD your variable to change the colour of the bunny here...
// ADD Your bunny colour variable here...
ID3D10EffectVectorVariable* g_pObjectColour;
D3DXVECTOR4					g_objectColour;


D3DXMATRIX                  g_World;
D3DXMATRIX                  g_View;
D3DXMATRIX                  g_Projection;
// **********************************************
ID3D10Texture2D*			g_pDepthStencilBuffer = NULL;
ID3D10DepthStencilView*		g_pDepthStencilView = NULL;


// *************************************************
// Added:
ID3D10Buffer*   g_pVertexBufferTriangles = NULL;
ID3D10Buffer*   g_pIndexBufferTriangles = NULL;
int             g_sizeOfTriangleIndexBuffer = 0;

ID3D10Buffer*	g_pVertexBufferBall = NULL;
ID3D10Buffer*	g_pIndexBufferBall = NULL;
int             g_sizeOfBallIndexBuffer = 0;

float g_WorldLimitLeft = -5.0f;
float g_WorldLimitRight = 5.0f;
float g_WorldLimitTop = 4.0f;
float g_WorldLimitBottom = -4.0f;




























































// Shh! You didn't see this secret copy and paste area! Tell NO ONE!!!
//
// ID3D10EffectVectorVariable* g_pBunnyColourVariable;// = NULL;