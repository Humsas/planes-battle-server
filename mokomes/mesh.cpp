#include "mesh.h"
#include "console.h"
extern Console gServerConsole;

Mesh::~Mesh()
{
	meshList *L;
	while(MESH)
	{
		L = MESH;
		MESH= MESH->next;
		L->mesh.mesh->Release();
		//delete[] L->mesh.d3dxMaterials;
		delete[] L->mesh.meshMaterials;
		delete[] L->mesh.meshTextures;


		delete[] L->mesh.bounds;
		L->mesh.m_vb->Release();
		L->mesh.m_ib->Release();
		delete L;
	}
	delete MESH;
}



// pakrauna mesha is x failo i sarasa
void Mesh::loadMesh(string path, string filename, string id, LPDIRECT3DDEVICE9 &d3)
{

	meshList *M = new meshList;
	M -> next = MESH;
	M -> ID = id;
  
	//LPD3DXBUFFER materialBuffer;
	ID3DXBuffer* materialBuffer;
 
  

	HRESULT hr=D3DXLoadMeshFromXA( STL(path + filename), D3DXMESH_MANAGED, 
								d3, NULL, 
								&materialBuffer, NULL, &M->mesh.numMaterials, 
								&M->mesh.mesh );

	if(FAILED(hr))
	{
		gServerConsole.addLine("Modelis id: \""+id+"\" neuzkrautas", GAME_CONSOLE_ERROR);
		delete M;
	}
	else
	{

		M->mesh.d3dxMaterials = (D3DXMATERIAL*)materialBuffer->GetBufferPointer();
		M->mesh.meshMaterials = new D3DMATERIAL9[M->mesh.numMaterials];
		M->mesh.meshTextures  = new IDirect3DTexture9*[M->mesh.numMaterials];
  

		for (DWORD i=0; i<M->mesh.numMaterials; i++)
		{
			// Copy the material
			M->mesh.meshMaterials[i] = M->mesh.d3dxMaterials[i].MatD3D;

			// Set the ambient color for the material (D3DX does not do this)
			M->mesh.meshMaterials[i].Ambient = M->mesh.meshMaterials[i].Diffuse;
     
			// Create the texture if it exists - it may not
			M->mesh.meshTextures[i] = NULL;
			if (M->mesh.d3dxMaterials[i].pTextureFilename)
			{
				wstringstream ss;
				ss << path.c_str() <<  M->mesh.d3dxMaterials[i].pTextureFilename;
				M->mesh.meshTextures[i] = textureManager->loadTexture(ss.str());
				//D3DXCreateTextureFromFileA(d3,  STL(path + M->mesh.d3dxMaterials[i].pTextureFilename),     &M->mesh.meshTextures[i]); 
			}
		}


		// mesho optimizacija
		DWORD* pAdjacency = NULL;
		pAdjacency = new DWORD[3*M->mesh.mesh->GetNumFaces()];
		M->mesh.mesh->GenerateAdjacency(0.0f, pAdjacency);

		const DWORD dwOptimizationFlags = D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT |  D3DXMESHOPT_VERTEXCACHE | D3DXMESHOPT_DONOTSPLIT;

		M->mesh.mesh->OptimizeInplace( dwOptimizationFlags, pAdjacency, NULL, NULL, NULL) ;

		materialBuffer->Release();



		BYTE* Vertices=NULL;
		hr=M->mesh.mesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&Vertices);
		D3DXVECTOR3 minBounds,maxBounds;
		D3DXComputeBoundingBox((D3DXVECTOR3*)Vertices, M->mesh.mesh->GetNumVertices(), D3DXGetFVFVertexSize(M->mesh.mesh->GetFVF()), &minBounds, &maxBounds);
		M->mesh.mesh->UnlockVertexBuffer();

		M->mesh.bounds = new D3DXVECTOR3[8];

		/*minBounds.x = 0.0;
		minBounds.y = 0.0;
		minBounds.z = 0.0;

		maxBounds.x = 1.0;
		maxBounds.y = 1.0;
		maxBounds.z = 1.0;*/


		// We have min and max values, use these to get the 8 corners of the bounding box
		M->mesh.bounds[0] = D3DXVECTOR3( minBounds.x, minBounds.y, minBounds.z ); // xyz
		M->mesh.bounds[1] = D3DXVECTOR3( maxBounds.x, minBounds.y, minBounds.z ); // Xyz
		M->mesh.bounds[2] = D3DXVECTOR3( minBounds.x, maxBounds.y, minBounds.z ); // xYz
		M->mesh.bounds[3] = D3DXVECTOR3( maxBounds.x, maxBounds.y, minBounds.z ); // XYz
		M->mesh.bounds[4] = D3DXVECTOR3( minBounds.x, minBounds.y, maxBounds.z ); // xyZ
		M->mesh.bounds[5] = D3DXVECTOR3( maxBounds.x, minBounds.y, maxBounds.z ); // XyZ
		M->mesh.bounds[6] = D3DXVECTOR3( minBounds.x, maxBounds.y, maxBounds.z ); // xYZ
		M->mesh.bounds[7] = D3DXVECTOR3( maxBounds.x, maxBounds.y, maxBounds.z ); // XYZ

		M->mesh.radius =	sqrt(maxBounds.x*maxBounds.x + 
							maxBounds.y*maxBounds.y + 
							maxBounds.z*maxBounds.z);
		M->mesh.maxBound = 0;

		if(maxBounds.x - minBounds.x > M->mesh.maxBound) M->mesh.maxBound = maxBounds.x - minBounds.x;
		if(maxBounds.y - minBounds.y > M->mesh.maxBound) M->mesh.maxBound = maxBounds.y - minBounds.y;
		if(maxBounds.z - minBounds.z > M->mesh.maxBound) M->mesh.maxBound = maxBounds.z - minBounds.z;

	
		d3->CreateVertexBuffer( 8*sizeof(MYVERTEX),
						D3DUSAGE_WRITEONLY, MYVERTEX_FVF,
						D3DPOOL_MANAGED, &M->mesh.m_vb, NULL );

		HRESULT Lock( UINT OffsetToLock, UINT SizeToLock, VOID **data, DWORD Flags );

		MYVERTEX* pVertices;
		M->mesh.m_vb->Lock( 0, 0, (void**)&pVertices, 0 );

		for( int i = 0; i < 8; i++ )
		{
			pVertices[i].p.x = M->mesh.bounds[i].x;
			pVertices[i].p.y = M->mesh.bounds[i].y;
			pVertices[i].p.z = M->mesh.bounds[i].z;
			pVertices[i].colour = D3DCOLOR_XRGB(250, 100, 100);
		}
		M->mesh.m_vb->Unlock();




		// lines * taskai_linijom * 2
		d3->CreateIndexBuffer(12*2*2,D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &M->mesh.m_ib, NULL);

		WORD *indices=NULL;
		M->mesh.m_ib->Lock( 0, 0, (void**)&indices, 0 );

		indices[0]=0;
		indices[1]=1;
				
		indices[2]=1;
		indices[3]=3;

		indices[4]=3;
		indices[5]=2;

		indices[6]=2;
		indices[7]=0;

		indices[8]=4;
		indices[9]=5;

		indices[10]=5;
		indices[11]=7;

		indices[12]=7;
		indices[13]=6;

		indices[14]=6;
		indices[15]=4;

		indices[16]=0;
		indices[17]=4;

		indices[18]=1;
		indices[19]=5;

		indices[20]=3;
		indices[21]=7;

		indices[22]=2;
		indices[23]=6;


		M->mesh.m_ib->Unlock();

 
		this->MESH = M;

		gServerConsole.addLine("Modelis id: \""+id+"\" uzkrautas");
	}

}










