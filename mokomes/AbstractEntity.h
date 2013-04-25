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

class ChunkManager;
class AbstractEntity
{
public:
	void *_motherBranch;

protected:
	//

	Vector		position;
	Vector		rotarionYawPitchRoll;
	float		scale;
	EntityType	entityType;
	meshInfo *	pMesh;
	Mesh *		pMeshManager;
	int			mType;
	string		mMeshID;
	ChunkManager *CM;




	// konvertuoti i directx matricas
	//D3DXVECTOR3   pozicija;
	D3DXMATRIX      rotationMatrix;
	D3DXMATRIX      scaleMatrix;
	D3DXMATRIX      transformMatrix;


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
	AbstractEntity(Mesh *m, string mesh_ID, Vector &position, Vector &rotationYPR, float scale, EntityType eEntitType)
	{
		this->position = Vector(position);
		this->rotarionYawPitchRoll = Vector(rotationYPR);
		this->scale = scale;
		this->entityType = eEntitType;

		this->pMeshManager = m;
		mMeshID = mesh_ID;
		this->pMesh = m->getPointer(mesh_ID);

		//gCons.add("ASDF", GAME_CONSOLE_OK);

		setScale(scale);
		rotateYPR(&rotarionYawPitchRoll);
		combine();
	}


	~AbstractEntity()
	{
		//delete position;
		//delete rotarionYawPitchRoll;
	}

	// get
	Vector *getPosition()
	{
		return &position;
	}

	Vector *getRotation()
	{
		return &rotarionYawPitchRoll;
	}

	EntityType getEntityType()
	{
		return entityType;
	}

	int getType()
	{
		return mType;
	}

	float getRadius()
	{
		return scale/2; // spindulys yra puse skersmens
	}

	void setMeshManager(Mesh *m)
	{
		pMeshManager = m;
	}

	void setChunkManager(ChunkManager *cm)
	{
		CM = cm;
	}



	// virtual
	virtual void Update(float deltaTime) = 0;
	virtual void Render()
	{
		pMeshManager->drawMesh(pMesh, &position, transformMatrix);

		//gCons.add("Rendering...");

	}


};
#endif