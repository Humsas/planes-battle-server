#include <d3d9.h>
#include <d3dx9.h>

#include <tchar.h>

#include<sstream>
#include<string>
#include<omp.h>





using namespace std;


struct terVert {float X, Y, Z; 
				D3DXVECTOR3 normal;
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

const DWORD VertexFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEX2 | D3DFVF_TEX3 | D3DFVF_TEX4;




//D3DVERTEXELEMENT9 customVertexFormat[6];









class splittedTerrain
{
private:
	terVertLink* link;
	int linkSize;
	int splW, splH;
	int terWidth, terHeight;

	LPDIRECT3DVERTEXBUFFER9* v_buff; // new
	terVert* terrainas; // new



public:
	splittedTerrain(LPDIRECT3DDEVICE9 &d3,LPDIRECT3DVERTEXBUFFER9 *v_buff, terVert* vert, DWORD* drawMap, int terWidth, int terHeight, int splitWidth, int splitHeight) 
	{
		this->v_buff = v_buff;
		terrainas = vert; // new
		this->terWidth = terWidth;
		this->terHeight = terHeight;

		splW = splitWidth;
		splH = splitHeight;

		int sW = terWidth / splitWidth;
		int sH = terHeight / splitHeight;

		linkSize = sW * sH;

		link = new terVertLink[linkSize];
		/*
		makeChunk(d3, vert, drawMap, terWidth, terHeight, splitWidth, splitHeight, 0);
		makeChunk(d3, vert, drawMap, terWidth, terHeight, splitWidth, splitHeight, 1);
		makeChunk(d3, vert, drawMap, terWidth, terHeight, splitWidth, splitHeight, 16);
		makeChunk(d3, vert, drawMap, terWidth, terHeight, splitWidth, splitHeight, 17);
		*/
		
		

#pragma omp parallel  for /*num_threads(omp_get_num_procs())*/
		for(int i = 0; i < linkSize; i++)
		{
			makeChunk(d3, vert, drawMap, terWidth, terHeight, splitWidth, splitHeight, i);
		}
		


		
		
		//vert =  new terVert2[10];
	}

	~splittedTerrain()
	{
		for(int a = 0; a < linkSize; a++)
		{
			//delete[] link[a].drawMap;
			//delete[] link[a].vert;

			if(link[a].m_ib != NULL)	link[a].m_ib->Release();
			//if(link[a].v_buff != NULL)	link[a].v_buff->Release();
		}
		delete[] link;
	}


private:
	void makeChunk(LPDIRECT3DDEVICE9 &d3, terVert* vert, DWORD* drawMap, int terWidth, int terHeight, int splitWidth, int splitHeight, int chunkId);

public:
	void render(LPDIRECT3DDEVICE9 &d3, int chunkId);
	int getSize();




};