LPCSTR Mesh::STL(string a)
{
int ln = a.length();
char *b = new char[ln+1];

	 for(int o = 0; o <ln; o++)
	 {
	 b[o] = a[o];
	 }
b[ln]='\0';

	 return b;
}





void Mesh::drawMesh(LPDIRECT3DDEVICE9 &d3, string id, double x, double y, double z, double scale, float rx, float ry, float rz)
{
	D3DXMATRIX worldsave, meshworld, scaleM, combo;
	meshList *M;
	for (M = MESH; M != NULL; M = M->next) 
	{

		if(M->ID == id)
		{
			d3->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

			d3->GetTransform( D3DTS_WORLD, &worldsave );
			meshworld = worldsave;
			meshworld._41 = x; 
			meshworld._42 = y; 
			meshworld._43 = z;

			D3DXMatrixScaling(&scaleM, scale/M->mesh.maxBound, scale/M->mesh.maxBound, scale/M->mesh.maxBound);


		//////////////
		D3DXMATRIX matRotationX,
					matRotationY,
					matRotationZ;


			//x rotation (NEW)
			D3DXMatrixRotationX(&matRotationX,DEGTORAD(rx));
			//y rotation (NEW)
			D3DXMatrixRotationY(&matRotationY,DEGTORAD(ry));
			//z rotation (NEW)
			D3DXMatrixRotationZ(&matRotationZ,DEGTORAD(rz));


		//////////////
 
 
			combo = scaleM * matRotationZ * matRotationX * matRotationY * meshworld;




			d3->SetTransform( D3DTS_WORLD, &combo );



			for (DWORD i=0; i<M->mesh.numMaterials; i++)
			{
				// Set the material and texture for this subset
				d3->SetMaterial(&M->mesh.meshMaterials[i]);
				d3->SetTexture(0,M->mesh.meshTextures[i]);
        
				// Draw the mesh subset
				M->mesh.mesh->DrawSubset(i);
  
			}



			d3->SetRenderState(D3DRS_NORMALIZENORMALS, false);
			  
			  
			//d3->SetTransform( D3DTS_WORLD, &worldsave);



			//d3->SetTransform( D3DTS_WORLD, &combo );
		
			//d3->SetMaterial(NULL);
			//d3->SetTexture(0,NULL);

			//d3->SetRenderState( D3DRS_LIGHTING, false );
			////d3->SetRenderState(D3DRS_ALPHATESTENABLE, false);

			//d3->SetTexture(0,NULL);
			//d3->SetStreamSource( 0, M->mesh.m_vb,0, sizeof(MYVERTEX) );
			//d3->SetFVF( MYVERTEX_FVF );
			//d3->SetIndices( M->mesh.m_ib);
   
			//// draw a triangle list using 24 vertices and 12 triangles

			//d3->DrawIndexedPrimitive( D3DPT_LINELIST,0,0,24,0,12); 
			//
			//
			////d3->SetRenderState(D3DRS_ALPHATESTENABLE, true);
			//d3->SetRenderState( D3DRS_LIGHTING, true );
			d3->SetTransform( D3DTS_WORLD, &worldsave);
			  
		}

	}

}


