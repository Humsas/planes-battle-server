// Vertex-shaded plastic using directional light

/************* TWEAKABLES **************/
float4x4 worldInverseTrans : WorldInverseTranspose;
float4x4 wvp : WorldViewProjection;
float4x4 world : World;
float4x4 viewInvTrans : ViewInverse;

//float3 ViewVector : Direction;

float3 ViewVector : Direction
<
    string UIName = "Viewvector";
    string Object = "CameraDirection";
> = { 1.0f, 1.0f, 0.0f };



//texture layer1Tex: Diffuse;
//texture layer2Tex: Diffuse;


//texture Tex1: Diffuse;
//texture Tex2: Diffuse;
//texture Tex3: Diffuse;
//texture Tex4: Diffuse;


texture		AlphaMap1_texture;
texture		AlphaMap2_texture;

texture		Tex1_texture;
texture		Tex2_texture;
texture		Tex3_texture;
texture		Tex4_texture;

texture		Tex5_texture;
texture		Tex6_texture;
texture		Tex7_texture;
texture		Tex8_texture;

texture		detail_texture;
texture		light_texture;

texture		bumpMap_texture;


float BumpConstant = 1.0;
/*
texture NormalMap;
sampler2D bumpSampler = sampler_state {
    Texture = (NormalMap);
    MinFilter = Linear;
    MagFilter = Linear;
    AddressU = Wrap;
    AddressV = Wrap;
};*/


float4 AmbientColor = float4(0.7, 0.7, 0.7, 1);
float AmbientIntensity = 0.15;

float Shininess = 200;
float4 SpecularColor = float4(1, 1, 1, 1);    
float SpecularIntensity = 0.1;
//float3 ViewVector = float3(1, 0, 0);


sampler AlphaMap1 = sampler_state
{
	Texture = <AlphaMap1_texture>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	ADDRESSU = WRAP;
	ADDRESSV = WRAP;
	ADDRESSW = WRAP;
};

sampler AlphaMap2 = sampler_state
{
	Texture = <AlphaMap2_texture>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	ADDRESSU = WRAP;
	ADDRESSV = WRAP;
	ADDRESSW = WRAP;
};


sampler Tex1= sampler_state
{
	Texture = <Tex1_texture>;
	MipFilter = POINT;
	MinFilter = POINT;
	MagFilter = LINEAR;
	ADDRESSU = WRAP;
	ADDRESSV = WRAP;
	ADDRESSW = WRAP;
};

sampler Tex2= sampler_state
{
	Texture = <Tex2_texture>;
	MipFilter = POINT;
	MinFilter = POINT;
	MagFilter = LINEAR;
	ADDRESSU = WRAP;
	ADDRESSV = WRAP;
	ADDRESSW = WRAP;
};

sampler Tex3= sampler_state
{
	Texture = <Tex3_texture>;
	MipFilter = POINT;
	MinFilter = POINT;
	MagFilter = LINEAR;
	ADDRESSU = WRAP;
	ADDRESSV = WRAP;
	ADDRESSW = WRAP;
};

sampler Tex4= sampler_state
{
	Texture = <Tex4_texture>;
	MipFilter = POINT;
	MinFilter = POINT;
	MagFilter = LINEAR;
	ADDRESSU = WRAP;
	ADDRESSV = WRAP;
	ADDRESSW = WRAP;
};

sampler Tex5= sampler_state
{
	Texture = <Tex5_texture>;
	MipFilter = POINT;
	MinFilter = POINT;
	MagFilter = LINEAR;
	ADDRESSU = WRAP;
	ADDRESSV = WRAP;
	ADDRESSW = WRAP;
};

sampler Tex6= sampler_state
{
	Texture = <Tex6_texture>;
	MipFilter = POINT;
	MinFilter = POINT;
	MagFilter = LINEAR;
	ADDRESSU = WRAP;
	ADDRESSV = WRAP;
	ADDRESSW = WRAP;
};

