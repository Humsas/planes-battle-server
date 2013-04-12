// Data structures:
struct MATERIAL
{
	float4	ambient;
	float4	diffuse;
	float4	specular;
	float	intensity;
};

struct LIGHT
{
	float4	ambient;
	float4	diffuse;
	float4	specular;
	float3	direction;
};

struct FOG
{
	float3	color;
	float	start;
	float	range;
	float	intensity;
};

// Global variables:
float		g_fTime : TIME;
float3		g_f3EyePos; // Position of camera.
float4x4	g_FXF[35];  // Array of bone matrices.
float4x4	g_W : WORLD;
float4x4	g_WVP : WORLDVIEWPROJECTION;
float4x4	g_WIT : WORLDINVERSETRANSPOSE;
MATERIAL	g_Material;
LIGHT		g_Light;
FOG			g_Fog;
texture		g_Texture0;
texture		g_Texture1;
texture		g_Texture2;
texture		g_Texture3;
texture		g_Texture4;
texture		g_TextureSky;
float2		g_f2Offset2D0;
float2		g_f2Offset2D1;

sampler TextureSampler0 = sampler_state
{
	Texture = <g_Texture0>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

sampler TextureSampler1 = sampler_state
{
	Texture = <g_Texture1>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

sampler TextureSampler2 = sampler_state
{
	Texture = <g_Texture2>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

sampler TextureSampler3 = sampler_state
{
	Texture = <g_Texture3>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

sampler TextureSampler4 = sampler_state
{
	Texture = <g_Texture4>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

sampler TextureSamplerSky = sampler_state
{
	Texture = <g_TextureSky>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

static float3	g_fFogColor = {0.5f, 0.5f, 0.5f};
static float	g_fFogStart = 1.0f;
static float	g_fFogRange = 250.0f;

//------------------------------------------------------------------
//	Shader: Basic
//	Desc: basic methods and data structures required
//	for shader to work.
//------------------------------------------------------------------

struct VS_INPUT
{
	float3 f3Pos		: POSITION;
	float3 f3Normal		: NORMAL0;
	float2 f2Tex		: TEXCOORD;	
};

struct VS_OUTPUT
{
	float4 f4Pos		: POSITION;
	float4 f4Normal		: TEXCOORD1;
	float2 f2Tex		: TEXCOORD2;
};

struct PS_INPUT
{
	float4 f4Pos		: TEXCOORD0;
	float4 f4Normal		: TEXCOORD1;
	float2 f2Tex		: TEXCOORD2;
};

float4 PS (PS_INPUT data) : COLOR
{	
	// Color value.
	float3 f3Color = tex2D(TextureSampler0, data.f2Tex).rgb;
	return float4(f3Color, 1.0f);
}

float fnFog (float3 f3Pos)
{
	float fDist = distance(f3Pos, g_f3EyePos);
	return saturate((fDist - g_Fog.start) / g_Fog.range * g_Fog.intensity);
}

//------------------------------------------------------------------
//	Shader: Sky
//	Desc: handles sky.
//------------------------------------------------------------------

struct VS_SKYINPUT
{
	float3 f3Pos	: POSITION0;
};

struct VS_SKYOUTPUT
{
	float4 f4Pos	: POSITION0;
	float3 f3Tex	: TEXCOORD0;
};

VS_SKYOUTPUT VSSky (VS_SKYINPUT data)
{
	VS_SKYOUTPUT output = (VS_SKYOUTPUT)0;
	
	// Set z to w so that z/w would be equal to 1,
	// thus skydome always on far plane.
	output.f4Pos = mul(float4(data.f3Pos, 1.0f), g_WVP).xyww;
	
	// Use mesh vertex position in local space, as index into cubemap.
	output.f3Tex = data.f3Pos;
	
	return output;
}

struct PS_SKYINPUT
{
	float3 f3Tex	: TEXCOORD0;	
};

float4 PSSky (PS_SKYINPUT data) : COLOR
{
	return texCUBE(TextureSamplerSky, data.f3Tex);
}

technique Sky
{
	pass P0
	{
		VertexShader = compile vs_2_0 VSSky();
		PixelShader = compile ps_2_0 PSSky();
		
		CullMode = None;
		//ZFunc = Always; // Always writes sky into depth buffer;
		//StencilEnable = true;
		//StencilFunc = Always;
		//StencilPass = Replace;
		//StencilRef = 0; // Clear to zero.
	}
}


//------------------------------------------------------------------
//	Shader: Character
//	Desc: handles movement and other character object attributtes.
//------------------------------------------------------------------

struct VS_CHARACTERINPUT
{
	float3 f3Pos		: POSITION;
	float3 f3Normal		: NORMAL;
	float2 f2Tex		: TEXCOORD;
	float fWeight		: BLENDWEIGHT;
	int4 i4Indices		: BLENDINDICES;
};

VS_OUTPUT VSCharacter (VS_CHARACTERINPUT data)
{
	// Initialize default values.
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	// Calculate other weight.
	float fWeight2 = 1.0f - data.fWeight;
	
	// Calculate new vertex position.
	float4 f4Pos = data.fWeight * mul(float4(data.f3Pos, 1.0f), g_FXF[data.i4Indices[0]]);
	f4Pos += fWeight2 * mul(float4(data.f3Pos, 1.0f), g_FXF[data.i4Indices[1]]);
	f4Pos.w = 1.0f;
	
	// Recalculate normals.
	float4 f4Normal = data.fWeight * mul(float4(data.f3Normal, 1.0f), g_FXF[data.i4Indices[0]]);
	f4Normal += fWeight2 * mul(float4(data.f3Normal, 1.0f), g_FXF[data.i4Indices[1]]);
	f4Normal.w = 1.0f;
	
	// Transform vertex from object space to projection.
	output.f4Pos = mul(f4Pos, g_WVP);
	
	// Normalize normal.
	output.f4Normal = normalize(f4Normal);
	
	// Pass given texture coordinates.
	output.f2Tex = data.f2Tex;
	
	// Return the vertex after all calculations.
	return output;
}

technique Character
{
	pass P0
	{
		VertexShader = compile vs_2_0 VSCharacter();
		PixelShader = compile ps_2_0 PS();
	}
}

//------------------------------------------------------------------
//	Shader: Terrain
//	Desc: handles terrain.
//------------------------------------------------------------------

static float g_fTexScale = 64.0f;

struct VS_TERRAINOUTPUT
{
	float4 f4Pos		: POSITION0;
	float2 f2Tex		: TEXCOORD0;
	float2 f2TexTiled	: TEXCOORD1;
	float fShade		: TEXCOORD2;
	float fFog			: TEXCOORD3;
};

VS_TERRAINOUTPUT VSTerrain (VS_INPUT data)
{
	// Initialize default values.
	VS_TERRAINOUTPUT output = (VS_TERRAINOUTPUT)0;
	
	// Transform vertex from object space to projection.
	output.f4Pos = mul(float4(data.f3Pos, 1.0f), g_WVP);
	
	// Pass given original and tiled texture coordinates.
	output.f2Tex = data.f2Tex;
	output.f2TexTiled = data.f2Tex * g_fTexScale;
	
	// Calculate shade.
	output.fShade = saturate(max(dot(-g_Light.direction, data.f3Normal), 0.0f) + 0.25f);
	
	// Compute fog.
	output.fFog = fnFog(data.f3Pos);
	
	// Return the vertex after all calculations.
	return output;
}

struct PS_TERRAININPUT
{
	float2 f2Tex		: TEXCOORD0;
	float2 f2TexTiled	: TEXCOORD1;
	float fShade		: TEXCOORD2;
	float fFog			: TEXCOORD3;
};

float4 PSTerrain (PS_TERRAININPUT data) : COLOR
{
	
	// Take color information from tiled textures.
	float3 f3Color0 = tex2D(TextureSampler0, data.f2TexTiled).rgb;
	float3 f3Color1 = tex2D(TextureSampler1, data.f2TexTiled).rgb;
	float3 f3Color2 = tex2D(TextureSampler2, data.f2TexTiled).rgb;
	// Take color information from not tiled blend map.
	float3 f3Color3 = tex2D(TextureSampler3, data.f2Tex).rgb;

	// Find the inverse of all the blend weights.
	float fTotalInverse = 1.0f / (f3Color3.r + f3Color3.g + f3Color3.b);

	// Scale colors by its corresponding weight.
	f3Color0 *= f3Color3.r * fTotalInverse;
	f3Color1 *= f3Color3.g * fTotalInverse;
	f3Color2 *= f3Color3.b * fTotalInverse;
	
	// Calculate and return final color value.
	float3 f3Color = (f3Color0 + f3Color1 + f3Color2) * data.fShade;
	
	// Compute fog.
	float3 f3FinalColor = lerp(f3Color, g_Fog.color, data.fFog);
	
	return float4(f3FinalColor, 1.0f);
}

technique Terrain
{
	pass P0
	{
		//FillMode = Wireframe;
		VertexShader = compile vs_2_0 VSTerrain();
		PixelShader = compile ps_2_0 PSTerrain();
	}
}

//------------------------------------------------------------------
//	Shader: Object
//	Desc: handles simple objects.
//------------------------------------------------------------------

struct VS_OBJECTOUTPUT
{
	float4 f4Pos		: POSITION0;
	float4 f4DiffAmb	: COLOR0;
	float4 f4Spec		: COLOR1;
	float2 f2Tex		: TEXCOORD0;
	float fFog			: TEXCOORD1;
};

VS_OBJECTOUTPUT VSObject (VS_INPUT data)
{	
	// Initialize default values.
	VS_OBJECTOUTPUT output = (VS_OBJECTOUTPUT)0;
	
	// Transform vertex from object space to projection.
	output.f4Pos = mul(float4(data.f3Pos, 1.0f), g_WVP);
	
	float3 f3Pos = mul(float4(data.f3Pos, 1.0f), g_W);
	
	// Transform normal to world space.
	float3 f3Normal = mul(float4(data.f3Normal, 0.0f), g_WIT).xyz;
	f3Normal = normalize(f3Normal);
	
	// Calculate diffuse and ambient color intensity.
	float fDirectional = max(dot(-g_Light.direction, f3Normal), 0.0f);
	float3 f3Diff = fDirectional * (g_Material.diffuse * g_Light.diffuse).rgb;
	float3 f3Amb = g_Material.ambient * g_Light.ambient;
	output.f4DiffAmb.rgb = f3Diff + f3Amb;
	output.f4DiffAmb.a = g_Material.diffuse.a;
	
	// Calculate specular intensity.
	float3 f3View = normalize(g_f3EyePos - f3Pos);
	float3 f3Reflection = reflect(g_Light.direction, f3Normal);
	float fSpecular = pow(max(dot(f3View, f3Reflection), 0.0f), g_Material.intensity);
	float3 f3Spec = fSpecular * (g_Material.specular * g_Light.specular).rgb;
	output.f4Spec = float4(f3Spec, 1.0f);	
	
	// Pass given texture coordinates.
	output.f2Tex = data.f2Tex;
	
	// Compute fog.
	output.fFog = fnFog(f3Pos);
	
	// Return the data after all calculations.
	return output;
}

struct PS_OBJECTINPUT
{
	float4 f4DiffAmb	: COLOR0;
	float4 f4Spec		: COLOR1;
	float2 f2Tex		: TEXCOORD0;
	float fFog			: TEXCOORD1;
};

float4 PSObject (PS_OBJECTINPUT data) : COLOR
{	
	// Color value.
	float4 f4TexColor = tex2D(TextureSampler0, data.f2Tex);
	
	float3 f3Color = f4TexColor.rgb * data.f4DiffAmb.rgb + data.f4Spec.rgb;
	
	// Compute fog.
	float3 f3FinalColor = lerp(f3Color, g_Fog.color, data.fFog);
	
	return float4(f3FinalColor, f4TexColor.a * data.f4DiffAmb.a);
}

technique Object
{
	pass P0
	{
		VertexShader = compile vs_2_0 VSObject();
		PixelShader = compile ps_2_0 PSObject();	
	}
}

//------------------------------------------------------------------
//	Shader: Water
//	Desc: creates water effect.
//------------------------------------------------------------------

static float g_fReflectivity = 0.5f;

struct VS_WATEROUTPUT
{
	float4 f4Pos : POSITION;
};

VS_WATEROUTPUT VSWater (VS_INPUT data)
{	
	// Initialize default values.
	VS_WATEROUTPUT output = (VS_WATEROUTPUT)0;
	
	output.f4Pos = mul(float4(data.f3Pos, 1.0f), g_WVP);
	
	return output;
}

struct PS_WATERINPUT
{
};
	
float4 PSWater (PS_WATERINPUT data) : COLOR
{		
	return float4(g_Material.diffuse.rgb, g_Material.diffuse.a);
}
	
technique Water
{
	pass P0
	{
		VertexShader = compile vs_2_0 VSWater();
		PixelShader = compile ps_2_0 PSWater();
		
		//AlphaBlendEnable = true;
		//SrcBlend = SrcAlpha;
		//DestBlend = InvSrcAlpha;
	}
}