void Mesh::drawMesh(LPDIRECT3DDEVICE9 &d3, string id, double x, double y, double z, double scale, D3DXMATRIX &rotMat)
{
	D3DXMATRIX worldsave, meshworld, scaleM, combo;
	meshList *M;
	for (M = MESH; M != NULL; M = M->next) 
	{

		if(M->ID == id)
		{
			d3->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

			d3->GetTransform( D3DTS_WORLD, &worldsave );
			meshworld = worldsave;
			meshworld._41 = x; 
			meshworld._42 = y; 
			meshworld._43 = z;

			D3DXMatrixScaling(&scaleM, scale/M->mesh.maxBound, scale/M->mesh.maxBound, scale/M->mesh.maxBound);


		//////////////
		//D3DXMATRIX matRotationX,
		//			matRotationY,
		//			matRotationZ;


		//	//x rotation (NEW)
		//	D3DXMatrixRotationX(&matRotationX,DEGTORAD(rx));
		//	//y rotation (NEW)
		//	D3DXMatrixRotationY(&matRotationY,DEGTORAD(ry));
		//	//z rotation (NEW)
		//	D3DXMatrixRotationZ(&matRotationZ,DEGTORAD(rz));


		//////////////
 
 
			combo = scaleM * rotMat * meshworld;




			d3->SetTransform( D3DTS_WORLD, &combo );



			for (DWORD i=0; i<M->mesh.numMaterials; i++)
			{
				// Set the material and texture for this subset
				d3->SetMaterial(&M->mesh.meshMaterials[i]);
				d3->SetTexture(0,M->mesh.meshTextures[i]);
        
				// Draw the mesh subset
				M->mesh.mesh->DrawSubset(i);
  
			}



			d3->SetRenderState(D3DRS_NORMALIZENORMALS, false);
			  
			  
			//d3->SetTransform( D3DTS_WORLD, &worldsave);



			//d3->SetTransform( D3DTS_WORLD, &combo );
		
			//d3->SetMaterial(NULL);
			//d3->SetTexture(0,NULL);

			//d3->SetRenderState( D3DRS_LIGHTING, false );
			////d3->SetRenderState(D3DRS_ALPHATESTENABLE, false);

			//d3->SetTexture(0,NULL);
			//d3->SetStreamSource( 0, M->mesh.m_vb,0, sizeof(MYVERTEX) );
			//d3->SetFVF( MYVERTEX_FVF );
			//d3->SetIndices( M->mesh.m_ib);
   
			//// draw a triangle list using 24 vertices and 12 triangles

			//d3->DrawIndexedPrimitive( D3DPT_LINELIST,0,0,24,0,12); 
			//
			//
			////d3->SetRenderState(D3DRS_ALPHATESTENABLE, true);
			//d3->SetRenderState( D3DRS_LIGHTING, true );
			d3->SetTransform( D3DTS_WORLD, &worldsave);
			  
		}

	}

}

