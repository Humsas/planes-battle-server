#include "terrain.h"

#include "console.h"
extern Console gServerConsole;


void terrain::setVal()
	{  
		///////////////// NENAUDOTI ///////////////////
		///////////////// mem leak  ///////////////////
	//startX = 0;
	//startZ = 0;

	////loadBMP("Maps/Map0/map.BMP");

	//vertX = width;
	//vertZ = height;

	//cellsX = vertX - 1;
	//cellsZ = vertZ - 1;

	//


	//stepX = 10;
	//stepZ = 10;	

	//m_ib = NULL;
	//v_buff = NULL;

	//vert = new terVert[vertX * vertZ];

	//	
	}

void terrain::loadShader(LPDIRECT3DDEVICE9 &d3)
	{
		

		D3DXCreateEffectFromFileA(d3,"shaders/terrain.fx",NULL,NULL,0,NULL,&pEffect,NULL);
		
		//pEffect->FindNextValidTechnique(NULL,&hTech);
		hTech = pEffect->GetTechniqueByName("Terrain");


	}


void terrain::generateTerain()
{


	loadShader(d3);
	//hTech = pEffect->GetTechniqueByName("TVertexShaderOnly");

	// textures
	//D3DXCreateTextureFromFile(d3, L"Maps/Map0/tex/ter-text.bmp", &terText);


	t1Tex = textureManager->loadTexture(L"Maps/Map0/tex/tex_1.dds");
	t2Tex = textureManager->loadTexture(L"Maps/Map0/tex/tex_2.dds");
	t3Tex = textureManager->loadTexture(L"Maps/Map0/tex/tex_3.dds");
	t4Tex = textureManager->loadTexture(L"Maps/Map0/tex/tex_4.dds");
	t5Tex = textureManager->loadTexture(L"Maps/Map0/tex/tex_5.dds");
	t6Tex = textureManager->loadTexture(L"Maps/Map0/tex/tex_6.dds");
	t7Tex = textureManager->loadTexture(L"Maps/Map0/tex/tex_7.dds");
	t8Tex = textureManager->loadTexture(L"Maps/Map0/tex/tex_8.dds");

	//water = textureManager->loadTexture(L"Maps/Map0/tex/water2.png");
	detail = textureManager->loadTexture(L"Maps/Map0/tex/detail.jpg");

	bumpMap = textureManager->loadTexture(L"Textures/bumpMaps/normal_map.jpg");


	/*
	D3DXCreateTextureFromFile(d3, L"Maps/Map0/tex/tex_1.dds", &t1Tex);
	D3DXCreateTextureFromFile(d3, L"Maps/Map0/tex/tex_2.dds", &t2Tex);
	D3DXCreateTextureFromFile(d3, L"Maps/Map0/tex/tex_3.dds", &t3Tex);
	D3DXCreateTextureFromFile(d3, L"Maps/Map0/tex/tex_4.dds", &t4Tex);
	D3DXCreateTextureFromFile(d3, L"Maps/Map0/tex/tex_5.dds", &t5Tex);
	D3DXCreateTextureFromFile(d3, L"Maps/Map0/tex/tex_6.dds", &t6Tex);
	D3DXCreateTextureFromFile(d3, L"Maps/Map0/tex/tex_7.dds", &t7Tex);
	D3DXCreateTextureFromFile(d3, L"Maps/Map0/tex/tex_8.dds", &t8Tex);
	*/

	/*
	D3DXCreateTextureFromFile(d3, L"Maps/Map0/tex/water2.png", &water);
	D3DXCreateTextureFromFile(d3, L"Maps/Map0/tex/detail.jpg", &detail);

	D3DXCreateTextureFromFile(d3, L"Textures/bumpMaps/normal_map.jpg", &bumpMap);
	*/

	// light maps
	//D3DXCreateTextureFromFile(d3, L"Maps/Map0/lightmap.png", &lMap);
	//D3DXCreateTextureFromFile(d3, L"Maps/Map0/speclightmap.png", &slMap);



	// drawmapas

	//d3->CreateIndexBuffer((cellsX * cellsZ * 2)*3*sizeof(DWORD),D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_ib, NULL);


	//m_ib->Lock( 0, 0, (void**)&drawMap, 0 );

	drawMap = new unsigned int[(cellsX * cellsZ * 2)*3];


	hMap = new float[vertX * vertZ];
	//int drawMap[96];
	int count = 0;
	int vIndex=0;

	int eilNum = 0;

	for (int z = 0; z < this->cellsZ; z++)
	{
		eilNum = (z%2==0)?0:1; // 0:1

		for (int x = 0; x < this->cellsX; x++)
		{
			if((((x+eilNum)%2==0)?1:0) == 0) // 0:1
			{
				// pirmas kubas
				//  ____
				//  |\ |
				//  ----

 
				// first triangle
				this->drawMap[count++]=vIndex;
				this->drawMap[count++]=vIndex+1;
				this->drawMap[count++]=vIndex+vertX;
        

				// second triangle
				this->drawMap[count++]=vIndex + 1;
				this->drawMap[count++]=vIndex+vertX+1;
				this->drawMap[count++]=vIndex+vertX;

			}
			else
			{
				// antras kubas 
				//  ____
				//  |/ |
				//  ----


				// first triangle
				this->drawMap[count++]=vIndex+vertX+1;
				this->drawMap[count++]=vIndex+vertX;
				this->drawMap[count++]=vIndex;


				// second triangle
				this->drawMap[count++]=vIndex + 1;
				this->drawMap[count++]=vIndex+vertX+1;
				this->drawMap[count++]=vIndex;
			}

			vIndex++;

		 }
		 vIndex++;
	}

	//m_ib->Unlock();


	// mapas


	count = 0;

	int bitas = 0;
	//data[0];
	float daugiklis = 1.0f; // aukscio daugiklis

	for(int z = 0; z < vertZ; z++)
	{
		for(int x = 0; x < vertX; x++)
		{
			vert[count].tangent.x = 0.0f;
			vert[count].tangent.y = 0.0f;
			vert[count].tangent.z = 0.0f;

			vert[count].biNormal.x = 0.0f;
			vert[count].biNormal.y = 0.0f;
			vert[count].biNormal.z = 0.0f;


			


			vert[count].X = startX + (stepX * x);
			vert[count].Z = startZ + (stepZ * z);
	
			if(z > 0 && x > 0 && z < vertZ-1 && x < vertX-1 && false)
			{

				// 3*3 avg smooth
		
				hMap[count] = vert[count].Y = ((
	   					((float)(*(data+bitas+RGB_BYTE_SIZE*vertX-RGB_BYTE_SIZE)) + (float)(*(data+bitas+RGB_BYTE_SIZE*vertX)) + (float)(*(data+bitas+RGB_BYTE_SIZE*vertX+RGB_BYTE_SIZE)))+


	   					((float)(*(data+bitas-RGB_BYTE_SIZE)) + (float)(*(data+bitas)) + (float)(*(data+bitas+RGB_BYTE_SIZE)))+


						((float)(*(data+bitas-RGB_BYTE_SIZE*vertX-RGB_BYTE_SIZE)) + (float)(*(data+bitas-RGB_BYTE_SIZE*vertX)) + (float)(*(data+bitas-RGB_BYTE_SIZE*vertX+RGB_BYTE_SIZE)))
						)/9) * daugiklis;
				
				//vert[count].Y = (float)(*(data+bitas))*daugiklis;
		
			}
			else
			{
				//vert[count].Y = (float)(*(data+bitas-1));

				hMap[count] = vert[count].Y = ((float)((data[bitas+0]))* 65536 +
								(float)((data[bitas+1]))* 256 +
								(float)((data[bitas+2]))* 1) / 100;
				
				
				
				
				/* + (float)(*(data+bitas+1))/1 + (float)(*(data+bitas+2))/1 ;*/
			}



			vert[count].normal = D3DXVECTOR3(0, 0, 0);

	
			// Alpha mapo uztempimas
			//vert[count].fV = z* ((1.0-(1.0/cellsZ))/cellsZ);
			vert[count].fV = z* (1.0/cellsZ);
			vert[count].fU = x* (1.0/cellsX);


	
			// Teksturos uztempimas
			int multiplier = 300;
			vert[count].fV2 = multiplier * z * (-1.0/cellsZ);
			vert[count].fU2 = multiplier * x * (1.0/cellsX);



			int multiplier2 = 5000;
			vert[count].fV3 = multiplier2 * z * (1.0/cellsZ);
			vert[count].fU3 = multiplier2 * x * (1.0/cellsX);

			//// Light mapu uztempimas
			//vert[count].fV3 = z* (-1.0/vertZ);
			//vert[count].fU3 = x* (1.0/vertX);

			vert[count].fV4 = -vert[count].fV;
			vert[count].fU4 = vert[count].fU;
	
				//set texture (NEW)
	
	

			bitas +=RGB_BYTE_SIZE;
			count++;
		}
		//bitas += 1;
	}



	// normaliu paskaiciavimas apsvietimui
	calculateTerrainNormals();



	// create a vertex buffer interface called v_buffer
	d3->SetVertexDeclaration(terrainCustomVertex);
    d3->CreateVertexBuffer(vertX * vertZ * sizeof(terVert) ,
                               0,
                               0,
                               D3DPOOL_MANAGED,
                               &v_buff,
                               NULL);



    VOID* pVoid;    // a void pointer

    // lock v_buffer and load the vertices into it
    v_buff->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vert, sizeof(terVert)  * (vertX * vertZ));
    v_buff->Unlock();

	//delete []pVoid;







	/////////////////// WATER BLOCK

	//vertWater[0].X = startX;
	//vertWater[0].Z = startZ;
	//vertWater[0].Y = 222;
	//vertWater[0].fU = 0;
	//vertWater[0].fV = 0;



	//vertWater[1].X = startX + cellsX*stepX;
	//vertWater[1].Z = startZ;
	//vertWater[1].Y = 222;
	//vertWater[1].fU = 100;
	//vertWater[1].fV = 0;

	//vertWater[2].X = startX;
	//vertWater[2].Z = startZ + cellsZ*stepZ;
	//vertWater[2].Y = 222;
	//vertWater[2].fU = 0;
	//vertWater[2].fV = 100;

	//vertWater[3].X = startX + cellsX*stepX;
	//vertWater[3].Z = startZ + cellsZ*stepZ;
	//vertWater[3].Y = 222;
	//vertWater[3].fU = 100;
	//vertWater[3].fV = 100;




	//// create a vertex buffer interface called v_buffer
 //   d3->CreateVertexBuffer(4 * sizeof(terVert) ,
 //                              0,
 //                              VertexFVF,
 //                              D3DPOOL_MANAGED,
 //                              &v_buffWater,
 //                              NULL);



 //   VOID* pVoid2;    // a void pointer

 //   // lock v_buffer and load the vertices into it
 //   v_buffWater->Lock(0, 0, (void**)&pVoid2, 0);
	//memcpy(pVoid2, vertWater, sizeof(terVert)  * (4));
 //   v_buffWater->Unlock();

	////delete []pVoid2;



	//d3->CreateIndexBuffer((4 * 2)*3*sizeof(DWORD),D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_ibWater, NULL);


	//m_ibWater->Lock( 0, 0, (void**)&drawMapWater, 0 );

	//drawMapWater[0] = 0;
	//drawMapWater[1] = 1;
	//drawMapWater[2] = 2;


	//drawMapWater[3] = 1;
	//drawMapWater[4] = 3;
	//drawMapWater[5] = 2;

	//m_ibWater->Unlock();



	d3->CreateIndexBuffer((cellsX * cellsZ * 2)*3*sizeof(DWORD),D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_ibQuad[0], NULL);
	d3->CreateIndexBuffer((cellsX * cellsZ * 2)*3*sizeof(DWORD),D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_ibQuad[1], NULL);

	int mapSize = cellsX;
	int blurLevel = 5;

	unsigned char *text1;
	unsigned char *text2;
	unsigned char *text3;
	unsigned char *text4;
	unsigned char *text5;
	unsigned char *text6;
	unsigned char *text7;
	unsigned char *text8;

	//omp_set_dynamic(0);
	#pragma omp parallel/* num_threads(4)/* sections/* )*/
	{
		#pragma omp sections
		{
			//#pragma omp section
			
				//cons.add("1");
				text1 = textureGenerator(235*3,1000*3,40,90,mapSize,mapSize,255,blurLevel);	// akmuo grubus
			
			#pragma omp section
			
				//cons.add("2");
				text2 = textureGenerator(235*3,1000*3,26,40,mapSize,mapSize,255,blurLevel);	// akmuo 
			
			#pragma omp section
			
				//cons.add("3");
				text3 = textureGenerator(0*3,240*3,40,90,mapSize,mapSize,255,blurLevel);	// akmuo pilkas
			
			#pragma omp section
			
				//cons.add("4");
				text4 = textureGenerator(0*3,235*3,35,40,mapSize,mapSize,255,blurLevel);	// zvyras
			
			#pragma omp section
			
				//cons.add("5");
				text5 = textureGenerator(0*3,235*3,0,35,mapSize,mapSize,255,blurLevel);	// smelis
			
			#pragma omp section
			
				//cons.add("6");
				text6 = textureGenerator(235*3,360*3,0,28,mapSize,mapSize,255,blurLevel);	// tamsi zole
			
			#pragma omp section
			
				//cons.add("7");
				text7 = textureGenerator(340*3,470*3,0,28,mapSize,mapSize,255,blurLevel);	// tamsi zole su p;etmais
			
			#pragma omp section
			
				//cons.add("8");
				text8 = textureGenerator(450*3,1000*3,0,28,mapSize,mapSize,255,blurLevel);	// tamsi praplikus zole
			
		}
	//unsigned char *text8 = textureGenerator(430,720,0,100,mapSize,mapSize,255,1);	// tamsi praplikus zole
	}


	//#pragma omp master

	//#pragma omp parallel sections
	//{

	alphaCombo  = textureRGBA(d3, mapSize, mapSize, text1, text2, text3, text4);
	//#pragma omp section
	alphaCombo2 = textureRGBA(d3, mapSize, mapSize, text5, text6, text7, text8);
	//}


	delete[] text1;
	delete[] text2;
	delete[] text3;
	delete[] text4;
	delete[] text5;
	delete[] text6;
	delete[] text7;
	delete[] text8;

			 

	lMap = trace2(d3, cellsX, cellsZ, lightDir, 4);

	//Splitas = new splittedTerrain(d3, &v_buff, vert, drawMap, cellsX, cellsZ, 32, 32); 
			 
}



