#ifndef MESH_H
#define MESH_H

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <sstream>

#include"matematika.h"
#include "textureManager.h"
using namespace std;




struct meshInfo
{


	DWORD numMaterials;            
	LPD3DXMESH mesh;

	D3DXMATERIAL* d3dxMaterials;
	D3DMATERIAL9 *meshMaterials;
	IDirect3DTexture9 **meshTextures;

	D3DXVECTOR3* bounds;
	float radius;
	float maxBound;

	LPDIRECT3DVERTEXBUFFER9 m_vb;
	LPDIRECT3DINDEXBUFFER9 m_ib;

};

struct meshList
{
	string ID;
	meshInfo mesh;
	meshList *next;
};



struct MYVERTEX
{
	D3DXVECTOR3 p;
	DWORD colour;
};

const DWORD MYVERTEX_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

class Mesh
{
private:
	TextureManager* textureManager;
	meshList *MESH;
	//LPD3DXMESH mesh;
	//LPDIRECT3DDEVICE9 *d3d;

public:

	//LPDIRECT3DDEVICE9* d3d;

	~Mesh();
	Mesh(TextureManager* textureManager) 
	{ 
		this->textureManager = textureManager;
		MESH = NULL;  
	}
	

	// pakrauna mesha i sarasa
	void loadMesh(string path, string filename, string id, LPDIRECT3DDEVICE9 &d3);

	// uzloadina mesha i directx pagal mesho ID
	void drawMesh(LPDIRECT3DDEVICE9 &d3, string id, double x, double y, double z, double scale, float rx, float ry, float rz);

	void drawMesh(LPDIRECT3DDEVICE9 &d3, string id, double x, double y, double z, double scale, D3DXMATRIX &rotMat);

	// uzloadina mesha i directx pagal mesho pointeri
	void drawMesh(LPDIRECT3DDEVICE9 &d3, meshInfo *mesh, double x, double y, double z, double scale, float rx, float ry, float rz);



	// uzloadina mesha i directx pagal mesho pointeri
	void drawMesh(LPDIRECT3DDEVICE9 &d3, meshInfo *mesh, D3DXVECTOR3 &pozicija, D3DXMATRIX &transformMatrix);




	// konvertuoja tekstines eilutes (skirta loadMesh funkcijai)
	LPCSTR STL(string a);

	// pointeri i mesho struktura grazina
	meshInfo* getPointer(string id);



};

#endif