void Mesh::drawMesh(LPDIRECT3DDEVICE9 &d3, meshInfo *mesh, double x, double y, double z, double scale, float rx, float ry, float rz)
{
	D3DXMATRIX worldsave, meshworld, scaleM, combo;


	if(mesh != NULL)
	{
		d3->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

		d3->GetTransform( D3DTS_WORLD, &worldsave );
		meshworld = worldsave;
		meshworld._41 = x; 
		meshworld._42 = y; 
		meshworld._43 = z;

		D3DXMatrixScaling(&scaleM, scale/mesh->maxBound, scale/mesh->maxBound, scale/mesh->maxBound);


		//////////////
		D3DXMATRIX	matRotationX,
					matRotationY,
					matRotationZ;


		//x rotation (NEW)
		D3DXMatrixRotationX(&matRotationX,DEGTORAD(rx));
		//y rotation (NEW)
		D3DXMatrixRotationY(&matRotationY,DEGTORAD(ry));
		//z rotation (NEW)
		D3DXMatrixRotationZ(&matRotationZ,DEGTORAD(rz));


		//////////////
 
 
		combo = scaleM * matRotationZ * matRotationX * matRotationY * meshworld;




		d3->SetTransform( D3DTS_WORLD, &combo );



		for (DWORD i=0; i<mesh->numMaterials; i++)
		{
			// Set the material and texture for this subset
			d3->SetMaterial(&mesh->meshMaterials[i]);
			d3->SetTexture(0,mesh->meshTextures[i]);
        
			// Draw the mesh subset
			mesh->mesh->DrawSubset(i);
		}



		d3->SetRenderState(D3DRS_NORMALIZENORMALS, false);
			  
			  
		//d3->SetTransform( D3DTS_WORLD, &worldsave);



		//d3->SetTransform( D3DTS_WORLD, &combo );
		
		//d3->SetMaterial(NULL);
		//d3->SetTexture(0,NULL);

		//d3->SetRenderState( D3DRS_LIGHTING, false );
		////d3->SetRenderState(D3DRS_ALPHATESTENABLE, false);

		//d3->SetTexture(0,NULL);
		//d3->SetStreamSource( 0, M->mesh.m_vb,0, sizeof(MYVERTEX) );
		//d3->SetFVF( MYVERTEX_FVF );
		//d3->SetIndices( M->mesh.m_ib);
   
		//// draw a triangle list using 24 vertices and 12 triangles

		//d3->DrawIndexedPrimitive( D3DPT_LINELIST,0,0,24,0,12); 
		//
		//
		////d3->SetRenderState(D3DRS_ALPHATESTENABLE, true);
		//d3->SetRenderState( D3DRS_LIGHTING, true );
		d3->SetTransform( D3DTS_WORLD, &worldsave);
			  
	}




}






void Mesh::drawMesh(LPDIRECT3DDEVICE9 &d3, meshInfo *mesh, D3DXVECTOR3 &pozicija, D3DXMATRIX &transformMatrix)
{
	D3DXMATRIX worldsave, meshworld, combo;


	if(mesh != NULL)
	{
		d3->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

		d3->GetTransform( D3DTS_WORLD, &worldsave );
		meshworld = worldsave;
		meshworld._41 = pozicija.x; 
		meshworld._42 = pozicija.y; 
		meshworld._43 = pozicija.z;


 
		combo = transformMatrix * meshworld;


		d3->SetTransform( D3DTS_WORLD, &combo );


		for (DWORD i=0; i<mesh->numMaterials; i++)
		{
			// Set the material and texture for this subset
			d3->SetMaterial(&mesh->meshMaterials[i]);
			d3->SetTexture(0,mesh->meshTextures[i]);
        
			// Draw the mesh subset
			mesh->mesh->DrawSubset(i);
		}

		d3->SetRenderState(D3DRS_NORMALIZENORMALS, false);
			  
		d3->SetTransform( D3DTS_WORLD, &worldsave);
			  
	}




}





meshInfo* Mesh::getPointer(string id)
{
	meshInfo *mInf = NULL;
	meshList *M;
	for (M = MESH; M != NULL; M = M->next) 
	{
		if(M->ID == id)
		{
			mInf = &M->mesh;
		}
	}
	return mInf;
}