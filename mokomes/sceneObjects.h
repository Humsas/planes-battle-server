#include <d3d9.h>
#include <d3dx9.h>
#include <string>

#include "mesh.h"
#include "linkedList.h"

using namespace std;

#ifndef _SCENEOBJECTS
#define _SCENEOBJECTS

struct objektas
{
	D3DXVECTOR3 pozicija;
	D3DXMATRIX	rotationMatrix;
	D3DXMATRIX	scaleMatrix;

	D3DXMATRIX transformMatrix;

	float testScale;

	meshInfo	*mesh;

	void scale(float scaleX, float scaleY, float scaleZ);
	void rotate(float rx, float ry, float rz);
	void rotateYPR(float rx, float ry, float rz);

	void combine();
};

struct renderListItem
{
	objektas *o;
	float distance;
};

//struct objectList
//{
//	/*string ID;*/
//	objektas mO;
//	objectList *next;
//};


class SceneObjects
{
private:
	//objectList *OLIST;
	MyLinkedList<objektas> *OLIST;
	Mesh *meshManager;


	LPDIRECT3DDEVICE9 d3;


	// rikiuotam renderinimui
	bool isUpdated;
	int counter;
	int renderListLen;
	renderListItem **renderList; // pointeriu masyvas


public:
	SceneObjects(LPDIRECT3DDEVICE9 &d3, Mesh *meshManager);
	~SceneObjects();

	objektas *add(string id/*, float x, float y, float z, float scale*/);

	void remove(objektas *o);


	void quickSort(int left, int right);

	void Update(float x, float y, float z);
	void Render();


};



#endif