sampler Tex7= sampler_state
{
	Texture = <Tex7_texture>;
	MipFilter = POINT;
	MinFilter = POINT;
	MagFilter = LINEAR;
	ADDRESSU = WRAP;
	ADDRESSV = WRAP;
	ADDRESSW = WRAP;
};

sampler Tex8= sampler_state
{
	Texture = <Tex8_texture>;
	MipFilter = POINT;
	MinFilter = POINT;
	MagFilter = LINEAR;
	ADDRESSU = WRAP;
	ADDRESSV = WRAP;
	ADDRESSW = WRAP;
};


sampler detail= sampler_state
{
	Texture = <detail_texture>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	ADDRESSU = WRAP;
	ADDRESSV = WRAP;
	ADDRESSW = WRAP;
};


sampler light= sampler_state
{
	Texture = <light_texture>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	ADDRESSU = WRAP;
	ADDRESSV = WRAP;
	ADDRESSW = WRAP;
};


sampler bumpMapT= sampler_state
{
	Texture = <bumpMap_texture>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	ADDRESSU = WRAP;
	ADDRESSV = WRAP;
	ADDRESSW = WRAP;
};




float4 lightDir1 : Direction
<
	string UIName = "Light Direction 1";
	string Object = "DirectionalLight";
    string Space = "World";
> = {1.0f, -.5f, 1.0f, 0.0f};

float4 liteColor1 : Diffuse
<
    string UIName = "Light Color 1";
    string Object = "DirectionalLight";
> = { 1.0f, 1.0f, 1.0f, 1.0f };


/*float4 lightDir2 : Direction
<
	string UIName = "Light Direction 2";
	string Object = "DirectionalLight";
    string Space = "World";
> = {-1.0f, .5f, 1.0f, 1.0f};
*/
/*
float4 liteColor2 : Diffuse
<
    string UIName = "Light Color 2";
    string Object = "DirectionalLight";
> = { 0.0f, 0.0f, 0.0f, 1.0f };
*/
float4 ambiColor : Ambient
<
    string UIName = "Ambient Light Color";
    string Space = "material";
> = {0.15f, 0.15f, 0.15f, 1.0f};

float4 surfColor : Diffuse
<
    string UIName = "Surface Color";
    string Space = "material";
> = {1.0f, 1.0f, 1.0f, 1.0f};

float4 surfSpecular : Specular
<
	string UIName = "Surface Specular";
	string Space = "material";
> = {0.1f, 0.1f, 0.1f, 1.0f};


float specExpon : SpecularPower
<
    string UIWidget = "slider";
    float UIMin = 1.0;
    float UIMax = 128.0;
    float UIStep = 1.0;
    string UIName = "specular power";
> = 30.0;

/************* DATA STRUCTS **************/

/* data from application vertex buffer */
struct appdata {
    float3 Position	: POSITION0;
    float3 Normal	: NORMAL0;
    float3 Binormal : BINORMAL0;
    float3 Tangent 	: TANGENT0;	
    float2 UV		: TEXCOORD0;   
    float2 alfa		: TEXCOORD1;
    float2 splat	: TEXCOORD2;
	float2 lit		: TEXCOORD3;    
};

/* data passed from vertex shader to pixel shader */
struct vertexOutput {
    float4 HPosition	: POSITION0;
	//float3 tangent 		: TANGENT;
    //float3 binormal 	: BINORMAL;
	//float4 Normal		: NORMAL;
    float2 TexCoord0	: TEXCOORD0;
    float2 TexCoord1	: TEXCOORD1;
    float2 TexCoord2	: TEXCOORD2;
	float2 TexCoord3	: TEXCOORD3;
	
	float3 Normal 		: TEXCOORD4;
    float3 Tangent 		: TEXCOORD5;
    float3 Binormal 	: TEXCOORD6;
	
    float4 diffCol		: COLOR0;
};

/*********** vertex shader ******/

