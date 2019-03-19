#ifndef VERTEX3D_H
#define VERTEX3D_H
struct Vertex3D
{
	float x, y, z;
	//float r, g, b;
	float u, v;
	float nx, ny, nz;
	float p1, p2, p3;
	float tx, ty, tz; //tangent 
	float bx, by, bz; //normal

}; 
#endif