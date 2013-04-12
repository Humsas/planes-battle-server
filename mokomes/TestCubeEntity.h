#ifndef TESTCUBEENTITY_H_
#define TESTCUBEENTITY_H_

#include "AbstractEntity.h"


class TestCubeEntity : public AbstractEntity
{


public:
	TestCubeEntity(LPDIRECT3DDEVICE9 &d3, Mesh *m, Vector &position, Vector &rotation) : AbstractEntity(d3, m, "kubas", position, rotation, rand() % 100 + 1, ENTITY_STATIC)
	{
		
	}


	~TestCubeEntity()
	{
	}


	void Update(float dt)
	{
		// do nothing :D
		// nes nejuda sitas daiktas

	}

	/*void Render()
	{


	}*/

};

#endif