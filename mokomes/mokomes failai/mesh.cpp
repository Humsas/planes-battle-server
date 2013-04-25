#include "mesh.h"

Mesh::~Mesh()
 {
 meshList *L;
 while(MESH)
  {
  L = MESH;
  MESH= MESH->next;
  /*delete L->mesh.d3dxMaterials;
  delete L->mesh.meshMaterials;
  delete L->mesh.meshTextures;*/
  delete L;
  }
 delete MESH;
 }





// pakrauna esha is x failo i sarasa
void Mesh::loadMesh(string path, string filename, string id)
{


  meshList *M = new meshList;
  M -> next = MESH;
  M -> ID = id;
  


LPD3DXBUFFER materialBuffer;


  

HRESULT hr=D3DXLoadMeshFromXA( STL(path + filename), D3DXMESH_SYSTEMMEM, 
                             *d3d, NULL, 
							 &materialBuffer, NULL, &M->mesh.numMaterials, 
                             &M->mesh.mesh );

M->mesh.d3dxMaterials = (D3DXMATERIAL*)materialBuffer->GetBufferPointer();

M->mesh.meshMaterials = new D3DMATERIAL9[M->mesh.numMaterials];
M->mesh.meshTextures  = new LPDIRECT3DTEXTURE9[M->mesh.numMaterials];
  




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


     D3DXCreateTextureFromFileA(*d3d,  STL(path + M->mesh.d3dxMaterials[i].pTextureFilename),     &M->mesh.meshTextures[i]); 


 }

}


DWORD* pAdjacency = NULL;
pAdjacency = new DWORD[3*M->mesh.mesh->GetNumFaces()];
M->mesh.mesh->GenerateAdjacency(0.0f, pAdjacency);


const DWORD dwOptimizationFlags = D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT |  D3DXMESHOPT_VERTEXCACHE | D3DXMESHOPT_DONOTSPLIT;

M->mesh.mesh->OptimizeInplace( dwOptimizationFlags, pAdjacency, NULL, NULL, NULL) ;

materialBuffer->Release();
 
  this->MESH = M;

}






void Mesh::setDev(PDIRECT3DDEVICE9 &d3)
{
this->d3d = &d3;
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





void Mesh::drawMesh(LPDIRECT3DDEVICE9 &d3, string id, float x, float y, float z, double scale)
{
D3DXMATRIX worldsave, meshworld, scaleM, combo;


meshList *M;
 for (M = MESH; M != NULL; M = M->next) 
 {

 if(M->ID == id)
 {
 d3->GetTransform( D3DTS_WORLD, &worldsave );
 meshworld = worldsave;
 meshworld._41 = x; 
 meshworld._42 = y; 
 meshworld._43 = z;

 D3DXMatrixScaling(&scaleM, scale, scale, scale);

 combo = scaleM * meshworld;

 d3->SetTransform( D3DTS_WORLD, &combo );



  for (DWORD i=0; i<M->mesh.numMaterials; i++)
  {
   // Set the material and texture for this subset
  d3->SetMaterial(&M->mesh.meshMaterials[i]);
  d3->SetTexture(0,M->mesh.meshTextures[i]);
        
  // Draw the mesh subset
  M->mesh.mesh->DrawSubset(i);
  
  }

  d3->SetTransform( D3DTS_WORLD, &worldsave);
 }

 }



}