void terrain::Render()

{
	/*while(writelock) {  }*/
	//renderlock = true;

	
	


#pragma omp critical (terrainRender)
	{


		


   d3->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	//d3->SetVertexDeclaration(terrainCustomVertex);
   //d3->SetFVF(VertexFVF);


	

   d3->SetRenderState( D3DRS_LIGHTING, TRUE );
   //d3->SetStreamSource(0, v_buff, 0, sizeof(terVert)); 
   


   d3->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


   


	pEffect->SetTechnique(hTech);

	d3->SetVertexDeclaration(terrainCustomVertex);
	//LPDIRECT3DVERTEXSHADER9 vs;
	//pEffect->GetVertexShader(hTech, &vs);
	//d3->SetVertexShader(vs);

	d3->SetStreamSource(0, v_buff, 0, sizeof(terVert));
	d3->SetIndices(m_ibQuad[drawindex]);


	//d3->GetTransform( D3DTS_WORLD, &worldsave );
	//D3DXMATRIX matProj, D3DXMATRIX matView, D3DXMATRIX matWorld)

	D3DXMATRIX ViewInverse, wi, wvp;

	D3DXMATRIX mProj, mView, mWorld;
	d3->GetTransform( D3DTS_VIEW, &mView);
	d3->GetTransform( D3DTS_PROJECTION, &mProj);
	d3->GetTransform( D3DTS_WORLD, &mWorld);

	

	D3DXMatrixInverse(&ViewInverse,0,&mView);
	D3DXMatrixInverse(&wi,0,&mWorld);
	D3DXMatrixTranspose(&wi, &wi);

	wvp = mWorld * mView * mProj;


             pEffect->SetMatrix("world", &mWorld);
			 pEffect->SetMatrix("worldInverseTrans", &wi);

             pEffect->SetMatrix("wvp",  &wvp);
			 pEffect->SetMatrix("viewInvTrans", &ViewInverse);

			 //pEffect->SetTexture("diffuseTexture",t8Tex);

			 //static double kampas = 20;
			 //static double kampas2 = 0;
			 //kampas += 0.005;
			 //kampas2 += 0.001;

			 //float a = sin(kampas);
			 //float b = cos(kampas);

			 //float aukstis = -abs(cos(kampas2)) * 5 ;

			 //float aukstis = -0.2;

			 D3DXVECTOR4 saule = D3DXVECTOR4(lightDir[0], lightDir[1], lightDir[2], 0.0f);
			 //D3DXVECTOR4 saule2 = D3DXVECTOR4(0.0f, -1.0f, 0.0f, 100.0f);

			 D3DXVECTOR3 camDir = D3DXVECTOR3(kamera->getCamSpin().x, kamera->getCamSpin().y, kamera->getCamSpin().z);
			 pEffect->SetFloatArray("lightDir1",saule,4);
			 pEffect->SetFloatArray("ViewVector",camDir,3);
			 //pEffect->SetFloatArray("lightDir2",saule2,4);


			 //float lightDir[] = {a, aukstis, b};

			 //unsigned char seselis[3] = {128, 128, 128};


			 //IntersectMap2(hMap, lightMap,seselis ,width, lightDir);
			 //if(lMap != NULL) lMap->Release();
			 //lMap = trace(d3, hMap, 256, lightDir);
			 

			 ////////////////////////////////////////////////////



			 /////////////////////////////////////////////////////


			 

			 pEffect->SetTexture("AlphaMap1_texture",alphaCombo);
			 pEffect->SetTexture("AlphaMap2_texture",alphaCombo2);

			 pEffect->SetTexture("Tex1_texture",t1Tex);
			 pEffect->SetTexture("Tex2_texture",t2Tex);
			 pEffect->SetTexture("Tex3_texture",t3Tex);
			 pEffect->SetTexture("Tex4_texture",t4Tex);

			 pEffect->SetTexture("Tex5_texture",t5Tex);
			 pEffect->SetTexture("Tex6_texture",t6Tex);
			 pEffect->SetTexture("Tex7_texture",t7Tex);
			 pEffect->SetTexture("Tex8_texture",t8Tex);

			 pEffect->SetTexture("detail_texture",detail);

			 pEffect->SetTexture("light_texture",lMap);



			 pEffect->SetTexture("bumpMap_texture",bumpMap);





	

	//pEffect->SetTexture(hTech,t1Tex);
	
	


	UINT numPasses;
	pEffect->Begin(&numPasses,0);

	
	//pEffect->

	for (UINT i=0;i<numPasses;i++) 
	{
		pEffect->BeginPass(i); // Set the pass
		 //render geometry e.g. DrawIndexedPrimitive
		//d3->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
     
		
		d3->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,vertX * vertZ,0,triCount[drawindex]);
	
	
	
		pEffect->EndPass();
	}


	pEffect->End();	

	/*d3->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	d3->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	d3->SetIndices(m_ibQuad[drawindex]);
	d3->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,vertX * vertZ,0,triCount[drawindex]);
	d3->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);*/




	///d3->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,vertX * vertZ,0,cellsX * cellsZ * 2);


	}





	





   d3->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
   
   //d3->SetRenderState(D3DRS_CULLMODE, 0);


   //d3->SetRenderState( D3DRS_LIGHTING, TRUE );
   d3->SetRenderState(D3DRS_ALPHATESTENABLE, true);

}



