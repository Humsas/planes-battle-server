#ifndef ENTITY_H_
#define ENTITY_H_

#include "mymath.h"
#include "mesh.h"

enum EntityType
{
	STATIC,
	DYNAMIC
};

class Entity
{
public:
	void *_motherBranch;

protected:
	// 
	Vector *position;
	Vector *rotarionYawPitchRoll;
	float scale;
	float radius;
	EntityType entityType;


	// konvertuota i directx matricas

	//shader

public:
	Entity(Mesh *m, string mesh_ID, Vector position, Vector rotationYPR, EntityType eEntitType)
	{
		this->position = new Vector(position);
		this->rotarionYawPitchRoll = new Vector(rotationYPR);
		this->entityType = eEntitType;
	}

	~Entity()
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
		return radius;
	}



	// virtual
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;


};
#endif