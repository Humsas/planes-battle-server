#ifndef ABSTRACTENTITY_H_
#define ABSTRACTENTITY_H_

#include "mymath.h"
#include "mesh.h"
#include "console.h"

extern Console gCons;

enum EntityType
{
	ENTITY_STATIC,
	ENTITY_DYNAMIC
};

class AbstractEntity
{
public:
	void *_motherBranch;

protected:
	// 
	Vector *position;
	Vector *rotarionYawPitchRoll;
	float scale;
	EntityType entityType;
	meshInfo *pMesh;
	Mesh *pMeshManager;
	LPDIRECT3DDEVICE9 d3;
	string mType;

	


	// konvertuoti i directx matricas
	//D3DXVECTOR3	pozicija;
	D3DXMATRIX	rotationMatrix;
	D3DXMATRIX	scaleMatrix;
	D3DXMATRIX	transformMatrix;


	void setScale(float s)
	{
		scale = s;
		D3DXMatrixScaling(&scaleMatrix, s/pMesh->maxBound, s/pMesh->maxBound, s/pMesh->maxBound);
	}

	void rotateYPR(Vector *v)
	{
		D3DXMatrixRotationYawPitchRoll(&rotationMatrix, DEGTORAD(v->x), DEGTORAD(v->y), DEGTORAD(v->z));
	}

	void combine()
	{
		transformMatrix = scaleMatrix * rotationMatrix;
	}



public:

	AbstractEntity(){}
	AbstractEntity(LPDIRECT3DDEVICE9 &d3, Mesh *m, string mesh_ID, Vector &position, Vector &rotationYPR, float scale, EntityType eEntitType)
	{
		this->d3 = d3;
		this->position = new Vector(position);
		this->rotarionYawPitchRoll = new Vector(rotationYPR);
		this->scale = scale;
		this->entityType = eEntitType;

		this->pMeshManager = m;
		this->pMesh = m->getPointer(mesh_ID);

		//gCons.add("ASDF", GAME_CONSOLE_OK);

		setScale(scale);
		rotateYPR(rotarionYawPitchRoll);
		combine();
	}
	
	void Create(LPDIRECT3DDEVICE9 &d3, Mesh *m, string mesh_ID, Vector &position, Vector &rotationYPR, float scale, EntityType eEntitType)
	{
		this->d3 = d3;
		this->position = new Vector(position);
		this->rotarionYawPitchRoll = new Vector(rotationYPR);
		this->scale = scale;
		this->entityType = eEntitType;

		this->pMeshManager = m;
		this->pMesh = m->getPointer(mesh_ID);

		//gCons.add("ASDF", GAME_CONSOLE_OK);

		setScale(scale);
		rotateYPR(rotarionYawPitchRoll);
		combine();
	}

	~AbstractEntity()
	{
		delete position;
		delete rotarionYawPitchRoll;
	}

	// get
	Vector *getPosition()
	{
		return position;
	}

	EntityType getEntityType()
	{
		return entityType;
	}

	float getRadius()
	{
		return scale/2; // spindulys yra puse skersmens
	}



	// virtual
	virtual void Update(float deltaTime) = 0;
	virtual void Render()
	{
		pMeshManager->drawMesh(d3, pMesh, position, transformMatrix);

		//gCons.add("Rendering...");

	}


};
#endif