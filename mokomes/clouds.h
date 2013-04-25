#ifndef _CLOUDS
#define _CLOUDS

#include <d3d9.h>
#include <d3dx9.h>
#include <omp.h>



struct cloudVert {	float X, Y, Z; 
					float fU, fV;	
				 };

const DWORD VertexFVFCloud = D3DFVF_XYZ | D3DFVF_TEX1;






//const D3DVERTEXELEMENT9 g_VBDecl_Geometry[] =
//{
//{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
//{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
//{0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0},
//{0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},
//{0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
//D3DDECL_END()
//};
//
//const D3DVERTEXELEMENT9 g_VBDecl_InstanceData[] =
//{
//{1, 0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
//{1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
//{1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3},
//{1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4},
//{1, 64, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0},
//D3DDECL_END()
//};





class Clouds
{
private:
	int width, height;
	double x, y;

	double poslinkisX, poslinkisY;

	IDirect3DTexture9* mTex0;

	LPDIRECT3DINDEXBUFFER9 m_ibClouds;
	LPDIRECT3DVERTEXBUFFER9 v_buffClouds; // water
	
	DWORD* drawMapClouds; 
	cloudVert* vertCloud;


	LPDIRECT3DDEVICE9 d3;

public:
	
	Clouds(LPDIRECT3DDEVICE9 &d3)
	{
		this->d3 = d3;
		x = -1;
		y = -1;
		width = 2;
		height = 2;
		D3DXCreateTextureFromFile(d3, L"Textures/splash.jpg", &mTex0);

		m_ibClouds = NULL;
		v_buffClouds = NULL;

		vertCloud = new cloudVert[4];

		///////////////// WATER BLOCK

		vertCloud[0].X = x;
		vertCloud[0].Z = y;
		vertCloud[0].Y = 0;
		vertCloud[0].fU = 0;
		vertCloud[0].fV = 0;



		vertCloud[1].X = x + width;
		vertCloud[1].Z = y;
		vertCloud[1].Y = 0;
		vertCloud[1].fU = 1;
		vertCloud[1].fV = 0;

		vertCloud[2].X = x;
		vertCloud[2].Z = y + height;
		vertCloud[2].Y = 0;
		vertCloud[2].fU = 0;
		vertCloud[2].fV = 1;

		vertCloud[3].X = x + width;
		vertCloud[3].Z = y + height;
		vertCloud[3].Y = 0;
		vertCloud[3].fU = 1;
		vertCloud[3].fV = 1;


		d3->CreateVertexBuffer(4 * sizeof(cloudVert) ,
                               0,
                               VertexFVFCloud,
                               D3DPOOL_MANAGED,
                               &v_buffClouds,
                               NULL);



		VOID* pVoid2;    // a void pointer

		// lock v_buffer and load the vertices into it
		v_buffClouds->Lock(0, 0, (void**)&pVoid2, 0);
		memcpy(pVoid2, vertCloud, sizeof(cloudVert)  * (4));
		v_buffClouds->Unlock();



		d3->CreateIndexBuffer((4 * 2)*3*sizeof(DWORD),D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_ibClouds, NULL);


		m_ibClouds->Lock( 0, 0, (void**)&drawMapClouds, 0 );

		drawMapClouds[0] = 0;
		drawMapClouds[1] = 1;
		drawMapClouds[2] = 2;


		drawMapClouds[3] = 1;
		drawMapClouds[4] = 3;
		drawMapClouds[5] = 2;

		m_ibClouds->Unlock();





	}
	~Clouds()
	{
		mTex0->Release();
		m_ibClouds->Release();
		v_buffClouds->Release();
	}

	void Update(double x, double y);
	void Render();


};

#endif