vertexOutput plasticDVS(appdata IN,
    uniform float4x4 WorldViewProj,
    uniform float4x4 worldInverseTrans,
    uniform float4x4 World,
    uniform float4x4 ViewInvTrans,
    uniform float4 ambiColor,
    uniform float4 surfColor,
    uniform float4 liteColor1,
    //uniform float4 liteColor2,
    uniform float specExpon,
    uniform float3 DirFromLight1
    //uniform float3 DirFromLight2
) {
    vertexOutput OUT;
	
	
	
	
    float3 worldNormal = mul(IN.Normal, worldInverseTrans).xyz;
    worldNormal = normalize(worldNormal);

    

    //compute worldspace position
    float3 worldSpacePos = mul(IN.Position, World).xyz;
    //IN.Position.w = 1;
    float3 LightVec1 = normalize(-DirFromLight1);
    //float3 LightVec2 = normalize(-DirFromLight2);
    float ldn1 = dot(LightVec1,worldNormal);
    //float ldn2 = dot(LightVec2,worldNormal);
    float diffComp1 = max(0,ldn1);
    //float diffComp2 = max(0,ldn2);
    float4 diffContrib = surfColor * ( (diffComp1 * liteColor1) + /*+ (diffComp2 * liteColor2)*/ + ambiColor);

	//OUT.Normal = IN.Normal;
	
    OUT.TexCoord0 = IN.UV;
    OUT.TexCoord1 = IN.alfa;
    OUT.TexCoord2 = IN.splat;
	OUT.TexCoord3 = IN.lit;
	
	OUT.Normal = normalize(mul(IN.Normal, worldInverseTrans));
    OUT.Tangent = normalize(mul(IN.Tangent, worldInverseTrans));
    OUT.Binormal = normalize(mul(IN.Binormal, worldInverseTrans));
	
	

    float3 EyePos = ViewInvTrans[3].xyz;
    float3 vertToEye = normalize(EyePos - worldSpacePos);
    float3 halfAngle1 = normalize(vertToEye + LightVec1);
    //float3 halfAngle2 = normalize(vertToEye + LightVec2);
    float hdn1 = pow(max(0,dot(halfAngle1,worldNormal)),specExpon);
    //float hdn2 = pow(max(0,dot(halfAngle2,worldNormal)),specExpon);

    float4 specContrib = surfSpecular * ((hdn1 * liteColor1) /*+ (hdn2 * liteColor2)*/);
    diffContrib += specContrib;
    
    diffContrib = max(0, diffContrib);
	diffContrib = min(1, diffContrib);
    
	OUT.diffCol = diffContrib;
	OUT.diffCol.w = 1.0;
    
    // transform into homogeneous-clip space
    float4 tmpPos = {IN.Position.x,IN.Position.y,IN.Position.z,1};
    
    OUT.HPosition = mul(tmpPos, WorldViewProj);
	
    return OUT;
}




/////////////////////////////////////////////


