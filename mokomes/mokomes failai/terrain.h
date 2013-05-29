#include <d3d9.h>
#include <d3dx9.h>

//#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_TEX1);

struct terVert {FLOAT X, Y, Z; DWORD COLOR;};
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)


class terrain
{
private:
	int startX; // = -50;
	int startY; // = -50;

	int cellsX; // = 100;
	int cellsY; // = 100;
	int vertX; // = cellsX +1;
	int vertY; // = cellsY +1;

	double stepX; // = 15;
	double stepY; // = 15;

	double tmpZ;

	//const int size =vertX;

	LPDIRECT3DINDEXBUFFER9 m_ib;
	LPDIRECT3DVERTEXBUFFER9 v_buffer; // = NULL; 

	WORD *drawMap; //=NULL;
	//terVert vert[10201];
	terVert *vert;

public:
	

	~terrain() { 
		delete [] drawMap; 
		delete [] vert; 
	v_buffer->Release();
	}

	void setVal();


	void generateTerain(LPDIRECT3DDEVICE9 &d3);
	void drawTerain(LPDIRECT3DDEVICE9 &d3);

	D3DXVECTOR3 getNormal(D3DXVECTOR3 vect);
	float getH(D3DXVECTOR3 vect);



};