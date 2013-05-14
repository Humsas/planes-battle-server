#include "sceneManager.h"

#include "console.h"
extern Console gServerConsole;

//konstruktorius
SceneManager::SceneManager(LPDIRECT3DDEVICE9 &d3)
{
	this->d3 = d3;

	
	textureManager	= new TextureManager(d3);
	meshManager		= new Mesh(d3, textureManager);

	kamera			= new Kamera();
	CM				= new ChunkManager(d3, textureManager, kamera);

	//ter				= new Terrain(d3, textureManager, kamera, "Maps/Map0/map.BMP");
	
	
	sceneObjects	= new SceneObjects(d3, meshManager);
	hud				= new Hud(d3, textureManager);
	debesis = new Clouds(d3);
	slopeScale = 0.0;
	depthBias = 0.0;

	wireframe = false;
	//renderConsole = false;

	LoadObjects();

	//ter->generateTerain();
	//ter->Update();
	//ter->update(6000, 6000, 0, 0);
}

//destruktorius
SceneManager::~SceneManager()
{
	delete CM;
	//if(ter != NULL)				delete ter;
	if(kamera != NULL)			delete kamera;
	if(meshManager != NULL)		delete meshManager;
	if(sceneObjects != NULL)	delete sceneObjects;
	if(hud != NULL)				delete hud;
	delete						textureManager;

}

void SceneManager::Render()
{
	// cia uzkrauti kalna ivairiu directX parametru renderinimui
	d3->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(250, 210, 180), 1.0f, 0);
	d3->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	d3->BeginScene();

	//D3DXMATRIX matView;    // the view transform matrix

	//D3DXMatrixLookAtLH(&matView,
	//	&D3DXVECTOR3 (kamera->getCamP().x, kamera->getCamP().y, kamera->getCamP().z),    // the camera position
	//	&D3DXVECTOR3 (kamera->getCamW().x, kamera->getCamW().y, kamera->getCamW().z),	// the look-at position
	//	&D3DXVECTOR3 (0.0f, 1.0f, 0.0f));    // the up direction

	//d3->SetTransform(D3DTS_VIEW, &matView);    // set the view transform to matView

	//D3DXMATRIX matProjection;     // the projection transform matrix

	//D3DXMatrixPerspectiveFovLH(&matProjection,
	//						   D3DXToRadian(45),    // the horizontal field of view
	//						   (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
	//						   1.0f,    // the near view-plane
	//						   50000.0f);    // the far view-plane

	//d3->SetTransform(D3DTS_PROJECTION, &matProjection);    // set the projection
	//d3->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//d3->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//// renderinti scenos objektus
	//d3->SetRenderState( D3DRS_LIGHTING, FALSE );
	//d3->SetRenderState(D3DRS_FOGENABLE, FALSE);
	//meshManager->drawMesh(d3, "skybox", kamera->getCamP().x, kamera->getCamP().y, kamera->getCamP().z, 50000, 0, 0, 0 );
	//d3->SetRenderState(D3DRS_FOGENABLE, TRUE);
	//d3->SetRenderState( D3DRS_LIGHTING, TRUE );
	//
	//if(wireframe)
	//	d3->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//else
	//	d3->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//
	//
	//	// RenderStates
	//D3DRS_SLOPESCALEDEPTHBIAS, // Defaults to zero
	//D3DRS_DEPTHBIAS,           // Defaults to zero


	//stringstream ss1;//create a stringstream
	//ss1 << "Slope: " << slopeScale << " Biass " << depthBias;
	//

	//gServerConsole.addLine(ss1.str(),GAME_CONSOLE_WARNING);
	
	//Vector camSpin;
	//camSpin = kamera->getCamSpin();
	//ter->Render(camSpin.x, camSpin.y, camSpin.z);

	//d3->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, (DWORD)slopeScale);
	//d3->SetRenderState(D3DRS_DEPTHBIAS, (DWORD)depthBias);

	

	//CM->Render();
	//sceneObjects->Render();

	//// render hud
	//hud->Render();
	gServerConsole.render(d3);

	//debesis->Render();

	d3->EndScene();
	d3->Present(NULL, NULL, NULL, NULL);
}

void SceneManager::Update(float dt)
{
	CM->Update(dt);
	kamera->Update();

	//ter->Update();
	//Vector &vv = kamera->getCamP();
	//sceneObjects->Update(vv.x, vv.y, vv.z);
	//sceneObjects->Update(0, 0, 0);
}


void SceneManager::LoadObjects()
{
	meshManager->loadMesh("skybox/","skybox.x","skybox",d3);
	meshManager->loadMesh("media/","kubas.x","kubas",d3);
	
	meshManager->loadMesh("Models/Aircraft/cesna/","cesna.x", "cesna", d3);
	meshManager->loadMesh("Models/Aircraft/B-17/","B-17.x", "B17", d3);
	meshManager->loadMesh("Models/","bomb.x", "bomba", d3);
	meshManager->loadMesh("Models/","sfera.x", "sfera", d3);
	meshManager->loadMesh("Models/","cannon.x", "cannon", d3);
	meshManager->loadMesh("Models/","cannonBarrel.x", "cannonBarrel", d3);


	meshManager->loadMesh("Models/Tree/tree1/","t1.x","t1",d3);
	meshManager->loadMesh("Models/Tree/tree2/","t2.x","t2",d3);
	meshManager->loadMesh("Models/Tree/tree3/","t3.x","t3",d3);
	meshManager->loadMesh("Models/Tree/tree4/","t4.x","t4",d3);
	meshManager->loadMesh("Models/Tree/tree5/","t5.x","t5",d3);
	meshManager->loadMesh("Models/Tree/tree6/","t6.x","t6",d3);
	meshManager->loadMesh("Models/Tree/tree7/","t7.x","t7",d3);

	meshManager->loadMesh("Models/Tree/eglute/","eglute.x","eglute",d3);

	meshManager->loadMesh("Models/Buildings/Fishing_house/","fish_house.x","fish_house",d3);

}

objektas *SceneManager::addObject(string ID, Vector position, Vector rotation, float scale)
{
	objektas *oo = NULL;
	oo = sceneObjects->add(ID);

	oo->pozicija.x = position.x;
	oo->pozicija.y = position.y;
	oo->pozicija.z = position.z;

	oo->scale(scale, scale, scale);

	oo->rotateYPR(rotation.x, rotation.y, rotation.z);

	return oo;
}

objektas *SceneManager::addObject(string ID)
{
	return sceneObjects->add(ID);
}


void SceneManager::removeObject(objektas *oo)
{
	sceneObjects->remove(oo);
}

Kamera *SceneManager::getCamera()
{
	return kamera;
}


//Terrain *SceneManager::getTerrain()
//{
//	return ter;
//}

Mesh *SceneManager::getMeshManager()
{
	return meshManager;
}

Hud *SceneManager::getHud()
{
	return hud;
}

void SceneManager::changeRenderMode()
{
	if(wireframe)
		wireframe = false;
	else
		wireframe = true;
}


/*
void SceneManager::onOffConsolerender()
{
	if(renderConsole)
		renderConsole = false;
	else
		renderConsole = true;
}*/

void SceneManager::slope(float delta)
{
	slopeScale += delta;
}

void SceneManager::bias(float delta)
{
	depthBias += delta;
}

ChunkManager *SceneManager::getChunkManager()
{
	return CM;
}

/*
terrain *SceneManager::getTerrainAtPoint(D3DXVECTOR3 pos)
{
	return ter;
}

double SceneManager::get*/
