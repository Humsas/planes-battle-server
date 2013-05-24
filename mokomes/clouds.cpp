#include "clouds.h"

void Clouds::Render()
{
#pragma omp critical (cloudsRender)
	{


		//d3->SetStreamSourceFreq(0,1);
		//d3->SetStreamSourceFreq(1,1);



		//Update(10000, 10000);
		d3->SetRenderState(D3DRS_FOGENABLE, FALSE);

		d3->SetTexture(0, mTex0);
		d3->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
					d3->SetRenderState( D3DRS_LIGHTING, FALSE );
					d3->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
					//d3->SetTexture(0, mTex0);
					d3->SetFVF(VertexFVFCloud);
					d3->SetStreamSource(0, v_buffClouds, 0, sizeof(cloudVert)); 
					d3->SetIndices(m_ibClouds);
	
					d3->SetTexture(0, mTex0);

					D3DXMATRIX worldsave, meshworld, scaleM, combo;
	
					d3->GetTransform( D3DTS_WORLD, &worldsave );
					meshworld = worldsave;

					D3DXMATRIX	matRotationX,
								matRotationY,
								matRotationZ;

					//x rotation (NEW)
					D3DXMatrixRotationX(&matRotationX,90);
					//y rotation (NEW)
					D3DXMatrixRotationY(&matRotationY,0);
					//z rotation (NEW)
					D3DXMatrixRotationY(&matRotationZ,0);

		for(int layer = 0; layer < 0; layer++)
		{

			for(int i = 0; i < 150; i++)
			{
				for(int j = 0; j < 150; j++)
				{

					
					meshworld._41 = 10000+ i*100; 
					meshworld._42 = 3000; 
					meshworld._43 = 10000+ j*100;

					D3DXMatrixScaling(&scaleM, 10, 10, 10);

					

					


					

					combo = scaleM * matRotationZ * matRotationX * matRotationY * meshworld;
					d3->SetTransform( D3DTS_WORLD, &combo );
				//////////////
 
 
					






					//d3->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
					//d3->SetRenderState( D3DRS_LIGHTING, FALSE );
					//d3->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
					////d3->SetTexture(0, mTex0);
					//d3->SetFVF(VertexFVFCloud);
					//d3->SetStreamSource(0, v_buffClouds, 0, sizeof(cloudVert)); 
					//d3->SetIndices(m_ibClouds);
	
					//d3->SetTexture(0, mTex0);
					d3->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,4,0,2);
					/*d3->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
					d3->SetRenderState( D3DRS_LIGHTING, TRUE );
					d3->SetRenderState(D3DRS_ALPHATESTENABLE, true);*/





					


				}
			}
		}

		d3->SetTransform( D3DTS_WORLD, &worldsave);

		d3->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
					d3->SetRenderState( D3DRS_LIGHTING, TRUE );
					d3->SetRenderState(D3DRS_ALPHATESTENABLE, true);

		d3->SetRenderState(D3DRS_FOGENABLE, TRUE);
	}

}

void Clouds::Update(double x, double y)
{
#pragma omp critical (cloudsRender)
	{
	this->x = x;
	this->y = y;

	poslinkisX += 1.1;
	poslinkisY += 2.2;

	if(abs(poslinkisX) > width)
	{
		poslinkisX += (-poslinkisX/abs(poslinkisX))*width;
	}


	if(abs(poslinkisY) > height)
	{
		poslinkisY += (-poslinkisY/abs(poslinkisY))*height;
	}
	}
}