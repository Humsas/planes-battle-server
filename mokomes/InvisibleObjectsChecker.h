#ifndef INVISIBLEOBJECTSCHECKER_H_
#define INVISIBLEOBJECTSCHECKER_H_

//#include <d3d9.h>
#include <d3dx9.h>

#include "declarations.h"
#include "mymath.h"

enum frustrumCullingCheckLevel
{
	FRUSTRUM_FULL = 3,
	FRUSTRUM_NORMAL = 1,
	FRUSTRUM_NONE
};

class InvisibleObjectsChecker
{
private:
	LPDIRECT3DDEVICE9 d3;
	Vector *tmpCamPos;
	D3DXVECTOR3* pPosition;
	D3DXPLANE clipingPlanes[6];
	D3DXMATRIX viewProjection;
	D3DXMATRIX m_view;
	D3DXMATRIX m_proj;

public:
	InvisibleObjectsChecker(LPDIRECT3DDEVICE9 d3)
	{
		this->d3 = d3;
		tmpCamPos = new Vector(0, 0, 0);
		pPosition = new D3DXVECTOR3(0, 0, 0);
	}

	~InvisibleObjectsChecker()
	{
		delete tmpCamPos;
		delete pPosition;
	}

	void updateCamera(Vector *camPos)
	{
		tmpCamPos->x = camPos->x;
		tmpCamPos->y = camPos->y;
		tmpCamPos->z = camPos->z;

		d3->GetTransform(D3DTS_VIEW, &m_view );
		d3->GetTransform(D3DTS_PROJECTION, &m_proj );
		
		D3DXMatrixMultiply( &viewProjection, &m_view, &m_proj );

		// Left plane
		clipingPlanes[0].a = viewProjection._14 + viewProjection._11;
		clipingPlanes[0].b = viewProjection._24 + viewProjection._21;
		clipingPlanes[0].c = viewProjection._34 + viewProjection._31;
		clipingPlanes[0].d = viewProjection._44 + viewProjection._41;
 
		// Right plane
		clipingPlanes[1].a = viewProjection._14 - viewProjection._11;
		clipingPlanes[1].b = viewProjection._24 - viewProjection._21;
		clipingPlanes[1].c = viewProjection._34 - viewProjection._31;
		clipingPlanes[1].d = viewProjection._44 - viewProjection._41;
 
		// Top plane
		clipingPlanes[2].a = viewProjection._14 - viewProjection._12;
		clipingPlanes[2].b = viewProjection._24 - viewProjection._22;
		clipingPlanes[2].c = viewProjection._34 - viewProjection._32;
		clipingPlanes[2].d = viewProjection._44 - viewProjection._42;
 
		// Bottom plane
		clipingPlanes[3].a = viewProjection._14 + viewProjection._12;
		clipingPlanes[3].b = viewProjection._24 + viewProjection._22;
		clipingPlanes[3].c = viewProjection._34 + viewProjection._32;
		clipingPlanes[3].d = viewProjection._44 + viewProjection._42;
 
		// Near plane
		clipingPlanes[4].a = viewProjection._13;
		clipingPlanes[4].b = viewProjection._23;
		clipingPlanes[4].c = viewProjection._33;
		clipingPlanes[4].d = viewProjection._43;
 
		// Far plane
		clipingPlanes[5].a = viewProjection._14 - viewProjection._13;
		clipingPlanes[5].b = viewProjection._24 - viewProjection._23;
		clipingPlanes[5].c = viewProjection._34 - viewProjection._33;
		clipingPlanes[5].d = viewProjection._44 - viewProjection._43;

		// Normalize planes
		for ( int i = 0; i < 6; i++ )
		{
			D3DXPlaneNormalize(&clipingPlanes[i], &clipingPlanes[i] );
		}
	}


	bool isVisible(Vector * objectPosition, float radius, frustrumCullingCheckLevel level = FRUSTRUM_NORMAL)
	{
		pPosition->x = objectPosition->x;
		pPosition->y = objectPosition->z;
		pPosition->z = objectPosition->y;

		// matymo kampo karpymas
		if(level != FRUSTRUM_NONE)
		{
			for ( int i = 0; i < 6; i++ )
			{
				if ( D3DXPlaneDotCoord( &clipingPlanes[i], pPosition ) + (radius * level) < 0 )
				{
					// Outside the frustum, reject it!
					return FALSE;
				}
			}
		}

		// atstumo karpymas
		float distance = Vector(tmpCamPos->x - objectPosition->x, 
								tmpCamPos->y - objectPosition->y,
								tmpCamPos->z - objectPosition->z).Magnitude();

		return (2*RADTODEG(atan(radius/distance)) >= OBJECT_VIEW_ANGLE_CULLING);
	}

};

#endif