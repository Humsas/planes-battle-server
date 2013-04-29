#ifndef _SCENEMANAGER
#define _SCENEMANAGER

// standartiniai
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
using namespace std;


// mano
#include "textureManager.h"
#include "terrain.h"
#include "altimeter.h"
#include "speedometer.h"
#include "clouds.h"
#include "sceneObjects.h"
#include "kamera.h"
#include "hud.h"
#include "mymath.h"
#include "chunkManager.h"



class SceneManager
{
private:
	LPDIRECT3DDEVICE9 d3;


	TextureManager	*textureManager;
	ChunkManager	*CM;

	//Terrain			*ter;
	Kamera			*kamera;
	Mesh			*meshManager;
	SceneObjects	*sceneObjects;
	Hud				*hud;

	bool			wireframe;
	//bool			renderConsole;
	
	
	Clouds *debesis;

	float			slopeScale;
	float			depthBias;

	//terrain		*getTerrainAtPoint(D3DXVECTOR3 pos);

public:
	SceneManager(LPDIRECT3DDEVICE9 &d3);
	~SceneManager();

	void Render();
	void Update(float dt);

	void LoadObjects();

	Kamera		*getCamera();
	//Terrain		*getTerrain();
	Mesh		*getMeshManager();
	Hud			*getHud();
	ChunkManager *getChunkManager();

	objektas *addObject(string ID, Vector position, Vector rotation, float scale);
	objektas *addObject(string ID);

	void removeObject(objektas *oo);

	//double getTerrainHeight(D3DXVECTOR3 pos);


	void changeRenderMode();
	//void onOffConsolerender();

	void slope(float delta);
	void bias(float delta);
};


#endif