D3DXVECTOR3 terrain::getNormal(D3DXVECTOR3 vect)
{



	if((vect.x >= startX && vect.z >= startZ) &&
	((vect.x <= (startX + (cellsX * stepX)))  && (vect.z <= (startZ + (cellsZ * stepZ)))))

	{

		int cellX = (vect.x - startX) / stepX;
		int cellZ = (vect.z - startZ) / stepZ;


		double dx= stepX-((startX + ((cellX+1) * stepX)) - vect.x);
		double dz= stepZ-((startZ + ((cellZ+1) * stepZ)) - vect.z);

		//int dx=Obj.getCamP('x')- -50;
		//int dy=Obj.getCamP('y')- -50;


		int cNr = cellZ * cellsX + cellX;
		int start = 0;


		int eilNum = (cellZ%2==0)?0:1; // 0:1
 
		if((((cellX+eilNum)%2==0)?1:0) == 0) // 0:1
		{ 	
			dz= stepZ - dz; 

		} 


		D3DXVECTOR3 A, B, N;

		if(dx < dz) 
		{
			// pirmas trikampis
			start = cNr * 6;
		}
		else
		{
			// antras trikampis
			start = cNr * 6 + 3;
		}

		/*
		O.x = vert[drawMap[start]].X;
		O.y = vert[drawMap[start]].Y;
		O.z = vert[drawMap[start]].Z;

		S.x = vert[drawMap[start+1]].X;
		S.y = vert[drawMap[start+1]].Y;
		S.z = vert[drawMap[start+1]].Z;

		D.x = vert[drawMap[start+2]].X;
		D.y = vert[drawMap[start+2]].Y;
		D.z = vert[drawMap[start+2]].Z;
		*/

		A.x = (vert[drawMap[start+1]].X - vert[drawMap[start+2]].X);
		A.y = (vert[drawMap[start+1]].Y - vert[drawMap[start+2]].Y);
		A.z = (vert[drawMap[start+1]].Z - vert[drawMap[start+2]].Z);

		B.x = (vert[drawMap[start]].X - vert[drawMap[start+2]].X);
		B.y = (vert[drawMap[start]].Y - vert[drawMap[start+2]].Y);
		B.z = (vert[drawMap[start]].Z - vert[drawMap[start+2]].Z);



		N.x = (A.y * B.z - B.y * A.z);
		N.y = -(A.x * B.z - B.x * A.z);
		N.z = (A.x * B.y - B.x * A.y);




		//tmpY = vert[drawMap[start+2]].Y; 




		return N;
	}
	return D3DXVECTOR3(0,0,0);
}

double terrain::getTmpH(D3DXVECTOR3 vect)
{



	if((vect.x >= startX && vect.z >= startZ) &&
	((vect.x <= (startX + (cellsX * stepX)))  && (vect.z <= (startZ + (cellsZ * stepZ)))))

	{

		int cellX = (vect.x - startX) / stepX;
		int cellZ = (vect.z - startZ) / stepZ;


		double dx= stepX-((startX + ((cellX+1) * stepX)) - vect.x);
		double dz= stepZ-((startZ + ((cellZ+1) * stepZ)) - vect.z);

		//int dx=Obj.getCamP('x')- -50;
		//int dy=Obj.getCamP('y')- -50;


		int cNr = cellZ * cellsX + cellX;
		int start = 0;


		int eilNum = (cellZ%2==0)?0:1; // 0:1
 
		if((((cellX+eilNum)%2==0)?1:0) == 0) // 0:1
		{ 	
			dz= stepZ - dz; 

		} 


		

		if(dx < dz) 
		{
			// pirmas trikampis
			start = cNr * 6;
		}
		else
		{
			// antras trikampis
			start = cNr * 6 + 3;
		}

		 

		return vert[drawMap[start+2]].Y;
	}
	return 0.0;
}

double terrain::getH(D3DXVECTOR3 vect)
{
	D3DXVECTOR3 N = getNormal(vect);
	double tmpY = getTmpH(vect);

	if(vect.x != 0 && vect.y != 0 && vect.z != 0)
	{

		int cellX = (vect.x - startX) / stepX;
		int cellZ = (vect.z - startZ) / stepZ;

		//double dx= (startX + (cellX * stepX)) - vect.x;
		//double dz= (startZ + (cellZ * stepZ)) - vect.z;

		double dx= stepX-((startX + ((cellX+1) * stepX)) - vect.x);
		double dz= stepZ-((startZ + ((cellZ+1) * stepZ)) - vect.z);


		int eilNum = (cellZ%2==0)?0:1; // 0:1
  
		if((((cellX+eilNum)%2==0)?1:0) == 0)  // 0:1
		{ 	
			dz= stepZ - dz; 
			//dx= stepX - dx;

			return tmpY + ((N.x * dx - N.z * dz) / -N.y);
		} 
		else 
		{

			return tmpY + ((N.x * dx + N.z * dz) / -N.y);
		}


		//return tmpY + ((N.x * dx + N.z * dz) / -N.y) + 1.8;
	}
	else
	{
		return 0;
	}


}







