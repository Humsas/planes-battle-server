#include <d3d9.h>
#include <d3dx9.h>

#ifndef MATEMATIKA_H
#define MATEMATIKA_H


#define DEGTORAD(degree) ((D3DX_PI / 180.0f) * (degree)) // converts from degrees to radians
#define RADTODEG(radian) ((180.0f / D3DX_PI) * (radian)) // converts from radians to degrees


//void MatrixRotation(D3DXMATRIX &mat, float rx, float ry, float rz)
//{
//	D3DXMATRIX	matRotationX,
//				matRotationY,
//				matRotationZ;
//
//	//x rotation (NEW)
//	D3DXMatrixRotationX(&matRotationX,DEGTORAD(rx));
//	//y rotation (NEW)
//	D3DXMatrixRotationY(&matRotationY,DEGTORAD(ry));
//	//z rotation (NEW)
//	D3DXMatrixRotationZ(&matRotationZ,DEGTORAD(rz));
//
//	mat = matRotationZ * matRotationX * matRotationY;
//
//}
//
//
//void MatrixRotationYPR(D3DXMATRIX &mat, float x, float y, float z)
//{
//	D3DXMatrixRotationYawPitchRoll(&mat, x, y, z);
//}
//
//void MatrixScale(D3DXMATRIX &mat, float scaleX, float scaleY, float scaleZ)
//{
//	D3DXMatrixScaling(&mat, scaleX, scaleY, scaleZ);
//}



#endif