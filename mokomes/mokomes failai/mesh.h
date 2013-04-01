#include <d3d9.h>
#include <d3dx9.h>
#include <string>

using namespace std;



struct meshInfo
{


DWORD numMaterials;            
LPD3DXMESH mesh;

D3DXMATERIAL* d3dxMaterials;
D3DMATERIAL9 *meshMaterials;
LPDIRECT3DTEXTURE9 *meshTextures;

};

struct meshList
{
string ID;
meshInfo mesh;
meshList *next;
};

class Mesh
{
private:
	meshList *MESH;
	LPD3DXMESH mesh;
	//LPDIRECT3DDEVICE9 *d3d;

public:

	LPDIRECT3DDEVICE9* d3d;

	~Mesh();

	Mesh() { MESH = NULL;  }
	//Mesh(LPDIRECT3DDEVICE9 d3d): d3d(d3d) { MESH = NULL; }

	

	void setDev(LPDIRECT3DDEVICE9 &d3);
	void loadMesh(string path, string filename, string id);
	void drawMesh(LPDIRECT3DDEVICE9 &d3, string id, float x, float y, float z, double scale);

	LPCSTR STL(string a);



};