void terrain::calculateTerrainNormals()
{

	D3DXVECTOR3 v1;
    D3DXVECTOR3 v2;
    D3DXVECTOR3 v3;
    D3DXVECTOR3 vTangent;
    D3DXVECTOR3 vBiNormal;
    D3DXVECTOR3 vNormal;


	D3DXVECTOR3 A, B, N;
	int start=0;

	for(int i = 0; i < cellsX*cellsZ*2; i++)
	{
		// siame cikle kiekvienas trikampis apdirbmas
start = i*3;



		

		v1 = D3DXVECTOR3(vert[drawMap[start+0]].X, vert[drawMap[start+0]].Y, vert[drawMap[start+0]].Z);
        v2 = D3DXVECTOR3(vert[drawMap[start+1]].X, vert[drawMap[start+1]].Y, vert[drawMap[start+1]].Z);
        v3 = D3DXVECTOR3(vert[drawMap[start+2]].X, vert[drawMap[start+2]].Y, vert[drawMap[start+2]].Z);


		createTangentSpaceVectors( &v1,&v2,&v3,
                                   vert[drawMap[start+0]].fU2, vert[drawMap[start+0]].fV2,
                                   vert[drawMap[start+1]].fU2, vert[drawMap[start+1]].fV2,
                                   vert[drawMap[start+2]].fU2, vert[drawMap[start+2]].fV2,
                                   &vTangent, &vBiNormal, &vNormal );
		
		//vNormal.y *= -1;

		vert[drawMap[start+0]].tangent += vTangent;
		vert[drawMap[start+2]].tangent += vTangent;
		vert[drawMap[start+1]].tangent += vTangent;

		vert[drawMap[start+0]].biNormal += vBiNormal;
		vert[drawMap[start+2]].biNormal += vBiNormal;
		vert[drawMap[start+1]].biNormal += vBiNormal;

		vert[drawMap[start+0]].normal += vNormal;
		vert[drawMap[start+1]].normal += vNormal;
		vert[drawMap[start+2]].normal += vNormal;

		A.x = (vert[drawMap[start+1]].X - vert[drawMap[start+2]].X);
		A.y = (vert[drawMap[start+1]].Y - vert[drawMap[start+2]].Y);
		A.z = (vert[drawMap[start+1]].Z - vert[drawMap[start+2]].Z);

		B.x = (vert[drawMap[start]].X - vert[drawMap[start+2]].X);
		B.y = (vert[drawMap[start]].Y - vert[drawMap[start+2]].Y);
		B.z = (vert[drawMap[start]].Z - vert[drawMap[start+2]].Z);

		// paskaiciuojama trikampio normale
		N.x = (A.y * B.z - B.y * A.z);
		N.y = -(A.x * B.z - B.x * A.z);
		N.z = (A.x * B.y - B.x * A.y);

		D3DXVec3Normalize((D3DXVECTOR3*)&N, &N );


		/*stringstream ss;

		ss << N.x << " " << N.y << " " << N.z << " : " << vNormal.x << " " << vNormal.y << " " << vNormal.z;

		cons.add(ss.str(), GAME_CONSOLE_ERROR);
*/
		


		/*vert[drawMap[start]].normal  += N;
		vert[drawMap[start+1]].normal += N;
		vert[drawMap[start+2]].normal += N;*/

		//D3DXVec3Normalize((D3DXVECTOR3*)&vert[drawMap[start]].normal, &(vert[drawMap[start]].normal + N ));
		//D3DXVec3Normalize((D3DXVECTOR3*)&vert[drawMap[start+1]].normal, &(vert[drawMap[start+1]].normal + N ));
		//D3DXVec3Normalize((D3DXVECTOR3*)&vert[drawMap[start+2]].normal, &(vert[drawMap[start+2]].normal + N ));
		

	}


	for(int i = 0; i < vertX*vertZ; i++)
	{
		D3DXVec3Normalize((D3DXVECTOR3*)&vert[i].biNormal, &vert[i].biNormal );
		D3DXVec3Normalize((D3DXVECTOR3*)&vert[i].tangent, &vert[i].tangent );
		D3DXVec3Normalize((D3DXVECTOR3*)&vert[i].normal, &vert[i].normal );

	}


	for(int a = 10; a < vertX-10; a++)
	{
		for(int b = 10; b < vertZ-10; b++)
		{
			int expo = a*vertX+b;
			vert[expo].normal= 
				
				
				vert[(expo)+vertX-1].normal + vert[(expo)+vertX].normal + vert[(expo)+vertX+1].normal +
				vert[(expo)-1].normal + vert[expo].normal + vert[(expo)+1].normal +
				vert[(expo)-vertX-1].normal + vert[(expo)-vertX].normal + vert[(expo)-vertX+1].normal;

			D3DXVec3Normalize((D3DXVECTOR3*)&vert[expo].normal, &vert[expo].normal );

			

		}

	}






	for(int j = 0; j < vertX*vertZ; j++)
	{
		D3DXVec3Normalize((D3DXVECTOR3*)&vert[j].normal, &vert[j].normal );
	}


}





void terrain::createTangentSpaceVectors( D3DXVECTOR3 *v1,
                                D3DXVECTOR3 *v2,
                                D3DXVECTOR3 *v3,
                                float v1u, float v1v,
                                float v2u, float v2v,
                                float v3u, float v3v,
                                D3DXVECTOR3 *vTangent,
                                D3DXVECTOR3 *vBiNormal,
                                D3DXVECTOR3 *vNormal )
{
	//A.x = (vert[drawMap[start+1]].X - vert[drawMap[start+2]].X);
		//A.y = (vert[drawMap[start+1]].Y - vert[drawMap[start+2]].Y);
		//A.z = (vert[drawMap[start+1]].Z - vert[drawMap[start+2]].Z);

		//B.x = (vert[drawMap[start]].X - vert[drawMap[start+2]].X);
		//B.y = (vert[drawMap[start]].Y - vert[drawMap[start+2]].Y);
		//B.z = (vert[drawMap[start]].Z - vert[drawMap[start+2]].Z);


    // Create edge vectors from vertex 1 to vectors 2 and 3.
    D3DXVECTOR3 vDirVec_v2_to_v1 = *v1 - *v3;
    D3DXVECTOR3 vDirVec_v3_to_v1 = *v2 - *v3;

    // Create edge vectors from the texture coordinates of vertex 1 to vector 2.
    float vDirVec_v2u_to_v1u = v1u - v3u;
    float vDirVec_v2v_to_v1v = v1v - v3v;

    // Create edge vectors from the texture coordinates of vertex 1 to vector 3.
    float vDirVec_v3u_to_v1u = v2u - v3u;
    float vDirVec_v3v_to_v1v = v2v - v3v;

    float fDenominator = vDirVec_v2u_to_v1u * vDirVec_v3v_to_v1v - 
                         vDirVec_v3u_to_v1u * vDirVec_v2v_to_v1v;

    if( fDenominator < 0.0001f && fDenominator > -0.0001f  && false)
    {
        // We're too close to zero and we're at risk of a divide-by-zero! 
        // Set the tangent matrix to the identity matrix and do nothing.
        *vTangent  = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
        *vBiNormal = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
        *vNormal   = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
    }
    else
    {
        // Calculate and cache the reciprocal value
        float fScale1 = 1.0f / fDenominator;

        D3DXVECTOR3 T;
        D3DXVECTOR3 B;
        D3DXVECTOR3 N;

        T = D3DXVECTOR3((vDirVec_v3v_to_v1v * vDirVec_v2_to_v1.x - vDirVec_v2v_to_v1v * vDirVec_v3_to_v1.x) * fScale1,
                        (vDirVec_v3v_to_v1v * vDirVec_v2_to_v1.y - vDirVec_v2v_to_v1v * vDirVec_v3_to_v1.y) * fScale1,
                        (vDirVec_v3v_to_v1v * vDirVec_v2_to_v1.z - vDirVec_v2v_to_v1v * vDirVec_v3_to_v1.z) * fScale1);

        B = D3DXVECTOR3((-vDirVec_v3u_to_v1u * vDirVec_v2_to_v1.x + vDirVec_v2u_to_v1u * vDirVec_v3_to_v1.x) * fScale1,
                        (-vDirVec_v3u_to_v1u * vDirVec_v2_to_v1.y + vDirVec_v2u_to_v1u * vDirVec_v3_to_v1.y) * fScale1,
                        (-vDirVec_v3u_to_v1u * vDirVec_v2_to_v1.z + vDirVec_v2u_to_v1u * vDirVec_v3_to_v1.z) * fScale1);

        // The normal N is calculated as the cross product between T and B
        D3DXVec3Cross( &N, &T, &B );

        // Calculate and cache the reciprocal value
        float fScale2 = 1.0f / ((T.x * B.y * N.z - T.z * B.y * N.x) + 
                                (B.x * N.y * T.z - B.z * N.y * T.x) + 
                                (N.x * T.y * B.z - N.z * T.y * B.x));

        //
        // Use the temporary T (Tangent), (B) Binormal, and N (Normal) vectors 
        // to calculate the inverse of the tangent matrix that they represent.
        // The inverse of the tangent matrix is what we want since we need that
        // to transform the light's vector into tangent-space.
        //

        D3DXVECTOR3 vTemp;

        (*vTangent).x =   D3DXVec3Cross( &vTemp, &B, &N )->x * fScale2;
        (*vTangent).y = -(D3DXVec3Cross( &vTemp, &N, &T )->x * fScale2);
        (*vTangent).z =   D3DXVec3Cross( &vTemp, &T, &B )->x * fScale2;
        D3DXVec3Normalize( &(*vTangent), &(T) );

        (*vBiNormal).x = -(D3DXVec3Cross( &vTemp, &B, &N )->y * fScale2);
        (*vBiNormal).y =   D3DXVec3Cross( &vTemp, &N, &T )->y * fScale2;
        (*vBiNormal).z = -(D3DXVec3Cross( &vTemp, &T, &B )->y * fScale2);
        D3DXVec3Normalize( &(*vBiNormal), &(B) );

        (*vNormal).x =   D3DXVec3Cross( &vTemp, &B, &N )->z * fScale2;
        (*vNormal).y = -(D3DXVec3Cross( &vTemp, &N, &T )->z * fScale2);
        (*vNormal).z =   D3DXVec3Cross( &vTemp, &T, &B )->z * fScale2;
        D3DXVec3Normalize( &(*vNormal), &(N) );

        //
        // NOTE: Since the texture-space of Direct3D and OpenGL are laid-out 
        //       differently, a single normal map can't look right in both 
        //       unless you make some adjustments somewhere.
        //
        //       You can adjust or fix this problem in three ways:
        //
        //       1. Create two normal maps: one for OpenGL and one for Direct3D.
        //       2. Flip the normal map image over as you load it into a texture 
        //          object.
        //       3. Flip the binormal over when computing the tangent-space
        //          matrix.
        //
        // Since the normal map used by this sample was created for Direct3D,
        // no adjustment is necessary.
        //
        //*vBiNormal = *vBiNormal * -1.0f;
    }
}








