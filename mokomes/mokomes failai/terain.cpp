#include "terrain.h"



void terrain::setVal()
	{  
	startX = -50;
	startY = -50;

	cellsX = 100;
	cellsY = 100;
	vertX = cellsX + 1;
	vertY = cellsY + 1;

	stepX = 15;
	stepY = 15;	

	v_buffer = NULL;

	vert = new terVert[vertX * vertY];


	//vert[10201];
	}


void terrain::generateTerain(LPDIRECT3DDEVICE9 &d3)
{

int count = 0;


for(int y = 0; y < vertY; y++)
{
	for(int x = 0; x < vertX; x++)
	{
    
	vert[count].X = startX + (stepX * x);
	vert[count].Y = startY + (stepY * y);
	vert[count].Z = 50* sin((double) x/10)+ 50* sin((double) y/10);
		//(5* sin((double) x/10) * cos((double) y)) + sin((double) x);
	vert[count].COLOR = D3DCOLOR_XRGB(255, 255, 255);

	count++;
	}
}




// create a vertex buffer interface called v_buffer
    d3->CreateVertexBuffer(vertX * vertY * sizeof(terVert) ,
                               0,
                               CUSTOMFVF,
                               D3DPOOL_MANAGED,
                               &v_buffer,
                               NULL);

    VOID* pVoid;    // a void pointer

    // lock v_buffer and load the vertices into it
    v_buffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, vert, sizeof(vert));
    v_buffer->Unlock();





d3->CreateIndexBuffer((cellsX * cellsY * 2)*3*2,D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_ib, NULL);


m_ib->Lock( 0, 0, (void**)&drawMap, 0 );


//int drawMap[96];
count = 0;
int vIndex=0;
for (int y = 0; y < cellsY; y++)
{
    for (int x = 0; x<cellsX; x++)
    {
        // first triangle
		drawMap[count++]=vIndex+vertX+1;
        drawMap[count++]=vIndex+vertX;
        drawMap[count++]=vIndex;


        // second triangle
		drawMap[count++]=vIndex + 1;
        drawMap[count++]=vIndex+vertX+1;
        drawMap[count++]=vIndex;
       
        vIndex++;
     }
     vIndex++;
}

m_ib->Unlock();

}



void terrain::drawTerain(LPDIRECT3DDEVICE9 &d3)

{
   d3->SetIndices(m_ib);

   d3->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
   
   d3->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,vertX * vertY,0,cellsX * cellsY * 2); 
   
   d3->SetRenderState(D3DRS_CULLMODE, 0);
}



D3DXVECTOR3 terrain::getNormal(D3DXVECTOR3 vect)
{



	if((vect.x > -50 && vect.y > -50) &&
	((vect.x < (-50 + (100 * 15)))  && (vect.y < (-50 + (100 * 15)))))

{

int cellX = (vect.x - -50) / 15;
int cellY = (vect.y - -50) / 15;


double dx= (-50 + (cellX * 15)) - vect.x;
double dy= (-50 + (cellY * 15)) - vect.y;

//int dx=Obj.getCamP('x')- -50;
//int dy=Obj.getCamP('y')- -50;


int cNr = cellY * 100 + cellX;
int start = 0;


D3DXVECTOR3 A, B, N;

if(dx < dy) 
{
// pirmas trikampis
start = cNr * 6;

}
else
{
// antras trikampis
start = cNr * 6 + 3;

}

A.x = (vert[drawMap[start+1]].X - vert[drawMap[start+2]].X);
A.y = (vert[drawMap[start+1]].Y - vert[drawMap[start+2]].Y);
A.z = (vert[drawMap[start+1]].Z - vert[drawMap[start+2]].Z);

B.x = (vert[drawMap[start]].X - vert[drawMap[start+2]].X);
B.y = (vert[drawMap[start]].Y - vert[drawMap[start+2]].Y);
B.z = (vert[drawMap[start]].Z - vert[drawMap[start+2]].Z);


N.x = (A.y * B.z - B.y * A.z);
N.y = -(A.x * B.z - B.x * A.z);
N.z = (A.x * B.y - B.x * A.y);

tmpZ = vert[drawMap[start+2]].Z;


return N;
}
return D3DXVECTOR3(0,0,0);
}


float terrain::getH(D3DXVECTOR3 vect)
{
D3DXVECTOR3 N = getNormal(vect);
if(vect.x != 0 && vect.y != 0 && vect.z != 0)
{

int cellX = (vect.x - -50) / 15;
int cellY = (vect.y - -50) / 15;

double dx= (-50 + (cellX * 15)) - vect.x;
double dy= (-50 + (cellY * 15)) - vect.y;

return tmpZ + ((N.x * dx + N.y * dy) / N.z) + 10;
}
else
{
return 0;
}


}
