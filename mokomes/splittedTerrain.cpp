#include "splittedTerrain.h"
#include "console.h"
extern Console gServerConsole;

void splittedTerrain::makeChunk(LPDIRECT3DDEVICE9 &d3, terVert* vert, DWORD* drawMap, int terWidth, int terHeight, int splitWidth, int splitHeight, int chunkId)
{
	// toks trenktas chunko eilutes ir stulpelio ieskojimas
	int eile = 0;
	int stulpas = 0;
	int tmpID = -1;

	for(int i = 0; i < terHeight/splitHeight; i++)
	{		
		for(int j = 0; j < terWidth/splitWidth; j++)
		{			
			tmpID ++;
			if(tmpID == chunkId)
			{  
				eile = i;
				stulpas = j;
				break; 
			}
		}
	}


	


	 

	
	// chunko gamyba
	int chunkStartWidth = stulpas * splitWidth;
	int chunkStartHeight = eile * splitHeight;

	//terVert *tV = new terVert[(splitWidth+1)*(splitHeight+1)];
	//int counter = 0; 

	//for(int i = chunkStartHeight; i < chunkStartHeight + splitHeight+1; i++)
	//{
	//	for(int j = chunkStartWidth; j < chunkStartWidth + splitWidth+1; j++)
	//	{
	//		tV[counter++] = vert[i*(terWidth+1) + j];
	//		
	//	}
	//}

	//link[chunkId].vert = tV;

	//// create a vertex buffer interface called v_buffer
 //   d3->CreateVertexBuffer((splitWidth+1) * (splitHeight+1) * sizeof(terVert) ,
 //                              0,
 //                              VertexFVF,
 //                              D3DPOOL_MANAGED,
 //                              &link[chunkId].v_buff,
 //                              NULL);



 //   VOID* pVoid;    // a void pointer

 //   // lock v_buffer and load the vertices into it
 //   link[chunkId].v_buff->Lock(0, 0, (void**)&pVoid, 0);
	//memcpy(pVoid, link[chunkId].vert, sizeof(terVert)  * ((splitWidth+1) * (splitHeight+1)));
 //   link[chunkId].v_buff->Unlock();

	//delete []pVoid;


	
  TCHAR str[255];

	 _stprintf(str, _T("result: %d"), chunkStartWidth);
 //MessageBox(0, str, 0, MB_OK);

	 //DWORD *dM = new DWORD[splitWidth * splitHeight * 6];
	 //d3->CreateIndexBuffer((cellsX * cellsZ * 2)*3*sizeof(DWORD),D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_ib, NULL);

	d3->CreateIndexBuffer((splitWidth * splitHeight * 2)*3*sizeof(DWORD),D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &link[chunkId].m_ib, NULL);
	link[chunkId].m_ib->Lock( 0, 0, (void**)&link[chunkId].drawMap, 0 );
	
	/*
	int count = 0;	//naujo drawmapo numeracijai

	for(int i = chunkStartHeight; i < chunkStartHeight + splitHeight - 1; i++)
	{
		for(int j = chunkStartWidth; j < chunkStartWidth + splitWidth - 1; j++)
		{
			// kopijavimo ciklas
			for(int k = 0; k < 6; k++)
			{
				link[chunkId].drawMap[count++] = drawMap[(i*(terWidth-1) + j)*6 + k];
			}		
		}
	}
	*/





	int vertX = terWidth+1;
	int count = 0;
	int vIndex= chunkStartHeight * vertX + chunkStartWidth; // buvo = 0;

	//vIndex = 0;



	int eilNum = 0;

	//int vertX = splitWidth+1;




	for (int z = 0; z < splitHeight; z++)
	{
	


		eilNum = (z%2==0)?0:1;

		for (int x = 0; x < splitWidth; x++)
		{
			if((((x+eilNum)%2==0)?0:1) == 0)
			{
				// pirmas kubas
				//  ____
				//  |\ |
				//  ----

 
				// first triangle
				link[chunkId].drawMap[count++]=vIndex;
				link[chunkId].drawMap[count++]=vIndex+1;
				link[chunkId].drawMap[count++]=vIndex+vertX; 
        

				// second triangle
				link[chunkId].drawMap[count++]=vIndex + 1;
				link[chunkId].drawMap[count++]=vIndex+vertX+1; 
				link[chunkId].drawMap[count++]=vIndex+vertX; 
			}
			else
			{
				// antras kubas 
				//  ____
				//  |/ |
				//  ----


				// first triangle
				link[chunkId].drawMap[count++]=vIndex+vertX+1;
				link[chunkId].drawMap[count++]=vIndex+vertX;
				link[chunkId].drawMap[count++]=vIndex;


				// second triangle
				link[chunkId].drawMap[count++]=vIndex + 1;
				link[chunkId].drawMap[count++]=vIndex+vertX+1;
				link[chunkId].drawMap[count++]=vIndex;
			}
			vIndex++;
		 }
		vIndex += vertX - (splitWidth);
	}



	link[chunkId].m_ib->Unlock();

	stringstream ss1;//create a stringstream
	ss1 << omp_get_thread_num();
	//cons.add("Sugeneruotas zemelapio gabalas nr: "+ss1.str());
	
}


void splittedTerrain::render(LPDIRECT3DDEVICE9 &d3, int chunkId)
{
	d3->SetFVF(VertexFVF);

	//d3->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,vertX * vertZ,0,cellsX * cellsZ * 2);

	d3->SetRenderState( D3DRS_LIGHTING, TRUE );
	//d3->SetStreamSource(0, link[chunkId].v_buff, 0, sizeof(terVert)); 
	d3->SetStreamSource(0, *v_buff, 0, sizeof(terVert)); 
	d3->SetIndices(link[chunkId].m_ib);
	//d3->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,(splW+1) * (splH+1),0,splW * splH * 2);
	d3->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,terWidth* terHeight,0,splW * splH * 2);
}

int splittedTerrain::getSize()
{
	return linkSize;
}