#include"sceneObjects.h"

#include "console.h"
extern Console cons;

SceneObjects::SceneObjects(LPDIRECT3DDEVICE9 &d3, Mesh *meshManager)
{
	this->d3 = d3;
	OLIST = new MyLinkedList<objektas>();
	//OLIST = NULL;
	this->meshManager = meshManager;
	isUpdated = false;
	counter = 0;
	renderList = NULL;
	renderListLen = 0;
}


SceneObjects::~SceneObjects()
{
	/*objectList *L;
	while(OLIST)
	{
		L = OLIST;
		OLIST= OLIST->next;
		
		delete L;
	}*/
	delete OLIST;
}


objektas *SceneObjects::add(string id/*, float x, float y, float z, float scale*/)
{
	//objectList *O = new objectList;
	//O -> next = this->OLIST;
	//O -> ID = id;

	objektas *mo = new objektas;

	mo->mesh = meshManager->getPointer(id);
	//O->mO.pozicija = D3DXVECTOR3(x, y, z);


	//O->mO.testScale = scale;

	OLIST->add(mo);
	//this->OLIST = O;
	this->counter++;
	this->isUpdated = true;
	return mo;
}


void SceneObjects::remove(objektas *o)
{
	OLIST->remove(o);
	this->isUpdated = true; 
	counter--;
}


void SceneObjects::Render(/*LPDIRECT3DDEVICE9 &d3*/)
{
	//meshManager->drawMesh(d3, "cesna", 0, 0 ,0, 100, 0, 0, 0);

	//Update(250, 250, 250);


	for(int i = 0; i < OLIST->count(); i++)
	{
		//renderList[i].o->mesh
		if(renderList[i]->o->mesh != NULL && renderList[i]->distance < 10000)
		{
			//meshManager->drawMesh(d3, O->mO.mesh, O->mO.pozicija.x, O->mO.pozicija.y, O->mO.pozicija.z, O->mO.testScale, 0, 0, 0);
			renderList[i]->o->combine();
			//O->mO.combine();
			meshManager->drawMesh(d3, renderList[i]->o->mesh, renderList[i]->o->pozicija, renderList[i]->o->transformMatrix);
		}
	}


	//objectList *O;
	//for (O = OLIST; O != NULL; O = O->next) 
	//{
	//	if(O->mO.mesh != NULL)
	//	{
	//		//meshManager->drawMesh(d3, O->mO.mesh, O->mO.pozicija.x, O->mO.pozicija.y, O->mO.pozicija.z, O->mO.testScale, 0, 0, 0);
	//		O->mO.combine();
	//		meshManager->drawMesh(d3, O->mO.mesh, O->mO.pozicija, O->mO.transformMatrix);
	//	}

	//}
}


// rikiuoja renderinimo masyva
void SceneObjects::Update(float x, float y, float z)
{
	if(isUpdated)
	{
		if(renderListLen != 0)
		{ 
			for(int i = 0; i < this->renderListLen; i++)
			{
				delete this->renderList[i];
			}
			delete [] this->renderList; 
		}
		this->renderList = new renderListItem *[OLIST->count()];
		renderListLen = this->OLIST->count();

		//uzpildome render lista
		//objectList *O;
		int cnt = 0;
		
		objektas *o = NULL;

		OLIST->iteratorReset();
		while((o = OLIST->getNext()) != NULL)
		{

			if(cnt < renderListLen)
			{
				//stringstream ss1;//create a stringstream
				//ss1 << cnt;
				//cons.add("lololo: "+ss1.str());
				//ss1.clear();
				//renderList[cnt].o = OLIST->mO;

				renderList[cnt] = new renderListItem;
				renderList[cnt]->o = o;
				//O->mO.
			}
			cnt++;
		}

		

		isUpdated = false;
	}


	// skaiciuojame atstuma nuo kameros iki objektu
	for(int i = 0; i < this->counter; i++)
	{
		renderList[i]->distance =	(float)sqrt(
									((renderList[i]->o->pozicija.x-x) * (renderList[i]->o->pozicija.x-x)) +
									((renderList[i]->o->pozicija.y-y) * (renderList[i]->o->pozicija.y-y)) +
									((renderList[i]->o->pozicija.z-z) * (renderList[i]->o->pozicija.z-z))
									);

		//stringstream ss1;//create a stringstream
		//		ss1 << renderList[i]->distance;
		//		cons.add("lololo: "+ss1.str());

	}

	quickSort(0, renderListLen-1);

	int ok = 0;
	int no = 0;

	float last = renderList[0]->distance;
	for(int i = 0; i < this->counter; i++)
	{
		if(renderList[i]->distance <= last)
		{
			ok++;
		}
		else
		{
			no++;
		}
		//stringstream ss;//create a stringstream
		//ss << renderList[i]->distance;

		//cons.add("OK: "+ss.str());

	}

	//stringstream ss1, ss2;//create a stringstream
	//ss1 << ok;
	//ss2 << no;
	//cons.add("OK: "+ss1.str()+" NO: "+ss2.str());
	
}





void SceneObjects::quickSort(int left, int right)
{
	if(counter > 0)
	{

		int i = left; 
		int j = right;

		//renderListItem *temp;

		float pivot = renderList[(left + right) / 2]->distance;

		 /* partition */

		while (i <= j) 
		{
			while (renderList[i]->distance > pivot)
			{
				i++;
			}

			while (renderList[j]->distance < pivot)
			{
				j--;
			}

			if (i <= j) 
			{
				renderListItem *temp = renderList[i];

				renderList[i] = renderList[j];

				renderList[j] = temp;

				i++;

				j--;

			}

		}

 

		/* recursion */

		if (left < j)
		{
			quickSort(left, j);
		}

		if (i < right)
		{
			quickSort(i, right);
		}

	}
}







// struktas

void objektas::scale(float scaleX, float scaleY, float scaleZ)
{
	
	D3DXMatrixScaling(&scaleMatrix, scaleX/mesh->maxBound, scaleY/mesh->maxBound, scaleZ/mesh->maxBound);
}


void objektas::rotate(float rx, float ry, float rz)
{
	D3DXMATRIX	matRotationX,
				matRotationY,
				matRotationZ;

	//x rotation (NEW)
	D3DXMatrixRotationX(&matRotationX,DEGTORAD(rx));
	//y rotation (NEW)
	D3DXMatrixRotationY(&matRotationY,DEGTORAD(ry));
	//z rotation (NEW)
	D3DXMatrixRotationZ(&matRotationZ,DEGTORAD(rz));

	rotationMatrix = matRotationZ * matRotationX * matRotationY;
}


void objektas::rotateYPR(float rx, float ry, float rz)
{
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, DEGTORAD(rx), DEGTORAD(ry), DEGTORAD(rz));
}

void objektas::combine()
{
	transformMatrix = scaleMatrix * rotationMatrix;
	//combo = scaleM * matRotationZ * matRotationX * matRotationY * meshworld;
}