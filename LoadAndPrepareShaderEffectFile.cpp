#include "global.h"

bool LoadAndPrepareShaderEffectFile( std::wstring shaderFileName, std::string effectName, std::wstring &error )
{
	error = L"OK";
	HRESULT hr = S_OK;

    // Create the effect
    DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3D10_SHADER_DEBUG flag if in debug mode
    dwShaderFlags |= D3D10_SHADER_DEBUG;
    #endif
    hr = D3DX10CreateEffectFromFile( shaderFileName.c_str(), 
		                             NULL, 
									 NULL, 
									 "fx_4_0", 
									 dwShaderFlags, 
									 0, 
									 g_pd3dDevice, 
									 NULL,
									 NULL, 
									 &g_pEffect, 
									 NULL, 
									 NULL );
    if( FAILED( hr ) )
    {
		error = L"ERROR: Can't find the shader (fx) file.";
        return false;
    }

    // Obtain the technique
    g_pTechnique = g_pEffect->GetTechniqueByName( effectName.c_str() );

    // Obtain the variables
    g_pWorldVariable = g_pEffect->GetVariableByName( "World" )->AsMatrix();
    g_pViewVariable = g_pEffect->GetVariableByName( "View" )->AsMatrix();
    g_pProjectionVariable = g_pEffect->GetVariableByName( "Projection" )->AsMatrix();

	g_pObjectColour = g_pEffect->GetVariableByName( "objectColour" )->AsVector();
	// Prepare the effect...
	
	// Define the input layout
    D3D10_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = sizeof( layout ) / sizeof( layout[0] );

    // Create the input layout
    D3D10_PASS_DESC PassDesc;
    g_pTechnique->GetPassByIndex( 0 )->GetDesc( &PassDesc );
    hr = g_pd3dDevice->CreateInputLayout( layout, numElements, PassDesc.pIAInputSignature,
                                          PassDesc.IAInputSignatureSize, &g_pVertexLayout );
    if( FAILED( hr ) )
	{
		error = L"ERORR: Unable to set the shader pass layout";
		return false;
	}

	// Add code to get at the variable inside the shader...
	// (sort of like you are 'pointing' to the variable...


	return true;
}

















































































// You aren't supposed to be all the way down here. 
// Don't you have better things to do that snoop around for a secret
//	copy and paste area? Really? 
//
// Well, I guess it paid off...

	//// Get the bunny colour...
	//::g_pBunnyColourVariable = g_pEffect->GetVariableByName( "BunnyColour" )->AsVector();