float4 PSs(vertexOutput IN) : COLOR0
 {

 	float4 baseColor = {0.0f, 0.0f, 0.0f, 1.0f};;
	
 	float4 Color1 = tex2D(Tex1, IN.TexCoord1.xy );
 	float4 Color2 = tex2D(Tex2, IN.TexCoord1.xy );
 	float4 Color3 = tex2D(Tex3, IN.TexCoord1.xy );
 	float4 Color4 = tex2D(Tex4, IN.TexCoord1.xy );
	
 	float4 Color5 = tex2D(Tex5, IN.TexCoord1.xy );
 	float4 Color6 = tex2D(Tex6, IN.TexCoord1.xy );
 	float4 Color7 = tex2D(Tex7, IN.TexCoord1.xy );
 	float4 Color8 = tex2D(Tex8, IN.TexCoord1.xy );
	
 	float4 det = tex2D(detail, IN.TexCoord2.xy );
 	float4 lll = tex2D(light, IN.TexCoord0.xy );
	
 	float4 blendMap1 = tex2D(AlphaMap1, IN.TexCoord0.xy);
 	float4 blendMap2 = tex2D(AlphaMap2, IN.TexCoord0.xy);
 	
 	float4 result =  lerp(baseColor, Color1, blendMap1.r);
 	result =  lerp(result, Color2, blendMap1.g);
 	result =  lerp(result, Color3, blendMap1.b);
 	result =  lerp(result, Color4, blendMap1.a);

 	result =  lerp(result, Color5, blendMap2.r);
 	result =  lerp(result, Color6, blendMap2.g);
 	result =  lerp(result, Color7, blendMap2.b);
 	result =  lerp(result, Color8, blendMap2.a);



	result = result * (det*3);
	//result = result * IN.diffCol; 
	result = result * lll.a;
	
///////////////////////////////
	
	
	// Calculate the normal, including the information in the bump map
    float3 bump = BumpConstant * (tex2D(bumpMapT, IN.TexCoord1) - (0.5, 0.5, 0.5));
    float3 bumpNormal = IN.Normal + (bump.x * IN.Tangent + bump.y * IN.Binormal);
    bumpNormal = normalize(bumpNormal);

    // Calculate the diffuse light component with the bump map normal
    float diffuseIntensity = dot(-normalize(lightDir1), bumpNormal);
    if(diffuseIntensity < 0.0)
        diffuseIntensity = 0.0;

	diffuseIntensity += 0.3;
//diffuseIntensity = diffuseIntensity + IN.diffCol
    // Calculate the specular light component with the bump map normal
    float3 light = -normalize(lightDir1);
    float3 r = normalize(2 * dot(light, bumpNormal) * bumpNormal - light);
    ///// --*************** float3 v = normalize(mul(normalize(ViewVector), World));
    //float3 vv = {0.5, 0.5, 1};
    float3 v = normalize(mul(normalize(ViewVector), world));
    float dotProduct = dot(r, v);

    float4 specular = SpecularIntensity * SpecularColor * max(pow(dotProduct, Shininess), 0) * diffuseIntensity;

    // Calculate the texture color
    //float4 textureColor = tex2D(detail, IN.TexCoord0);
    result.a = 1;
    //textureColor.a = 1;

    // Combine all of these values into one (including the ambient light)
    result = saturate(result * (diffuseIntensity) + AmbientColor * AmbientIntensity + specular);
	
	result.a= 1;
	
///////////////////////////////
	
	



//float4 result =  blendMap;

//return blendMap;
	return result;
//return IN.diffCol;
 }


/////////////////////////////////////////////




technique Terrain
{
    pass p0 
    {		
		VertexShader = compile vs_1_1 plasticDVS(wvp,worldInverseTrans,
					world,viewInvTrans,
					ambiColor,surfColor,liteColor1, 
					specExpon,lightDir1);
PixelShader = compile ps_2_0 PSs();

        //ColorOp[0]   = SelectArg1;
        //ColorArg1[0] = Diffuse;
        //ColorArg2[0] = Texture;
        //AlphaOp[0]   = SelectArg1;
        //AlphaArg1[0] = Diffuse;
        //AlphaArg2[0] = Texture;
        
        
        /*ColorOp[0]   = SelectArg1;
        ColorArg1[0] = Diffuse;
        ColorArg2[0] = Texture;
        AlphaOp[0]   = SelectArg1;
        AlphaArg1[0] = Diffuse;
        AlphaArg2[0] = Texture;*/

		
    }
}

technique textured
{
    pass p0 
    {		
		VertexShader = compile vs_1_1 plasticDVS(wvp,worldInverseTrans,
					world,viewInvTrans,
					ambiColor,surfColor,liteColor1, /*liteColor2,*/
					specExpon,lightDir1/*, lightDir2*/);

		//Texture[0] = <diffuseTexture>;
  		MinFilter[0] = Linear;
  		MagFilter[0] = Linear;
  		MipFilter[0] = Linear;

        ColorOp[0]   = Modulate;
        ColorArg1[0] = Diffuse;
        ColorArg2[0] = Texture;
        AlphaOp[0]   = Modulate;
        AlphaArg1[0] = Diffuse;
        AlphaArg2[0] = Texture;
    }
}



/***************************** eof ***/