/////////////////////////////////////////////




/*
   d3->SetTexture(0,t1Alfa);
   d3->SetTexture(1,t1Tex);
   d3->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,vertX * vertZ,0,cellsX * cellsZ * 2); 

   d3->SetTexture(0,t2Alfa);
   d3->SetTexture(1,t2Tex);
   d3->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,vertX * vertZ,0,cellsX * cellsZ * 2); 

   d3->SetTexture(0,t3Alfa);
   d3->SetTexture(1,t3Tex);
   d3->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,vertX * vertZ,0,cellsX * cellsZ * 2);

   d3->SetTexture(0,t4Alfa);
   d3->SetTexture(1,t4Tex);
   d3->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,vertX * vertZ,0,cellsX * cellsZ * 2); 

   d3->SetTexture(0,t5Alfa);
   d3->SetTexture(1,t5Tex);
   d3->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,vertX * vertZ,0,cellsX * cellsZ * 2); 

   d3->SetTexture(0,t6Alfa);
   d3->SetTexture(1,t6Tex);
   d3->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,vertX * vertZ,0,cellsX * cellsZ * 2); 

   d3->SetTexture(0,t7Alfa);
   d3->SetTexture(1,t7Tex);
   d3->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,vertX * vertZ,0,cellsX * cellsZ * 2); 

   d3->SetTexture(0,t8Alfa);
   d3->SetTexture(1,t8Tex);
   d3->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,vertX * vertZ,0,cellsX * cellsZ * 2); 
   */

 /*
   // splatingo isjungimas
	d3->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);	
	d3->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
   
*/

















 LPDIRECT3DTEXTURE9 terrain::trace(LPDIRECT3DDEVICE9 &d3, float *heightmap, int width, int height, float saule[3], int textMultiply)
 {
	 unsigned char *lightmap = new unsigned char[width*height];
	 
	 for(int a = 0; a < width*height; a++)
	 {
		 lightmap[a] = 255;
	 }

	 // sviesos sklidimo kryptis
	 int xZ, yZ;

	 if(saule[0] >= 0) { xZ = 1; } else { xZ = -1; }
	 if(saule[2] >= 0) { yZ = 1; } else { yZ = -1; }
	 
	 //float kryptiesKoef = saule[2] / saule[0];
	 int eilute, stulpelis;

	 if(xZ == 1 && yZ == 1)   { eilute = 0;					stulpelis = 0; }
	 if(xZ == 1 && yZ == -1)  { eilute = height - 1;		stulpelis = 0; }
	 if(xZ == -1 && yZ == 1)  { eilute = 0;					stulpelis = width - 1; }
	 if(xZ == -1 && yZ == -1) { eilute = height - 1;		stulpelis = width - 1; }

	 

	 int cEilute, cStulpelis;
	 cEilute = eilute;
	 

	 while(1)
	 {	// eina per eilutes

		 cStulpelis = stulpelis;
		 while(1)
		 {	// eina per stulpelius

			 int cell = cEilute * width + cStulpelis;

			 if(lightmap[cell] == 255)
			 {	// langelio metama seseli skaiciuojame tik tada jei jis pats nera seselyje

				 /////////////////////////////////////////////////////////
				 /////// skaiciavimai kur uzmeta seseli esama cele ///////
				 /////////////////////////////////////////////////////////

				 


				 float px, py;
				 px = cStulpelis;
				 py = cEilute;

				 //int darkness = 128;
				 while(1) /*darkness<255*/
				 {
					 px += saule[0];
					 py += saule[2];

					 int ppx, ppy;

					 ppx = floor(px + 0.5);
					 ppy = floor(py + 0.5);

					 if(ppx < 0 || ppx >= width || ppy < 0 || ppy >= height) { break; }

					 int cele = ppy * width + ppx;
	

					 // get distance from original point to current point
					 //float distance = sqrt( (cStulpelis-px)*(cStulpelis-px) + (cEilute-py)*(cEilute-py) );

					float distance = sqrt( (stepX*(cStulpelis-px))*(stepX*(cStulpelis-px)) + (stepZ*(cEilute-py))*(stepZ*(cEilute-py)) );
 
					// get height at current point while traveling along light ray
					float hh = heightmap[cell] + saule[1]*distance;

					 
						 if(hh > heightmap[cele])
						 {
							 lightmap[cele] = 50 ; //darkness				
						 }
						 else
						 {
							 break;
						 }
					 //darkness+=1;
				 }




				 //////////////////////////////////////////////////////////

			 }
		

			 cStulpelis += xZ;
			 // vidinio ciklo stabdymo salyga
			 if(0 > cStulpelis || cStulpelis > width-1)
			 break;
		 }

		 cEilute += yZ;
		 // isorinio ciklo stabdymo salyga
		 if(0 > cEilute || cEilute > height-1)
		 break;
	 }


	 //lightmapo minkstiklis :D

	 unsigned char *lmap = new unsigned char[width*height];  // laikinas lightmapas


	 int skaitlius = 0;
	 while(skaitlius < 20)
	 {

	 for(int a = 1; a < height -1; a++)
	 {
		 for(int b = 1; b < width -1; b++)
		 {
			 lmap[a*width + b] = (lightmap[(a+1)*width + b-1] + lightmap[(a+1)*width + b] + lightmap[(a+1)*width + b+1]+
									lightmap[a*width + b-1] + lightmap[a*width + b] + lightmap[a*width + b+1]+
									lightmap[(a-1)*width + b-1] + lightmap[(a-1)*width + b] + lightmap[(a-1)*width + b+1]) / 9;
		 }
	 }


	 for(int a = 1; a < height -1; a++)
	 {
		 for(int b = 1; b < width -1; b++)
		 {
			 lightmap[a*width + b] = lmap[a*width + b];
		 }
	 }


	 skaitlius++;
	 }

	 
	 

	 for(int a = 0; a < width*height; a++)
	 {
		 if(lightmap[a] < 128) { lightmap[a] = 50; } else { lightmap[a] = 255; }
	 }


	 // resizinimas i 1024 * 1024

	 // 1024  mapas
	 int resSize = 1024;

	 int resWidth = width * textMultiply;
	 int resHeight = height * textMultiply;

	 int size = 256;
	 unsigned char *resizedLMap = new unsigned char[resWidth*resHeight];

	 

	 for(int i = 0; i < resHeight; i++)
	 {	// eilutes
		 for(int j = 0; j < resWidth; j++)
		 {	// stulpeliai
			 int cell = i * resWidth + j;

			 int miniX = floor((float)width/resWidth * j);
			 int miniY = floor((float)height/resHeight * i);

			 int miniCell = miniY * width + miniX;

			 //if(cell < 1024*1024 && miniCell < size*size)
			 resizedLMap[cell] = lightmap[miniCell];
		 }
	 }

	 
	 unsigned char *rlmap = new unsigned char[resWidth * resHeight];





	 skaitlius = 0;
	 while(skaitlius < 0)
	 {

	 for(int a = 1; a < resHeight -1; a++)
	 {
		 for(int b = 1; b < resWidth -1; b++)
		 {
			 rlmap[a*resWidth + b] = (resizedLMap[(a+1)*resWidth + b-1] + resizedLMap[(a+1)*resWidth + b] + resizedLMap[(a+1)*resWidth + b+1]+
									resizedLMap[a*resWidth + b-1] + resizedLMap[a*resWidth + b] + resizedLMap[a*resWidth + b+1]+
									resizedLMap[(a-1)*resWidth + b-1] + resizedLMap[(a-1)*resWidth + b] + resizedLMap[(a-1)*resWidth + b+1]) / 9;
		 }
	 }


	 for(int a = 1; a < resHeight -1; a++)
	 {
		 for(int b = 1; b < resWidth -1; b++)
		 {
			 resizedLMap[a*resWidth + b] = rlmap[a*resWidth + b];
		 }
	 }


	 skaitlius++;
	 }



	 
	 
	 ////////////////////////////////

	 
	 // shadow mapas perkeliamas i tekstura
	 DWORD *atvaizdas; 
	 DWORD *paveikslas = new DWORD[resWidth * resHeight];

	 for(int a = 0; a < resWidth*resHeight; a++)
		{
			paveikslas[a] = D3DCOLOR_ARGB(255, resizedLMap[a], resizedLMap[a], resizedLMap[a]);		
		}

	D3DLOCKED_RECT r;
	LPDIRECT3DTEXTURE9 lMapTexture;
	d3->CreateTexture(resWidth, resHeight, 1,D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &lMapTexture,NULL);
	lMapTexture->LockRect(0,&r,NULL, D3DLOCK_DISCARD | D3DLOCK_NOOVERWRITE);
	atvaizdas=(DWORD*)r.pBits;

	for (int i=0; i<resWidth*resHeight; i++)
		atvaizdas[i]=paveikslas[i];

	lMapTexture->UnlockRect(0);	



	 //////////////////////////////////////

	 delete[] paveikslas;
	 delete[] lmap;
	 delete[] rlmap;
	 delete[] resizedLMap;
	 delete[] lightmap;

	 //char strBuffer[2048];
	 //strBuffer = "img.png";
	 //D3DXSaveTextureToFileA("img.png", D3DXIFF_PNG, lMapTexture, NULL);

	 return lMapTexture;
 }



 void terrain::setSunDirection(LPDIRECT3DDEVICE9 &d3, float saule[3])
 {
	 if(saule[0] != lightDir[0] || saule[1] != lightDir[1] || saule[2] != lightDir[2])
	 {
		 lightDir[0] = saule[0];
		 lightDir[1] = saule[1];
		 lightDir[2] = saule[2];

		 if(lMap != NULL) lMap->Release();
		 //lMap = trace(d3, hMap, width, height, lightDir, 4);
		 lMap = trace2(d3, cellsX, cellsZ, lightDir, 1);
	 }
 }







  LPDIRECT3DTEXTURE9 terrain::trace2(LPDIRECT3DDEVICE9 &d3, int width, int height, float saule[3], int textMultiply)
 {
	 gServerConsole.add("Generuojamas seseliu zemelapis", GAME_CONSOLE_WARNING);
	 unsigned char *lightmap = new unsigned char[width*height];
	 
	 for(int a = 0; a < width*height; a++)
	 {
		 lightmap[a] = 255;
	 }

	 // sviesos sklidimo kryptis
	 int xZ, yZ;

	 if(saule[0] >= 0) { xZ = 1; } else { xZ = -1; }
	 if(saule[2] >= 0) { yZ = 1; } else { yZ = -1; }
	 
	 //float kryptiesKoef = saule[2] / saule[0];
	 int eilute, stulpelis;

	 if(xZ == 1 && yZ == 1)   { eilute = 0;					stulpelis = 0; }
	 if(xZ == 1 && yZ == -1)  { eilute = height - 1;		stulpelis = 0; }
	 if(xZ == -1 && yZ == 1)  { eilute = 0;					stulpelis = width - 1; }
	 if(xZ == -1 && yZ == -1) { eilute = height - 1;		stulpelis = width - 1; }

	 

	 int cEilute, cStulpelis;
	 cEilute = eilute;
	 

	 while(1)
	 {	// eina per eilutes

		 cStulpelis = stulpelis;
		 while(1)
		 {	// eina per stulpelius

			 int cell = cEilute * width + cStulpelis;

			 if(lightmap[cell] == 255)
			 {	// langelio metama seseli skaiciuojame tik tada jei jis pats nera seselyje

				 /////////////////////////////////////////////////////////
				 /////// skaiciavimai kur uzmeta seseli esama cele ///////
				 /////////////////////////////////////////////////////////

				 


				 float px, py;
				 px = cStulpelis;
				 py = cEilute;

				 

				 //int darkness = 128;
				 while(1) /*darkness<255*/
				 {
					 px += saule[0];
					 py += saule[2];

					 int ppx, ppy;

					 ppx = floor(px + 0.5);
					 ppy = floor(py + 0.5);

					 if(ppx < 0 || ppx >= width || ppy < 0 || ppy >= height) { break; }

					 int cele = ppy * width + ppx;
	

					 // get distance from original point to current point
					 //float distance = sqrt( (cStulpelis-px)*(cStulpelis-px) + (cEilute-py)*(cEilute-py) );

					float distance = sqrt( 
					(cellsX*stepX/width*cStulpelis-cellsX*stepX/width*px)*(cellsX*stepX/width*cStulpelis-cellsX*stepX/width*px)
					+
					(cellsZ*stepZ/height*cEilute-cellsZ*stepZ/height*py)*(cellsZ*stepZ/height*cEilute-cellsZ*stepZ/height*py) );
 
					// get height at current point while traveling along light ray
					float hh = getH(D3DXVECTOR3(startX+cellsX*stepX/width*cStulpelis,1,startZ+cellsZ*stepZ/height*cEilute)) + saule[1]*distance;

					//getH(D3DXVECTOR3(cellsX*stepX/width*ppx,1,cellsZ*stepZ/height*ppy), d3);

					

					 
						 if(hh > getH(D3DXVECTOR3(startX+cellsX*stepX/width*px,1,startZ+cellsZ*stepZ/height*py)))
						 {
							 lightmap[cele] = 0 ; //darkness				
						 }
						 else
						 {
							 break;
						 }
					 //darkness+=1;
				 }




				 //////////////////////////////////////////////////////////

			 }
		

			 cStulpelis += xZ;
			 // vidinio ciklo stabdymo salyga
			 if(0 > cStulpelis || cStulpelis > width-1)
			 break;
		 }

		 cEilute += yZ;
		 // isorinio ciklo stabdymo salyga
		 if(0 > cEilute || cEilute > height-1)
		 break;
	 }


	 //lightmapo minkstiklis :D

	 unsigned char *lmap = new unsigned char[width*height];  // laikinas lightmapas


	 int skaitlius = 0;
	 while(skaitlius < 30)
	 {

	 for(int a = 1; a < height -1; a++)
	 {
		 for(int b = 1; b < width -1; b++)
		 {
			 lmap[a*width + b] = (lightmap[(a+1)*width + b-1] + lightmap[(a+1)*width + b] + lightmap[(a+1)*width + b+1]+
									lightmap[a*width + b-1] + lightmap[a*width + b] + lightmap[a*width + b+1]+
									lightmap[(a-1)*width + b-1] + lightmap[(a-1)*width + b] + lightmap[(a-1)*width + b+1]) / 9;
		 }
	 }


	 for(int a = 1; a < height -1; a++)
	 {
		 for(int b = 1; b < width -1; b++)
		 {
			 lightmap[a*width + b] = lmap[a*width + b];
		 }
	 }


	 skaitlius++;
	 }

	 
	 
	 
	 for(int a = 0; a < width*height; a++)
	 {
		 if(lightmap[a] < 128) { lightmap[a] = 128; } else { lightmap[a] = 255; }
	 }
	 

	 // resizinimas i 1024 * 1024

	 // 1024  mapas
	 //int resSize = 1024;

	 int resWidth = width * textMultiply;
	 int resHeight = height * textMultiply;

	 //int size = 256;
	 unsigned char *resizedLMap = new unsigned char[resWidth*resHeight];

	 

	 for(int i = 0; i < resHeight; i++)
	 {	// eilutes
		 for(int j = 0; j < resWidth; j++)
		 {	// stulpeliai
			 int cell = i * resWidth + j;

			 int miniX = floor((float)width/resWidth * j);
			 int miniY = floor((float)height/resHeight * i);

			 int miniCell = miniY * width + miniX;

			 //if(cell < 1024*1024 && miniCell < size*size)
			 resizedLMap[cell] = lightmap[miniCell];
		 }
	 }

	 
	 unsigned char *rlmap = new unsigned char[resWidth * resHeight];





	 skaitlius = 0;
	 while(skaitlius < 30)
	 {

	 for(int a = 1; a < resHeight -1; a++)
	 {
		 for(int b = 1; b < resWidth -1; b++)
		 {
			 rlmap[a*resWidth + b] = (resizedLMap[(a+1)*resWidth + b-1] + resizedLMap[(a+1)*resWidth + b] + resizedLMap[(a+1)*resWidth + b+1]+
									resizedLMap[a*resWidth + b-1] + resizedLMap[a*resWidth + b] + resizedLMap[a*resWidth + b+1]+
									resizedLMap[(a-1)*resWidth + b-1] + resizedLMap[(a-1)*resWidth + b] + resizedLMap[(a-1)*resWidth + b+1]) / 9;
		 }
	 }


	 for(int a = 1; a < resHeight -1; a++)
	 {
		 for(int b = 1; b < resWidth -1; b++)
		 {
			 resizedLMap[a*resWidth + b] = rlmap[a*resWidth + b];
		 }
	 }


	 skaitlius++;
	 }



	 
	 
	 ////////////////////////////////
	 
	 
	 // shadow mapas perkeliamas i tekstura
	 unsigned char *atvaizdas; 
	 //DWORD *paveikslas = new DWORD[resWidth * resHeight];

	 //D3DCOLOR
	 //D3DCOLOR_
	 /*for(int a = 0; a < resWidth*resHeight; a++)
		{
			paveikslas[a] = D3DCOLOR_ARGB(resizedLMap[a], resizedLMap[a], resizedLMap[a], resizedLMap[a]);		
		}*/

	D3DLOCKED_RECT r;
	LPDIRECT3DTEXTURE9 lMapTexture;
	d3->CreateTexture(resWidth, resHeight, 1,D3DUSAGE_DYNAMIC, D3DFMT_A8, D3DPOOL_DEFAULT, &lMapTexture,NULL);
	lMapTexture->LockRect(0,&r,NULL, D3DLOCK_DISCARD | D3DLOCK_NOOVERWRITE);
	atvaizdas=(unsigned char*)r.pBits;

	for (int i=0; i<resWidth*resHeight; i++)
		atvaizdas[i]=resizedLMap[i];

	lMapTexture->UnlockRect(0);	



	 //////////////////////////////////////

	 //delete[] paveikslas;
	 delete[] lmap;
	 delete[] rlmap;
	 delete[] resizedLMap;
	 delete[] lightmap;

	 //char strBuffer[2048];
	 //strBuffer = "img.png";
	 //D3DXSaveTextureToFileA("C:\img.png", D3DXIFF_PNG, lMapTexture, NULL);

	 stringstream ss1, ss2;//create a stringstream
	 ss1 << width;
	 ss2 << height;

	 gServerConsole.add("Sugeneruotas seseliu zemelapis: "+ss1.str()+"x"+ss2.str());

	 return lMapTexture;
 }

















 unsigned char *terrain::textureGenerator(float minHeight, float maxHeight, float minAngle, float maxAngle, int width, int height, int density, int blur)
 {
	 //cons.add("Generuojamas teksturu zemelapis...");
	 unsigned char *textureMap = new unsigned char[width*height];
	 
	 for(int a = 0; a < width*height; a++)
	 {
		 textureMap[a] = 0;
	 }
	 //MessageBox(NULL, L"Input device error", L"Klaida", MB_ICONERROR);
	 //cellsX*stepX/width*cStulpelis
	 
	 for(int i = 0; i < height; i++)
	 {
		 float z = startZ + cellsZ*stepZ/height * i;
		 if(z <= startZ) z = startZ + 0.1;
		 if(z >= startZ + cellsZ*stepZ) z = startZ + cellsZ*stepZ - 0.1;
		 for(int j = 0; j < width; j++)
		 {
			 float x = startX + cellsX*stepX/width * j;
			 if(x <= startX) x = startX + 0.1;
			 if(x >= startX + cellsX*stepX) x = startX + cellsX*stepX - 0.1;
			 
			 
			 float h = getH(D3DXVECTOR3(x, 1, z));
			 
			 D3DXVECTOR3 normale = getNormal(D3DXVECTOR3(x, 1, z));
			 D3DXVec3Normalize((D3DXVECTOR3*)&normale, &normale);
			 float angle = acos(normale.y)*(180/3.14159265);

			 if(minHeight <= h && h <= maxHeight && minAngle <= angle && angle <= maxAngle)
			 {
				 // cia jau atzymime, kad gaima auginti zoles ar akmenis
				 textureMap[i * width + j] = density;
			 }
			 

		 }	 
	 }


	 unsigned char *tmpMap = new unsigned char[width * height];


	 int skaitlius = 0;
	 while(skaitlius < blur)
	 {

	 for(int a = 1; a < width -1; a++)
	 {
		 for(int b = 1; b < height -1; b++)
		 {
			 tmpMap[a*width + b] = (textureMap[(a+1)*width + b-1] + textureMap[(a+1)*width + b] + textureMap[(a+1)*width + b+1]+
									textureMap[a*width + b-1] + textureMap[a*width + b] + textureMap[a*width + b+1]+
									textureMap[(a-1)*width + b-1] + textureMap[(a-1)*width + b] + textureMap[(a-1)*width + b+1]) / 9;
		 }
	 }


	 for(int a = 1; a < height -1; a++)
	 {
		 for(int b = 1; b < width -1; b++)
		 {
			 textureMap[a*width + b] = tmpMap[a*width + b];
		 }
	 }


	 skaitlius++;
	 }


	 delete[] tmpMap;


	 stringstream ss1, ss2, ss3;//create a stringstream
	 ss1 << width;
	 ss2 << height;
	 ss3 << blur;

	 gServerConsole.add("Sugeneruotas teksturu zemelapis: "+ss1.str()+"x"+ss2.str()+" Blur level: "+ss3.str());


	 return textureMap;
 }




 LPDIRECT3DTEXTURE9 terrain::textureRGBA(LPDIRECT3DDEVICE9 &d3, int width, int height, unsigned char *tex1, unsigned char *tex2, unsigned char *tex3, unsigned char *tex4)
 {
	 DWORD *atvaizdas; 
	 DWORD *paveikslas = new DWORD[width * height];

	 for(int a = 0; a < width*height; a++)
		{	
			paveikslas[a] = D3DCOLOR_ARGB(tex4[a], tex1[a], tex2[a], tex3[a]);		
		}

	D3DLOCKED_RECT r;

	LPDIRECT3DTEXTURE9 textureMap;
	d3->CreateTexture(width, height, 1,D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &textureMap,NULL);
	textureMap->LockRect(0,&r,NULL, D3DLOCK_DISCARD | D3DLOCK_NOOVERWRITE);
	atvaizdas=(DWORD*)r.pBits;
	//r.Pitch = ;
	

	int cnt = 0;
	for (DWORD y=0; y<height; y++)
	{
		for (DWORD x=0; x<width; x++)
		{
		   int index=(x+(y*(r.Pitch)/4));
        
		   atvaizdas[index]=paveikslas[cnt];

		   //// Blue
		   //BYTE b=bytePointer[index];

		   //// Green
		   //BYTE g=bytePointer[index+1];

		   //// Red
		   //BYTE r=bytePointer[index+2];

		   //// Alpha
		   //BYTE a=bytePointer[index+3];  
		   cnt++;
	   }
	}
	
	//stringstream ss; 
	//ss << "Bits: " << r.pBits << " Pitch: " << r.Pitch;
	//cons.add(ss.str(), GAME_CONSOLE_ERROR);
	/*
	for (int i=0; i < width*height; i++)
		atvaizdas[i]=paveikslas[i];*/

	
	//textureMap->PreLoad();
	textureMap->UnlockRect(0);	
	delete[] paveikslas;
	
	//stringstream ss;
	//ss << mName << "lolo.tga";

	
	//D3DXSaveTextureToFileA(ss.str().c_str(), D3DXIFF_TGA, textureMap, NULL);
	//textureMap->Release();

	//D3DXCreateTextureFromFileA(d3, ss.str().c_str(), &textureMap);

	return textureMap;
 }



