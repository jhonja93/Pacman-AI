#ifndef _MAPEARTABLA_H_
#define _MAPEARTABLA_H_

struct Point
 {
 	int x;
 	int y;
 }; 
    
int track_pos(int x, int y);
Point track_inverse(int n);

#endif