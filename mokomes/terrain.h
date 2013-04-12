#ifndef _TERRAIN
#define _TERRAIN

#include <d3d9.h>
#include <d3dx9.h>
#include <omp.h>

#include "Bitmap.h"
#include "kamera.h"
#include "textureManager.h"



struct terVert {float X, Y, Z; 
				D3DXVECTOR3 normal;
				D3DXVECTOR3 biNormal;
				D3DXVECTOR3 tangent;
				float fU, fV;
				float fU2, fV2;
				float fU3, fV3;
				float fU4, fV4;			
				};

struct terVertLink	{
					//terVert *vert;
					DWORD* drawMap;
					LPDIRECT3DINDEXBUFFER9 m_ib;
					//LPDIRECT3DVERTEXBUFFER9 v_buff;
					};



const D3DVERTEXELEMENT9 customVertexFormat[] =
{
  {0, 0,	D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
  {0, 12,	D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,		0},
  {0, 24,	D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL,	0},
  {0, 36,	D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,	0},
  {0, 48,	D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
  {0, 56,	D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	1},
  {0, 64,	D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	2},
  {0, 72,	D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	3},
  D3DDECL_END() // this macro is needed as the last item!
};




class Terrain: public Bitmap
{
private:
	int startX; // koordinate nuo kurios pradedamas mapo piesimas ( 0 )
	int startZ; // koordinate nuo kurios pradedamas mapo piesimas ( 0 )

	int cellsX; // celiu kiekis i ploti
	int cellsZ; // celiu kiekis i ilgi
	int vertX; // = cellsX +1;
	int vertZ; // = cellsZ +1;  nes verteksu 1 daugiau nei celiu ta kryptimi

	double stepX; // = 15;
	double stepZ; // = 15;

	double tmpY;

	LPDIRECT3DDEVICE9 d3;
	TextureManager* textureManager;
	Kamera* kamera;
	


	ID3DXEffect* pEffect;
	D3DXHANDLE hTech;



	string mName; /// test



	//LPDIRECT3DINDEXBUFFER9 m_ib;
	IDirect3DVertexBuffer9 *v_buff;  // terrain
	IDirect3DVertexDeclaration9* terrainCustomVertex;


	LPDIRECT3DTEXTURE9 	alphaCombo,
						alphaCombo2,
						

						t1Tex, 
						t2Tex,
						t3Tex,
						t4Tex,
						t5Tex, 
						t6Tex,
						t7Tex,
						t8Tex,
						lMap,
						//water,
						detail,
						bumpMap;

	unsigned int* drawMap; 
	
	terVert* vert;



	// QUAD start
	LPDIRECT3DINDEXBUFFER9 m_ibQuad[2];	/////////////
	DWORD* drawMapQuad[2];					/////////////
	double leftAngle, rightAngle, wAngle;
	bool angleReverse;
	int pX, pY, *radius, levelCount;
	double lastX, lastY;
	int triCount[2];
	int indichesCount[2];
	bool writelock, renderlock;
	int updateindex, drawindex;
	// QUAD END


	float *hMap;


	float lightDir[3];
	



	double getTmpH(D3DXVECTOR3 vect);
	void createTangentSpaceVectors( D3DXVECTOR3 *v1,
								D3DXVECTOR3 *v2,
								D3DXVECTOR3 *v3,
								float v1u, float v1v,
								float v2u, float v2v,
								float v3u, float v3v,
								D3DXVECTOR3 *vTangent,
								D3DXVECTOR3 *vBiNormal,
								D3DXVECTOR3 *vNormal );




public:
	
	Terrain(LPDIRECT3DDEVICE9 &d3, TextureManager* textureManager, Kamera* c, string mapname, int x = 0, int y = 0) 
	{  

		this->d3 = d3;
		this->textureManager = textureManager;
		this->kamera = c;
		this->mName = mapname;

		d3->CreateVertexDeclaration(customVertexFormat, &terrainCustomVertex);

		startX = x;
		startZ = y;
		//"Maps/Map0/map.BMP"
		loadBMP(mapname.c_str());

		vertX = width;
		vertZ = height;

		cellsX = vertX - 1;
		cellsZ = vertZ - 1;

	


		stepX = stepZ = 50;
	


		v_buff = NULL;

		drawMap = NULL;

		vert = new terVert[vertX * vertZ];
		

		static double kampas = 45;
			 
		float a = sin(kampas);
		float b = cos(kampas);

		float aukstis = -0.2;

		lightDir[0] = a;
		lightDir[1] = aukstis;
		lightDir[2] = b;



		updateindex = 0;
		drawindex   = 1;

		triCount[0] = triCount[1] = 0;
		indichesCount[0] = indichesCount[1] = 0;

		lastX = lastY = -100000;

		angleReverse = false;
		wAngle = -1;
		leftAngle = wAngle + 23;
		rightAngle = wAngle - 23;
		
		if(leftAngle > 360)
		{
			leftAngle -= 360;
			rightAngle -= 360;
		}
		
		
		if(rightAngle < 0) { rightAngle +=360; angleReverse = true; }

		pX = pY = 100;
		
		int dddd = 2;
		levelCount = 1;

		while(dddd < cellsX)
		{
			dddd *= 2;
			levelCount++;
		}

		radius = new int[levelCount];
		int rd = 40;

		for(int i = levelCount-1; i >= 0; i--)
		{
			radius[i] = rd;
			rd *= 2.0; //2.5
		}

		
	}

	~Terrain() { 
	
		delete[] vert; 

		delete[] hMap;
		
		

		
		delete[] drawMap;

		
		if(v_buff != NULL) v_buff->Release();
		if(m_ibQuad[0] != NULL) m_ibQuad[0]->Release();
		if(m_ibQuad[1] != NULL) m_ibQuad[1]->Release();
		
		
		//if(alphaCombo != NULL) alphaCombo->Release();
		//if(alphaCombo2 != NULL) alphaCombo2->Release();

		
		//if(t1Tex != NULL) t1Tex->Release();
		//if(t2Tex != NULL) t2Tex->Release();
		//if(t3Tex != NULL) t3Tex->Release();
		//if(t4Tex != NULL) t4Tex->Release();
		//if(t5Tex != NULL) t5Tex->Release();
		//if(t6Tex != NULL) t6Tex->Release();
		//if(t7Tex != NULL) t7Tex->Release();
		//if(t8Tex != NULL) t8Tex->Release();

		//if(water != NULL) water->Release();
		//if(detail != NULL) detail->Release();

		if(lMap != NULL) lMap->Release();

		//if(bumpMap != NULL) bumpMap->Release();
		

		if(terrainCustomVertex != NULL) terrainCustomVertex->Release();

		

		if(pEffect != NULL) pEffect->Release();

		
		delete[] radius;
	
	}

	void setVal();


	void generateTerain();

	void Update();
	void Render();

	D3DXVECTOR3 getNormal(D3DXVECTOR3 vect);
	double getH(D3DXVECTOR3 vect);

	void calculateTerrainNormals();


	void loadShader(LPDIRECT3DDEVICE9 &d3);
	

	LPDIRECT3DTEXTURE9 trace(LPDIRECT3DDEVICE9 &d3, float *heightmap, int width, int height, float saule[3], int textMultiply);

	LPDIRECT3DTEXTURE9 trace2(LPDIRECT3DDEVICE9 &d3, int width, int height, float saule[3], int textMultiply);

	void setSunDirection(LPDIRECT3DDEVICE9 &d3, float saule[3]);
	

	unsigned char *textureGenerator(float minHeight, float maxHeight, float minAngle, float maxAngle, int width, int height, int density, int blur);
	LPDIRECT3DTEXTURE9 textureRGBA(LPDIRECT3DDEVICE9 &d3, int width, int height, unsigned char *tex1, unsigned char *tex2, unsigned char *tex3, unsigned char *tex4);


	
	void tree(int level, int x, int y, int w, int h, double tx, double ty, double atstumas);
	bool colision(int x, int y, int w, int h, double tx, double ty, int level, double &dist);
	bool fullColision(int x, int y, int w, int h, double tx, double ty, int level);
	double distance(int x, int y, int w, int h, double tx, double ty);
	double distance2(int x, int y, double tx, double ty);
	double angle(double px, double py, double x, double y);
	int xyToint(int x, int y);

	float getWidth();
	float getHeight();
	
};

#endif