void terrain::Update()
{

	double x = kamera->getCamP().x - startX; 
	double y = kamera->getCamP().z - startZ; 
	double wX = kamera->getCamW().x - startX; 
	double wY = kamera->getCamW().z - startZ;

	double angl = angle(x, y, wX, wY);
	double dst = sqrt((x-lastX)*(x-lastX)+(y-lastY)*(y-lastY));
	if(/*wAngle != angl || */dst > ((stepX+stepZ)/2) /*|| true*/)
	{
		/*while(renderlock) {  }
		
		writelock = true;*/
		lastX = x;
		lastY = y;

		wAngle = angl;
		if(wAngle >= 360) wAngle = 0;
		leftAngle = wAngle + 35;
		rightAngle = wAngle - 35;        
		if(leftAngle > 360)
		{
			leftAngle -= 360;
			rightAngle -= 360;
		}
		if(rightAngle < 0)
		{ 
			rightAngle +=360; 
			angleReverse = true; 
		}
		else
		{
			angleReverse = false;
		}

		triCount[updateindex] = 0;
		indichesCount[updateindex] = 0;


		m_ibQuad[updateindex]->Lock( 0, 0, (void**)&drawMapQuad[updateindex], 0 );

		tree(0, 0, 0, cellsX, cellsZ, x/stepX, y/stepZ, 0);
		m_ibQuad[updateindex]->Unlock();

		//stringstream ss1;//create a stringstream
		//ss1 << triCount[updateindex];
		//cons.add("Trikampiu: "+ss1.str());

		//while(renderlock) {} 
#pragma omp critical (terrainRender)
		{
		if(updateindex == 0)
		{
			updateindex = 1;
			drawindex   = 0;
		}
		else
		{
			updateindex = 0;
			drawindex   = 1;
		}
		}
		/*writelock = false;*/

	}
}


