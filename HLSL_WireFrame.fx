//--------------------------------------------------------------------------------------
// File: Tutorial04.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
matrix World;
matrix View;
matrix Projection;


//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( float4 Pos : POSITION, float4 Color : COLOR )
{
    VS_OUTPUT output = (VS_OUTPUT)0;

	// Combine the matrices first...
	matrix matFinal = mul( World, View );
	matFinal = mul( matFinal, Projection );

	output.Pos = Pos;
	// 
	output.Pos = mul( output.Pos, matFinal );

	//output.Pos = mul( Pos, World );
    //output.Pos = mul( output.Pos, View );
    //output.Pos = mul( output.Pos, Projection );

    output.Color = Color;

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
    return input.Color;

	//return objectColour;
}

RasterizerState WireFrameNoCull
{
    //FillMode = Solid;
	FillMode = WireFrame;
	CullMode = None;			// Front AND back facing triangles
};

//--------------------------------------------------------------------------------------
technique10 Render
{
    pass P0
    {
		SetRasterizerState( WireFrameNoCull );
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}




































// Top secret Area-51 copy and paste area....


// *** From the variable declaration part...
//float4 BunnyColour;


// *** From the vertex shader....
//output.Color = BunnyColour;