void terrain::tree(int level, int x, int y, int w, int h, double tx, double ty, double atstumas)
{
	double dist = 0;
	bool c = colision(x, y, w, h, tx, ty, level, dist);
	 
	if(level < levelCount-1 &&
			(
			c
			))
	{
		// sakojasi
		tree(level+1, x,        y,      (int)(w/2), (int)(h/2), tx, ty, dist);
		tree(level+1, x+w/2,    y,      (int)(w/2), (int)(h/2), tx, ty, dist);
		tree(level+1, x,        y+h/2,  (int)(w/2), (int)(h/2), tx, ty, dist);
		tree(level+1, x+w/2,    y+h/2,  (int)(w/2), (int)(h/2), tx, ty, dist);
	}
	else
	{
		double a1 = angle(tx, ty, x, y);
		double a2 = angle(tx, ty, x+w, y);
		double a3 = angle(tx, ty, x, y+h);
		double a4 = angle(tx, ty, x+w, y+h);
		if(true ||											////////////////// fustrum culling on/off :D
			(!angleReverse && (  
			(leftAngle >= a1 && a1 >= rightAngle) ||
			(leftAngle >= a2 && a2 >= rightAngle) ||
			(leftAngle >= a3 && a3 >= rightAngle) ||
			(leftAngle >= a4 && a4 >= rightAngle)))
				||
			(angleReverse  &&  (
			(leftAngle >= a1 || a1 >= rightAngle) ||
			(leftAngle >= a2 || a2 >= rightAngle) ||
			(leftAngle >= a3 || a3 >= rightAngle) ||
			(leftAngle >= a4 || a4 >= rightAngle)))
                    
                    
				)
		{
		// piesia kvadrata
            
		//g2d.setPaint(colors[level]);
		//g2d.drawRect(x, y, w, h);

			// formuoti quado drawmapa

			int cX, cY, ltX, ltY, rtX, rtY, rbX, rbY, lbX, lbY,
				lX, lY, tX, tY, rX, rY, bX, bY;

			cX = x + w/2;
			cY = y + h/2;

			ltX = x;
			ltY = y+h;

			rtX = x + w;
			rtY = y + h;

			rbX = x + w;
			rbY = y;

			lbX = x;
			lbY = y;

			lX = x;
			lY = y + h/2;

			tX = x + w/2;
			tY = y + h;

			rX = x + w;
			rY = y + h/2;

			bX = x + w/2;
			bY = y;


			/* 
				+-+-+
				|\|/|
				+-+-+
				|/|\|
				+-+-+
			*/


			drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(cX, cY);
			drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(ltX, ltY);
			drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(tX, tY);

			drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(cX, cY);
			drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(tX, tY);
			drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(rtX, rtY);

			drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(cX, cY);
			drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(rtX, rtY);
			drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(rX, rY);

			drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(cX, cY);
			drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(rX, rY);
			drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(rbX, rbY);

			drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(cX, cY);
			drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(rbX, rbY);
			drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(bX, bY);

			drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(cX, cY);
			drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(bX, bY);
			drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(lbX, lbY);

			drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(cX, cY);
			drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(lbX, lbY);
			drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(lX, lY);

			drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(cX, cY);
			drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(lX, lY);
			drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(ltX, ltY);
				
			triCount[updateindex] += 8;

			if(atstumas+w*2 > radius[level-1])
			{
				if(!fullColision(x, y, w, h, tx, ty, level-1))
				{
					/*	
						+---+
						|\ /|
						| + |
						|/ \|
						+---+
					*/

					drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(cX, cY);
					drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(ltX, ltY);
					drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(rtX, rtY);

					drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(cX, cY);
					drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(rtX, rtY);
					drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(rbX, rbY);

					drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(cX, cY);
					drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(rbX, rbY);
					drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(lbX, lbY);

					drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(cX, cY);
					drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(lbX, lbY);
					drawMapQuad[updateindex][indichesCount[updateindex]++] = xyToint(ltX, ltY);

					triCount[updateindex] += 4;
				}
			}
		}
	}
}

 bool terrain::colision(int x, int y, int w, int h, double tx, double ty, int level, double &dist)
 {
	 if(
             ((dist = distance(x,y, w, h, tx, ty)) <= radius[level]) 
                ||
                (
                tx >= x && tx <= x+w &&
                ty >= y && ty <= y+h 
                )  
          )
        {
            return true;
        }
        
        return false;
 }


 bool terrain::fullColision(int x, int y, int w, int h, double tx, double ty, int level)
    {
        if(
                distance2(x,y, tx, ty) <= radius[level] &&
                distance2(x+w,y, tx, ty) <= radius[level] &&
                distance2(x,y+h, tx, ty) <= radius[level] &&
                distance2(x+w,y+h, tx, ty) <= radius[level]
                )
        {
            return true;
        }

        return false;
    }


 double terrain::distance(int x, int y, int w, int h, double tx, double ty)
    {
        double distanceToCenter = sqrt(
                ((x+(w/2.0)-tx)*(x+(w/2.0)-tx))+((y+(h/2.0)-ty)*(y+(h/2.0)-ty)));
        
        double colisionRadius = sqrt(
                ((w/2.0)*(w/2.0))+((h/2.0)*(h/2.0)));
        
        return distanceToCenter-colisionRadius;
    } 

 double terrain::distance2(int x, int y, double tx, double ty)
    {
        return sqrt(((x-tx)*(x-tx))+((y-ty)*(y-ty)));
    }


 double terrain::angle(double px, double py, double x, double y)
    {
        x = x - px;
        y = y - py;
        
       
		double lenght = sqrt((double)(x*x+y*y));             
        double sinOf = y/lenght;       
        double angle = asin(sinOf)* (180/3.14);       
        
        if(x < 0)
        {
            if(y < 0)
            {
                angle = 180 + abs(angle);
            }
            else
            {
                angle = 180-angle;
            }
        }
        else
        {
            if(y < 0)
            {
                angle = 360 + angle;
            }
        }

        return angle;
    }

 int terrain::xyToint(int x, int y)
 {
	 return vertX * y + x;
 }

 float terrain::getWidth()
 {
	 return (float)cellsX * stepX;
 }

 float terrain::getHeight()
 {
	 return (float)cellsZ * stepZ;
 }